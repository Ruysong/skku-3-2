#include <errno.h>
#include <fcntl.h>
#include <editline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "helper.h"

/* Open a file and connect it to stdin (0) or stdout (1). */
static int redirect_fd(const char *path, int flags, int target_fd) {
    int fd = open(path, flags, 0644);
    if (fd == -1) {
        perror(path);
        return -1;
    }
    if (dup2(fd, target_fd) == -1) {
        perror("dup2");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

/* The Week 7 exercise allows at most one pipe OR one redirection. */
int parse(Token *tokens, Pipeline *pipeline) {
    memset(pipeline, 0, sizeof(*pipeline));

    size_t current = 0;
    int operators = 0;

    for (size_t i = 0; tokens[i].type != TOKEN_END; ++i) {
        Token token = tokens[i];
        Command *cmd = &pipeline->commands[current];

        if (token.type == TOKEN_PIPE) {
            if (++operators > 1 || current != 0 || cmd->num_args == 0) {
                fprintf(stderr, "unsupported or invalid pipe\n");
                goto invalid;
            }
            current = 1;
            pipeline->num_commands = 2;
        } else if (is_redirection(token)) {
            if (++operators > 1 || current != 0 || cmd->num_args == 0 ||
                tokens[i + 1].type == TOKEN_END ||
                (tokens[i + 1].type != TOKEN_ARGUMENT &&
                 tokens[i + 1].type != TOKEN_COMMAND)) {
                fprintf(stderr, "invalid redirection\n");
                goto invalid;
            }

            char *filename = tokens[++i].data;
            if (token.type == TOKEN_REDIRECT_IN) {
                cmd->stdin = filename;
            } else {
                cmd->stdout = filename;
                cmd->append_stdout = (token.type == TOKEN_REDIRECT_APPEND);
            }
        } else if (token.type == TOKEN_COMMAND ||
                   token.type == TOKEN_ARGUMENT) {
            if (cmd->num_args >= MAX_TOKENS - 1) {
                fprintf(stderr, "too many arguments\n");
                goto invalid;
            }
            cmd->args[cmd->num_args++] = token.data;
            cmd->args[cmd->num_args] = NULL;
            if (pipeline->num_commands == 0)
                pipeline->num_commands = 1;
        } else {
            /* Background commands and separators are outside this exercise. */
            fprintf(stderr, "unsupported operator\n");
            goto invalid;
        }
    }

    if (pipeline->num_commands != 0 &&
        pipeline->commands[pipeline->num_commands - 1].num_args == 0) {
        fprintf(stderr, "missing command\n");
        goto invalid;
    }
    return 0;

invalid:
    pipeline->num_commands = 0; /* evaluate() still calls interpret(). */
    return -1;
}

/* waitpid() may be interrupted by a signal; retry in that case. */
static int wait_for_child(pid_t pid) {
    int status;
    while (waitpid(pid, &status, 0) == -1) {
        if (errno == EINTR)
            continue;
        perror("waitpid");
        return -1;
    }
    return 0;
}

int interpret(Pipeline *pipeline, Job *job) {
    job->num_processes = 0;

    if (pipeline->num_commands == 0)
        return 0;

    if (pipeline->num_commands == 1) {
        Command *cmd = &pipeline->commands[0];
        int stdin_copy = -1;
        int stdout_copy = -1;
        int result = 0;

        /* do_exec() forks internally, so temporarily redirect this shell's
           descriptors and restore them when do_exec() finishes. */
        if (cmd->stdin != NULL) {
            stdin_copy = dup(STDIN_FILENO);
            if (stdin_copy == -1) {
                perror("dup stdin");
                return -1;
            }
            if (redirect_fd(cmd->stdin, O_RDONLY, STDIN_FILENO) == -1) {
                result = -1;
                goto restore;
            }
        }

        if (cmd->stdout != NULL) {
            stdout_copy = dup(STDOUT_FILENO);
            if (stdout_copy == -1) {
                perror("dup stdout");
                result = -1;
                goto restore;
            }
            int flags = O_WRONLY | O_CREAT |
                        (cmd->append_stdout ? O_APPEND : O_TRUNC);
            if (redirect_fd(cmd->stdout, flags, STDOUT_FILENO) == -1) {
                result = -1;
                goto restore;
            }
        }

        fflush(stdout);
        result = (int)do_exec(cmd, false);

restore:
        if (stdin_copy != -1) {
            if (dup2(stdin_copy, STDIN_FILENO) == -1)
                perror("restore stdin");
            close(stdin_copy);
        }
        if (stdout_copy != -1) {
            if (dup2(stdout_copy, STDOUT_FILENO) == -1)
                perror("restore stdout");
            close(stdout_copy);
        }
        return result;
    }

    if (pipeline->num_commands == 2) {
        Command *left = &pipeline->commands[0];
        Command *right = &pipeline->commands[1];
        int pipefd[2];
        pid_t pids[2];

        if (pipe(pipefd) == -1) {
            perror("pipe");
            return -1;
        }

        pids[0] = fork();
        if (pids[0] == -1) {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            return -1;
        }
        if (pids[0] == 0) {                    /* first command: output */
            close(pipefd[0]);
            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                perror("dup2 stdout");
                _exit(127);
            }
            close(pipefd[1]);
            execvp(left->args[0], left->args);
            perror("execvp");
            _exit(127);
        }

        pids[1] = fork();
        if (pids[1] == -1) {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            wait_for_child(pids[0]);
            return -1;
        }
        if (pids[1] == 0) {                    /* second command: input */
            close(pipefd[1]);
            if (dup2(pipefd[0], STDIN_FILENO) == -1) {
                perror("dup2 stdin");
                _exit(127);
            }
            close(pipefd[0]);
            execvp(right->args[0], right->args);
            perror("execvp");
            _exit(127);
        }

        /* Parent must close BOTH ends so readers can see EOF. */
        close(pipefd[0]);
        close(pipefd[1]);
        job->num_processes = 2;
        job->processes[0].pid = pids[0];
        job->processes[1].pid = pids[1];
        int left_result = wait_for_child(pids[0]);
        int right_result = wait_for_child(pids[1]);
        return (left_result == -1 || right_result == -1) ? -1 : 0;
    }

    return -1;
}

int main(void) {
    char *cmd;
    while ((cmd = readline("$ ")) != NULL) {
        if (*cmd != '\0')
            add_history(cmd);
        evaluate(cmd);
        free(cmd);
    }
    return 0;
}
