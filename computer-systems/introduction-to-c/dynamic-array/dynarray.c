#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define STARTING_CAPACITY 8

typedef struct DA {
  size_t size;
  size_t capacity;
  void **arr;
} DA;

DA *DA_new(void) {
  DA *da = malloc(sizeof(DA));
  da->size = 0;
  da->capacity = STARTING_CAPACITY;
  da->arr = malloc(sizeof(void *) * da->capacity);
  return da;
}

void DA_free(DA *da) {
  free(da->arr);
  free(da);
}

size_t DA_size(DA *da) { return da->size; }

void DA_push(DA *da, void *x) {
  if (da->size == da->capacity) {
    da->capacity <<= 1;
    da->arr = realloc(da->arr, sizeof(void *) * da->capacity);
    printf("Resized to %d\n", (int)da->capacity);
  }
  da->arr[da->size++] = x;
}

void *DA_pop(DA *da) {
  if (da->size == 0)
    return NULL;
  if (da->size * 2 <= da->capacity && da->capacity > STARTING_CAPACITY) {
    da->capacity >>= 1;
    da->arr = realloc(da->arr, sizeof(void *) * da->capacity);
    printf("Resized to %d\n", (int)da->capacity);
  }
  void *x = da->arr[--da->size];
  return x;
}

void DA_set(DA *da, void *x, size_t i) {
  if (0 <= i <= da->size)
    da->arr[i] = x;
}

void *DA_get(DA *da, size_t i) {
  if (0 <= i <= da->size) {
    return da->arr[i];
  } else {
    return NULL;
  }
}

int main() {
  DA *da = DA_new();

  assert(DA_size(da) == 0);

  // basic push and pop test
  int x = 5;
  float y = 12.4;
  DA_push(da, &x);
  DA_push(da, &y);
  assert(DA_size(da) == 2);

  assert(DA_pop(da) == &y);
  assert(DA_size(da) == 1);

  assert(DA_pop(da) == &x);
  assert(DA_size(da) == 0);
  assert(DA_pop(da) == NULL);

  // basic set/get test
  DA_push(da, &x);
  DA_set(da, &y, 0);
  assert(DA_get(da, 0) == &y);
  DA_pop(da);
  assert(DA_size(da) == 0);

  // expansion test
  DA *da2 = DA_new(); // use another DA to show it doesn't get overriden
  DA_push(da2, &x);
  int i, n = 100 * STARTING_CAPACITY, arr[n];
  for (i = 0; i < n; i++) {
    arr[i] = i;
    DA_push(da, &arr[i]);
  }
  assert(DA_size(da) == n);
  for (i = 0; i < n; i++) {
    assert(DA_get(da, i) == &arr[i]);
  }
  for (; n; n--)
    DA_pop(da);
  assert(DA_size(da) == 0);
  assert(DA_pop(da2) == &x); // this will fail if da doesn't expand

  // nested test
  DA_push(da, &da2);
  assert(DA_pop(da) == &da2);

  DA_free(da);
  DA_free(da2);
  printf("OK\n");
}
