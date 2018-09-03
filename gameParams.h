//
// Created by amirlande on 9/1/2018.
//

#ifndef FINAL_GAMEPARAMS_H
#define FINAL_GAMEPARAMS_H

#include <stdlib.h>


/* the struct that represents a cell in the Sudoku board
 * each cell contains information regarding its value, whether it is fixed and whether it is valid*/
typedef struct cell {
    int value;
    int isFixed;
    int isValid; /* isValid == 1 means it's not erroneous, isValid == 0 means value is erroneous */
} cell;


enum gameMode {
    INIT_MODE,
    SOLVE_MODE,
    EDIT_MODE
};

/* cellChangeRecNode is a node of a single-linked list of all set operations
 * made by the user - in case of a "SET" operation the list includes only one node
 * in case of a "AUTOFILL" operation the list includes a cellChangeRecNode for each
 * cell that has been automatically set, with its previous value and current value*/
typedef struct cellChangeRecNode {
    int x; /*x coordinate of cell*/
    int y; /*y coordinate of cell*/
    cell *prevVal;
    cell *currVal;
    struct cellChangeRecNode *next; /*pointer to next node*/
} cellChangeRecNode;


/* a node of the doubly linked list listOfMoves
 * each node's "data" is a pointer to a cellChangeRecNode
 * (which is the first node of a linked list of cellChangeRecNode) */
typedef struct userMoveNode {
    cellChangeRecNode *change;
    struct userMoveNode *next;
    struct userMoveNode *prev;
} userMoveNode;


/* a doubly linked list of nodes of type userMoveNode
 * head is pointer to the head of the list
 * head of list is an empty node which is not counter at size
 * currentMove is a pointer to the last move made by the user*/
typedef struct listOfMoves {
    userMoveNode *head;
    userMoveNode *currentMove;
    int size;
} listOfMoves;


/* the main struct that represents the Sudoku game */
typedef struct gameParams {
    enum gameMode mode;
    int markErrors;
    int n;
    int m;
    int N;
    cell ***userBoard;
    cell ***solution;
    int counter;
    listOfMoves *movesList;
} gameParams;





#endif //FINAL_GAMEPARAMS_H
