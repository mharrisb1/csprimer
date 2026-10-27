#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 9876
#define SEED 0x1234
#define SIZE (1 << 20)

int main() {
  int checksum, max, sock, conn, n;
  unsigned short port = PORT;
  struct sockaddr_in self, client; 
  unsigned int clientlen;

  srand(SEED);
  max = SIZE / sizeof(int);

  sock = socket(AF_INET, SOCK_STREAM, 0);
  memset(&self, 0, sizeof(self)); 
  self.sin_family = AF_INET;
  self.sin_addr.s_addr = htonl(INADDR_ANY);
  self.sin_port = htons(port);
  bind(sock, (struct sockaddr*)&self, sizeof(self));
  listen(sock, 10);
}
