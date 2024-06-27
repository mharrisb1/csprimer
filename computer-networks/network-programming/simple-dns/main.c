#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dns.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <query_id> <domain>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sockfd;
  unsigned char rbuf[MAX_MESSAGE_SIZE], wbuf[MAX_MESSAGE_SIZE];
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_port        = htons(0);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port   = htons(53);
  inet_pton(AF_INET, "8.8.8.8", &client_addr.sin_addr);

  DNSMessage *query = createQuery(atoi(argv[1]), argv[2]);
  int sendlen       = serializeMessage(query, wbuf);

  if ((sendto(sockfd, wbuf, sendlen, 0, (struct sockaddr *)&client_addr,
              client_addr_len)) < 0) {
    perror("sendto failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  int recvlen = recvfrom(sockfd, rbuf, MAX_MESSAGE_SIZE, MSG_WAITALL,
                         (struct sockaddr *)&client_addr, &client_addr_len);

  if (recvlen < 0) {
    perror("recvfrom failed or timed out");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  DNSMessage *response = deserializeMessage(rbuf);
  assert(query->header->id == response->header->id);

  char ip_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, response->answer->rdata, ip_str, INET_ADDRSTRLEN);

  printf("INET address: %s\n", ip_str);

  freeMessage(query);
  freeMessage(response);
  close(sockfd);
  exit(EXIT_SUCCESS);
}
