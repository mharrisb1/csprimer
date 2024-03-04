#include <assert.h>
#include <stdio.h>

extern int sum_to_n(int n);

int main(void) {
  assert(sum_to_n(0) == 0);
  assert(sum_to_n(1) == 1);
  assert(sum_to_n(3) == 6);
  assert(sum_to_n(10) == 55);
  assert(sum_to_n(1000) == 500500);
  printf("OK\n");
}
