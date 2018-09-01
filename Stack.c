//
// Created by amirlande on 8/28/2018.
//

#include "Stack.h"

/* Allocates memory for Stack and initializes top->NULL, count->0 */
Stack *initializeStack() {
    Stack *stack;
    stack = (Stack *) malloc(1 * sizeof(Stack)); /* TODO free memory */
    if (stack == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    stack->count = 0;
    stack->top = NULL;

    return stack;
}

/* Allocates memory for Stack element (doesn't initialize its fields!) */
element *allocateMemoryForStackElement() {
    element *newElement;
    newElement = (element *) malloc(1 * sizeof(element));
    if (newElement == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    return newElement;
}

void push(Stack *stack, int row, int col, int value) {
    element *newElement = NULL;
    newElement = allocateMemoryForStackElement();

    /* Set newElement's fields: */
    newElement->row = row;
    newElement->col = col;
    newElement->value = value;

    /* Update stack: */
    newElement->next = stack->top;
    stack->top = newElement;
    (stack->count)++;
}

/* Precondition: stack.isEmpty() == FALSE
 * Returns pointer to top of stack element, user must free memory after using it*/
element *pop(Stack *stack) {
    element *removedElement;
    removedElement = stack->top;
    stack->top = stack->top->next;
    removedElement->next = NULL;
    (stack->count)--;

    return removedElement;
}

/* Returns TRUE (1) if stack->count == 0, FALSE (0) otherwise */
int isEmpty(Stack *stack) {
    if (stack->count == 0) return TRUE;
    else if (stack->count > 0) return FALSE;
    else if (stack->count < 0) {
        printErrorInCodeFlow("isEmpty", "Stack.c");
        return FALSE;
    }
}

#ifdef OldStack
/* Precondition: boardSnapshot is a copy of the board status (its memory already allocated) */
void push(Stack *stack, BOARD boardSnapshot)
{
    element *newStackElement = NULL;
    newStackElement = (element *)malloc(sizeof( element)); /* Allocate memory for the "box", the stack element */
    if (newStackElement == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    newStackElement->currentBoard = boardSnapshot; /* update data */
    newStackElement->next = stack->top;
    stack->top = newStackElement;
    stack->count++;
}

/* Precondition: stack.isEmpty() == FALSE */
BOARD pop(Stack *stack) {
    /* Sanity check (to remove) */
    if (stack->count <= 0) {
        printErrorInCodeFlow("pop", "Stack.c");
    }

    BOARD board;
    element *p = NULL;

    board = stack->top->currentBoard;
    p = stack->top;
    stack->top = stack->top->next;
    stack->count--;
    free(p);
    return board;
}

int isEmpty(Stack *stack) {
    if (stack->count == 0) return TRUE;
    else return FALSE;
}

#endif
