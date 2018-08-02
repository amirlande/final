//
// Created by eran on 31/07/18.
//

/* cellChangeRecNode is a node of a single-linked list of all set operations
 * made by the user - in case of a "SET" operation the list includes only one node
 * in case of a "AUTOFILL" operation the list includes a cellChangeRecNode for each
 * cell that has been automatically set, with its previous value and current value*/
typedef struct cellChangeRecNode {
    int x; /*x coordinate of cell*/
    int y; /*y coordinate of cell*/
    int prevVal;
    int currVal;
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
 * currentMove is a pointer to the last move made by the user*/
typedef struct listOfMoves {
    userMoveNode *head;
    userMoveNode *currentMove;
    int size; /* maybe unnecessary - to be decided later */
} listOfMoves;

enum gameMode {init, solve, edit};
/* the main struct that represents the sudoku game */
typedef struct gameParams {
    enum gameMode mode;
    int markErrors;
    listOfMoves movesList;
    int **userBoard;
    int **solution;
    int **fixedValues;
    int counter;
} gameParams;




#ifndef FINAL_COMMANDS_H
#define FINAL_COMMANDS_H

#endif //FINAL_COMMANDS_H
