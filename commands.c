//
// Created by eran on 31/07/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "commands.h"


/* prints the Sudoku board */
void printBoard(gameParams *game) {

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
            cellState = ' ';
// TODO: check the new logic
            if (!(game->userBoard[i][j]->isValid) && (game->markErrors)) {
                cellState = '*';
            }
            if ((game->userBoard[i][j]->isFixed) && (game->mode != edit)) {
                cellState = '.';
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
    printBoard(game); /* prints the board with or without displaying erroneous cells - according to X */
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

#if 0
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

#endif

/* Sets new value Z for cell X Y
 *
 * Preconditions:
 * command is valid
 * x,y,z are valid and integers
 *
 * Post:
 * value z is set to [x][y], currentMove holds the right data with the change.
 * lists and nodes are updated properly */
int set(int x, int y, int z, gameParams *game) {

#if 0
    ~optional to check here~

    if (!(checkIfValid(x - 1, y - 1, z, game->userBoard))) {
        printf("Error: value is invalid\n");
        return 0;
    }

#endif

    /* no cell is considered fixed when on edit mode, according to forum */
    if (game->mode != edit && game->userBoard[x][y]->isFixed) {
        printf("Error: cell is fixed\n");
        return 0;
    }

    getNewCurrentMove(game);
    game->movesList->currentMove->change->x = x;
    game->movesList->currentMove->change->y = y;
    game->movesList->currentMove->change->currVal->value = z;
    game->movesList->currentMove->change->prevVal = game->userBoard[x - 1][y - 1];

    /* according to z value - increment or decrement game counter
     * if z was already set to (x,y) cell - don't change counter */
    if ((z == 0) && (game->userBoard[x - 1][y - 1] != 0)) { /* when a non-zero cell is set back to zero (emptied) */
        game->counter--;
    } else if ((z != 0) && (game->userBoard[x - 1][y - 1] == 0)) { /* when a zero cell is set to z (!=0) */
        game->counter++;
    }


    /* sets the value */
    game->userBoard[x - 1][y - 1]->value = z;
    game->userBoard[x - 1][y - 1]->isValid = 1;

    if (game->markErrors == TRUE && checkIfValid(x, y, z, game) == FALSE) {
        game->userBoard[x][y]->isValid = 0;
    }

    printBoard(game);

    if ((game->mode == solve) && (game->counter == game->n * game->m)) {
        if (validate(game) == TRUE) {
            printf("Puzzle solved successfully\n");
            game->mode = init;
            // TODO should follow by "Enter your command:\n"
        } else {
            printf("Puzzle solution erroneous\n");
            // TODO the user will have to undo the move to continue solving ?? where to implement
        }
    }
    return 1;

}


/* Pre:
 * command is valid
 * game is at edit or solve mode
 *
 * Post:
 * last command was undone
 * lists and nodes are updated properly */
int undo(gameParams *game) {

    printf("#1");

    cellChangeRecNode *moveToUndo, *moveToPrint;

    if (game->movesList->size == 0) {
        printf("Error: no moves to undo\n");
        return 0;
    }

    printf("#2");

    moveToUndo = game->movesList->currentMove->change;
    moveToPrint = moveToUndo;

    game->movesList->currentMove = game->movesList->currentMove->prev;
    game->movesList->size--;

    printf("#3");

    if(moveToUndo != NULL){
        printf("%d\n",moveToUndo->currVal->value);
    }

    while (moveToUndo != NULL) {

        game->userBoard[moveToUndo->x - 1][moveToUndo->y - 1] = moveToUndo->prevVal;
        moveToUndo = moveToUndo->next;
    }

    printf("#4");
    printBoard(game);
    printChanges(game,moveToPrint,0);

    printf("#5");
// makeRecChanges(game, moveToUndo);  not used


    return 1;


}


/* Pre:
 * command is valid
 * game is at edit or solve mode
 *
 * Post:
 * last command that was undone is redone
 * lists and nodes are updated properly */
int redo(gameParams *game){
    cellChangeRecNode *moveToRedo, *moveToPrint;

    if(game->movesList->currentMove->next == NULL){
        printf("Error: no moves to redo\n");
        return 0;
    }

    moveToRedo = game->movesList->currentMove->change;
    moveToPrint = moveToRedo;
    game->movesList->currentMove = game->movesList->currentMove->next;
    game->movesList->size++;


    while(moveToRedo != NULL){
        game->userBoard[moveToRedo->x - 1][moveToRedo->y - 1] = moveToRedo->currVal;
        moveToRedo = moveToRedo->next;
    }
    printBoard(game);

    printChanges(game,moveToPrint,1);


    return 1;
}



#if 0
previous implementation

/* Checks if value z does not appear his 3x3 square in the matrix */
int checkIfSquareValid(int x, int y, int z, int **userBoard) {

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
    return 1;
}

/* Checks if value z does not appear in row x */
int checkIfRowValid(int x, int y, int z, int **userBoard) {

    int j;

    for (j = 0; j < 9; j++) {
        if (j != y) { /* exclude cell (x,y) from the square check */
            if (userBoard[x][j] == z) {
                return 0;
            }
        }
    }
    return 1;
}

/* Checks if value z does not appear in column y */
int checkIfColumnValid(int x, int y, int z, int **userBoard) {

    int i;

    for (i = 0; i < 9; i++) {
        if (i != x) { /* exclude cell (x,y) from the square check */
            if (userBoard[i][y] == z) {
                return 0;
            }
        }
    }
    return 1;
}

#endif

