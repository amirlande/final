//
// Created by eran on 31/07/18.
//

#include <stdlib.h>
#include <stdio.h>
#include "gameUtils.h"
#include "commands.h"
#include "errorMessages.h"
#include "memoryAllocation.h"


/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int checkErrCells(gameParams *game) {

    int i, j, N;
    N = game->N;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            cell *c = game->userBoard[i][j];
            if (c->isValid == FALSE) {
                return TRUE; /* invalid cell found! */
            }
        }
    }
    /* All board cells are valid - therefore checkErrCells returns FALSE */
    return FALSE;
}

/* Allocates memory for a new board and copies values of
 * board_to_be_copied.
 * Returns pointer to the new board struct (Notice - it is a cell ****) */
BOARD*copyBoard(cell ***board_to_be_copied, int N) {
    int i, j;
    BOARD*pointerToBoard;
    BOARD copyOfBoard;

    pointerToBoard = (BOARD*) malloc(sizeof(BOARD));
    copyOfBoard = allocateCellMatrix(N);

    /* Copy cell by cell values: */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            copyOfBoard[i][j]->value = board_to_be_copied[i][j]->value;
            copyOfBoard[i][j]->isFixed = board_to_be_copied[i][j]->isFixed;
            copyOfBoard[i][j]->isValid = board_to_be_copied[i][j]->isValid;
        }
    }
    *pointerToBoard = copyOfBoard;
    printNotImplementedMessage("copyBoard"); /* TODO remove this when we fix allocateCellMatrix */
    return pointerToBoard;
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

/* "Constructor" - creates a cell with the passed value. By default new cells are valid and no fixed TODO */
cell *createCell(int value) {
    cell *newCell = (cell *) malloc(sizeof(cell));
    if (newCell == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    newCell->value = value;
    newCell->isValid = TRUE;
    newCell->isFixed = FALSE;
    return newCell;
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
    newCurr->next = NULL;
    newCurr->change = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode *));
    newCurr->change->currVal = (cell *) malloc(sizeof(cell *));
    newCurr->change->next = NULL;
    game->movesList->currentMove = newCurr;
    game->movesList->size++;
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

        /* switching values of prev and curr at redo */
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
int checkIfSquareValid(int x, int y, int z, gameParams *game) {

    int i, j, m, n;
    m = game->m;
    n = game->n;

    for (i = x - x % m; i < x - x % m + m; i++) {
        for (j = y - y % n; j < y - y % n + n; j++) {

            if (game->userBoard[i][j]->value == z) {
                if (!((i == x) && (j == y))) { /* exclude cell (x,y) from the square check */
                    return 0;
                }
            }
        }
    }
    return 1;
}

/* Checks if value z does not appear in row x */
int checkIfRowValid(int x, int y, int z, gameParams *game) {

    int j, N;
    N = game->n * game->m;

    for (j = 0; j < N; j++) {
        if (j != y) { /* exclude cell (x,y) from the square check */
            if (game->userBoard[x][j]->value == z) {
                return 0;
            }
        }
    }


    return 1;
}

/* Checks if value z does not appear in column y */
int checkIfColumnValid(int x, int y, int z, gameParams *game) {


    int i, N;
    N = game->n * game->m;

    for (i = 0; i < N; i++) {
        if (i != x) { /* exclude cell (x,y) from the square check */
            if (game->userBoard[i][y]->value == z) {
                return 0;
            }
        }
    }

    return 1;
}

/* Returns the only legal value
 * for the empty Cell [x][y]
 * returns FALSE - iff has 0, or more than 1 values */
int doesCellHasASingleLegalValue(gameParams *game, int x, int y) {

    int i, counter, N, value;
    N = game->n * game->m;
    counter = 0;
    for (i = 1; i < N + 1; i++) {
        if (checkIfValid(x, y, i, game)) {
            counter++;
            value = i;
            if (counter > 1) { return FALSE; }
        }
    }

    if (counter == 1) {
        return value;
    }
    return FALSE;

}


/* sets a new value z to cell [x][y] */
void setValue(gameParams *game, int x, int y, int z) {


    /* according to z value - increment or decrement game counter
    * if z was already set to (x,y) cell - don't change counter */
    if ((z == 0) && (game->userBoard[x][y] != 0)) { /* when a non-zero cell is set back to zero (emptied) */
        game->counter--;
    } else if ((z != 0) && (game->userBoard[x][y] == 0)) { /* when a zero cell is set to z (!=0) */
        game->counter++;
    }

    /* sets the value */
    game->userBoard[x][y]->value = z;
    game->userBoard[x][y]->isValid = checkIfValid(x, y, z, game);
    game->userBoard[x][y]->isFixed = 0;
}


/* Called by autoFill
 * returns the list of changes */
cellChangeRecNode *getAutoFillChangeList(gameParams *game, int *numOfChanges) {
    int i, j, N, legalValue, changes;
    cellChangeRecNode *changeListHead, *currentChange;
    currentChange = NULL;
    changeListHead = NULL;
    N = game->n * game->m;
    changes = 0;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            legalValue = doesCellHasASingleLegalValue(game, i, j);
            if (legalValue != FALSE) {
                if (changes == 0) {
                    /* keep the first node */
                    changeListHead = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode *));
                    currentChange = changeListHead;
                } else {
                    currentChange->next = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode *));
                    currentChange = currentChange->next;
                }
                currentChange->prevVal = game->userBoard[i][j];
                game->userBoard[i][j] = (cell *) malloc(sizeof(cell *));
                setValue(game, i, j, legalValue);
                currentChange->currVal = game->userBoard[i][j];
                currentChange->x = i + 1;
                currentChange->y = j + 1;
                currentChange->next = NULL;
                changes++;
                printf("Cell <%d,%d> set to %d\n", i + 1, j + 1, legalValue);
            }
        }
    }

    *numOfChanges = changes;
    return changeListHead;
}

/* Called by autoFill */
void setNewChangeListToGame(gameParams *game, cellChangeRecNode *changeListHead) {

    userMoveNode *newMove;
    freeAllUserMoveNodes(game->movesList->currentMove->next);
    newMove = (userMoveNode *) malloc(sizeof(userMoveNode *));
    newMove->prev = game->movesList->currentMove;
    newMove->next = NULL;
    newMove->change = changeListHead;
    game->movesList->currentMove->next = newMove;
    game->movesList->currentMove = newMove;
    game->movesList->size++;
}


/* gets a gameParams instance after one malloc */
int createNewGame(gameParams *game, int n, int m) {
// TODO : to be tested
    game->n = n;
    game->m = m;
    game->N = m * n;
    game->markErrors = 0;
    game->counter = 0;
    /* TODO Amir changed the call to allocateCellMatrix */
    allocateCellMatrix(game->N);
    allocateCellMatrix(game->N);
    game->movesList = (listOfMoves *) malloc(sizeof(listOfMoves *));
    if (game->movesList == NULL) {
        printf("Error: malloc has failed\n");
        free(game->movesList);
        return 0;
    }
    game->movesList->size = 0;
    game->movesList->head = (userMoveNode *) malloc(sizeof(userMoveNode *));
    if (game->movesList->head == NULL) {
        printf("Error: malloc has failed\n");
        free(game->movesList->head);
        return 0;
    }
    game->movesList->head->prev = NULL;
    game->movesList->head->next = NULL;
    game->movesList->head->change = NULL;
    game->movesList->currentMove = game->movesList->head;


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