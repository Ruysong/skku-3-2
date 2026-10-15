#include <fcntl.h>
#include <editline/readline.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FIFO_FILE "fifo_file"
#define MAX_SIZE 100

// In actual applications, this must be (de-)serialized
typedef struct message {
  char username[MAX_SIZE];
  char message[MAX_SIZE];
  struct tm timestamp;
} Message;

int main() {
  char* username;

  int fd;
  fd = open(FIFO_FILE, O_WRONLY);

  username = readline("Enter your username: ");
  char prompt[strlen(username) + 3];
  snprintf(prompt, sizeof(prompt), "%s: ", username);

  while (1) {
    char* message = readline(prompt);
    if (strncmp(message, "/exit", 5) == 0) {
      break;
    }

    Message msg;
    strncpy(msg.username, username, sizeof(msg.username));
    strncpy(msg.message, message, sizeof(msg.message));
    time_t t = time(NULL);
    msg.timestamp = *localtime(&t);

    write(fd, (char*)&msg, sizeof(Message));
  }

  close(fd);

  return 0;
}


