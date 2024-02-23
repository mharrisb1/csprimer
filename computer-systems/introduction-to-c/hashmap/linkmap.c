#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char *key;
  void *value;
  struct Node *next;
} Node;

Node *Node_new(char *key, void *value) {
  Node *node = malloc(sizeof(Node));
  node->key = key;
  node->value = value;
  node->next = NULL;
  return node;
}

void Node_free(Node *n) {
  Node *next = n->next;
  while (next != NULL) {
    Node *curr = next;
    next = next->next;
    free(curr);
  }
  free(n);
}

typedef struct LinkMap {
  Node *head;
} LinkMap;

LinkMap *LinkMap_new() {
  LinkMap *ll = malloc(sizeof(LinkMap));
  ll->head = NULL;
  return ll;
}

void LinkMap_free(LinkMap *lm) {
  if (lm->head != NULL)
    Node_free(lm->head);
  free(lm);
}

bool LinkMap_is_empty(LinkMap *lm) { return lm->head == NULL; }

void LinkMap_add(LinkMap *lm, char *key, void *value) {
  if (LinkMap_is_empty(lm)) {
    lm->head = Node_new(key, value);
    return;
  }
  Node *next = lm->head;
  Node *last = next;
  while (next != NULL) {
    if (strcmp(next->key, key) == 0) {
      next->value = value;
      return;
    }
    last = next;
    next = next->next;
  }
  last->next = Node_new(key, value);
}

void LinkMap_delete(LinkMap *lm, char *key) {
  if (LinkMap_is_empty(lm))
    return;
  Node *prev = lm->head;
  Node *curr = lm->head;
  while (curr != NULL) {
    if (strcmp(curr->key, key) == 0) {
      prev->next = curr->next;
      Node_free(curr);
      break;
    } else {
      prev = curr;
      curr = prev->next;
    }
  }
}

#ifdef UNIT_TESTING
#include <assert.h>
#include <stdio.h>

unsigned long LinkMap_size(LinkMap *lm) {
  unsigned long n = 0;
  Node *next = lm->head;
  while (next != NULL) {
    n++;
    next = next->next;
  }
  return n;
}

int main(void) {
  // test construction
  LinkMap *lm = LinkMap_new();
  assert(lm->head == NULL);
  assert(LinkMap_is_empty(lm));

  // test add when emtpy
  int x = 10;
  LinkMap_add(lm, "foo", &x);
  assert(!LinkMap_is_empty(lm));
  assert(LinkMap_size(lm) == 1);

  // test append
  LinkMap_add(lm, "bar", &x);
  assert(LinkMap_size(lm) == 2);

  // test overwrite
  LinkMap_add(lm, "foo", &x);
  LinkMap_add(lm, "bar", &x);
  assert(LinkMap_size(lm) == 2);

  // test delete
  LinkMap_delete(lm, "bar");
  assert(LinkMap_size(lm) == 1);
  LinkMap_delete(lm, "foo");
  assert(LinkMap_is_empty(lm));
  LinkMap_delete(lm, "baz"); // test that invalid keys do not error

  LinkMap_free(lm);
  printf("ok\n");
}
#endif
