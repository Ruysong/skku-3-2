#ifndef HELPER
#define HELPER

#include <stdint.h>

// includes boolean header if not C23
#if defined __STDC_VERSION__ 
#if __STDC_VERSION__ < 202311L
#include <stdbool.h>
#endif
#endif

#define MAX_TOKENS 1000

#define MAX_JOBS 8192

// For PA2
// #define MAX_PROCESSES_IN_A_JOB 2063244
// For this week's exercise,
#define MAX_PROCESSES_IN_A_JOB 2

typedef enum {
  TOKEN_COMMAND,
  TOKEN_ARGUMENT,
  TOKEN_PIPE,             // |
  TOKEN_REDIRECT_IN,      // <
  TOKEN_REDIRECT_OUT,     // >
  TOKEN_REDIRECT_APPEND,  // >>
  TOKEN_BACKGROUND,       // &, not needed for this exercise
  TOKEN_SEP,              // ; or \n, not needed for this exercise
  TOKEN_END,
} TokenType;

typedef struct token {
  TokenType type;
  char* data;
} Token;

typedef struct {
  char* args[MAX_TOKENS];	
	char* stdin;
	char* stdout;
	size_t num_args;
	bool append_stdout;
} Command;

typedef struct {
	pid_t pid;
	int32_t status;
	bool is_running;
	Command* command;
} Process;

typedef enum {
	JOB_FOREGROUND,
	JOB_BACKGROUND,
	JOB_STOPPED
} JobState;

typedef struct {
	JobState state;
	uint64_t num_processes;
	pid_t pgid;
	Process processes[MAX_PROCESSES_IN_A_JOB];
} Job;

typedef struct {
	bool is_in_background;
	Command commands[MAX_PROCESSES_IN_A_JOB];
	uint64_t num_commands;
} Pipeline;


// These have been implemented already
bool is_background(Token token);
bool is_special_char(char c);
bool is_pipe(Token token);
bool is_redirection(Token token);
void lex(const char* cmd, Token* tokens);
void free_tokens(Token* tokens);
// int64_t run_command(char* command, char* arg[], bool is_background);
int64_t do_exec(Command* command, bool is_in_background);
int64_t evaluate(const char* cmd);

// What you have to implement
int parse(Token* tokens, Pipeline* pipeline);
int interpret(Pipeline* pipeline, Job* job);
#endif