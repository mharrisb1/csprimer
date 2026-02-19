#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(int argc, char *argv[]) {
  FILE *stream;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  stream = fopen(argv[1], "r");
  if (stream == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  long pos = ftell(stream);

  parser_t *parser = parser_new();
  parser_scan(parser, stream);

  if (parser->state != OK) {
    fseek(stream, pos, SEEK_SET);
    parser_print_err(parser, stream, argv[1]);
    exit(EXIT_FAILURE);
  }

  parser_free(parser);

  fclose(stream);
  exit(EXIT_SUCCESS);
}
