#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_HEX_LEN 8

typedef enum {
  IDLE,
  CONSUME,
  CONVERT,
  ERROR,
  ACCEPT,
} State;

void process(const char t, const char *line, State state, uint8_t *buf,
             uint8_t ix) {
  switch (state) {
  case IDLE:
    if (t == '#') {
      //
    } else {
      printf("%c", t);
    }
    break;
  case CONSUME:
    break;
  case CONVERT:
    break;
  case ERROR:
    break;
  case ACCEPT:
    break;
  }
}

int main() {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while ((nread = getline(&line, &len, stdin)) != -1) {
    State state = IDLE;
    uint8_t buf[MAX_HEX_LEN + 1];
    uint8_t ix = 0;

    for (uint8_t i = 0; i < nread; i++) {
      process(line[i], line, state, buf, ix);
    }
  }
}
