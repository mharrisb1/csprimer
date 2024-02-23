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

typedef struct LinkSet {
  Node *head;
} LinkSet;

LinkSet *LinkSet_new() {
  LinkSet *ll = malloc(sizeof(LinkSet));
  ll->head = NULL;
  return ll;
}

void LinkSet_free(LinkSet *ls) {
  if (ls->head != NULL)
    Node_free(ls->head);
  free(ls);
}

bool LinkSet_empty(LinkSet *ls) { return ls->head == NULL; }

void LinkSet_add(LinkSet *ls, char *key, void *value) {
  if (LinkSet_empty(ls)) {
    ls->head = Node_new(key, value);
    return;
  }
  Node *next = ls->head;
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

void LinkSet_delete(LinkSet *ls, char *key) {
  if (LinkSet_empty(ls))
    return;
  Node *prev = ls->head;
  Node *curr = ls->head;
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

unsigned long LinkSet_size(LinkSet *ls) {
  unsigned long n = 0;
  Node *next = ls->head;
  while (next != NULL) {
    n++;
    next = next->next;
  }
  return n;
}

int main(void) {
  // test construction
  LinkSet *ls = LinkSet_new();
  assert(ls->head == NULL);
  assert(LinkSet_empty(ls));

  // test add when emtpy
  int x = 10;
  LinkSet_add(ls, "foo", &x);
  assert(!LinkSet_empty(ls));
  assert(LinkSet_size(ls) == 1);

  // test append
  LinkSet_add(ls, "bar", &x);
  assert(LinkSet_size(ls) == 2);

  // test overwrite
  LinkSet_add(ls, "foo", &x);
  LinkSet_add(ls, "bar", &x);
  assert(LinkSet_size(ls) == 2);

  // test delete
  LinkSet_delete(ls, "bar");
  assert(LinkSet_size(ls) == 1);
  LinkSet_delete(ls, "foo");
  assert(LinkSet_empty(ls));
  LinkSet_delete(ls, "baz"); // test that invalid keys do not error

  LinkSet_free(ls);
  printf("ok\n");
}
#endif
