#include <stdio.h>              // printf
#include <stdlib.h>             // free
#include <string.h>             // strtok_r, sprintf
#include <sys/wait.h>           // waitpid
#include <unistd.h>             // execv
#include <editline/readline.h>  // readline
#include <editline/history.h>   // add_history
#include <errno.h>
#define MAX_ARGS 100

enum { 
  READLINE_EXIT=-1,
  SUCCESS, 
  PARSE_ARGS_ERROR,
  EVALUATE_ERROR
}; // you can edit this or not use it at all.

// helper functions 
int get_input(char** cmd); // saves input to cmd, returns READLINE_EXIT on EOT (ctrl+D), SUCCESS on success
int parse_args(char* cmd, char* args[]); // parses `cmd` into `args`, returns PARSE_ARGS_ERROR on failure, SUCCESS on success

/** WRITE YOUR CODE BELOW **/
// You can add more functions here if needed like `handle_builtin_commands` (i.e., exit) or `execute_command` (i.e., execvp)

int execute_external(char *args[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return EVALUATE_ERROR;
    }

    // 자식 프로세스
    if (pid == 0) {
        execvp(args[0], args);

        int saved_errno = errno;

        // PATH에서 실행하지 못했다면 현재 디렉터리 시도
        if (strchr(args[0], '/') == NULL) {
            char *path = malloc(strlen(args[0]) + 3);

            if (path == NULL) {
                perror("malloc");
                _exit(1);
            }

            sprintf(path, "./%s", args[0]);
            execvp(path, args);

            if (errno != ENOENT && errno != ENOTDIR) {
                saved_errno = errno;
            }

            free(path);
        }

        // 실행 실패
        if (saved_errno == ENOENT ||
            saved_errno == ENOTDIR) {
            fprintf(stderr, "%s: command not found\n",
                    args[0]);
            _exit(127);
        }

        errno = saved_errno;
        perror(args[0]);
        _exit(126);
    }

    // 부모 프로세스
    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid");
        return EVALUATE_ERROR;
    }

    return SUCCESS;
}


int evaluate(char* args[]) {

	if (args[0] == NULL)
        return SUCCESS;
    // 1. exit 명령인지 확인
    if (strcmp(args[0], "exit") == 0) {
	
	if (args[1] == NULL)
            exit(EXIT_SUCCESS);
        // 2. 인자가 너무 많은지 확인
        if (args[2] != NULL) {
            fprintf(stderr, "exit: too many arguments\n");
            return EVALUATE_ERROR;
        }

        // 3. 인자가 없으면 성공 종료
        if (args[1] == NULL) {
            exit(EXIT_SUCCESS);
        }

        // 4. 문자열을 정수로 변환
        char* end;
        errno = 0;

        long status = strtol(args[1], &end, 10);

        // 5. 유효한 정수인지 확인
        if (end == args[1] || *end != '\0') {
            fprintf(stderr,
                    "exit: %s: invalid integer\n",
                    args[1]);

            return EVALUATE_ERROR;
        }

        // 6. 종료 상태 범위 확인
        if (errno == ERANGE || status < 0 || status > 255) {
            exit(255);
        }

        // 7. Shell 종료
        exit((int)status);
    }

    return execute_external(args);
}

int main() {
  int result = SUCCESS;

  do {
    char* cmd = NULL;

    result = get_input(&cmd);

    if (cmd == NULL)
      goto loop_end;
    
    char* args[MAX_ARGS];
  
    if (((result = parse_args(cmd, args)) != SUCCESS))
      goto loop_end;

    result = evaluate(args);
  loop_end:
    free(cmd);
  } while (result != READLINE_EXIT);
}

// helper functions

// You have to free the memory allocated by readline
int get_input(char** cmd) {
  if ((*cmd = readline("$ ")) == NULL) {
    return READLINE_EXIT;
  }
  if (**cmd == '\0') {
    free(*cmd);
    *cmd = NULL;
  } else add_history(*cmd);
  return SUCCESS;
}

int parse_args(char* cmd, char* args[]) {
  char* save_ptr;
  char* ptr = strtok_r(cmd, " ", &save_ptr);
  int i = 0;

  while (ptr != NULL) {
    if (i >= MAX_ARGS - 1) {
      fprintf(stderr, "Error: Too many arguments\n");
      return PARSE_ARGS_ERROR;
    }
    args[i++] = ptr;
    ptr = strtok_r(NULL, " ", &save_ptr);
  }

  args[i] = NULL;
  return SUCCESS;
}
