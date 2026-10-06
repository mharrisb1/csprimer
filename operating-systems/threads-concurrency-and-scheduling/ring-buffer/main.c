#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_SIZE(x) printf("sizeof(" #x ")\t=\t%zu\n", sizeof(x)); 

typedef struct {
  size_t capacity;
  long*  items;
  size_t head;
  size_t tail;
} queue_t;

queue_t* q_init(size_t capacity);
void     q_free(queue_t* q);

void  q_push(queue_t* q, long item);
long  q_pop (queue_t* q);

int main() {
  queue_t* q = q_init(4);
  if (q == NULL) {
    fprintf(stderr, "Failed to initialize queue\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 100; i++) {
    size_t head, tail;
    head = q->head;
    tail = q->tail;

    printf("push=\tq[%zu]\t=\t%i\n", tail, i);
    q_push(q, i);
    printf("pop=\tq[%zu]\t=\t%ld\n", head, q_pop(q));
  }
  q_free(q);
  return 0;
}

queue_t* q_init(size_t capacity) {
  queue_t* q = malloc(sizeof(queue_t));
  if (q == NULL) {
    return NULL;
  }

  q->capacity = capacity;
  q->head     = 0;
  q->tail     = 0;

  q->items = malloc(q->capacity * sizeof(long));
  if (q->items == NULL) {
    free(q);
    return NULL;
  }

  return q;
}

void q_free(queue_t* q) {
  free(q->items);
  q->items = NULL;

  free(q);
  q = NULL;
}

void q_push(queue_t* q, long item) {
  q->items[q->tail] = item;
  q->tail           = ++q->tail % q->capacity;
} 

long q_pop(queue_t* q) {
  long item = q->items[q->head];
  q->head   = ++q->head % q->capacity;
  return item;
}
