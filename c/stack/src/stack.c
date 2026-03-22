
/* stack.c */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define ERR "\033[31m"        /* ANSI Red.               */
#define WARN "\033[33m"       /* ANSI Yellow.            */
#define SUCC "\033[32m"       /* ANSI Green.             */
#define BLUE "\033[34m"       /* ANSI Blue.              */
#define CYAN "\033[36m"       /* ANSI Cyan.              */
#define MAGENTA "\033[35m"    /* ANSI Magenta.           */
#define COLOR_RESET "\033[0m" /* RESET Color to Default. */

/* stack
 * 
 * This structured data actually implements the ADT. The field 'vector'
 * stores a pointer to an array of 'stack_key_t' type.
 * The field 'capacity' stores an index representing the fixed dimension
 * of the vector pointed by 'vector'. The 'top' field stores an index
 * representing the number of elements stored in the stack. */
struct stack_t {
  stack_key_t *vector;
  stack_capacity_t capacity;
  stack_top_t top;
};

/* create
 * 
 * This function:
 * (1) allocates memory for a new stack.
 * (2) initializes the vector field with the fixed capacity.
 * (3) initializes the capacity field with the fixed capacity.
 * (4) initializes the top field at zero. */
stack stack_create(size_t cap) {
  stack newstack = (stack) malloc(sizeof(struct stack_t));
  if (!newstack) {
    fprintf(stderr, ERR "error: stack: create:" COLOR_RESET
            " cannot allocate memory for a new stack.");
    exit(1);
  }
  newstack->vector = (stack_key_t *) malloc(cap * sizeof(stack_key_t));
  if (!newstack->vector) {
    fprintf(stderr, ERR "error: stack: create:" COLOR_RESET
            " cannot allocate memory for stack's vector field.")
    exit(1);
  }
  newstack->capacity = cap;
  newstack->top = 0;
  return newstack;
}

/* destroy
 * 
 * This function deallocates memory from a 'stack_t' struct, emptying 
 * it first if not empty. */
void stack_destroy(stack s) {
  if (!s) {
    fprintf(stderr, ERR "error: stack: destroy:" COLOR_RESET
            " cannot deallocate memory from a NULL stack.")
    exit(1);
  } else {
    if (s->vector) free(s->vector);
    free(s);
    return;
  }
}



/* end of stack.c */
