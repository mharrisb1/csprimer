#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const int table[10] = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};

void reverse(char *s, int len) {
  int left  = 0;
  int right = len - 1;
  while (left < right) {
    char temp = s[left];
    s[left]   = s[right];
    s[right]  = temp;
    left++;
    right--;
  }
}

int ctoi(char c) { return c ^ 0x30; }

int is_valid(char *digits) {
  int n   = 0;
  int len = strlen(digits);

  char *s = malloc((len + 1) * sizeof(char));
  strcpy(s, digits);
  reverse(s, len);

  int check = ctoi(s[0]);

  for (int i = 1; i < len; i++) {
    int t = ctoi(s[i]);
    n += i % 2 == 0 ? t : table[t];
  }

  free(s);

  return check == 10 - (n % 10);
}

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
