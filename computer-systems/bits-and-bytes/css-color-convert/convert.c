#include <bits/time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
  ACCEPT,
  CONSUME,
  CONVERT,
  ERROR,
} State;

#ifdef USE_EXTERN

extern void process_token(const char t, State *state, char *buf, char *ix);

#else

void process_token(const char t, State *state, char *buf, char *ix) {
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
    } else {
      *state = ERROR;
    }
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

#endif

int main() {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  State state = ACCEPT;
  char buf[6];
  char ix = 0;

  struct timespec start, end;
  double total_time = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);

  while ((nread = getline(&line, &len, stdin)) != -1) {
    for (char i = 0; i < nread; i++) {
      process_token(line[i], &state, buf, &ix);
      if (state == ERROR) {
        printf("ERROR: Failed to parse CSS at token `%c` (col %u of %zd)\n",
               line[i], i + 1, nread);
        return 1;
      }
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  total_time =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  printf("Total execution time: %f seconds\n", total_time);
  return 0;
}
