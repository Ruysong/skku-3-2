#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	pid_t pid;
	int shell_fd = STDIN_FILENO;
	switch (pid = fork()) {
		case -1:
			exit(1);
		case 0: {
			setpgid(0, 0);
			tcsetpgrp(shell_fd, getpgid(0));
			puts("Child process running in foreground! Ctrl+D to exit!");
			execlp("cat", "cat", NULL);
			exit(0);
			break;
		}
		default: {
			struct sigaction sa;
			memset(&sa, 0, sizeof(sa));
			sa.sa_handler = SIG_IGN;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = 0;
			sigaction(SIGTTOU, &sa, NULL);



			pid_t child_pid = pid;
			setpgid(child_pid, child_pid);
			pid_t shell_pgid = getpgid(0);
			// Make child run in foreground and stop it
			tcsetpgrp(shell_fd, child_pid);
			kill(child_pid, SIGTSTP);
			waitpid(child_pid, NULL, WUNTRACED);
			tcsetpgrp(shell_fd, shell_pgid);
			puts("Parent reclaimed terminal!");
			// Continue the child process
			tcsetpgrp(shell_fd, child_pid);
			kill(child_pid, SIGCONT);
			waitpid(child_pid, NULL, WUNTRACED);
			// Reclaim terminal again once child is done
			tcsetpgrp(shell_fd, shell_pgid);
			puts("Parent reclaimed terminal again!");
		}
	}
	exit(0);
	return 0;
}
