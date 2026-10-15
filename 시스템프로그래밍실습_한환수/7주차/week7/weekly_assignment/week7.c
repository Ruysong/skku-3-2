#include <fcntl.h>
#include <editline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "helper.h"
// Add other header files you need here

// if you need to add helper functions, structs, enums, you can add them here
// end

// TODO: implement this function
int parse(Token* tokens, Pipeline* pipeline) {
  // For this exercise (not PA2), there are only one out of three cases at a
  // given time
  // 1. Simple command (+ args)    (operator: TOKEN_END for convenience)
  // 2. Pipe        ( a | b)       (operator: TOKEN_PIPE)
  // 3. Redirection (a>b, a>>b, a<b)       (operator: TOKEN_REDIRECT_*)
  // In the actual PA2, you have to consider multiple pipes and redirections

  // But for this exercise, you can just fill the pipeline struct with the values you need for interpret
  pipeline->num_commands = 0;
  pipeline->is_in_background = false;
  size_t pipeline_i = 0;

  pipeline->commands[0] = (Command){ .args = {NULL}, .stdin = NULL, .stdout = NULL, .num_args = 0, .append_stdout = false}; // note num_commands is not yet set to 1

  // TODO: set up pipeline
  // remember that you only need at most 2 commands for this exercise, so commands[1] should be the last one and num_commands should be at most 2

  return 0;
}

// TODO: implement this function
int interpret(Pipeline* pipeline, Job* job){
  job->num_processes = 0;

  switch (pipeline->num_commands) {
    case 0:
      break;
    case 1:
      Command* command = &pipeline->commands[0];
      int stdin_copy = -2;
      int stdout_copy = -2;

      if (command->stdin != NULL) {
        // TODO: add code here
      }

      if (command->stdout != NULL) {
        // TODO: add code here
      }

      do_exec(command, false);
      
      if (stdin_copy != -2) {
        // TODO: add code here
      }

      if (stdout_copy != -2) {
        // TODO: add code here
      }

      break;
    case 2:
      Command* command1 = &pipeline->commands[0];
      Command* command2 = &pipeline->commands[1];

      // No need to consider redirection for this exercise if there's a pipe (only PA2)
      // For this exercise, you just need to deal with pipe

      pid_t pids[MAX_PROCESSES_IN_A_JOB]; 
      int pipefd[MAX_PROCESSES_IN_A_JOB];

      // TODO: add code here
     
      if ((pids[0] = fork()) == 0) {
        // TODO: add code here
      }

      if ((pids[1] = fork()) == 0) {
        // TODO: add code here
      }

      // TODO: add code here to close pipe and wait for child processes
      break;
    default:
      break;
  }  
  return 0;
}

int main() {
  size_t size;
  char* cmd;
 
  while (1) {
    char* cmd;

    if ((cmd = readline("$ ")) == NULL)
      break;

    add_history(cmd);
    evaluate(cmd);
    free(cmd);
  }

  return 0;
}