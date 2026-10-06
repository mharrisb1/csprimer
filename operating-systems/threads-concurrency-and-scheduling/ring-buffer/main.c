#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_SIZE (x) printf("sizeof(" #x ")\t=\t%zu\n", sizeof(x)); 

typedef struct {
  size_t capacity;
  size_t head;
  size_t tail;
  long*  items;
} queue_t;

queue_t* q_init (size_t capacity);
void     q_free (queue_t* q);
void     q_debug(queue_t* q);

void  q_push(queue_t* q, long item);
long  q_pop (queue_t* q);

int main() {
  // Disable buffering for stdout
  setvbuf(stdout, NULL, _IONBF, 0);

  queue_t* q = q_init(4);
  if (q == NULL) {
    fprintf(stderr, "Failed to initialize queue\n");
    return -1;
  }

  q_push(q, 1);
  q_pop (q);
  q_pop (q);
  q_pop (q);
  q_pop (q);

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

  q->items = calloc(q->capacity, sizeof(long));
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

void q_debug(queue_t* q) {
    printf("queue_t {\n");
    printf("  capacity = %zu\n", q->capacity); 
    printf("  head     = %zu\n", q->head);
    printf("  tail     = %zu\n", q->tail);
    printf("  items    = {\n");
    for (size_t i = 0; i < q->capacity; i++) {
      char hptr[7] = "";
      char tptr[6] = "";
      if (i == q->head) strcpy(hptr, "[<-h] ");
      if (i == q->tail) strcpy(tptr, "[<-t]");
      printf("    [%zu] = %ld %s%s\n", i, q->items[i], hptr, tptr);
    }
    printf("  }\n");
    printf("}\n");
}

void q_push(queue_t* q, long item) {
  q->items[q->tail] = item;
#ifdef DEBUG
  q_debug(q);
#endif
  q->tail           = ++q->tail % q->capacity;
} 

long q_pop(queue_t* q) {
  long item         = q->items[q->head];
  // q->items[q->head] = 0; // RESET
#ifdef DEBUG
  q_debug(q);
#endif
  q->head           = ++q->head % q->capacity;
  return item;
}
