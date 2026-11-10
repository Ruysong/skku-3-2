#include <ctype.h>
#include <editline/readline.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "helper.h"

volatile sig_atomic_t pending_sigchld = 0;
static volatile sig_atomic_t pending_sigint = 0;
static pid_t shell_pid;

void sigint_handler(int signal_number) {
    (void)signal_number;
    pending_sigint = 1;
}
void sigchld_handler(int signal_number) {
    (void)signal_number;
    pending_sigchld = 1;
}
status_t handle_pending_sigint(void) {
    pending_sigint = 0;
    (void)write(STDOUT_FILENO, "\n", 1);
    return 130;
}
status_t handle_pending_sigchld(void) {
    Job *job = NULL;
    Process *process;
    do {
        pending_sigchld = 0;
        while ((process = wait_for_any_process(&job)) != NULL) {
            (void)process;
            if (job && job->id && has_completed(job)) {
                printf("pa2: Job %lu, '%s' has ended\n",
                       (unsigned long)job->id,
                       job->associated_command ? job->associated_command : "");
                remove_job_from_table(job, NULL);
                free_job(job);
            }
        }
    } while (pending_sigchld);
    return 0;
}

status_t run_exit(Command *cmd, Jobs *jobs, status_t last_status) {
    if (cmd->num_args > 2) {
        fprintf(stderr, "exit: too many arguments\n"); return 1;
    }
    long long code = last_status;
    if (cmd->num_args == 2) {
        char *end;
        errno = 0;
        code = strtoll(cmd->args[1], &end, 10);
        if (errno || end == cmd->args[1] || *end) {
            fprintf(stderr, "exit: %s: invalid integer\n", cmd->args[1]); return 1;
        }
    }
    if (getpid() == shell_pid && !is_job_table_empty(jobs)) {
        fprintf(stderr, "exit: There are still running jobs\n");
        return 1;
    }
    exit((code < 0 || code > 255) ? 255 : (int)code);
}

// Normalize logical paths so that PWD retains symbolic link components.
static char *logical_path(const char *base, const char *path) {
    size_t a = strlen(base), b = strlen(path);
    if (a > SIZE_MAX - b - 3) return NULL;
    char *combined = malloc(a + b + 3);
    if (!combined) return NULL;
    if (path[0] == '/') strcpy(combined, path);
    else snprintf(combined, a + b + 3, "%s/%s", base, path);
    char *result = calloc(a + b + 3, 1);
    if (!result) { free(combined); return NULL; }
    size_t len = 1;
    result[0] = '/';
    char *save = NULL;
    for (char *p = strtok_r(combined, "/", &save); p; p = strtok_r(NULL, "/", &save)) {
        if (!strcmp(p, ".") || !*p) continue;
        if (!strcmp(p, "..")) {
            if (len > 1) {
                while (len > 1 && result[len - 1] != '/') len--;
                if (len > 1) len--;
                result[len] = '\0';
            }
            continue;
        }
        if (len > 1) result[len++] = '/';
        size_t n = strlen(p);
        memcpy(result + len, p, n);
        len += n;
        result[len] = '\0';
    }
    free(combined);
    return result;
}
status_t run_cd(Command *cmd) {
    const char *path = cmd->num_args < 2 || !strcmp(cmd->args[1], "~") ? getenv("HOME") : cmd->args[1];
    if (!path) { fprintf(stderr, "cd: HOME not set\n"); return 1; }
    const char *pwd = getenv("PWD");
    char *physical = getcwd(NULL, 0);
    char *logical = logical_path(pwd && *pwd == '/' ? pwd : (physical ? physical : "/"), path);
    free(physical);
    if (!logical) { fprintf(stderr, "cd: %s\n", strerror(ENOMEM)); return 1; }
    if (chdir(logical) == -1) {
        fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
        free(logical); return 1;
    }
    if (setenv("PWD", logical, 1) == -1) perror("setenv PWD");
    free(logical);
    return 0;
}
status_t run_pwd(Command *cmd) {
    int physical = 0;
    for (size_t i = 1; i < cmd->num_args; i++) {
        if (!strcmp(cmd->args[i], "-L")) physical = 0;
        else if (!strcmp(cmd->args[i], "-P")) physical = 1;
        else { fprintf(stderr, "pwd: invalid option: %s\n", cmd->args[i]); return 1; }
    }
    if (!physical) {
        const char *pwd = getenv("PWD");
        if (pwd && *pwd) { puts(pwd); return 0; }
    }
    char *cwd = getcwd(NULL, 0);
    if (!cwd) { fprintf(stderr, "pwd: %s\n", strerror(errno)); return 1; }
    puts(cwd); free(cwd); return 0;
}

status_t run_command(Command *cmd, Jobs *jobs, status_t last_status) {
    if (cmd->type == COMMAND_BUILTIN)
        return run_builtin_command(cmd, jobs, last_status);
    if (cmd->type == COMMAND_IMPLEMENTED) {
        char *bin = get_bin_dir();
        if (!bin) return 1;
        size_t n = strlen(bin) + strlen(cmd->args[0]) + 2;
        char *path = malloc(n);
        if (!path) { free(bin); return 1; }
        snprintf(path, n, "%s/%s", bin, cmd->args[0]);
        execv(path, cmd->args);
        int error = errno;
        free(bin); free(path);
        if (error == ENOENT) {
            fprintf(stderr, "%s: command not found\n", cmd->args[0]); return 127;
        }
        fprintf(stderr, "%s: %s\n", cmd->args[0], strerror(error));
        return error == EACCES ? 126 : 1;
    }
    execvp(cmd->args[0], cmd->args);
    int error = errno;
    if (error == ENOENT) {
        fprintf(stderr, "%s: command not found\n", cmd->args[0]); return 127;
    }
    fprintf(stderr, "%s: %s\n", cmd->args[0], strerror(error));
    return error == EACCES ? 126 : 1;
}

status_t setup_redirection(Process *p) {
    Command *cmd = p->command;
    if (cmd->stdin) {
        int fd = open(cmd->stdin, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "pa2_shell: %s: %s\n", cmd->stdin, strerror(errno)); return 1;
        }
        if (p->stdin_fd >= 0) close(p->stdin_fd);
        p->stdin_fd = fd;
    }
    if (cmd->stdout) {
        int flags = O_WRONLY | O_CREAT | (cmd->append_stdout ? O_APPEND : O_TRUNC);
        int fd = open(cmd->stdout, flags, 0644);
        if (fd == -1) {
            fprintf(stderr, "pa2_shell: %s: %s\n", cmd->stdout, strerror(errno)); return 1;
        }
        if (p->stdout_fd >= 0) close(p->stdout_fd);
        p->stdout_fd = fd;
    }
    return 0;
}
status_t dup2_stdio(Process *p) {
    if (p->stdin_fd >= 0 && dup2(p->stdin_fd, STDIN_FILENO) == -1) {
        perror("dup2 input"); return 1;
    }
    if (p->stdout_fd >= 0 && dup2(p->stdout_fd, STDOUT_FILENO) == -1) {
        perror("dup2 output"); return 1;
    }
    if (p->stdin_fd > STDERR_FILENO) close(p->stdin_fd);
    if (p->stdout_fd > STDERR_FILENO && p->stdout_fd != p->stdin_fd) close(p->stdout_fd);
    return 0;
}
status_t setup_pipe(Process *p, int fd[2]) {
    if (pipe(fd) == -1) { perror("pipe"); return 1; }
    p->stdout_fd = fd[1];
    if (p->next_process) p->next_process->stdin_fd = fd[0];
    return 0;
}

status_t run_single_builtin_command(Command *cmd, Jobs *jobs, status_t last_status) {
    int saved_in = dup(STDIN_FILENO), saved_out = dup(STDOUT_FILENO);
    if (saved_in == -1 || saved_out == -1) {
        perror("dup"); if (saved_in >= 0) close(saved_in); if (saved_out >= 0) close(saved_out);
        return 1;
    }
    Job *job = default_job();
    if (!job) { close(saved_in); close(saved_out); return 1; }
    set_foreground_job(job, jobs);
    Process p = {.stdin_fd = -1, .stdout_fd = -1, .command = cmd};
    status_t status = 1;
    if (!setup_redirection(&p) && !dup2_stdio(&p)) {
        status = run_builtin_command(cmd, jobs, last_status);
    }
    fflush(stdout);
    if (dup2(saved_in, STDIN_FILENO) == -1 || dup2(saved_out, STDOUT_FILENO) == -1)
        perror("restore stdio");
    close(saved_in); close(saved_out);
    if (jobs->table[0] == job) remove_foreground_job(jobs, true);
    // fg() may already have freed the sentinel job.
    return status;
}

static void reset_child_signals(void) {
    int signals[] = {SIGINT, SIGQUIT, SIGTSTP, SIGTTIN, SIGTTOU, SIGCHLD};
    for (size_t i = 0; i < sizeof signals/sizeof signals[0]; i++) {
        struct sigaction sa = {0};
        sa.sa_handler = SIG_DFL;
        sigemptyset(&sa.sa_mask);
        sigaction(signals[i], &sa, NULL);
    }
}
status_t run_pipeline(Pipeline *pipeline, Jobs *jobs, char *cmd, status_t last_status) {
    Job *job = default_job();
    if (!job) return 1;
    job->associated_command = strdup(cmd);
    if (!job->associated_command) { free_job(job); return 1; }
    job->associated_command[strcspn(job->associated_command, "\r\n")] = '\0';
    job->state = pipeline->is_in_background ? JOB_BACKGROUND : JOB_FOREGROUND;
    if (job->state == JOB_FOREGROUND) set_foreground_job(job, jobs);

    sigset_t block, oldmask;
    sigemptyset(&block);
    sigaddset(&block, SIGCHLD);
    sigprocmask(SIG_BLOCK, &block, &oldmask);
    int prev_read = -1;
    int spawned = 0;
    status_t result = 1;
    for (Command *c = pipeline->first_command; c; c = c->next_command) {
        int next[2] = {-1, -1};
        if (c->next_command && pipe(next) == -1) { perror("pipe"); goto failure; }
        Process *p = default_process();
        if (!p) {
            if (next[0] != -1) { close(next[0]); close(next[1]); }
            goto failure;
        }
        p->command = c;
        if (!job->first_process) job->first_process = p;
        else job->last_process->next_process = p;
        job->last_process = p;
        job->num_processes++;
        p->stdin_fd = prev_read;
        p->stdout_fd = next[1];
        fflush(NULL);
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            if (next[0] != -1) { close(next[0]); close(next[1]); }
            goto failure;
        }
        if (pid == 0) {
            pid_t group = job->pgid == -1 ? getpid() : job->pgid;
            if (setpgid(0, group) == -1) _exit(1);
            // The child also assigns terminal control, as required by the spec.
            if (job->state == JOB_FOREGROUND && isatty(STDIN_FILENO))
                (void)tcsetpgrp(STDIN_FILENO, group);
            reset_child_signals();
            sigprocmask(SIG_SETMASK, &oldmask, NULL);
            if (next[0] >= 0) close(next[0]);
            if (setup_redirection(p) || dup2_stdio(p)) _exit(1);
            _exit(run_command(c, jobs, last_status));
        }
        p->pid = pid; p->is_running = true; spawned++;
        if (job->pgid == -1) job->pgid = pid;
        if (setpgid(pid, job->pgid) == -1 && errno != EACCES && errno != ESRCH)
            perror("setpgid");
        if (prev_read >= 0) close(prev_read);
        if (next[1] >= 0) close(next[1]);
        prev_read = next[0];
    }
    if (prev_read >= 0) close(prev_read);
    if (job->state == JOB_FOREGROUND && isatty(STDIN_FILENO))
        (void)tcsetpgrp(STDIN_FILENO, job->pgid);
    if (job->state == JOB_BACKGROUND) {
        add_job_to_table(job, jobs);
        result = 0;
        sigprocmask(SIG_SETMASK, &oldmask, NULL);
        return result;
    }
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    return handle_job(job, jobs);

failure:
    if (prev_read >= 0) close(prev_read);
    if (spawned && job->pgid > 0) {
        kill(-job->pgid, SIGTERM);
        for (Process *p = job->first_process; p; p = p->next_process) {
            if (p->pid > 0) {
                while (waitpid(p->pid, NULL, 0) == -1 && errno == EINTR) {}
            }
        }
    }
    if (job->state == JOB_FOREGROUND) {
        if (isatty(STDIN_FILENO)) (void)tcsetpgrp(STDIN_FILENO, getpgrp());
        remove_foreground_job(jobs, false);
    }
    free_job(job);
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    return result;
}

static void install_handler(int sig, void (*fn)(int), int flags) {
    struct sigaction sa = {0};
    sa.sa_handler = fn;
    sa.sa_flags = flags;
    sigemptyset(&sa.sa_mask);
    if (sigaction(sig, &sa, NULL) == -1) { perror("sigaction"); exit(1); }
}
int main(int argc, char **argv) {
    rl_catch_signals = 0;
    shell_pid = getpid();
    Jobs jobs = {0};
    set_jobs_global(&jobs);
    install_handler(SIGQUIT, SIG_IGN, 0);
    install_handler(SIGTSTP, SIG_IGN, SA_RESTART);
    install_handler(SIGTTIN, SIG_IGN, 0);
    install_handler(SIGTTOU, SIG_IGN, 0);
    install_handler(SIGINT, sigint_handler, 0);
    install_handler(SIGCHLD, sigchld_handler, SA_RESTART);
    status_t last_status = 0;
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) { fprintf(stderr, "pa2sh: %s: %s\n", argv[1], strerror(errno)); return 1; }
        char *line = NULL;
        size_t capacity = 0;
        while (getline(&line, &capacity, file) != -1) {
            if (pending_sigchld) (void)handle_pending_sigchld();
            if (!line_is_empty(line)) {
                char *copy = strdup(line);
                if (!copy) { last_status = 1; break; }
                last_status = evaluate(&copy, &jobs, last_status);
            }
        }
        free(line);
        fclose(file);
    } else {
        for (;;) {
            if (pending_sigchld) (void)handle_pending_sigchld();
            char *cmd = readline("$ ");
            if (!cmd) {
                if (pending_sigint) { last_status = handle_pending_sigint(); continue; }
                break;
            }
            if (pending_sigint) last_status = handle_pending_sigint();
            if (pending_sigchld) (void)handle_pending_sigchld();
            if (line_is_empty(cmd)) { free(cmd); continue; }
            add_history(cmd);
            last_status = evaluate(&cmd, &jobs, last_status);
            if (pending_sigchld) (void)handle_pending_sigchld();
        }
    }
    if (pending_sigchld) (void)handle_pending_sigchld();
    // Jobs are not killed on EOF; free only the bookkeeping structures.
    for (size_t i = 0; i <= MAX_JOBS; i++) {
        if (jobs.table[i]) free_job(jobs.table[i]);
    }
    return last_status;
}
