//
// Created by amirlande on 9/1/2018.
//

#ifndef FINAL_LIST_H
#define FINAL_LIST_H

#include <stdlib.h>
#include "gameParams.h"
#include "errorMessages.h"

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
    int size; /* TODO maybe unnecessary - to be decided later */
} listOfMoves;

listOfMoves *allocateMoveList();

/* Allocates memory to new nodes
 * sets the curr and prev pointers
 * -- no data is added -- */
void getNewCurrentMove(gameParams *game);

/* frees all the userMoveNode
 * starting from node to the end */
void freeAllUserMoveNodes(userMoveNode *moveToFree);

/* frees all the freeCellChangeRecNode
 * starting from change to the end */
void freeCellChangeRecNode(cellChangeRecNode *changeToFree);


#endif //FINAL_LIST_H
