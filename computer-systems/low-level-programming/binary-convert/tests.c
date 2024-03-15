#include <assert.h>
#include <stdio.h>

extern int binary_convert(char *bits);

int main(void) {
  assert(binary_convert("0") == 0);
  assert(binary_convert("1") == 1);
  assert(binary_convert("110") == 6);
  assert(binary_convert("1111") == 15);
  assert(binary_convert("10101101") == 173);
  printf("OK\n");
}
