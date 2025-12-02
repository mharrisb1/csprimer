#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_DEPTH 2

typedef struct {
  int *arr;
  int  n;
  int  depth;
} sortargs_t;

void merge(int *arr, int n, int mid) {
  int i;
  int left  = 0;
  int right = mid;

  int *x = malloc(n * sizeof(int));

  // copy the ith item from either the left of right part
  for (i = 0; i < n; i++) {
    if (right == n) {
      x[i] = arr[left++];
    } else if (left == mid) {
      x[i] = arr[right++];
    } else if (arr[right] < arr[left]) {
      x[i] = arr[right++];
    } else {
      x[i] = arr[left++];
    }
  }

  // transfer from temp array back to provided
  for (i = 0; i < n; i++) {
    arr[i] = x[i];
  }

  free(x);
}

void sort(int *arr, int n, int depth);

void *start(void *args) {
  int *arr   = ((sortargs_t *)args)->arr;
  int  n     = ((sortargs_t *)args)->n;
  int  depth = ((sortargs_t *)args)->depth;
  sort(arr, n, depth);
  return NULL;
}

void sort(int *arr, int n, int depth) {
  if (n < 2) return;

  int mid = n / 2;

  if (depth > MAX_DEPTH) {
    // merge
    sort(arr, mid, depth + 1);
    sort(arr + mid, n - mid, depth + 1);
  } else {
    // child execute left-hand side
    pthread_t  t;
    sortargs_t args = {arr, mid, depth + 1};
    pthread_create(&t, NULL, start, (void *)&args);

    // parent execute right-hand side
    sort(arr + mid, n - mid, depth + 1);

    // wait for child
    pthread_join(t, NULL);
  }

  // merge
  merge(arr, n, mid);
}

int main() {
  int  n   = 1 << 25;
  int *arr = malloc(n * sizeof(int));

  // populate array with n randints
  srand(42);
  for (int i = 0; i < n; i++) {
    arr[i] = rand();
  }

  printf("Sorting %d random integers\n", n);

  sort(arr, n, 0);

  // assert that the output is sorted
  for (int i = 0; i < n - 1; i++) {
    if (arr[i] > arr[i + 1]) {
      printf("error: arr[%d] = %d > arr[%d] = %d", i, arr[i], i + 1, arr[i + 1]);
      return 1;
    }
  }
  return 0;
}
