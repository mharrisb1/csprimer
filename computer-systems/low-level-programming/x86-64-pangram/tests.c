#include <assert.h>
#include <stdio.h>

extern int pangram(char *);

int main(void) {
  assert(pangram("") == 0);
  assert(pangram("abcdefghijklmnopqrstuvwxyz") == 1);
  assert(pangram("the quick brown fox jumps over teh lazy dog") == 1);
  assert(pangram("abc, def! ghi... jkl25; mnopqrstuvwxyz") ==
         1);                                          // ignore punctuation
  assert(pangram("abcdefghijklmnopqrstuvwxy") == 0);  // incomplete
  assert(pangram("ABCdefGHIjklMNOpqrSTUvwxYZ") == 1); // mixed case
  assert(pangram("!bcdefghijklmnopqrstuvwxyz") ==
         0); // close-match symbols should not be false positive
  assert(pangram("\1bcdefghijklmnopqrstuvwxyz") ==
         0); // close-match control code should not be false positive
  assert(pangram("\7abcdefghijklmnopqrstuvwxyz") ==
         1); // other control codes are fine
  printf("OK\n");
}
