#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "stack.h"

typedef struct {
  char value;
  int  line;
  int  col;
} token_t;

typedef enum { OK, ERR_UNOPENED, ERR_UNCLOSED } parser_state_t;

typedef struct {
  stack_t       *stack;
  parser_state_t state;
  token_t       *err_tok;
} parser_t;

parser_t *parser_new();
void      parser_free(parser_t *parser);
void      parser_scan(parser_t *parser, FILE *stream);
void      parser_print_err(parser_t *parser, FILE *stream, const char *filename);

#endif // PARSER_H
