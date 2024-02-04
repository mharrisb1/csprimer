#include <assert.h>
#include <stdint.h>

int bitcount(uint32_t n) {
  int count;
  count = 0;
  while (n > 0) {
    if ((n & 0x1) == 0x1) {
      count++;
    }
    n >>= 1;
  };
  return count;
}

int main(void) {
  assert(bitcount(0) == 0); // 0b0000
  assert(bitcount(1) == 1); // 0b0001
  assert(bitcount(2) == 1); // 0b0010
  assert(bitcount(3) == 2); // 0b0011
  assert(bitcount(4) == 1); // 0b0100
  assert(bitcount(5) == 2); // 0b0101
  assert(bitcount(6) == 2); // 0b0110
  assert(bitcount(7) == 3); // 0b0111
  assert(bitcount(8) == 1); // 0b1000
  assert(bitcount(0xffffffff) == 32);
}
