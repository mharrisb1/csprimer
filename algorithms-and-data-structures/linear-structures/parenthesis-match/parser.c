#define _GNU_SOURCE

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "stack.h"

#define ERR_TMPL "%s '%c' (%i:%i)\n"

parser_t *parser_new() {
  parser_t *parser = malloc(sizeof(parser_t));
  parser->state    = OK;
  parser->stack    = stack_new(sizeof(token_t), 1024);
  parser->err_tok  = malloc(sizeof(token_t));
  return parser;
}

void parser_free(parser_t *parser) {
  stack_free(parser->stack);
  free(parser->err_tok);
  free(parser);
}

static void parser_error(parser_t *parser, parser_state_t state, token_t *tok) {
  parser->state = state;
  memcpy(parser->err_tok, tok, sizeof(token_t));
}

void parser_scan(parser_t *parser, FILE *stream) {
  char   *data = NULL;
  size_t  len  = 0;
  ssize_t nread;

  token_t tok, out;
  tok.line = 0;
  tok.col  = 0;

  while ((nread = getline(&data, &len, stream)) != -1 && parser->state == OK) {
    tok.line++;
    tok.col = 0;
    for (size_t i = 0; i < len; i++) {
      tok.col++;
      tok.value = data[i];

      if (parser->state != OK) break;

      switch (tok.value) {
        case '(':
        case '[':
          stack_push(parser->stack, &tok);
          break;
        case ')':
          if (stack_pop(parser->stack, &out)) {
            if (out.value != '(') parser_error(parser, ERR_UNCLOSED, &out);
            break;
          }
          parser_error(parser, ERR_UNOPENED, &tok);
          break;
        case ']':
          if (stack_pop(parser->stack, &out)) {
            if (out.value != '[') parser_error(parser, ERR_UNCLOSED, &out);
            break;
          }
          parser_error(parser, ERR_UNOPENED, &tok);
          break;
        case ';':
        case '\n':
          i = len;
          break;
      }
    }
  }
}

void parser_print_err(parser_t *parser, FILE *stream, const char *filename) {
  char   *data = NULL;
  size_t  len  = 0;
  ssize_t nread;
  int     lineno = 0;

  const char *msg =
      parser->state == ERR_UNOPENED ? "unopened token" : "unclosed token";

  fprintf(stderr, "\033[31merror\033[0m: %s\n", msg);
  fprintf(stderr, "  --> %s:%i:%i\n", filename, parser->err_tok->line,
          parser->err_tok->col);
  fprintf(stderr, "    |\n");

  while ((nread = getline(&data, &len, stream)) != -1) {
    lineno++;
    if (lineno == parser->err_tok->line) {
      fprintf(stderr, "%3i | %s", lineno, data);
      fprintf(stderr, "    | ");
      for (int i = 1; i < parser->err_tok->col; i++)
        fputc(' ', stderr);
      fprintf(stderr, "\033[31m^ %s `%c`\033[0m\n", msg, parser->err_tok->value);
    }
  }

  fprintf(stderr, "    |\n");
  free(data);
}
