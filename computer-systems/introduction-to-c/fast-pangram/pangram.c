#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool ispangram(char *s) {
  int check = 0;
  for (int i = 0; i < strlen(s); i++) {
    if (isalpha(tolower(s[i]))) {
      check |= (1 << ((int)s[i] - 97));
    }
  }
  return check == 0x3FFFFFF; // all 26 of the LSBs should be flipped on if pangram
};

int main() {
  size_t len;
  ssize_t read;
  char *line = NULL;
  while ((read = getline(&line, &len, stdin)) != -1) {
    if (ispangram(line))
      printf("%s", line);
  }

  if (ferror(stdin))
    fprintf(stderr, "Error reading from stdin");

  free(line);
  fprintf(stderr, "ok\n");
}
