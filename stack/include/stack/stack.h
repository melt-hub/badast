
/* ===========================| ---------------|=============================
 *
 * Stack (Fixed Length Vector)
 *
 * A stack is an ADT that follows the LIFO (Last-In First-Out) property, 
 * meaning that the first nodes push in, is the last to be popped out. 
 *
 * The advantages of such a structure are a constant computational complexity
 * O(1) on most of the basic operations, and some advanced too with the right
 * tweaks. 
 *
 * A common analogy that can be expressed is the one between the LIFO property
 * and the nature of a deck of card, laying faced up on a table,
 * from which a player is only able to pick a card from the top at a time.
 *
 * A stack can be represented as:
 *
 *                        capacity -> [   ]
 *                                    [   ]
 *                                    [   ]
 *                                    [   ]
 *                             top -> [key]
 *                                    [key]
 *                                    [key]
 *                                    [key]
 *                                    [key]
 *                                    [key]
 *                                    [key]
 *                                    [key]
 *                                    [key]
 *                                   
 * This header file provides the declaration of both the structured data and
 * the functions implementing basic operations and algorithms computable on a 
 * stack, such as:
 *
 *   - push(S, x): inserts the node 'x' on top of the stack.
 *   - pop(S): deletes the node on top of the pile and returns its key.
 *   - top(S): return the key stored in the node on top of the pile.
 *   - stack_empty(S): checks if the stack is empty.
 *   - stack_full(S): checks if the stack is full.
 *
 * ===========================| ---------------|============================= */

#ifndef STACK_H
#define STACK_H

#include <stddef.h>

struct stack_t;
typedef struct stack_t *stack;

typedef size_t stack_capacity_t;
typedef size_t stack_top_t;
typedef char stack_key_t;

stack stack_create();
stack stack_destroy();

int stack_push(stack, stack_key_t);

stack_key_t stack_pop(stack);

stack_key_t stack_top(stack);

int stack_empty(stack);
int stack_full(stack);

/* end of stack.h */
