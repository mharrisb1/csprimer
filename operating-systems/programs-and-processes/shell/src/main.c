#include <stdio.h>
#include <stdlib.h>

#include "readline.h"

#define MAXARG 256
#define MAXBUF 4096

static const char *const PROMPT = "> ";

int main(void) {
  char buf[MAXBUF];
  while ((readline(PROMPT, buf)) == 0) {
    printf("%s\n", buf);
  }
}
