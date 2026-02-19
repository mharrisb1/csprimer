#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct {
  void  *data;
  size_t elem_size;
  size_t count;
  size_t capacity;
} stack_t;

stack_t *stack_new(size_t elem_size, size_t capacity);
void     stack_free(stack_t *stack);

void stack_push(stack_t *stack, const void *elem);
int  stack_pop(stack_t *stack, void *out);
int  stack_is_empty(stack_t *stack);

#endif // STACK_H
