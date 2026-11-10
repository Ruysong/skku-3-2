
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <editline/readline.h>

#define MAX_TOKENS 128
#define MAX_CMDS 32
#define MAX_JOBS 32

// 명령어 하나의 정보
typedef struct {
    char *argv[MAX_TOKENS];
    int argc;

    char *infile;
    char *outfile;
    int append;
} Command;

// 백그라운드/포그라운드 작업 정보
typedef struct {
    int used;
    int id;
    int nproc;
    int background;

    pid_t pgid;
    pid_t pids[MAX_CMDS];

    // 0 = Running, 1 = Stopped, 2 = Done
    unsigned char state[MAX_CMDS];

    char *line;
} Job;

static Job jobs[MAX_JOBS];
static int next_job_id = 1;

static int interactive;
static pid_t shell_pgid;
static volatile sig_atomic_t child_event = 0;


// ============================================================
// 1. 시그널 처리
// ============================================================

static void on_sigchld(int sig) {
    (void)sig;
    child_event = 1;
}

static void set_handler(int sig, void (*fn)(int)) {
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = fn;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(sig, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

static void init_shell(void) {
    interactive = isatty(STDIN_FILENO);

    if (interactive) {
        // Shell이 포그라운드 프로세스 그룹이 될 때까지 대기
        while (tcgetpgrp(STDIN_FILENO) != getpgrp()) {
            kill(-getpgrp(), SIGTTIN);
        }

        // Shell 자신은 터미널 제어 시그널을 무시
        set_handler(SIGINT, SIG_IGN);
        set_handler(SIGQUIT, SIG_IGN);
        set_handler(SIGTSTP, SIG_IGN);
        set_handler(SIGTTIN, SIG_IGN);
        set_handler(SIGTTOU, SIG_IGN);

        shell_pgid = getpid();

        if (setpgid(0, shell_pgid) == -1 &&
            errno != EACCES &&
            errno != EPERM) {
            perror("setpgid shell");
            exit(1);
        }

        if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1) {
            perror("tcsetpgrp shell");
            exit(1);
        }
    }

    set_handler(SIGCHLD, on_sigchld);
}


// ============================================================
// 2. 명령어 파싱
// ============================================================

static int is_redirection(const char *s) {
    return strcmp(s, "<") == 0 ||
           strcmp(s, ">") == 0 ||
           strcmp(s, ">>") == 0;
}

// 성공: 명령어 개수 반환
// 실패: -1 반환
static int parse(
    char *tokens[],
    int ntok,
    Command commands[],
    int *background
) {
    memset(commands, 0, sizeof(Command) * MAX_CMDS);

    *background = 0;

    // 마지막에 &가 있으면 백그라운드 실행
    if (ntok > 0 &&
        strcmp(tokens[ntok - 1], "&") == 0) {
        *background = 1;
        ntok--;
    }

    if (ntok == 0) {
        return -1;
    }

    int count = 1;

    for (int i = 0; i < ntok; i++) {
        Command *cur = &commands[count - 1];

        // &는 맨 마지막에서만 허용
        if (strcmp(tokens[i], "&") == 0) {
            return -1;
        }

        // 파이프 분석
        if (strcmp(tokens[i], "|") == 0) {
            if (cur->argc == 0 || count >= MAX_CMDS) {
                return -1;
            }

            count++;
        }

        // 리다이렉션 분석
        else if (is_redirection(tokens[i])) {
            char *op = tokens[i];

            i++;

            if (i >= ntok ||
                is_redirection(tokens[i]) ||
                strcmp(tokens[i], "|") == 0 ||
                strcmp(tokens[i], "&") == 0) {
                return -1;
            }

            char *filename = tokens[i];

            if (strcmp(op, "<") == 0) {
                cur->infile = filename;
            }
            else {
                cur->outfile = filename;
                cur->append = (strcmp(op, ">>") == 0);
            }
        }

        // 일반 명령어 인자
        else {
            if (cur->argc >= MAX_TOKENS - 1) {
                return -1;
            }

            cur->argv[cur->argc++] = tokens[i];
            cur->argv[cur->argc] = NULL;
        }
    }

    if (commands[count - 1].argc == 0) {
        return -1;
    }

    return count;
}


// ============================================================
// 3. 파일 리다이렉션
// ============================================================

static int redirect_files(const Command *cmd) {

    // 입력 리다이렉션 <
    if (cmd->infile != NULL) {
        int fd = open(cmd->infile, O_RDONLY);

        if (fd == -1) {
            perror(cmd->infile);
            return -1;
        }

        if (dup2(fd, STDIN_FILENO) == -1) {
            perror("dup2 input");
            close(fd);
            return -1;
        }

        close(fd);
    }

    // 출력 리다이렉션 >, >>
    if (cmd->outfile != NULL) {
        int flags = O_WRONLY | O_CREAT;

        if (cmd->append) {
            flags |= O_APPEND;
        }
        else {
            flags |= O_TRUNC;
        }

        int fd = open(cmd->outfile, flags, 0644);

        if (fd == -1) {
            perror(cmd->outfile);
            return -1;
        }

        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2 output");
            close(fd);
            return -1;
        }

        close(fd);
    }

    return 0;
}


// ============================================================
// 4. 작업(Job) 관리
// ============================================================

static Job *new_job(
    const char *line,
    int nproc,
    int background
) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].used) {
            continue;
        }

        Job *job = &jobs[i];

        memset(job, 0, sizeof(*job));

        job->line = strdup(line);

        if (job->line == NULL) {
            perror("strdup");
            return NULL;
        }

        job->used = 1;
        job->id = next_job_id++;
        job->nproc = nproc;
        job->background = background;

        return job;
    }

    fprintf(stderr, "Too many jobs\n");
    return NULL;
}

static void delete_job(Job *job) {
    free(job->line);
    memset(job, 0, sizeof(*job));
}

static int job_done(const Job *job) {
    for (int i = 0; i < job->nproc; i++) {
        if (job->state[i] != 2) {
            return 0;
        }
    }

    return 1;
}

static int job_stopped(const Job *job) {
    int any_stopped = 0;

    for (int i = 0; i < job->nproc; i++) {
        if (job->state[i] == 0) {
            return 0;
        }

        if (job->state[i] == 1) {
            any_stopped = 1;
        }
    }

    return any_stopped;
}

static const char *job_status(const Job *job) {
    if (job_done(job)) {
        return "Done";
    }

    if (job_stopped(job)) {
        return "Stopped";
    }

    return "Running";
}

static Job *find_job(int id) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].used && jobs[i].id == id) {
            return &jobs[i];
        }
    }

    return NULL;
}

// waitpid()의 결과를 작업 테이블에 반영
static void mark_pid(pid_t pid, int status) {
    for (int k = 0; k < MAX_JOBS; k++) {
        Job *job = &jobs[k];

        if (!job->used) {
            continue;
        }

        for (int i = 0; i < job->nproc; i++) {
            if (job->pids[i] != pid) {
                continue;
            }

            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                job->state[i] = 2;
            }
            else if (WIFSTOPPED(status)) {
                job->state[i] = 1;
            }
            else if (WIFCONTINUED(status)) {
                job->state[i] = 0;
            }

            return;
        }
    }
}

// 종료되거나 상태가 변한 자식 회수
static void reap_background(void) {
    int status;
    pid_t pid;

    child_event = 0;

    while ((pid = waitpid(
                -1,
                &status,
                WNOHANG | WUNTRACED | WCONTINUED
            )) > 0) {
        mark_pid(pid, status);
    }

    for (int i = 0; i < MAX_JOBS; i++) {
        Job *job = &jobs[i];

        if (!job->used || !job->background) {
            continue;
        }

        if (job_done(job)) {
            printf("[%d] Done     %s\n",
                   job->id, job->line);

            delete_job(job);
        }
    }
}


// ============================================================
// 5. 포그라운드 작업 기다리기
// ============================================================

static void wait_foreground(Job *job) {
    int status;

    while (!job_done(job) && !job_stopped(job)) {
        pid_t pid = waitpid(
            -job->pgid,
            &status,
            WUNTRACED
        );

        if (pid > 0) {
            mark_pid(pid, status);
        }
        else if (pid == -1 && errno == EINTR) {
            continue;
        }
        else if (pid == -1) {
            if (errno != ECHILD) {
                perror("waitpid foreground");
            }

            break;
        }
    }

    // 터미널 제어권을 부모 Shell로 돌려줌
    if (interactive) {
        if (tcsetpgrp(STDIN_FILENO, shell_pgid) == -1) {
            perror("tcsetpgrp restore");
        }
    }

    if (job_done(job)) {
        delete_job(job);
    }
    else if (job_stopped(job)) {
        job->background = 1;

        printf("[%d] Stopped  %s\n",
               job->id, job->line);
    }
}


// ============================================================
// 6. 파이프라인 실행
// ============================================================

static void launch(
    Command commands[],
    int count,
    int background,
    const char *line
) {
    Job *job = new_job(line, count, background);

    if (job == NULL) {
        return;
    }

    int previous_read = -1;
    int started = 0;

    for (int i = 0; i < count; i++) {
        int next_pipe[2] = {-1, -1};

        // 마지막 명령어가 아니면 다음 파이프 생성
        if (i < count - 1) {
            if (pipe(next_pipe) == -1) {
                perror("pipe");
                goto fail;
            }
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");

            if (next_pipe[0] != -1) {
                close(next_pipe[0]);
                close(next_pipe[1]);
            }

            goto fail;
        }

        // ------------------------
        // 자식 프로세스
        // ------------------------
        if (pid == 0) {
            pid_t group = (i == 0)
                ? getpid()
                : job->pgid;

            if (setpgid(0, group) == -1) {
                perror("setpgid child");
                _exit(1);
            }

            // 자식은 기본 시그널 동작 복원
            set_handler(SIGINT, SIG_DFL);
            set_handler(SIGQUIT, SIG_DFL);
            set_handler(SIGTSTP, SIG_DFL);
            set_handler(SIGTTIN, SIG_DFL);
            set_handler(SIGTTOU, SIG_DFL);
            set_handler(SIGCHLD, SIG_DFL);

            // 이전 파이프에서 입력
            if (previous_read != -1) {
                if (dup2(previous_read, STDIN_FILENO) == -1) {
                    perror("dup2 pipe input");
                    _exit(1);
                }
            }

            // 다음 파이프로 출력
            if (next_pipe[1] != -1) {
                if (dup2(next_pipe[1], STDOUT_FILENO) == -1) {
                    perror("dup2 pipe output");
                    _exit(1);
                }
            }

            // 원본 파이프 fd 닫기
            if (previous_read != -1) {
                close(previous_read);
            }

            if (next_pipe[0] != -1) {
                close(next_pipe[0]);
            }

            if (next_pipe[1] != -1) {
                close(next_pipe[1]);
            }

            // <, >, >> 처리
            if (redirect_files(&commands[i]) == -1) {
                _exit(1);
            }

            // 프로그램 실행
            execvp(commands[i].argv[0],
                   commands[i].argv);

            perror(commands[i].argv[0]);
            _exit(127);
        }

        // ------------------------
        // 부모 프로세스
        // ------------------------
        if (i == 0) {
            job->pgid = pid;
        }

        if (setpgid(pid, job->pgid) == -1 &&
            errno != EACCES &&
            errno != ESRCH) {
            perror("setpgid parent");
        }

        job->pids[i] = pid;
        started++;

        if (previous_read != -1) {
            close(previous_read);
        }

        if (next_pipe[1] != -1) {
            close(next_pipe[1]);
        }

        previous_read = next_pipe[0];
    }

    if (previous_read != -1) {
        close(previous_read);
    }

    if (background) {
        // 백그라운드: 기다리지 않음
        printf("[%d] %ld\n",
               job->id, (long)job->pgid);
    }
    else {
        // 포그라운드: 터미널 제어권 전달
        if (interactive) {
            if (tcsetpgrp(STDIN_FILENO, job->pgid) == -1) {
                perror("tcsetpgrp foreground");
            }
        }

        wait_foreground(job);
    }

    return;

fail:
    if (previous_read != -1) {
        close(previous_read);
    }

    job->nproc = started;

    if (started == 0) {
        delete_job(job);
        return;
    }

    // 일부 자식 생성에 실패한 경우 정리
    kill(-job->pgid, SIGTERM);

    for (int i = 0; i < started; i++) {
        int status;

        while (waitpid(job->pids[i], &status, 0) == -1 &&
               errno == EINTR) {
        }
    }

    delete_job(job);
}


// ============================================================
// 7. fg / bg 실행
// ============================================================

static void resume_job(Job *job, int foreground) {
    if (job == NULL) {
        fprintf(stderr, "No such job\n");
        return;
    }

    // 정지 상태 해제
    for (int i = 0; i < job->nproc; i++) {
        if (job->state[i] == 1) {
            job->state[i] = 0;
        }
    }

    job->background = !foreground;

    if (foreground && interactive) {
        if (tcsetpgrp(STDIN_FILENO, job->pgid) == -1) {
            perror("tcsetpgrp fg");
        }
    }

    // 프로세스 그룹 전체에 계속 실행 시그널
    if (kill(-job->pgid, SIGCONT) == -1) {
        perror("SIGCONT");
    }

    if (foreground) {
        wait_foreground(job);
    }
    else {
        printf("[%d] Running  %s\n",
               job->id, job->line);
    }
}


// ============================================================
// 8. main: Shell 메인 루프
// ============================================================

int main(void) {
    init_shell();

    char *line;

    while ((line = readline("$ ")) != NULL) {
        reap_background();

        // 원본 명령어는 작업 목록 표시용으로 보관
        char *original = strdup(line);

        if (original == NULL) {
            perror("strdup");
            free(line);
            break;
        }

        char *tokens[MAX_TOKENS];
        int n = 0;
        int overflow = 0;

        // 공백 기준 토큰 분리
        char *save = NULL;

        for (char *p = strtok_r(line, " \t", &save);
             p != NULL;
             p = strtok_r(NULL, " \t", &save)) {

            if (n == MAX_TOKENS - 1) {
                overflow = 1;
                break;
            }

            tokens[n++] = p;
        }

        tokens[n] = NULL;

        if (overflow) {
            fprintf(stderr, "Too many tokens\n");
            goto next;
        }

        if (n == 0) {
            goto next;
        }

        // 명령어 분석
        Command commands[MAX_CMDS];
        int background;

        int count = parse(
            tokens,
            n,
            commands,
            &background
        );

        if (count < 0) {
            fprintf(stderr,
                    "Invalid syntax (spaces required around operators)\n");
            goto next;
        }

        char *name = commands[0].argv[0];

        // ------------------------
        // exit
        // ------------------------
        if (count == 1 && strcmp(name, "exit") == 0) {
            free(original);
            free(line);
            break;
        }

        // ------------------------
        // cd
        // ------------------------
        if (count == 1 && strcmp(name, "cd") == 0) {
            char *path = commands[0].argv[1];

            if (path == NULL) {
                path = getenv("HOME");
            }

            if (path == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
            }
            else if (chdir(path) == -1) {
                perror("cd");
            }

            goto next;
        }

        // ------------------------
        // jobs
        // ------------------------
        if (count == 1 && strcmp(name, "jobs") == 0) {
            reap_background();

            for (int i = 0; i < MAX_JOBS; i++) {
                if (jobs[i].used) {
                    printf("[%d] %-8s %s\n",
                           jobs[i].id,
                           job_status(&jobs[i]),
                           jobs[i].line);
                }
            }

            goto next;
        }

        // ------------------------
        // fg / bg
        // ------------------------
        if (count == 1 &&
            (strcmp(name, "fg") == 0 ||
             strcmp(name, "bg") == 0)) {

            if (commands[0].argc != 2) {
                fprintf(stderr,
                        "Usage: %s JOB_ID\n", name);
            }
            else {
                char *end;
                errno = 0;

                long id = strtol(
                    commands[0].argv[1],
                    &end,
                    10
                );

                if (errno != 0 ||
                    *end != '\0' ||
                    id <= 0 ||
                    id > 2147483647L) {

                    fprintf(stderr, "Invalid job ID\n");
                }
                else {
                    Job *job = find_job((int)id);

                    int foreground =
                        (strcmp(name, "fg") == 0);

                    resume_job(job, foreground);
                }
            }

            goto next;
        }

        // 파이프 안의 내장 명령어는 미지원
        int unsupported = 0;

        for (int i = 0; i < count; i++) {
            const char *s = commands[i].argv[0];

            if (strcmp(s, "cd") == 0 ||
                strcmp(s, "exit") == 0 ||
                strcmp(s, "jobs") == 0 ||
                strcmp(s, "fg") == 0 ||
                strcmp(s, "bg") == 0) {

                fprintf(stderr,
                        "Builtin in pipeline is not supported\n");

                unsupported = 1;
                break;
            }
        }

        // 외부 명령어 실행
        if (!unsupported) {
            launch(
                commands,
                count,
                background,
                original
            );
        }

next:
        free(original);
        free(line);
    }

    // 입문판: Shell 종료 시 남은 백그라운드 작업은
    // 강제로 종료하지 않음
    return 0;
}
