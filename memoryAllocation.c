//
// Created by amirlande on 8/28/2018.
//

#include "memoryAllocation.h"


/* frees all game components */
int freeGame(gameParams *game) {

    if (game->userBoard != NULL) {
        freeBoard(game->userBoard, game->m * game->n);
    }
    if (game->solution != NULL) {
        freeBoard(game->solution, game->m * game->n);
    }
    if (game->movesList != NULL) {
        freeAllUserMoveNodes(game->movesList->head);
        free(game->movesList);
    }
    free(game);

    return 1;
}

/* Frees memory for cell matrix mat with NxN values */
void freeBoard(cell ***mat, int N) {

    int i, j;
    if (mat == NULL) {
        return;
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            free(mat[i][j]);
        }
        free(mat[i]);
    }
    free(mat);
}

/* frees all the userMoveNode
 * starting from node to the end */
void freeAllUserMoveNodes(userMoveNode *moveToFree) {

    if (moveToFree == NULL) {
        return;
    }
    userMoveNode *nextMove = moveToFree->next;
    freeCellChangeRecNode(moveToFree->change);
    freeAllUserMoveNodes(nextMove);
    free(moveToFree);
}

/* frees all the freeCellChangeRecNode
 * starting from change to the end */
void freeCellChangeRecNode(cellChangeRecNode *changeToFree) {

    if (changeToFree == NULL) {
        return;
    }

    cellChangeRecNode *nextChange = changeToFree->next;
    free(changeToFree->prevVal);
    free(changeToFree->currVal);
    freeCellChangeRecNode(nextChange);
    free(changeToFree);

}

/* Allocates memory for cell matrix mat with NxN values
 * This call allocated memory for all cells, and it initializes each cell's fields to:
 * cell->value = 0
 * cell->isValid = TRUE (1)
 * cell-isFixed = FALSE (0)*/
cell ***allocateCellMatrix(int N) {
    /* TODO Eran - we need to change this function (a bit) after you agree with me */

    int i, j;
    cell ***mat;
    mat = (cell ***) malloc(N * sizeof(cell **));
    if (mat == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < N; i++) {
        mat[i] = (cell **) malloc(N * sizeof(cell *));
        if (mat[i] == NULL) {
            printMallocFailed();
            exit(EXIT_FAILURE);
        }
        for (j = 0; j < N; j++) {
            mat[i][j] = createCell(0);
        }
    }
    return mat;
}


