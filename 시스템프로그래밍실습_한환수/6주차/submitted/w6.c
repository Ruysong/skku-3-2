#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

volatile sig_atomic_t parent_pending_acks = 0; // int
volatile sig_atomic_t parent_pending_sigalrm = 0; // bool
volatile sig_atomic_t child_pending_signals = 0; // int
volatile sig_atomic_t child_pending_termination = 0; // bool

pid_t other_process_pid;

void set_signal(struct sigaction action, int signal, void (*handler)(int)) {
    // TODO: set up action
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(signal, &action, NULL);
}

void parent_ack_handler(int sig) {
    // TODO: flag ack
    (void)sig;
    parent_pending_acks++;
}

void parent_alarm_handler(int sig) {
    // TODO: flag sigalrm
    (void)sig;
    parent_pending_sigalrm = 1;
}

void child_signal_handler(int sig) {
    // TODO: flag signal
    (void)sig;
    child_pending_signals++;
}


void child_sigint_handler(int sig) {
    // TODO: flag sigint
       (void)sig;
    child_pending_termination = 1;
}


void parent_handle_pending_signals(long* num_acks_received, long num_signal_to_send) {
    // TODO: Handle pending SIGUSR1 (ack) and SIGALRM signals
    // When sending SIGINT, remember to exit the sender process as well!
    // When handling pending SIGALRM, remember to call `alarm(1)` again for the next alarm!
    // You have to increment num_acks_received!
	    while (parent_pending_acks > 0) {
        parent_pending_acks--;
        (*num_acks_received)++;

        if (*num_acks_received == num_signal_to_send) {
            kill(other_process_pid, SIGINT);
            printf("all signals have been sent!\n");
            exit(EXIT_SUCCESS);
        }
    }
	    if (parent_pending_sigalrm) {
        parent_pending_sigalrm = 0;

        long remaining = num_signal_to_send - *num_acks_received;

        printf("sender: total remaining signal(s): %ld\n", remaining);

        for (long i = 0; i < remaining; i++) {
            kill(other_process_pid, SIGUSR1);
        }

        alarm(1);
    }


}


void child_handle_pending_signals(long* num_signal_received) {
    // TODO: Handle pending SIGUSR1 (signal) and SIGINT signals
    // You have to increment num_signal_received!
        while (child_pending_signals > 0) {
        child_pending_signals--;
        (*num_signal_received)++;

        printf("receiver: received signal #%ld and sending ack\n",
               *num_signal_received);

        kill(other_process_pid, SIGUSR1);
    }

    	if (child_pending_termination) {
        child_pending_termination = 0;

        printf("receiver: received %ld signals\n",
               *num_signal_received);

        exit(EXIT_SUCCESS);
    }
}


int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <number of signals to send>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  long num_signal_to_send = strtol(argv[1], NULL, 10);

  if (num_signal_to_send <= 0) {
    fprintf(stderr, "Invalid number of signals to send\n");
    exit(EXIT_FAILURE);
  }

  printf("number of signals to send: %ld\n", num_signal_to_send);

  struct sigaction action;
  memset(&action, 0, sizeof(action));

  switch (other_process_pid = fork()) {
    case -1:
      perror("fork failed");
      exit(EXIT_FAILURE);
    case 0: // Child process (receiver)
      other_process_pid = getppid(); // Note: PID is now parent process (sender)!

      long num_signal_received = 0;
      /* TODO: set up handlers */

      set_signal(action, SIGUSR1, child_signal_handler);
      set_signal(action, SIGINT, child_sigint_handler);
      // set up child_signal_handler signal handler (i.e. SIGUSR1)
      // set up terminate_handler signal handler (i.e. SIGINT)
			
      while (1) {
        pause();
        child_handle_pending_signals(&num_signal_received);
      }
      break;
    default: // Parent process (sender)
      // other_process_pid = child id
      long num_acks_received = 0;

      /* TODO: set up handlers */
      // set up parent_ack_handler signal handler (i.e. SIGUSR1)      
      // signal  parent_alarm_handler signal handler (i.e. SIGALRM)
	
            set_signal(action, SIGUSR1, parent_ack_handler);
      set_signal(action, SIGALRM, parent_alarm_handler);
      // send back remaining signals after 1s
      // if sender doesn't receive all acks using alarm
      alarm(1);

      while (1) {
        pause();
        parent_handle_pending_signals(&num_acks_received, num_signal_to_send);
      }
      break;
  }

  return EXIT_SUCCESS;
}
