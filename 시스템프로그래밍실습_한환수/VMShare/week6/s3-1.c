#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

enum { N = 10 };
volatile sig_atomic_t ccount = 0;
typedef struct {
  int signal;
  pid_t pid;
} process_info;

process_info process_infos[N];


void handler(int sig) {
  pid_t id = wait(NULL);
  process_infos[ccount++] = (process_info) { .signal = sig, .pid = id };
}

int main(void) {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART; // restarts certain system calls if interrupted
  // sa.sa_flags = 0; // will not restart
  sigaction(SIGCHLD, &sa, NULL);

  for (size_t i = 0; i < N; i++) 
    if (fork() == 0)
      _exit(0); /* child */
  while (ccount < N) pause();
  for (size_t i = 0; i < N; i++)
    printf("Received ‘%s’ signal from pid %d\n", 
	          strsignal(process_infos[i].signal), 
	process_infos[i].pid);
  return 0;
}
