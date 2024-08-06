#include <assert.h>
#include <stdint.h>
#include <string.h>

static const uint8_t table[10] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};

int is_valid(char *digits) {
  uint8_t b, c;
  uint8_t i, k;
  uint16_t n = 0;

  i = k = strlen(digits) - 1;
  c     = digits[i] ^ 0x30;

  while (i--) {
    b = digits[i] ^ 0x30;
    n += (k - i) % 2 == 1 ? table[b] : b;
  }

  return c == 10 - (n % 10);
}

int main() {
  // Valid credit card numbers
  assert(is_valid("4532015112830366") == 1);
  assert(is_valid("4916239364668151") == 1);
  assert(is_valid("6011514433546201") == 1);
  assert(is_valid("379354508162306") == 1);

  // Invalid credit card numbers
  assert(is_valid("4532015112830367") == 0);
  assert(is_valid("4916239364668159") == 0);
  assert(is_valid("6011514433546202") == 0);
  assert(is_valid("379354508162307") == 0);

  // Edge cases: valid numbers with varying lengths
  assert(is_valid("79927398713") == 1); // 11 digits

  // Edge cases: invalid numbers with varying lengths
  assert(is_valid("1234567812345678") == 0); // 16 digits
  assert(is_valid("79927398710") == 0);      // 11 digits

  // Additional edge cases
  assert(is_valid("0000000000000000") == 0); // All zeroes
  assert(is_valid("4111111111111111") == 1); // Common test number
}
