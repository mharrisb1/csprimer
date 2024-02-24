#include "linkmap.c"
#include <stdlib.h>

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

typedef struct Hashmap {
  size_t buckets;
  LinkMap **maps;
} Hashmap;

Hashmap *Hashmap_new() {
  Hashmap *hm = malloc(sizeof(Hashmap));
  hm->buckets = STARTING_BUCKETS;
  hm->maps = malloc(sizeof(LinkMap) * hm->buckets);
  for (int i; i < hm->buckets; i++) {
    hm->maps[i] = NULL;
  }
  return hm;
}

void Hashmap_free(Hashmap *h) {
  for (int i; i < h->buckets; i++) {
    if (h->maps[i] != NULL) {
      LinkMap_free(h->maps[i]);
    }
  }
  free(h);
}

void Hashmap_set(Hashmap *h, char *k, void *v) {
  int i = hash(k, h->buckets);
  LinkMap *lm = h->maps[i];
  if (lm == NULL) {
    lm = LinkMap_new();
    lm->head = Node_new(k, v);
    h->maps[i] = lm;
  } else {
    LinkMap_set(lm, k, v);
  }
}

void *Hashmap_get(Hashmap *h, char *k) {
  int i = hash(k, h->buckets);
  LinkMap *lm = h->maps[i];
  if (lm != NULL) {
    Node *n = LinkMap_get(lm, k);
    if (n != NULL) {
      return n->value;
    }
  }
  return NULL;
}

void Hashmap_delete(Hashmap *h, char *k) {
  int i = hash(k, h->buckets);
  LinkMap *lm = h->maps[i];
  if (lm != NULL) {
    LinkMap_delete(lm, k);
  }
}
