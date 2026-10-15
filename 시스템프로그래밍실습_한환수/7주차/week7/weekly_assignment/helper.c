#include <fcntl.h>
#include <editline/readline.h>
// #include <editline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include "helper.h"

// You don't need to change these functions
// These are helper functions

bool is_special_char(char c) {
  return c == '<' || c == '>' || c == '|' || c == '\'' || c == '"' || c == '\0' || isspace(c) || c == '&';
}

bool is_background(Token token) {
  return token.type == TOKEN_BACKGROUND;
}


bool is_pipe(Token token) {
  return token.type == TOKEN_PIPE;
}

bool is_redirection(Token token) {
  return token.type == TOKEN_REDIRECT_IN || token.type == TOKEN_REDIRECT_OUT ||
         token.type == TOKEN_REDIRECT_APPEND;
}


void lex(const char* cmd, Token* tokens) {
  int token_i = 0;

  for (const char* curr_char = cmd; *curr_char != '\0'; curr_char++) {
    if (isspace(*curr_char) && *curr_char != '\n')
      continue;    

    switch (*curr_char) {
      case '#':
        while (*curr_char != '\n' && *curr_char != '\0' && *curr_char != ';') curr_char++;
        curr_char--;
        break;
      case ';':
        tokens[token_i++] = (Token){.type = TOKEN_SEP, .data = ";"};
        break;
      case '\n':
        tokens[token_i++] = (Token){.type = TOKEN_SEP, .data = "\n"};
        break;
      case '|':
        tokens[token_i++] = (Token){.type = TOKEN_PIPE, .data = "|"};
        break;
      case '<':
        tokens[token_i++] = (Token){.type = TOKEN_REDIRECT_IN, .data = "<"};
        break;
      case '>':
        if (*(curr_char + 1) == '>') {
          tokens[token_i++] =
              (Token){.type = TOKEN_REDIRECT_APPEND, .data = ">>"};
          curr_char++;
        } else {
          tokens[token_i++] = (Token){.type = TOKEN_REDIRECT_OUT, .data = ">"};
        }
        break;
      case '&':
        tokens[token_i++] = (Token){.type = TOKEN_BACKGROUND, .data = "&"};
        break;
      case '\'':
      case '\"': {
        // not required for PA2, it just makes checking commands much easier
        // since some commands almost require quotes (i.e awk)
        char quote = *curr_char;
        const char* start = ++curr_char;
        while (*curr_char != quote && *curr_char != '\0') {
          curr_char++;
        }
        tokens[token_i++] = (Token){
            .type = TOKEN_ARGUMENT,
            .data = strndup(start, curr_char - start),
        };
      } break;
      default: {
        const char* command_start = curr_char;

        TokenType type = (token_i == 0 || is_pipe(tokens[token_i - 1]))
                             ? TOKEN_COMMAND
                             : TOKEN_ARGUMENT;

        while (!is_special_char(*curr_char)) {
          curr_char++;
        }

        tokens[token_i++] =
            (Token){.type = type,
                    .data = strndup(command_start, curr_char - command_start)};
        curr_char--;
        break;
      }
    }
  }

  tokens[token_i] = (Token){.type = TOKEN_END, .data = NULL};  
}





void free_tokens(Token* tokens) {
  for (int i = 0; tokens[i].type != TOKEN_END; i++) {
    if (tokens[i].type == TOKEN_COMMAND || tokens[i].type == TOKEN_ARGUMENT) {
      free(tokens[i].data);
    }
  }
}

// int64_t run_command(char* command, char* arg[], bool is_background) {
int64_t do_exec(Command* command, bool is_in_background) {
  if (strncmp(command->args[0], "exit", 5) == 0) {
    if (command->args[1] != NULL) {
      if (command->args[2] != NULL) {
        fprintf(stderr, "exit: too many arguments\n");
        return 2;
      }

      char* ptr;
      int64_t exit_code = strtoll(command->args[1], NULL, 10);
      if (*ptr != '\0') {
        fprintf(stderr, "exit: %s: invalid integer\n", command->args[1]); 
        return 2;
      }

      // In PA2, don't do this. You have to clean up the shell before closing.
      // i.e., the shell cannot exit when there are running jobs
      exit(exit_code);
    }
    // In PA2, don't do this. You have to clean up the shell before closing.      
    exit(0);
  }

  pid_t pid;
  int32_t child_status;

  switch (pid = fork()) {
    case -1:
      perror("fork");
      exit(1);
      break;
    case 0:
      execvp(command->args[0], command->args);
      perror("execvp");
      exit(1);
      break;
    default:
      if (is_in_background) {
        return 0;
      }

      if (waitpid(pid, &child_status, 0) == -1) {
        perror("waitpid");
        exit(1);
      }

      if (WIFEXITED(child_status)) {
        return WEXITSTATUS(child_status);
      } else if (WIFSIGNALED(child_status)) {
        return -WTERMSIG(child_status);
      }

      break;
  }
}

int64_t evaluate(const char* cmd) {
  Token tokens[MAX_TOKENS];
  Pipeline pipeline;
  Job job;
  lex(cmd, tokens);
  parse(tokens, &pipeline);
  interpret(&pipeline, &job);
  free_tokens(tokens);
  return 0;
}
