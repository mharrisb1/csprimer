#ifndef DNS_H
#define DNS_H
#include <stdint.h>
#include <stdlib.h>

#define MAX_LABEL_SIZE   63
#define MAX_MESSAGE_SIZE 512

typedef struct {
  int16_t id;
  uint8_t qr : 1;
  uint8_t opcode : 4;
  uint8_t aa : 1;
  uint8_t tc : 1;
  uint8_t rd : 1;
  uint8_t ra : 1;
  uint8_t z : 3;
  uint8_t rcode : 4;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
} DNSMessageHeader;

typedef struct {
  char *qname;
  uint16_t qtype;
  uint16_t qclass;
} DNSMessageQuestion;

typedef struct {
  char *name;
  uint16_t type;
  uint16_t dclass;
  uint32_t ttl;
  uint16_t rdlength;
  uint8_t *rdata;
} DNSResourceRecord;

typedef struct {
  DNSMessageHeader *header;
  DNSMessageQuestion *question;
  DNSResourceRecord *answer;
  DNSResourceRecord *authority;
  DNSResourceRecord *additional;
} DNSMessage;

DNSMessage *createQuery(int id, char *domain);

DNSMessage *deserializeMessage(uint8_t *buffer);

int serializeMessage(DNSMessage *msg, uint8_t *buffer);

void freeMessage(DNSMessage *msg);
#endif
