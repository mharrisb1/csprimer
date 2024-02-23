#include <stdlib.h>
#include <string.h>

#ifndef STARTING_BUCKETS
#define STARTING_BUCKETS 8
#endif

int hash(char *s, unsigned long k) {
  int n = 0;
  while (*s != '\0') {
    n += (int)*s++;
  }
  return n % k;
}

typedef struct Node {
  char *k;
  void *v;
  void *next;
} Node;

Node *Node_new(char *k, void *v) {
  Node *n = malloc(sizeof(Node));
  n->k = k;
  n->v = v;
  n->next = NULL;
  return n;
}

void Node_free(Node *n) { free(n); }

typedef struct Hashmap {
  size_t buckets;
  void **nodes;
} Hashmap;

Hashmap *Hashmap_new() {
  Hashmap *hm = malloc(sizeof(Hashmap));
  hm->buckets = STARTING_BUCKETS;
  hm->nodes = malloc(sizeof(void *) * hm->buckets);
  return hm;
}

void Hashmap_free(Hashmap *hm) {
  free(hm->nodes);
  free(hm);
}

void Hashmap_set(Hashmap *hm, char *k, void *v) {
  int i = hash(k, hm->buckets);
  if (hm->nodes[i] != NULL) {
    Node *curr = hm->nodes[i];
    do {
      if (strcmp(curr->k, k)) {
        curr->v = v;
        break;
      }
      curr = curr->next;
    } while (curr->next != NULL);
    curr->next = Node_new(k, v);
  } else {
    hm->nodes[i] = Node_new(k, v);
  }
}

void *Hashmap_get(Hashmap *hm, char *k) {
  // TODO
}

void Hashmap_delete(Hashmap *hm, char *k) {
  // TODO
}
