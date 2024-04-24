/*
 * Analysis
 * --------
 *
 * Architecture:           aarch64
 * L1d                     128KB
 * LEVEL1_DCACHE_LINESIZE  64B
 * L2                      1MB
 *
 * Estimations:
 *   - `number` struct size = 12B
 *   - n = 1 << 26 = 2^6 * 2^20 ~= 64 M
 *   - total data size ~= 750MB
 *   - ~5 structs per line (64 // 12)
 *   - memory access per invocation of sum = 2 * n ~= 130 M
 *   - ordered access = (1/16 * 65 M) + (1/5 * 65 M) ~= 17 M misses
 *   - random access = (1/16 * 65 M) + (5/5 * 65 M) ~= 69 M misses
 *
 * Results:
 *
 *              Baseline      Ordered*     Random*
 *
 * L1 loads     6,270 M         136 M      148 M
 * L1 misses       94 M           4 M       67 M
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct number {
  short foo;
  int value;
  short bar;
};

int sum(struct number nums[], int indexes[], int n) {
  int idx, total = 0;

  for (int i = 0; i < n; i++) {
    idx = indexes[i];
    total += nums[idx].value;
  }
  return total;
}

void shuffle(int *array, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = n - 1; i > 0; i--) {
      size_t j = (unsigned int)(drand48() * (i + 1));
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

int main(int argc, char **argv) {
  srand(1);
  int i, n = (1 << 26);
  int *ordered = malloc(n * sizeof(int));
  int *random = malloc(n * sizeof(int));
  struct number *nums = malloc(n * sizeof(struct number));
  // construct arrays of indices
  for (i = 0; i < n; i++) {
    ordered[i] = i;
    random[i] = i;
  }
  shuffle(random, n);
  // pick random numbers to sum
  for (i = 0; i < n; i++)
    nums[i].value = rand();
  if (argc == 2 && strcmp(argv[1], "baseline") == 0)
    exit(0);
  clock_t start, end;
  if (argc == 1 || strcmp(argv[1], "ordered") == 0) {
    start = clock();
    int sum1 = sum(nums, ordered, n);
    end = clock();
    printf("Ordered: %d in %0.3fs\n", sum1,
           (double)(end - start) / CLOCKS_PER_SEC);
  }
  if (argc == 1 || strcmp(argv[1], "random") == 0) {
    start = clock();
    int sum2 = sum(nums, random, n);
    end = clock();
    printf("Random:  %d in %0.3fs\n", sum2,
           (double)(end - start) / CLOCKS_PER_SEC);
  }
}
