#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXLINE 80

int main(int argc, char* argv[]) {
  long n;
  int conn_fd;
  struct hostent* host_entry;
  struct sockaddr_in sock_addr;
  char buf[MAXLINE];
  if (argc != 3) {
    printf("usage: %s <host> <port>\n", argv[0]);
    exit(1);
  }
  char* host = argv[1];
  long port = strtol(argv[2], NULL, 10);
  if (errno == ERANGE || port < 0 || port > 65535) {
    printf("invalid port number %s\n", argv[2]);
    exit(1);
  }

  if ((conn_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("socket() failed.\n");
    exit(2);
  }

  if ((host_entry = gethostbyname(host)) == NULL) {
    printf("invalid hostname %s\n", host);
    exit(3);
  }

  memset(&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  memcpy(&sock_addr.sin_addr.s_addr, host_entry->h_addr_list[0],
         host_entry->h_length);
  sock_addr.sin_port = htons(port);
  if (connect(conn_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
    printf("connect() failed.\n");
    exit(4);
  }

  while ((n = read(STDIN_FILENO, buf, MAXLINE)) > 0) {
    write(conn_fd, buf, n);
    n = read(conn_fd, buf, MAXLINE);
    write(STDOUT_FILENO, buf, n);
  }
  close(conn_fd);
}