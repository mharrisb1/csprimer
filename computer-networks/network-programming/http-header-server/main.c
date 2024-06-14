#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFSIZE 1024

extern void hhtojson(char *readbuf, char *writebuf, ssize_t offset);

int main() {
  int sockfd, connfd;
  struct addrinfo hints;
  struct addrinfo *res;
  struct sockaddr_storage theirs;
  socklen_t sin_size;
  ssize_t num_bytes;
  char readbuf[BUFSIZE], writebuf[BUFSIZE];

  memset(&hints, 0, sizeof(hints));

  hints.ai_family   = AF_INET;     // IPv4
  hints.ai_socktype = SOCK_STREAM; // TCP/IP
  hints.ai_flags    = AI_PASSIVE;  // Use localhost

  (void)getaddrinfo(NULL, "9999", &hints, &res);
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  (void)bind(sockfd, res->ai_addr, res->ai_addrlen);
  (void)listen(sockfd, 1);

  freeaddrinfo(res);

  sin_size           = sizeof(theirs);
  connfd             = accept(sockfd, (struct sockaddr *)&theirs, &sin_size);
  num_bytes          = recv(connfd, readbuf, BUFSIZE - 1, 0);
  readbuf[num_bytes] = '\0';

  strcpy(writebuf, "HTTP/1.1 200 OK\nContent-Type: application/json\n\n");
  hhtojson(readbuf, writebuf, strlen(writebuf));

  printf("%s\n", readbuf);
  printf("%s\n", writebuf);
  printf("%lu\n", strlen(writebuf));

  send(connfd, writebuf, strlen(writebuf), 0);

  shutdown(sockfd, 2);
  shutdown(connfd, 2);
}
