
#ifndef FINAL_STACK_H
#define FINAL_STACK_H

#include "gameUtils.h"
#include "errorMessages.h"
#include <stdlib.h>

typedef struct element {
    int row;
    int col;
    int value;
    struct element *next;
} element;

typedef struct Stack {
    int count;
    element *top;
} Stack;

/* Allocates memory for Stack and initializes top->NULL, count->0 */
Stack *initializeStack();

/* Allocates memory for Stack element (doesn't initialize its fields!) */
element *allocateMemoryForStackElement();

/* Returns TRUE (1) on success or FALSE (0) on failure */
void push(Stack *stack, int row, int col, int value);

/* Precondition: stack.isEmpty() == FALSE */
element *pop(Stack *stack);

/* Returns TRUE (1) if stack->count == 0, FALSE (0) otherwise */
int isEmpty(Stack *stack);


#endif
