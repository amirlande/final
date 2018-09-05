//
// Created by eran on 31/07/18.
//


#include "commands.h"


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
        game->mode = EDIT_MODE;
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
    game->mode = EDIT_MODE;
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

    free(separatorRow);

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
    updateErrors(game);

    // TODO : handling the game when it's done

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
    if (solveUsingILP(game, ILP_COMMAND_VALIDATE) == FALSE) {
        printf("Validation failed: board is unsolvable\n");
        return FALSE; /* returns 0 */
    } else {
        printf("Validation passed: board is solvable\n");
        return TRUE; /* returns 1 */
    }
}

/* Assigns to possibleLegalVals all the legal values for cell <row,col>
 * Returns the number of legal values that were found */
int getPossibleValues(gameParams *game, int row, int col, int *possibleLegalVals) {
    int counter;
    int val;

    counter = 0;
    for (val = 0; val < game->N; val++) {
        if (checkIfValid(row, col, val, game)) {
            possibleLegalVals[counter] = val;
            counter++;
        }
    }
    return counter;
}

int getRandomLegalValue(gameParams *game, int row, int col) {
    int *possibleLegalVals;
    int N, numberOfLegalValues, randomIndex, randomValue;

    N = game->N;
    possibleLegalVals = (int *)calloc((size_t)N, sizeof(int));
    if (possibleLegalVals == NULL) {
        printCallocFailed();
        exit(EXIT_FAILURE);
    }

    numberOfLegalValues = getPossibleValues(game, row, col, possibleLegalVals);
    if (numberOfLegalValues == 0) { /* No possible values for cell <row, col> */
        return INVALID;
    }
    /* Randomly choose an index between 0 to (numberOfLegalValues - 1) inclusive */
    randomIndex = rand() % numberOfLegalValues;
    randomValue = possibleLegalVals[randomIndex];

    free(possibleLegalVals);
    return randomValue;
}

/* On success (1) game->userBoard holds a valid board with X FIXED cells */
int randomlyFillXCells(gameParams *game, int x) {
    int **matrixOfChoices; /* To track already-chosen cells */
    int counter, randomRow, randomCol, randomVal, N;

    N = game->N;
    matrixOfChoices = allocateIntMatrix(game->N);
    setToEmpty(matrixOfChoices, N);

    counter = 0;
    while (counter < x) {
        randomRow = rand() % N;
        randomCol = rand() % N;

        while (matrixOfChoices[randomRow][randomCol] == FULL) {
            randomRow = rand() % N;
            randomCol = rand() % N;
        }
        randomVal = getRandomLegalValue(game, randomRow, randomCol);
        if (randomVal == INVALID) { /*  */
            freeIntMatrix(matrixOfChoices, N);
            return FALSE;
        }
        game->userBoard[randomRow][randomCol]->value = randomVal;
        counter++;
    }
    freeIntMatrix(matrixOfChoices, N);
    return TRUE;

}

/* Return TRUE (1) on success, FALSE (0) on failure */
int randomlyFillXCellsAndSolve(gameParams *game, int x) {
    int i, success;
    for (i = 0; i < MAX_NUMBER_OF_ATTEMPTS; i++) {
        success = randomlyFillXCells(game, x); /* Works on game->userBoard */
        if (!success) {
            cleanUserBoardAndSolution(game); /* Cleans game->userBoard and game->solution values to zeros */
            continue; /* Attempt failed - continue to next iteration */
        }
        markFullCellsAsFixed(game->userBoard, game->N); /* Mark all x chosen cells as fixed before passing to ILP solver */
        /* At this point randomlyFillXCells should have filled X cells and marked them as FIXED */
        success = solveUsingILP(game, ILP_COMMAND_GENERATE);   /* On success: game->solution holds the solution */
        if (!success) {
            cleanUserBoardAndSolution(game); /* Cleans game->userBoard and game->solution */
            continue; /* Attempt failed - continue to next iteration */
        }
        /* Getting here means solution holds a valid complete board */
        break;
    }
    return ((success == TRUE) ? TRUE : FALSE);
}

void randomlyClearYCells(gameParams *game, int y) {
    /* TODO - implement */
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
    markFullCellsAsFixed(game->userBoard, game->N); /* Mark as FIXED all remaining cells */

    return TRUE;
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
        game->counter--;
    }

    updateErrors(game);
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
        game->counter++;
    }

    updateErrors(game);
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
        /*
        if (validate(game) == FALSE) {
            printf("Error: board validation failed\n");
            return FALSE;
        }
         */
    }
    //file = fopen("C:\\temp\\sudoku", "w");
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

    isSolvable = solveUsingILP(game,
                               ILP_COMMAND_HINT); /* returns boolean indication of solvability of board. solution is at game->solution */
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
    game->counter += numOfChanges;
    updateErrors(game);
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










