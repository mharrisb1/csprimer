#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MASK 0x07fffffe

bool ispangram(char *s) {
  uint32_t bs = 0;
  char c;
  while ((c = *s++) != '\0') {
    if (c < '@')
      continue; // ignore first 64 chars in ascii table
    bs |= 1 << (c & 0x1f);
  }
  return (bs & MASK) == MASK;
}

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
