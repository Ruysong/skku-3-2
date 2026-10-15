#include <fcntl.h>
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
  int fd;
  Message msg;

  mkfifo(FIFO_FILE, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  printf("Server waiting for client...\n");

  fd = open(FIFO_FILE, O_RDONLY);
  printf("Client connected!\n");

  while (read(fd, (char *)&msg, sizeof(Message)) > 0) {
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &msg.timestamp);
    printf("[%s] %s: %s\n", timestamp, msg.username, msg.message);
  }
  printf("Connection terminated...\nGoodbye!\n");
  close(fd);
  unlink(FIFO_FILE);

  return 0;
}
 