#include <assert.h>

extern int is_valid(char *digits);

int main() {
  // valid credit card numbers
  assert(is_valid("4532015112830366") == 1);
  assert(is_valid("4916239364668151") == 1);
  assert(is_valid("6011514433546201") == 1);
  assert(is_valid("379354508162306") == 1);

  // Invalid credit card numbers
  assert(is_valid("4532015112830367") == 0);
  assert(is_valid("4916239364668159") == 0);
  assert(is_valid("6011514433546202") == 0);
  assert(is_valid("379354508162307") == 0);
}
