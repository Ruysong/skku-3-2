#include "helper.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static Jobs *global_jobs;

void set_jobs_global(Jobs *jobs) { global_jobs = jobs; }
void update_highest_job_id(Jobs *jobs) {
    while (jobs->highest_job_id > 0 && !jobs->table[jobs->highest_job_id])
        jobs->highest_job_id--;
}
void set_foreground_job(Job *job, Jobs *jobs) {
    if (job->id > 0 && job->id <= MAX_JOBS && jobs->table[job->id] == job) {
        jobs->table[job->id] = NULL;
        if (jobs->num_jobs) jobs->num_jobs--;
        update_highest_job_id(jobs);
    }
    job->state = JOB_FOREGROUND;
    job->id = 0;
    jobs->table[0] = job;
    set_job_process_state(job, PROCESS_RUNNING);
}
void remove_foreground_job(Jobs *jobs, bool should_free) {
    Job *j = jobs->table[0];
    jobs->table[0] = NULL;
    if (should_free) free_job(j);
}
void add_job_to_table(Job *job, Jobs *jobs) {
    uint64_t id = 1;
    while (id <= MAX_JOBS && jobs->table[id]) id++;
    if (id > MAX_JOBS) {
        fprintf(stderr, "pa2: job table full\n");
        return;
    }
    job->id = id;
    jobs->table[id] = job;
    jobs->num_jobs++;
    if (id > jobs->highest_job_id) jobs->highest_job_id = id;
}
void remove_job_from_table(Job *job, Jobs *jobs) {
    if (!jobs) jobs = global_jobs;
    if (job && job->id <= MAX_JOBS && jobs->table[job->id] == job) {
        jobs->table[job->id] = NULL;
        if (job->id > 0 && jobs->num_jobs) jobs->num_jobs--;
    }
    update_highest_job_id(jobs);
}
bool is_job_table_empty(Jobs *jobs) { return jobs->num_jobs == 0; }
size_t get_highest_stopped_job_id(Jobs *jobs) {
    for (size_t i = jobs->highest_job_id; i > 0; i--) {
        Job *job = jobs->table[i];
        if (job && job->state == JOB_STOPPED) return i;
    }
    return 0;
}
Job *find_job_by_pid(Jobs *jobs, pid_t pid) {
    if (!jobs) jobs = global_jobs;
    for (size_t i = 0; i <= MAX_JOBS; i++) {
        Job *job = jobs->table[i];
        if (!job) continue;
        for (Process *p = job->first_process; p; p = p->next_process)
            if (p->pid == pid) return job;
    }
    return NULL;
}
Job *find_job_by_id(Jobs *jobs, uint64_t id) {
    if (!jobs) jobs = global_jobs;
    return id <= MAX_JOBS ? jobs->table[id] : NULL;
}
JobState set_job_state(Job *job, Process *process) {
    if (process && process->info.si_code == CLD_STOPPED)
        job->state = JOB_STOPPED;
    else if (process && process->info.si_code == CLD_CONTINUED && job->state != JOB_FOREGROUND)
        job->state = JOB_BACKGROUND;
    if (has_completed(job)) job->state = JOB_DONE;
    return job->state;
}
static void record_event(Job *job, siginfo_t info) {
    Process *p = get_process(job->first_process, info.si_pid);
    if (!p) return;
    p->info = info;
    p->is_running = (info.si_code == CLD_CONTINUED);
    set_job_state(job, p);
}
Process *wait_for_any_process(Job **job) {
    siginfo_t info = {0};
    *job = NULL;
    if (waitid(P_ALL, 0, &info, WEXITED | WSTOPPED | WCONTINUED | WNOHANG) == -1 || !info.si_pid)
        return NULL;
    *job = find_job_by_pid(NULL, info.si_pid);
    if (!*job) return NULL;
    record_event(*job, info);
    return get_process((*job)->first_process, info.si_pid);
}
JobState wait_for_processes(Job *job) {
    while (!has_completed(job) && !has_stopped(job)) {
        siginfo_t info = {0};
        if (waitid(P_PGID, job->pgid, &info, WEXITED | WSTOPPED | WCONTINUED) == -1) {
            if (errno == EINTR) continue;
            if (errno != ECHILD) perror("waitid");
            break;
        }
        if (info.si_pid) record_event(job, info);
    }
    return job->state;
}
status_t handle_job(Job *job, Jobs *jobs) {
    if (job->state == JOB_BACKGROUND) {
        add_job_to_table(job, jobs);
        return 0;
    }
    if (job->state != JOB_FOREGROUND) return 1;
    wait_for_processes(job);
    if (isatty(STDIN_FILENO) && tcsetpgrp(STDIN_FILENO, getpgrp()) == -1)
        perror("tcsetpgrp shell");
    int status = job->last_process ? job->last_process->info.si_status : 0;
    int code = job->last_process ? job->last_process->info.si_code : 0;
    if (code == CLD_KILLED || code == CLD_DUMPED || code == CLD_STOPPED) status += 128;
    if (job->state == JOB_STOPPED) {
        remove_foreground_job(jobs, false);
        add_job_to_table(job, jobs);
        printf("pa2: Job %lu has stopped\n", (unsigned long)job->id);
    } else {
        remove_foreground_job(jobs, true);
    }
    return status;
}
