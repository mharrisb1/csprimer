#include <stdlib.h>
#include <string.h>

#include "stack.h"

stack_t *stack_new(size_t elem_size, size_t capacity) {
  stack_t *stack   = malloc(sizeof(stack_t));
  stack->elem_size = elem_size;
  stack->capacity  = capacity;
  stack->count     = 0;
  stack->data      = calloc(stack->capacity, stack->elem_size);
  return stack;
}

static void stack_reserve(stack_t *stack, size_t capacity) {
  stack->capacity = capacity;
  stack           = realloc(stack, stack->elem_size * stack->capacity);
}

void stack_free(stack_t *stack) {
  free(stack->data);
  free(stack);
}

static int stack_is_full(stack_t *stack) {
  return stack->count == stack->capacity;
}

void stack_push(stack_t *stack, const void *elem) {
  if (stack_is_full(stack)) {
    stack_reserve(stack, stack->capacity * 2);
  }
  char *dest = (char *)stack->data + stack->count++ * stack->elem_size;
  memcpy(dest, elem, stack->elem_size);
}

int stack_pop(stack_t *stack, void *out) {
  if (stack->count == 0) return 0;
  char *src = (char *)stack->data + --stack->count * stack->elem_size;
  memcpy(out, src, stack->elem_size);
  return 1;
}

int stack_is_empty(stack_t *stack) {
  return stack->count == 0;
}
