//
// Created by eran on 31/07/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "commands.h"
#include "gurobi.h"
#include "memoryAllocation.h"


/* Starts a puzzle at SOLVE mode
 * Pre:
 * game is at SOLVE, EDIT or INIT mode (available in all modes)
 * If the file exists - it is contains valid data ans is well formatted
 * filePath is a well-formatted file path (not NULL)
 *
 * Post:
 * If file does not exist or cannot be opened - print error
 * If file exists - starts puzzle in solve mode
 * All fields of game struct are updated according to new board (loaded from file)
*/
int solve(gameParams *game, char *filePath) {
    FILE *file;

    /* Try to open file */
    file = fopen(filePath, "r");
    if (file == NULL) {
        printErrorOpeningFile(SOLVE_MODE); /* Prints the required error message according to mode */
        return FALSE;
    }
    /* File successfully opened - get rid of old fields (including memory allocation) */
    cleanSudokuGame(game);
    /* Load new fields from file */
    loadGameParamsFromFile(game, file, SOLVE_MODE);
    /* At this point game should hold the new parameters of the loaded board */
    fclose(file);
    return TRUE;
}


int edit(gameParams *game, char *filePath) {
    FILE *file;

    if (filePath == NULL) { /* no path was provided by user - enter EDIT mode with an empty 9x9 board */
        cleanSudokuGame(game);
        initializeSudokuGameFields(game, 3, 3);
        return TRUE;
    }

    file = fopen(filePath, "r");
    if (file == NULL) {
        printErrorOpeningFile(EDIT_MODE); /* Prints the required error message according to mode */
        return FALSE;
    }
    /* File successfully opened - get rid of old fields (including memory allocation) */
    cleanSudokuGame(game);
    /* Load new fields from file */
    loadGameParamsFromFile(game, file, EDIT_MODE);
    /* At this point game should hold the new parameters of the loaded board */
    fclose(file);
    return TRUE;
}

/* preconditions: 1. called only in SOLVE mode 2. X is either 0 or 1
 * (preconditions should be verified in parser module) */
void mark_errors(gameParams *game, int X) {
    game->markErrors = X;
}

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

    if ((game->mode == SOLVE_MODE) && (game->counter == game->N)) {
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

/* Preconditions: 1. called only on EDIT or SOLVE modes
 * the function first checks whether there are erroneous values
 * if no erroneous cells where found - uses ILP to determine whether the board is solvable */
int validate(gameParams *game) {
    if (hasErrCells(game) == TRUE) {
        printf("Error: board contains erroneous values\n");
        return FALSE; /* returns 0 */
    }
    /* getting here means all cells aren't erroneous - need to check if solvable */
    if (solveUsingILP(game, 1) == FALSE) {
        printf("Validation failed: board is unsolvable\n");
        return FALSE; /* returns 0 */
    } else {
        printf("Validation passed: board is solvable\n");
        return TRUE; /* returns 1 */
    }
}

/* Pre:
 * Available in EDIT mode only
 * x, y are valid integers */
int generate(gameParams *game, int x, int y) {
    int succeeded;

    if (!boardIsEmpty(game)) {
        printf("Error: board is not empty\n");
        return FALSE;
    }
    /* Try (at most) 1000 times to randomly fill X cells and solve the board: */
    succeeded = randomlyFillXCellsAndSolve(game, x);
    if (!succeeded) {
        printf("Error: puzzle generator failed\n");
        return FALSE;
    }
    /* Randomly clear Y cells: */
    randomlyClearYCells(game, y);
    return TRUE;
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

int save(gameParams *game, char *filePath) {
    FILE *file;

    /* In EDIT mode we require that no erroneous cells exist and that board is solvable: */
    if (game->mode == EDIT_MODE) {
        if (hasErrCells(game) == TRUE) {
            printf("Error: board contains erroneous values\n");
            return FALSE;
        }
        if (validate(game) == FALSE) {
            printf("Error: board validation failed\n");
            return FALSE;
        }
    }
    file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Error: File cannot be created or modified\n");
        return FALSE;
    }
    saveGameParamsToFile(game, file, game->mode);
    printf("Saved to: %s\n", filePath);
    fclose(file);
    return TRUE;
}

int hint(int x, int y, gameParams *game) {
    int isSolvable;
    int hint;

    if (hasErrCells(game) == TRUE) {
        printf("Error: board contains erroneous values\n");
        return FALSE;
    } else if ((game->userBoard[x - 1][y - 1]->isFixed) == TRUE) {
        printf("Error: cell is fixed\n");
        return FALSE;
    } else if ((game->userBoard[x - 1][y - 1]->value != EMPTY)) {
        printf("Error: cell already contains a value\n");
        return FALSE;
    }

    isSolvable = solveUsingILP(game ,3); /* returns boolean indication of solvability of board. solution is at game->solution */
    if (!isSolvable) {
        printf("Error: board is unsolvable\n");
        return FALSE;
    }
    /* TODO may need to change this if solution is int** */
    hint = game->solution[x - 1][y - 1]->value;
    printf("Hint: set cell to %d\n", hint);

}

/* preconditions: 1. called only on EDIT or SOLVE modes
 * prints the number of solutions for the current board
 * the function first checks whether there are erroneous values
 * if no erroneous cells were found - counts the number of possible solutions */
int numSolutions(gameParams *game) {
    int num_of_sols;

    if (hasErrCells(game) == TRUE) {
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

/* Automatically fill "obvious" values
 * cells which contain a single legal value
 *
 * Pre:
 * game is at Solve mode */
int autoFill(gameParams *game) {

    int numOfChanges;
    cellChangeRecNode *changeListHead;
    numOfChanges = 0;
    if (hasErrCells(game) == TRUE) {
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
    printf("Board reset\n");
    return 1;
}

/* Exits the game */
void exitGame(gameParams *game) {

    printf("Exiting...\n");
    freeSudokuGame(game);
    // TODO (Amir?) : close all open files
}










