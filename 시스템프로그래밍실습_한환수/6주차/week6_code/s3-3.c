#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

enum { N_CHILDREN = 10, BUF_SIZE = 100};

volatile sig_atomic_t sigchld_received = 0;

typedef enum {
  TERMINATED,
  STOPPED,
  CONTINUED,
  OTHER
} process_state;

const char* strstate(process_state state) {
  switch (state) {
    case TERMINATED:
      return "terminated";
    case STOPPED:
      return "stopped";
    case CONTINUED:
      return "continued";
    default:
      return "done something else";
  }
}

typedef struct {
  process_state state;
  pid_t pid;
} process_info;


void handler(int sig) {
  sigchld_received = 1;
}

void reap_children(process_info process_infos[BUF_SIZE], size_t* event_i, size_t* terminated_count) {
  pid_t id;
  int status;

  while ((id = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) { 
    if (*event_i >= BUF_SIZE) {
      fprintf(stderr, "Too many children, ignoring pid %d\n", id);
      break;
    }
    if (WIFEXITED(status) || WIFSIGNALED(status)) {
      process_infos[(*event_i)++] = (process_info) { .state = TERMINATED, .pid = id };
      (*terminated_count)++;
    } else if (WIFSTOPPED(status)) {
      process_infos[(*event_i)++] = (process_info) { .state = STOPPED, .pid = id };
      kill(id, SIGCONT);
    } else if (WIFCONTINUED(status)) {
      process_infos[(*event_i)++] = (process_info) { .state = CONTINUED, .pid = id };
    } else {
      process_infos[(*event_i)++] = (process_info) { .state = OTHER, .pid = id };
    }
  }
}

int main(void) {
  process_info process_infos[BUF_SIZE];
  size_t event_i = 0;
  size_t terminated_count = 0;

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGCHLD, &sa, NULL);

  for (size_t i = 0; i < N_CHILDREN; i++) 
	/* child */
    if (fork() == 0) {
	  	raise(SIGSTOP);
	    _exit(0); 
  	}

  while (terminated_count < N_CHILDREN) {
    if (!sigchld_received) pause();
    sigchld_received = 0;
    reap_children(process_infos, &event_i, &terminated_count);   
  }

  for (size_t i = 0; i < event_i; i++)
    printf("Process pid %d has %s.\n", process_infos[i].pid, strstate(process_infos[i].state));
  return 0;
}
