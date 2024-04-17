/*
 * INSTRUCTIONS
 *
 * Consider the code below, and ask yourself the following questions:
 *
 * - How many memory accesses does it take to compute the sum of nums?
 * - How many cache hits/misses do you expect to see for ordered vs random
 * access?
 * - What would change if you:
 *   - reordered the fields of the struct?
 *   - removed `foo` and `bar`?
 *   - changed `value` to a short?
 * - Are there certain values of N that would substantially change your
 * calculations?
 * - Whatever else you find interesting!
 *
 * Try to form concrete hypotheses, and compare to the results you see in perf
 * and/or cachegrind. Note: if the cachegrind simulation takes too long, try
 * lower values of n. Also, you may want to compile at -O1 to avoid redundant
 * memory access at -O0.
 *
 * Advanced/strech goal: approximate the expected execution time of both ordered
 * and random, and try to explain any differences you see with actual
 * measurements!
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

