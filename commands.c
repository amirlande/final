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
// TODO : check bug on 3X3
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
            if (!(game->userBoard[i][j]->isValid) && (game->markErrors)) {
                cellState = '*';
            }
            if ((game->userBoard[i][j]->isFixed) && (game->mode != EDIT_MODE)) {
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
    if (checkErrCells(game) == TRUE) {
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
 * if no erroneous cells were found - counts the number of possible solutions */
int numSolutions(gameParams *game) {
    int num_of_sols;

    if (checkErrCells(game) == TRUE) {
        printf("Error: board contains erroneous values\n");
        return FALSE; /* returns 0 */
    }
    /* getting here means all cells are *not* erroneous - count number of solutions */
    num_of_sols = countSolutions(game);
    printf("Number of solutions: %d\n", num_of_sols);

    if (num_of_sols == 1) {
        printf("This is a good board!\n");
        return TRUE;
    } else if (num_of_sols > 1) {
        printf("The puzzle has more than 1 solution, try to edit it further\n");
        return TRUE;
    }
    /* gets here in case num_of_sols == 0, board isn't solvable */
    return FALSE;
}


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

    if (checkIfValid(x - 1, y - 1, z, game) == FALSE) {
        printf("Error: value is invalid\n");
        return 0;
    }

    /* no cell is considered fixed when on edit mode, according to forum */
    if (game->mode != EDIT_MODE && game->userBoard[x - 1][y - 1]->isFixed) {
        printf("Error: cell is fixed\n");
        return 0;
    }

    getNewCurrentMove(game);
    game->movesList->currentMove->change->x = x;
    game->movesList->currentMove->change->y = y;
    game->movesList->currentMove->change->prevVal = game->userBoard[x - 1][y - 1];
    game->userBoard[x - 1][y - 1] = game->movesList->currentMove->change->currVal;

    setValue(game, x - 1, y - 1, z);
    printBoard(game);


    // TODO : handeling the game when it's done

    if ((game->mode == SOLVE_MODE) && (game->counter == game->n * game->m)) {
        if (validate(game) == TRUE) {
            printf("Puzzle solved successfully\n");
            game->mode = INIT_MODE;
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

    return undoEnveloped(game, 0);
}


/* the REAL undo.
 * enveloped by the func named "undo".
 * made this change for the reset func */
int undoEnveloped(gameParams *game, int isReset) {

    cellChangeRecNode *moveToUndo, *moveToPrint;

    if (game->movesList->size == 0) {
        printf("Error: no moves to undo\n");
        return 0;
    }

    moveToUndo = game->movesList->currentMove->change;
    moveToPrint = moveToUndo;
    game->movesList->currentMove = game->movesList->currentMove->prev;
    game->movesList->size--;


    while (moveToUndo != NULL) {
        game->userBoard[moveToUndo->x - 1][moveToUndo->y - 1] = moveToUndo->prevVal;
        moveToUndo = moveToUndo->next;
    }

    if (isReset == FALSE) {
        /* not printing anything on reset */
        printBoard(game);
        printChanges(game, moveToPrint, 0);
    }

    return 1;
}


/* Pre:
 * command is valid
 * game is at edit or solve mode
 *
 * Post:
 * last command that was undone is redone
 * lists and nodes are updated properly */
int redo(gameParams *game) {

    cellChangeRecNode *moveToRedo, *moveToPrint;

    if (game->movesList->currentMove->next == NULL) {
        printf("Error: no moves to redo\n");
        return 0;
    }
    game->movesList->currentMove = game->movesList->currentMove->next;
    moveToRedo = game->movesList->currentMove->change;
    moveToPrint = moveToRedo;
    game->movesList->size++;

    while (moveToRedo != NULL) {
        game->userBoard[moveToRedo->x - 1][moveToRedo->y - 1] = moveToRedo->currVal;
        moveToRedo = moveToRedo->next;
    }

    printBoard(game);
    printChanges(game, moveToPrint, 1);

    return 1;
}

/* Automatically fill "obvious" values
 * cells which contain a single legal value
 *
 * Pre:
 * game is at Solve mode */
int autoFill(gameParams *game) {

    int numOfChanges;
    cellChangeRecNode *changeListHead;
    numOfChanges = 0;
    if (checkErrCells(game) == TRUE) {
        printf("Error: board contains erroneous values\n");
        return 0;
    }

    changeListHead = getAutoFillChangeList(game, &numOfChanges);

    if (numOfChanges == 0) {
        /* no alloc were made at this point */
        return 1;
    }

    setNewChangeListToGame(game, changeListHead);

    printBoard(game);

    return 1;
}

/* resets all moves
 *
 * Pre:
 * game is at Solve mode
 *
 * Post:
 * all moves are undone
 * all move nodes are freed except head node
 * */
int reset(gameParams *game) {
    //TODO: not tested
    while (game->movesList->currentMove != game->movesList->head) {
        undoEnveloped(game, 1);
    }

    freeAllUserMoveNodes(game->movesList->head->next);
    return 1;
}


/* Exits the game */
void exitGame(gameParams *game) {

    printf("Exiting...\n");
    freeGame(game);
    // TODO (Amir?) : close all open files
}


