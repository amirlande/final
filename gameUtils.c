//
// Created by eran on 31/07/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include "gameUtils.h"
#include "commands.h"


/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int checkErrCells(gameParams *game) {

    int i, j, N;
    N = game->m * game->n;
    for (i = 0; i < N; i++) {
        for (j = 0; j++; j < 0) {
            cell *c = game->userBoard[i][j];
            if (c->isValid == FALSE) {
                return 1;
            }
        }
    }

    return 0;
}


/* allocates memory for a new board and copies values of
 * board_to_be_copied. returns pointer to the new board struct*/
cell **copyBoard(cell **board_to_be_copied) {
    /* to be implemented */
}


/* returns the line separator for print_board
 * consists 4N+m+1 dashes ('-')
 * exits with exit(0) if failed to malloc */
char *getLineSeparator(gameParams *game) {
    char *separator;
    int N, n, m, i;
    n = game->n;
    m = game->m;
    N = m * n;

    separator = malloc(sizeof(char) * (4 * N + m + 1));
    if (separator == NULL) {
        printf("Error: malloc has failed\n");
        exit(0);
    }

    for (i = 0; i < 4 * N + m + 1; i++) {
        separator[i] = '-';
    }
    separator[i] = '\0';


    return separator;
}


/* Allocates memory for cell matrix mat with NxN values */
cell ***allocateCellMatrix(cell ***mat, int N) {

    int i, j;
    mat = (cell ***) malloc(N * sizeof(cell ***));
    if (mat == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
    for (i = 0; i < N; i++) {
        mat[i] = (cell **) malloc(N * sizeof(cell **));
        if (mat[i] == NULL) {
            printf("Error: calloc has failed\n");
            exit(0);
        }
        for (j = 0; j < N; j++) {
            mat[i][j] = (cell *) malloc(N * sizeof(cell *));
        }
    }

    return mat;
}


/* Frees memory for cell matrix mat with NxN values */
void freeCellMatrix(cell ***mat, int N) {

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


int find_first_empty_cell(cell **board, int *row, int *col) {
    /* to implement */
    return 1;
}

/* Allocates memory to new nodes
 * frees all previous nodes that was next to current node
 * sets the curr and prev pointers
 * -- no data is added -- */
void getNewCurrentMove(gameParams *game) {

    freeAllUserMoveNodes(game->movesList->currentMove->next);
    userMoveNode *newPrev = game->movesList->currentMove;
    userMoveNode *newCurr = (userMoveNode *) malloc(sizeof(userMoveNode *));
    newPrev->next = newCurr;
    newCurr->prev = newPrev;
    newCurr->change = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode *));
    game->movesList->currentMove = newCurr;
    game->movesList->size++;
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

/* Checks if Z is a valid value for non-fixed cell <X,Y> */
int checkIfValid(int x, int y, int z, gameParams *game) {

    if (z == 0) return 1; /* always legal to set a non-fixed cell to 0 */
    if (checkIfSquareValid(x, y, z, game) == FALSE) {
        return 0;
    }

    if (checkIfRowValid(x, y, z, game) == FALSE) {
        return 0;
    }

    if (checkIfColumnValid(x, y, z, game) == FALSE) {
        return 0;
    }

    return 1;

}


/* prints the changes after undo/redo */
int printChanges(gameParams *game, cellChangeRecNode *moveToPrint, int isRedo) {
    int curr, prev, tmp;
    char *command;


    while (moveToPrint != NULL) {
        curr = moveToPrint->currVal->value;
        prev = moveToPrint->prevVal->value;

        /* switching vlues of prev and curr at redo */
        if (isRedo) {
            tmp = curr;
            curr = prev;
            prev = tmp;
        }

        command = isRedo ? "Redo" : "Undo";
        printf("%s %d,%d: ", command, moveToPrint->x, moveToPrint->y);


        if (!curr) { // curr is zero
            if (!prev) { // both zeros
                printf("from _ to _\n");
            } else { // curr zero, prev non zero
                printf("from _ to %d\n", prev);
            }
        } else { // curr is non zero
            if (!prev) { // prev is zero
                printf("from %d to _\n", curr);
            } else { // both non zeros
                printf("from %d to %d\n", curr, prev);
            }
        }
        moveToPrint = moveToPrint->next;
    }
    return 1;

}

/* Checks if value z does not appear his 3x3 square in the matrix */
int checkIfSquareValid(int x, int y, int z, gameParams **game) {

// TODO: change from prev implementation

#if 0
    int i;
    int j;

    for (i = x - x % 3; i < x - x % 3 + 3; i++) {
        for (j = y - y % 3; j < y - y % 3 + 3; j++) {

            if (userBoard[i][j] == z) {
                if (!((i == x) && (j == y))) { /* exclude cell (x,y) from the square check */
                    return 0;
                }
            }
        }
    }

#endif

    return 1;
}

/* Checks if value z does not appear in row x */
int checkIfRowValid(int x, int y, int z, gameParams **game) {

    // TODO: change from prev implementation
#if 0

    int j;

    for (j = 0; j < 9; j++) {
        if (j != y) { /* exclude cell (x,y) from the square check */
            if (userBoard[x][j] == z) {
                return 0;
            }
        }
    }

#endif
    return 1;
}

/* Checks if value z does not appear in column y */
int checkIfColumnValid(int x, int y, int z, gameParams **game) {

    // TODO: change from prev implementation
#if 0

    int i;

    for (i = 0; i < 9; i++) {
        if (i != x) { /* exclude cell (x,y) from the square check */
            if (userBoard[i][y] == z) {
                return 0;
            }
        }
    }

#endif
    return 1;
}


#if 0
/* Called by undo
 * implemented recursively for printing in thr right order:
 * make changes -> print board -> print changes
 * prints at the opposite order, MIGHT NOT BE USED!!
 * */
int makeRecChanges(gameParams *game, cellChangeRecNode *moveToUndo) {

    if (moveToUndo == NULL) {
        printBoard(game);
        return 1;
    }

    game->userBoard[moveToUndo->x - 1][moveToUndo->y - 1] = moveToUndo->prevVal;
    makeRecChanges(game, moveToUndo->next);
    printf("Undo %d,%d: from %d to %d\n", moveToUndo->x, moveToUndo->y, moveToUndo->currVal->value,
           moveToUndo->prevVal->value);

    return 1;
}
#endif