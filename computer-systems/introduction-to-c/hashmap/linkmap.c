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

void LinkMap_set(LinkMap *lm, char *key, void *value) {
  if (LinkMap_is_empty(lm)) {
    lm->head = Node_new(key, value);
    return;
  }
  Node *prev = NULL;
  Node *curr = lm->head;
  while (curr != NULL) {
    if (strcmp(curr->key, key) == 0) {
      curr->value = value;
      return;
    }
    prev = curr;
    curr = prev->next;
  }
  prev->next = Node_new(key, value);
}

void LinkMap_delete(LinkMap *lm, char *key) {
  if (LinkMap_is_empty(lm))
    return;
  Node *prev = NULL;
  Node *curr = lm->head;
  while (curr != NULL) {
    if (strcmp(curr->key, key) == 0) {
      if (prev == NULL) {
        lm->head = NULL;
      } else {
        prev->next = curr->next;
        curr->next = NULL;
      }
      Node_free(curr);
      break;
    } else {
      prev = curr;
      curr = prev->next;
    }
  }
}

Node *LinkMap_get(LinkMap *lm, char *key) {
  Node *curr = lm->head;
  while (curr != NULL) {
    if (strcmp(curr->key, key) == 0) {
      return curr;
    }
    curr = curr->next;
  }
  return NULL;
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
  float y = 1.33;
  LinkMap_set(lm, "foo", &x);
  assert(!LinkMap_is_empty(lm));
  assert(LinkMap_size(lm) == 1);

  // test append
  LinkMap_set(lm, "bar", &x);
  assert(LinkMap_size(lm) == 2);
  LinkMap_set(lm, "bat", &y);
  assert(LinkMap_size(lm) == 3);

  // test get
  assert(LinkMap_get(lm, "bar")->value == &x);
  assert(LinkMap_get(lm, "bat")->value == &y);
  assert(LinkMap_get(lm, "fox") == NULL);

  // test overwrite
  LinkMap_set(lm, "foo", &y);
  LinkMap_set(lm, "bar", &x);
  assert(LinkMap_size(lm) == 3);

  // test delete
  LinkMap_delete(lm, "bar");
  assert(LinkMap_size(lm) == 2);
  LinkMap_delete(lm, "foo");
  LinkMap_delete(lm, "bat");
  assert(LinkMap_is_empty(lm));
  LinkMap_delete(lm, "baz"); // test that delete on invalid keys do not error

  LinkMap_free(lm);
  printf("ok\n");
}
#endif
