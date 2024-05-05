#include <stdint.h>
#include <stdio.h>

extern void varint_encode(uint8_t *buf, uint64_t i);

extern uint64_t varint_decode(uint8_t *buf);

int main() {
  for (uint8_t p = 0; p <= 64; p++) {
    uint8_t buf[10] = {0};
    uint64_t i = (1 << p) - 1;

    varint_encode(buf, i);
    uint64_t decoded = varint_decode(buf);

    if (i != decoded) {
      printf("Failed to encode/decode for `%ld`\n", i);
      return 1;
    }
  }
  printf("Ok\n");
  return 0;
}
