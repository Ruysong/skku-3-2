
#undef __USE_MISC
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
  int listen_fd, conn_fd;
  socklen_t conn_addr_len;
  struct sockaddr_in sock_addr, conn_addr;
  char buf[MAXLINE];
  if (argc != 2) {
    printf("usage: %s <port>\n", argv[0]);
    exit(1);
  }

  long port = strtol(argv[1], NULL, 10);
  if (errno == ERANGE || port < 0 || port > 65535) {
    printf("invalid port number %s\n",
           argv[1]);  // Corrected argv[2] to argv[1]
    exit(1);
  }

  if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("socket() failed.\n");
    exit(1);
  }

  memset(&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_addr.sin_port = htons(port);
  if (bind(listen_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
    printf("bind() failed.\n");
    exit(2);
  }

  if (listen(listen_fd, 5) < 0) {
    printf("listen() failed.\n");
    exit(3);
  }

  while (1) {
    conn_addr_len = sizeof(conn_addr);
    if ((conn_fd = accept(listen_fd, (struct sockaddr*)&conn_addr,
                          &conn_addr_len)) < 0) {
      printf("accept() failed.\n");
      continue;
    }

    // echo
    while ((n = read(conn_fd, buf, MAXLINE)) > 0) {
      printf("got %ld bytes from client.\n", n);
      write(conn_fd, buf, n);
    }
    printf("connection terminated.\n");
    close(conn_fd);
  }
}