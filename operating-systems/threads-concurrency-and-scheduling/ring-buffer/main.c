#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  unsigned int    size;       // current size of buffer
  unsigned int    capacity;   // max capacity of buffer (immutable)
  unsigned int    wp;         // writer pointer
  unsigned int    rp;         // reader pointer
  unsigned int*   buf;        // items stored in buffer
  pthread_mutex_t lock;       // mutex
  pthread_cond_t  writeable;  // writeable condition variable
  pthread_cond_t  readable;   // readable condition variable
} queue_t;

void*        producer(void* arg);
void*        consumer(void* arg);

queue_t*     queue_init(unsigned int capacity);
void         queue_free(queue_t* q);

void         enqueue(queue_t* q, unsigned int item);
unsigned int dequeue(queue_t* q);

int main() {
  queue_t* q = queue_init(4);
  if (q == NULL) return 1;

  int i, n, m;
  n = 3;
  m = 5;
  pthread_t prod[n], cons[m];

  // create producers
  for (i = 0; i < n; i++) {
    pthread_create(&prod[i], NULL, producer, q);
  }

  // create consumers
  for (i = 0; i < m; i++) {
    pthread_create(&cons[i], NULL, consumer, q);
  }

  // join producers
  for (i = 0; i < n; i++) {
    pthread_join(prod[i], NULL);
  }

  // join consumers
  for (i = 0; i < m; i++) {
    pthread_join(cons[i], NULL);
  }

  queue_free(q);
}

void* producer(void* arg) {
  queue_t* q = (queue_t*)arg;
  int i = 0;
  while(1) {
    enqueue(q, i++ % 100);
  }
}

void* consumer(void* arg) {
  queue_t* q = (queue_t*)arg;
  while(1) {
    dequeue(q);
  }
}

queue_t* queue_init(unsigned int capacity) {
  queue_t* q = malloc(sizeof(queue_t));
  if (q == NULL) {
    return q;
  }

  q->size     = 0;
  q->capacity = capacity;
  q->wp       = 0;
  q->rp       = 0;
  q->buf      = calloc(q->capacity, sizeof(unsigned int));

  pthread_mutex_init(&q->lock, NULL);
  pthread_cond_init(&q->writeable, NULL);
  pthread_cond_init(&q->readable, NULL);

  return q;
}

void queue_free(queue_t* q) {
  if (q == NULL) return;

  pthread_mutex_destroy(&q->lock);
  pthread_cond_destroy(&q->writeable);
  pthread_cond_destroy(&q->readable);

  free(q->buf);
  q->buf = NULL;

  free(q);
}

void enqueue(queue_t* q, unsigned int item) {
  pthread_mutex_lock(&q->lock);
  // block to ensure no loss in data for the readers
  while (q->size == q->capacity) {
    pthread_cond_wait(&q->writeable, &q->lock);
  }
  q->buf[q->wp] = item;
  printf("[thread %lu] %2u -> [%2u]\n", pthread_self(), item, q->wp);
  q->wp = (q->size + 1) % q->capacity;
  q->size++;
  pthread_cond_signal(&q->readable);
  pthread_mutex_unlock(&q->lock);
}

unsigned int dequeue(queue_t* q) {
  pthread_mutex_lock(&q->lock);
  // don't allow reading if nothing is in queue
  while(q->size == 0) {
    pthread_cond_wait(&q->readable, &q->lock);
  }
  unsigned int item = q->buf[q->rp];
  printf("[thread %lu]       [%2u] -> %2u\n", pthread_self(), q->rp, item);
  q->rp = (q->wp + 1) % q->capacity;
  q->size--;
  pthread_cond_signal(&q->writeable);
  pthread_mutex_unlock(&q->lock);
  return item;
}
