//
// Created by eran on 31/07/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "commands.h"


/* prints the Sudoku board */
void print_board(gameParams *game) {
    /* need to implement function -
     * must check whether game->markErrors is 0 or 1 and
     * display or not erroneous values accordingly */

    int i, j, m, n, N;
    char cellRow, cellState, *separatorRow = NULL;

    n = game->n;
    m = game->m;
    N = n * m;
    separatorRow = getLineSeparator(game);
    cellRow = '|';

    for (i = 0; i < N; i++) {
        if (i % m == 0) {
            printf("%s\n", separatorRow);
        }
        for (j = 0; j < N; j++) {
            if (j % n == 0) {
                printf("%c", cellRow);
            }
            if (game->userBoard[i][j]->isFixed) {
                cellState = '.';
            }
                /* if cell is not fixed, we check if it's erroneous if we are in edit mode or markErrors */
            else if (!(game->userBoard[i][j]->isValid) && (game->mode == edit || game->markErrors)) {
                cellState = '*';
            }
            printf(" %2d%c", game->userBoard[i][j]->value, cellState);
        }
        printf("%c\n", cellRow);
    }
    printf("%s\n", separatorRow);

}

/* preconditions: 1. called only in SOLVE mode 2. X is either 0 or 1
 * (preconditions should be verified in parser module) */
void mark_errors(gameParams *game, int X) {
    game->markErrors = X;
    print_board(game); /* prints the board with or without displaying erroneous cells - according to X */
}

/* preconditions: 1. called only on EDIT or SOLVE modes
 * the function first checks whether there are erroneous values
 * if no erroneous cells where found - uses ILP to determine whether the board is solvable */
int validate(gameParams *game) {
    if (check_err_cells(game) == TRUE) {
        printf("Error: board contains erroneous values\n");
        return FALSE; /* returns 0 */
    }
    /* getting here means all cells aren't erroneous - need to check if solvable */
    if (solveUsingILP(game) == FALSE) {
        printf("Validation failed: board is unsolvable\n");
        return FALSE; /* returns 0 */
    } else {
        printf("Validation passed: board is solvable\n");
        return TRUE; /* returns 1 */
    }
}


/* preconditions: 1. called only on EDIT or SOLVE modes
 * prints the number of solutions for the current board
 * the function first checks whether there are erroneous values
 * if no erroneous cells where found - counts the number of possible solutions */
int num_solutions(gameParams *game) {
    int num_of_sols;

    if (check_err_cells(game) == TRUE) {
        printf("Error: board contains erroneous values\n");
        return FALSE; /* returns 0 */
    }
    /* getting here means all cells aren't erroneous - count number of solutions */
    num_of_sols = count_solutions(game);
    printf("Number of solutions: %d\n", num_of_sols);

    if (num_of_sols == 1) {
        printf("This is a good board!\n");
        return 1;
    } else if (num_of_sols > 1) {
        printf("The puzzle has more than 1 solution, try to edit it further\n");
        return 1;
    }
    /* gets here in case num_of_sols == 0 */
    return 0;
}

