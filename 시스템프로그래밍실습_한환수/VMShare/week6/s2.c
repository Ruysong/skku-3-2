#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define N 10


void int_handler(int sig) {
    if (sig == SIGINT) {
        printf("Process %d received signal %d\n", getpid(), sig);
        _exit(22);
    } else
        printf("This function does not handle signal %d\n", sig);
}

int main(void) {
    pid_t pid[N];
    int i, child_status;

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = int_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    for (i = 0; i < N; i++)
        if ((pid[i] = fork()) == 0)
            while (1); /* Child infinite loop */
    
    /* Parent terminates the child processes */
    for (i = 0; i < N; i++) {
        printf("Killing process %d\n", pid[i]);
        kill(pid[i], SIGINT);
    }

    /* Parent reaps terminated children */
    for (i = 0; i < N; i++) {
        pid_t wpid = wait(&child_status);
        if (WIFEXITED(child_status))
        printf("Child %d terminated with exit status %d\n", wpid,
                WEXITSTATUS(child_status));
        else
        printf("Child %d terminated abnormally\n", wpid);
    }
    return 0;
}

