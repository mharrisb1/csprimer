#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  ACCEPT,
  CONSUME,
  CONVERT,
  ERROR,
} State;

void process(const char t, State *state, char *buf, char *ix) {
  switch (*state) {

  case ACCEPT:
    if (t == '#') {
      *state = CONSUME;
      *ix = 0;
    } else {
      printf("%c", t);
    }
    break;

  case CONSUME:
    if (t == ';') {
      *state = CONVERT;
    } else if (isxdigit(t)) {
      buf[(*ix)++] = t;
    }
    *state = ERROR;
    break;

  case CONVERT:
    printf("rgb(");
    for (char i = 0; i < 6; i += 2) {
      char h[2] = {buf[i], buf[i + 1]};
      printf("%u", (int)strtol(h, NULL, 16));
      if (i != 4) {
        printf(" ");
      }
    }
    printf(");\n");
    *state = ACCEPT;
    break;

  case ERROR:
    break;
  }
}

int main() {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  State state = ACCEPT;
  char buf[6];
  char ix = 0;

  while ((nread = getline(&line, &len, stdin)) != -1) {
    for (char i = 0; i < nread; i++) {
      process(line[i], &state, buf, &ix);
      if (state == ERROR) {
        printf("ERROR: Failed to parse CSS at token `%c` (col %u of %zd)\n",
               line[i], i + 1, nread);
        return 1;
      }
    }
  }
}