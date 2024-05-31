/*
 * Simple shout server
 *
 * Code is a modified version of the "listener" and "talker" programs from
 * Beej's Guide to Network Programming (section 6.3).
 *
 */

#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT "8888"
#define MAXBUFSIZE 1024

int sockfd = -1;

void handle_sigint(int sig) {
  if (sockfd != -1) {
    shutdown(sockfd, 2);
  }
  exit(0);
}

char *upper(char *s) {
  unsigned int i = -1;
  for (unsigned int i = 0; s[i] != '\0'; i++) {
    if (s[i] >= 'a' && s[i] <= 'z') {
      s[i] = s[i] & ~(1 << 5); // flip bit 5 to off
    }
  }
  return s;
}

int main() {
  struct addrinfo hints, *res;
  struct sockaddr_storage dest;
  socklen_t addr_len;
  char buf[MAXBUFSIZE];

  // handle ctrl+c
  signal(SIGINT, handle_sigint);

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;      // IPv4
  hints.ai_socktype = SOCK_DGRAM; // UDP
  hints.ai_flags = AI_PASSIVE;    // Use host machine address

  (void)getaddrinfo(NULL, PORT, &hints, &res);

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  (void)bind(sockfd, res->ai_addr, res->ai_addrlen);

  freeaddrinfo(res);

  addr_len = sizeof(dest);
  while (1) {
    int bufsize = recvfrom(sockfd, buf, MAXBUFSIZE - 1, 0,
                           (struct sockaddr *)&dest, &addr_len);
    buf[bufsize] = '\0';
    sendto(sockfd, upper(buf), bufsize, 0, (struct sockaddr *)&dest, addr_len);
  }
}
