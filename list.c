//
// Created by amirlande on 9/1/2018.
//

#include "list.h"

listOfMoves *allocateMoveList() {
    listOfMoves *list;
    list = (listOfMoves *)malloc(sizeof(listOfMoves));
    if (list == NULL) {
        printMallocFailed();
        return NULL;
    }
    return list;
}

/* Allocates memory to new nodes
 * frees all previous nodes that was next to current node
 * sets the curr and prev pointers
 * -- no data is added -- */
void getNewCurrentMove(gameParams *game) {
    userMoveNode *newPrev;
    userMoveNode *newCurr;

    freeAllUserMoveNodes(game->movesList->currentMove->next);
    newPrev = game->movesList->currentMove;
    newCurr = (userMoveNode *) malloc(sizeof(userMoveNode *));
    newPrev->next = newCurr;
    newCurr->prev = newPrev;
    newCurr->next = NULL;
    newCurr->change = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode *));
    newCurr->change->currVal = (cell *) malloc(sizeof(cell *));
    newCurr->change->next = NULL;
    game->movesList->currentMove = newCurr;
    game->movesList->size++;
}

/* frees all the userMoveNode
 * starting from node to the end */
void freeAllUserMoveNodes(userMoveNode *moveToFree) {
    userMoveNode *nextMove;

    if (moveToFree == NULL) {
        return;
    }
    nextMove = moveToFree->next;
    freeCellChangeRecNode(moveToFree->change);
    freeAllUserMoveNodes(nextMove);
    free(moveToFree);
}

/* frees all the freeCellChangeRecNode
 * starting from change to the end */
void freeCellChangeRecNode(cellChangeRecNode *changeToFree) {
    cellChangeRecNode *nextChange;
    if (changeToFree == NULL) {
        return;
    }

    nextChange = changeToFree->next;
    free(changeToFree->prevVal);
    free(changeToFree->currVal);
    freeCellChangeRecNode(nextChange);
    free(changeToFree);

}