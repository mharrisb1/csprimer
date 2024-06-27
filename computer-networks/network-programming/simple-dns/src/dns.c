#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dns.h"

void _compress(char *domain, char *qname) {
  uint8_t i = 0;
  char *d   = domain;
  char *q   = qname;

  while (*d) {
    if (*d == '.') {
      *q++ = i;
      for (char *label = d - i; label < d; ++label) {
        *q++ = *label;
      }
      i = 0;
    } else {
      ++i;
    }
    ++d;
  }

  *q++ = i;
  for (char *label = d - i; label < d; ++label) {
    *q++ = *label;
  }
  *q++ = '\0';
}

DNSMessage *createQuery(int id, char *domain) {
  DNSMessageHeader *header = malloc(sizeof(DNSMessageHeader));
  header->id               = id;
  header->qr               = 0;
  header->opcode           = 0;
  header->aa               = 0;
  header->tc               = 0;
  header->rd               = 1;
  header->ra               = 0;
  header->z                = 0;
  header->rcode            = 0;
  header->qdcount          = 1;
  header->ancount          = 0;
  header->nscount          = 0;
  header->arcount          = 0;

  DNSMessageQuestion *question = malloc(sizeof(DNSMessageQuestion));
  question->qname              = malloc(MAX_LABEL_SIZE);
  _compress(domain, question->qname);
  question->qtype  = 1;
  question->qclass = 1;

  DNSMessage *message = malloc(sizeof(DNSMessage));
  message->header     = header;
  message->question   = question;
  message->answer     = NULL;
  message->authority  = NULL;
  message->additional = NULL;

  return message;
}

int serializeMessage(DNSMessage *msg, uint8_t *buffer) {
  int offset = 0;

  uint16_t id = htons(msg->header->id);
  memcpy(buffer + offset, &id, sizeof(id));
  offset += sizeof(id);

  buffer[offset++] = (msg->header->qr << 7) | (msg->header->opcode << 3) |
                     (msg->header->aa << 2) | (msg->header->tc << 1) |
                     (msg->header->rd);

  buffer[offset++] =
      (msg->header->ra << 7) | (msg->header->z << 4) | (msg->header->rcode);

  uint16_t qdcount = htons(msg->header->qdcount);
  memcpy(buffer + offset, &qdcount, sizeof(qdcount));
  offset += sizeof(qdcount);

  uint16_t ancount = htons(msg->header->ancount);
  memcpy(buffer + offset, &ancount, sizeof(ancount));
  offset += sizeof(ancount);

  uint16_t nscount = htons(msg->header->nscount);
  memcpy(buffer + offset, &nscount, sizeof(nscount));
  offset += sizeof(nscount);

  uint16_t arcount = htons(msg->header->arcount);
  memcpy(buffer + offset, &arcount, sizeof(arcount));
  offset += sizeof(arcount);

  int qname_len = strlen(msg->question->qname) + 1;
  memcpy(buffer + offset, msg->question->qname, qname_len);
  offset += qname_len;

  uint16_t qtype = htons(msg->question->qtype);
  memcpy(buffer + offset, &qtype, sizeof(uint16_t));
  offset += sizeof(uint16_t);

  uint16_t qclass = htons(msg->question->qclass);
  memcpy(buffer + offset, &qclass, sizeof(uint16_t));
  offset += sizeof(uint16_t);

  return offset;
}

DNSMessage *deserializeMessage(uint8_t *buffer) {
  int offset = 0;

  DNSMessageHeader *header = malloc(sizeof(DNSMessageHeader));

  uint16_t id;
  memcpy(&id, buffer + offset, sizeof(id));
  header->id = ntohs(id);
  offset += sizeof(id);

  header->qr     = (buffer[offset] & 0x80) >> 7;
  header->opcode = (buffer[offset] & 0x78) >> 3;
  header->aa     = (buffer[offset] & 0x4) >> 2;
  header->tc     = (buffer[offset] & 0x2) >> 1;
  header->rd     = (buffer[offset++] & 0x1);
  header->ra     = (buffer[offset] & 0x80) >> 7;
  header->z      = (buffer[offset] & 0x70) >> 4;
  header->rcode  = (buffer[offset++] & 0xF);

  uint16_t qdcount;
  memcpy(&qdcount, buffer + offset, sizeof(qdcount));
  header->qdcount = ntohs(qdcount);
  offset += sizeof(qdcount);

  uint16_t ancount;
  memcpy(&ancount, buffer + offset, sizeof(ancount));
  header->ancount = ntohs(ancount);
  offset += sizeof(ancount);

  uint16_t nscount;
  memcpy(&nscount, buffer + offset, sizeof(nscount));
  header->nscount = ntohs(nscount);
  offset += sizeof(nscount);

  uint16_t arcount;
  memcpy(&arcount, buffer + offset, sizeof(arcount));
  header->arcount = ntohs(arcount);
  offset += sizeof(arcount);

  DNSMessageQuestion *question = malloc(sizeof(DNSMessageQuestion));

  int qname_len   = strlen((char *)(buffer + offset)) + 1;
  question->qname = malloc(qname_len);
  strcpy(question->qname, (char *)(buffer + offset));
  offset += qname_len;

  uint16_t qtype;
  memcpy(&qtype, buffer + offset, sizeof(qtype));
  question->qtype = ntohs(qtype);
  offset += sizeof(qtype);

  uint16_t qclass;
  memcpy(&qclass, buffer + offset, sizeof(qclass));
  question->qclass = ntohs(qclass);
  offset += sizeof(qclass);

  DNSResourceRecord *answer = malloc(sizeof(DNSResourceRecord));

  // HACK: need to properly handle pointers
  answer->name = malloc(qname_len);
  strcpy(answer->name, question->qname);
  offset += 2;

  uint16_t type;
  memcpy(&type, buffer + offset, sizeof(type));
  answer->type = ntohs(type);
  offset += sizeof(type);

  uint16_t dclass;
  memcpy(&dclass, buffer + offset, sizeof(dclass));
  answer->dclass = ntohs(dclass);
  offset += sizeof(dclass);

  uint32_t ttl;
  memcpy(&ttl, buffer + offset, sizeof(ttl));
  answer->ttl = ntohs(ttl);
  offset += sizeof(ttl);

  uint16_t rdlength;
  memcpy(&rdlength, buffer + offset, sizeof(rdlength));
  answer->rdlength = ntohs(rdlength);
  offset += sizeof(rdlength);

  answer->rdata = malloc(answer->rdlength);
  memcpy(answer->rdata, buffer + offset, answer->rdlength);

  DNSMessage *message = malloc(sizeof(DNSMessage));
  message->header     = header;
  message->question   = question;
  message->answer     = answer;
  message->authority  = NULL; // skip
  message->additional = NULL; // skip

  return message;
}

void freeMessage(DNSMessage *msg) {
  if (msg->header) {
    free(msg->header);
  }
  if (msg->question) {
    if (msg->question->qname) {
      free(msg->question->qname);
    }
    free(msg->question);
  }
  if (msg->answer) {
    if (msg->answer->name) {
      free(msg->answer->name);
    }
    if (msg->answer->rdata) {
      free(msg->answer->rdata);
    }
    free(msg->answer);
  }
}
