#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PROC_COUNT
#define PROC_COUNT 1
#endif

#define ITER     100000000
#define EXPECTED (ITER * PROC_COUNT)

volatile int counter = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *args) {
  (void)args;
  for (int i = 0; i < ITER; i++) {
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main() {
  pthread_t *threads = malloc(PROC_COUNT * sizeof(pthread_t));

  for (int i = 0; i < PROC_COUNT; i++) {
    pthread_create(&threads[i], NULL, routine, NULL);
  }

  for (int i = 0; i < PROC_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("\n============== Starting ==============\n");
  printf("\ncores=%i\nactual=%i\nexpected=%i\n", PROC_COUNT, counter, EXPECTED);
  printf("\n============== Stopping ==============\n");
}
