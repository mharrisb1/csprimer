#include <stddef.h>
#include <stdio.h>
#define TESTS 1

typedef struct {
  const int *arr;
  size_t     len;
  int        target;
  int        expected;
} test_case_t;

static const int arr1[]  = {1, 3, 5, 7, 9};
static const int arr2[]  = {10, 20, 30, 40, 50};
static const int arr3[]  = {2};
static const int arr4[]  = {};
static const int arr5[]  = {1, 2, 3, 4, 5, 6};
static const int arr6[]  = {1, 2, 3, 4, 5, 6};
static const int arr7[]  = {-10, -5, 0, 5, 10};
static const int arr8[]  = {1, 1, 1, 1, 1};
static const int arr9[]  = {1, 2, 3, 4, 5};
static const int arr10[] = {1, 3, 5, 7, 9, 11, 13};

static const test_case_t tests[] = {
    // Basic cases
    {arr1, 5, 5, 2},
    {arr1, 5, 1, 0},
    {arr1, 5, 9, 4},
    {arr1, 5, 4, -1},

    // Even-length array
    {arr2, 5, 30, 2},
    {arr2, 5, 10, 0},
    {arr2, 5, 50, 4},
    {arr2, 5, 60, -1},

    // Single element array
    {arr3, 1, 2, 0},
    {arr3, 1, 1, -1},

    // Empty array
    {arr4, 0, 5, -1},

    // Target smaller or larger than all elements
    {arr5, 6, 0, -1},
    {arr5, 6, 7, -1},

    // Negative numbers
    {arr7, 5, -5, 1},
    {arr7, 5, 0, 2},
    {arr7, 5, 6, -1},

    // Duplicate values (any valid index acceptable)
    {arr8, 5, 1, 2},

    // Boundary checks
    {arr9, 5, 1, 0},
    {arr9, 5, 5, 4},

    // Larger odd-length array
    {arr10, 7, 7, 3},
    {arr10, 7, 13, 6},
    {arr10, 7, 2, -1},
};

static int num_tests = sizeof(tests) / sizeof(test_case_t);

int binary_search(const int *arr, size_t size, int target) {
  int l = 0, r = size;
  while (l < r) {
    int m = (r + l) / 2;
    if (target < arr[m]) {
      r = m;
    } else if (target > arr[m]) {
      l = m + 1;
    } else {
      return m;
    }
  }
  return -1;
}

int main() {
  for (int i = 0; i < num_tests; i++) {
    int actual = binary_search(tests[i].arr, tests[i].len, tests[i].target);
    if (actual != tests[i].expected) {
      printf("Failed test %i. Got %i, expected %i\n", i, actual,
             tests[i].expected);
    }
  }
}
