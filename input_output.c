

#include "input_output.h"

int saveGameParamsToFile(gameParams *game, FILE *destFile, enum gameMode mode) {
    int row, col;

    fprintf(destFile, "%d %d\n", game->m, game->n);
    for (row = 0; row < game->N; row++) {
        for (col = 0; col < game->N; col++) {
            fprintf(destFile, "%d", game->userBoard[row][col]->value);
            /* If in EDIT mode or cell is fixed - write an extra '.' next to digit */
            if (((game->userBoard[row][col]->isFixed == TRUE) && (game->userBoard[row][col]->value != EMPTY))
            || ((mode == EDIT_MODE) && game->userBoard[row][col]->value != EMPTY)) {
                fprintf(destFile, ".");
            }
            /* If haven't reached end of row write space */
            if (col < (game->N - 1)) {
                fprintf(destFile, " ");
            }
        }
        fprintf(destFile, "\n");
    }
    return TRUE;

}

/* Should explicitly update all game fields! */
int loadGameParamsFromFile(gameParams *game, FILE *sourceFile, enum gameMode mode) {
    int row, col, m, n, N;
    int numberOfFilledCells, c;
    cell ***newUserBoard, ***newSolution;
    /* Get new parameters for gameParams fields: */
    /* First get m, n (ignoring white spaces) */
    while (isspace(c = fgetc(sourceFile))) {}
    m = c - '0';
    while (isspace(c = fgetc(sourceFile))) {}
    n = c - '0';
    N = n * m;

    /* Allocate new matrix */
    newUserBoard = allocateCellMatrix(N);
    newSolution = allocateCellMatrix(N);

    /* Assign new fields to game: */
    game->mode = mode;
    game->markErrors = TRUE;
    game->m = m;
    game->n = n;
    game->N = N;
    game->userBoard = newUserBoard;
    game->solution = newSolution;

    newUserBoard = game->userBoard;
    numberOfFilledCells = 0;
    row = 0;
    /* Read file to assign new values to new cells: */
    while (row < N) {
        col = 0;
        while (col < N) {
            newUserBoard[row][col]->isValid = TRUE;
            /* Enter only if a non whitespace character has been read */
            if (isspace(c = fgetc(sourceFile)) == FALSE) {
                /* Classify into two options: '.' or a digit */

                if (c == '.') { /* FIXED CELL */
                    newUserBoard[row][col]->isFixed = TRUE;
                    col++;
                }

                else { /* DIGIT */
                    newUserBoard[row][col]->value = (c - '0');
                    if ((c - '0') != 0) { /* If a non-zero digit read increment counter */
                        numberOfFilledCells++;
                    }

                    /* Read next char and classify into three options: */
                    c = fgetc(sourceFile);

                    if (isspace(c)) { /* NON-FIXED SINGLE DIGIT */
                        newUserBoard[row][col]->isFixed = FALSE;
                        col++;
                    }
                    else if (c == '.') { /* FIXED SINGLE DIGIT */
                        newUserBoard[row][col]->isFixed = TRUE;
                        col++;
                    }
                    else { /* TWO-DIGIT NUMBER (second char also digit) */
                        newUserBoard[row][col]->value = 10* (newUserBoard[row][col]->value) + (c - '0'); /* Update value */
                        /* Read next char (third char of this iteration) to determine FIXED or NOT */
                        c = fgetc(sourceFile);
                        if (isspace(c)) {
                            newUserBoard[row][col]->isFixed = FALSE;
                        }
                        else {
                            newUserBoard[row][col]->isFixed = TRUE;
                        }
                        col++;
                    }
                }
            }
        }
        row++;
    }

    game->counter = numberOfFilledCells;
    /* TODO - should call a function that goes over all the board and updates cell->isValid fields */
    return TRUE;
}
