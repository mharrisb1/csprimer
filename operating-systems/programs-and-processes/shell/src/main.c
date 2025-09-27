#include <stdio.h>
#include <stdlib.h>

#include "readline.h"

int main(void) {
  char *line;

  while ((line = readline("> ")) != NULL) {
    printf("%s\n", line);
    free(line);
  }
}
