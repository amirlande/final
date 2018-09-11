
#include "gameUtils.h"


/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int hasErrCells(gameParams *game) {
    int i, j, N;
    cell *c;

    N = game->N;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c = game->userBoard[i][j];
            if (c->isValid == FALSE) {
                return TRUE; /* invalid cell found! */
            }
        }
    }
    /* All board cells are valid - therefore hasErrCells returns FALSE */
    return FALSE;
}

int boardIsEmpty(gameParams *game) {
    int i, j, N;
    cell *c;

    N = game->N;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            c = game->userBoard[i][j];
            if (c->value != EMPTY) {
                return FALSE; /* found an assigned cell - board is not empty*/
            }
        }
    }
    return TRUE;
}

/* Allocates memory for a new board and copies values of
 * board_to_be_copied.
 * Returns pointer to the new board struct (Notice - it is a cell ****) */
cell ***copyBoard(cell ***board_to_be_copied, int N) {
    int i, j;
    cell ***copyOfBoard;

    copyOfBoard = allocateCellMatrix(N);

    /* Copy cell by cell values: */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            copyOfBoard[i][j]->value = board_to_be_copied[i][j]->value;
            copyOfBoard[i][j]->isFixed = board_to_be_copied[i][j]->isFixed;
            copyOfBoard[i][j]->isValid = board_to_be_copied[i][j]->isValid;
        }
    }
    return copyOfBoard;
}

/* returns the line separator for print_board
 * consists 4N+m+1 dashes ('-')
 * exits with exit(0) if failed to malloc */
char *getLineSeparator(gameParams *game) {
    char *separator;
    int N, m, i;
    m = game->m;
    N = game->N;

    separator = malloc(sizeof(char) * (4 * N + m + 1) + 1);
    if (separator == NULL) {
        freeSudokuGame(game);
        printMallocFailed();
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 4 * N + m + 1; i++) {
        separator[i] = '-';
    }
    separator[i] = '\0';

    return separator;
}


/* Allocates memory to new nodes
 * frees all previous nodes that were next to current node
 * sets the curr and prev pointers
 * -- no data is added -- */
void getNewCurrentMove(gameParams *game) {

    userMoveNode *newPrev, *newCurr;

    /* First free all userMove nodes that are next to currentMove (NULL check needed in case moveList is empty) */
    if (game->movesList->currentMove != NULL) {
        freeAllUserMoveNodes(game->movesList->currentMove->next);
    }
    newPrev = game->movesList->currentMove;
    newCurr = (userMoveNode *) malloc(sizeof(userMoveNode));

    if (newCurr == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    if (newPrev != NULL) { /* Set previous's next to the new current move, unless prev was NULL */
        newPrev->next = newCurr;
    }
    game->movesList->currentMove = newCurr;
    newCurr->prev = newPrev;
    newCurr->next = NULL;

    /* CHANGE NODE */
    newCurr->change = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode));
    if (newCurr->change == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }

    newCurr->change->currVal = (cell *) malloc(sizeof(cell));
    if (newCurr->change->currVal == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }

    newCurr->change->next = NULL;
    if (game->movesList->currentMove->prev == NULL) { /* In case the new current move node becomes the head */
        game->movesList->head = game->movesList->currentMove;
    }

    /* game->movesList->size++; */
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
int printChanges(cellChangeRecNode *moveToPrint, int isRedo) {
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


        if (!curr) {
            if (!prev) {
                printf("from _ to _\n");
            } else {
                printf("from _ to %d\n", prev);
            }
        } else {
            if (!prev) {
                printf("from %d to _\n", curr);
            } else {
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
int doesCellHaveASingleLegalValue(gameParams *game, int x, int y) {

    int k, counter, N, value;
    value = 0;
    N = game->n * game->m;
    counter = 0;
    for (k = 1; k < N + 1; k++) {
        if (checkIfValid(x, y, k, game)) {
            counter++;
            value = k;
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
    if ((z == 0) && (game->userBoard[x][y]->value != 0)) { /* when a non-zero cell is set back to zero (emptied) */
        game->counter--;
    } else if ((z != 0) && (game->userBoard[x][y]->value == 0)) { /* when a zero cell is set to z (!=0) */
        game->counter++;
    }

    /* sets the value */
    game->userBoard[x][y]->value = z;
    game->userBoard[x][y]->isValid = FALSE; /* Doesn't matter what we assign - in the end we run a function that
 * goes over all the board and checks each cell's validity*/
    game->userBoard[x][y]->isFixed = FALSE;
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
            if (game->userBoard[i][j]->value == 0) {
                legalValue = doesCellHaveASingleLegalValue(game, i, j);
                if (legalValue != FALSE) {
                    if (changes == 0) {
                        /* keep the first node */
                        changeListHead = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode));
                        if (changeListHead == NULL) {
                            freeSudokuGame(game);
                            printMallocFailed();
                            exit(EXIT_FAILURE);
                        }
                        currentChange = changeListHead;
                        currentChange->next = NULL;
                    } else {
                        currentChange->next = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode));
                        if (currentChange->next == NULL) {
                            freeSudokuGame(game);
                            printMallocFailed();
                            exit(EXIT_FAILURE);
                        }
                        currentChange = currentChange->next;
                        currentChange->next = NULL;
                    }
                    currentChange->prevVal = createCell(0);
                    copyCell(game->userBoard[i][j], currentChange->prevVal);
                    free(game->userBoard[i][j]);
                    game->userBoard[i][j] = createCell(legalValue);
                    currentChange->currVal = createCell(legalValue);
                    currentChange->x = i + 1;
                    currentChange->y = j + 1;
                    currentChange->next = NULL;
                    changes++;
                    printf("Cell <%d,%d> set to %d\n", i + 1, j + 1, legalValue);
                }
            }
        }
    }
    *numOfChanges = changes;
    return changeListHead;
}

void setValuesBychangeListHead(gameParams *game, cellChangeRecNode *changeListNode) {

    int x, y;
    while (changeListNode != NULL) {
        x = changeListNode->x;
        y = changeListNode->y;
        setValue(game, x - 1, y - 1, changeListNode->currVal->value);
        changeListNode = changeListNode->next;
    }


}

/* Called by autoFill */
void setNewChangeListToGame(gameParams *game, cellChangeRecNode *changeListHead) {

    userMoveNode *newMove;
    if (game->movesList->currentMove != NULL && game->movesList->currentMove->next != NULL) {
        freeAllUserMoveNodes(game->movesList->currentMove->next);
    }
    newMove = (userMoveNode *) malloc(sizeof(userMoveNode *));
    if (newMove == NULL) {
        freeSudokuGame(game);
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    if (game->movesList->currentMove == NULL) {
        game->movesList->currentMove = newMove;
        game->movesList->head = newMove;
        newMove->prev = NULL;

    } else {
        newMove->prev = game->movesList->currentMove;
        game->movesList->currentMove->next = newMove;
        game->movesList->currentMove = newMove;
    }
    newMove->change = changeListHead;
    newMove->next = NULL;

    /* game->movesList->size++; */

}

/* Cleans game->userBoard and game->solution */
void cleanUserBoardAndSolution(gameParams *game) {
    int i, j;

    for (i = 0; i < game->N; i++) {
        for (j = 0; j < game->N; j++) {
            game->userBoard[i][j]->value = EMPTY;
            game->userBoard[i][j]->isFixed = FALSE;
            game->userBoard[i][j]->isValid = TRUE;
            game->solution[i][j]->value = EMPTY;
            game->solution[i][j]->isFixed = FALSE;
            game->solution[i][j]->isValid = TRUE;
        }
    }
}

void setToEmpty(int **matrix, int N) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matrix[i][j] = EMPTY;
        }
    }
}

void markFullCellsAsFixed(cell ***board, int N) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (board[i][j]->value != EMPTY) {
                board[i][j]->isFixed = TRUE;
            }
        }
    }
}


/* Iterates over each cell
 * and check if it is valid
 * or erroneous */
void updateErrors(gameParams *game) {

    int i, j, N, isValid;
    N = game->N;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            isValid = checkIfValid(i, j, game->userBoard[i][j]->value, game);
            game->userBoard[i][j]->isValid = isValid;
        }

    }
}


void *copyCell(cell *src, cell *dst) {
    dst->isValid = src->isValid;
    dst->value = src->value;
    dst->isFixed = src->isFixed;
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