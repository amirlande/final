
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
    printBoard(game);
    fclose(file);
    return TRUE;
}


int edit(gameParams *game, char *filePath) {
    FILE *file;

    if (filePath == NULL) { /* no path was provided by user - enter EDIT mode with an empty 9x9 board */
        cleanSudokuGame(game);
        initializeSudokuGameFields(game, 3, 3);
        game->mode = EDIT_MODE;
        printBoard(game);
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
    printBoard(game);
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
    n = game->n;
    m = game->m;
    N = n * m;
    separatorRow = getLineSeparator(game);
    cellRow = '|';

    updateErrors(game);
    for (i = 0; i < N; i++) {
        if (i % m == 0) {
            printf("%s\n", separatorRow);
        }
        for (j = 0; j < N; j++) {
            if (j % n == 0) {
                printf("%c", cellRow);
            }
            cellState = ' ';

            if ((!(game->userBoard[i][j]->isValid) && (game->markErrors)) ||
                (!(game->userBoard[i][j]->isValid) && game->mode == EDIT_MODE)) {
                cellState = '*';
            }
            if ((game->userBoard[i][j]->isFixed) && (game->mode != EDIT_MODE)) {
                cellState = '.';
            }

            if (game->userBoard[i][j]->value == 0) {
                printf("   %c", cellState);
            } else {
                printf(" %2d%c", game->userBoard[i][j]->value, cellState);
            }
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

    /* I took this out - this is not a requirement
     * Erroneous cells are allowed (see under SET.g.)
    if (checkIfValid(x - 1, y - 1, z, game) == FALSE) {
        printf("Error: value is invalid\n");
        return 0;
    }
     */

    int prevZ;
    /* no cell is considered fixed when on edit mode, according to forum */
    if (game->mode == SOLVE_MODE && game->userBoard[y - 1][x - 1]->isFixed) {
        printf("Error: cell is fixed\n");
        return 0;
    }

    /* only assign if it is a new value */
    if (z != game->userBoard[y - 1][x - 1]->value) {
        getNewCurrentMove(game); /* Clears all "next" moves and creates e new moveNode */
        game->movesList->currentMove->change->x = x;
        game->movesList->currentMove->change->y = y;
        game->movesList->currentMove->change->prevVal = createCell(0);
        copyCell(game->userBoard[y - 1][x - 1], game->movesList->currentMove->change->prevVal);
        prevZ = game->movesList->currentMove->change->prevVal->value;
        free(game->userBoard[y - 1][x - 1]);
        game->userBoard[y - 1][x - 1] = createCell(prevZ);
        setValue(game, x - 1, y - 1, z);
        copyCell(game->userBoard[y - 1][x - 1], game->movesList->currentMove->change->currVal);

    }
    updateErrors(game);
    printBoard(game);
    if ((game->mode == SOLVE_MODE) && (game->counter == game->N * game->N)) {
        if (solveUsingILP(game, ILP_COMMAND_VALIDATE) == TRUE) {
            printf("Puzzle solved successfully\n");
            game->mode = INIT_MODE;
        } else {
            printf("Puzzle solution erroneous\n");
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
        if (checkIfValid(col, row, val, game)) {
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
    possibleLegalVals = (int *) calloc((size_t) N, sizeof(int));
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
    success = FALSE;
    for (i = 0; i < MAX_NUMBER_OF_ATTEMPTS; i++) {
        success = randomlyFillXCells(game, x); /* Works on game->userBoard */
        if (!success) {
            cleanUserBoardAndSolution(game); /* Cleans game->userBoard and game->solution values to zeros */
            continue; /* Attempt failed - continue to next iteration */
        }
        markFullCellsAsFixed(game->userBoard,
                             game->N); /* Mark all x chosen cells as fixed before passing to ILP solver */
        /* At this point randomlyFillXCells should have filled X cells and marked them as FIXED */
        success = solveUsingILP(game, ILP_COMMAND_GENERATE);   /* On success: game->solution holds the solution */
        if (!success) {
            cleanUserBoardAndSolution(game); /* Cleans game->userBoard and game->solution */
            continue; /* Attempt failed - continue to next iteration */
        }
        /* Getting here means solution holds a valid complete board */
        cleanUserBoard(game);
        break;
    }
    return ((success == TRUE) ? TRUE : FALSE);
}

void randomlyClearAllButYCells(gameParams *game, int y) {
    int randomRow, randomCol, counter, N, cellsToRemove;

    N = game->N;

    counter = 0;
    cellsToRemove = N * N - y;
    while (counter < cellsToRemove) {
        randomRow = rand() % N;
        randomCol = rand() % N;

        if (game->solution[randomRow][randomCol]->value != EMPTY) {
            game->solution[randomRow][randomCol]->value = EMPTY;
            counter++;
        }
    }
}

/* Pre:
 * Available in EDIT mode only
 * x, y are valid integers */
int generate(gameParams *game, int x, int y) {
    int succeeded;
    cellChangeRecNode *changeListHead;

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
    randomlyClearAllButYCells(game, y);

    changeListHead = getGenerateChangeList(game);
    setNewChangeListToGame(game, changeListHead);
    setValuesByChangeListHead(game, changeListHead);

    markFullCellsAsFixed(game->userBoard, game->N); /* Mark as FIXED all remaining cells */
    game->counter += y;
    printBoard(game);
    return TRUE;
}

cellChangeRecNode *getGenerateChangeList(gameParams *game) {

    int i, j, N, changes, sol;
    cellChangeRecNode *changeListHead, *currentChange;
    N = game->N;
    currentChange = NULL;
    changeListHead = NULL;
    changes = 0;


    /* Copy solution to userBoard */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            sol = game->solution[j][i]->value;
            if (sol != 0) {
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
                copyCell(game->userBoard[j][i], currentChange->prevVal);
                free(game->userBoard[j][i]);
                game->userBoard[j][i] = createCell(-1);
                currentChange->currVal = createCell(sol);
                currentChange->x = i + 1;
                currentChange->y = j + 1;
                currentChange->next = NULL;
                changes++;
            }
        }
    }

    printf("changes are: %d\n", changes);
    return changeListHead;
}

/* the REAL undo.
 * enveloped by the func named "undo".
 * made this change for the reset func */
int undoEnveloped(gameParams *game, int isReset) {

    cellChangeRecNode *changeToUndo, *changeToPrint;

    if (game->movesList->currentMove == NULL) {
        printf("Error: no moves to undo\n");
        return FALSE;
    }

    changeToUndo = game->movesList->currentMove->change;
    changeToPrint = changeToUndo;

    /* Update new current move to point to previous move (may be NULL) */
    game->movesList->currentMove = game->movesList->currentMove->prev;

    /* Iterate over the (singly) linked list of cell changes until we reach NULL */
    while (changeToUndo != NULL) {
        free(game->userBoard[changeToUndo->y - 1][changeToUndo->x - 1]);
        game->userBoard[changeToUndo->y - 1][changeToUndo->x - 1] = createCell(0);
        copyCell(changeToUndo->prevVal, /* <- src */
                 game->userBoard[changeToUndo->y - 1][changeToUndo->x - 1]); /* Restore previous value */
        if (changeToPrint->prevVal->value == 0) { /* decrements counter only when deleting a value */
            if (changeToUndo->currVal->value != 0) {
                game->counter--;
            }
        } else if (changeToUndo->currVal->value == 0) {
            game->counter++;
        }
        changeToUndo = changeToUndo->next; /* Move to next cell change (done on the same turn) */
    }

    updateErrors(game);
    if (isReset == FALSE) {
        /* not printing anything on reset */
        printBoard(game);
        printChanges(changeToPrint, 0);
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

    cellChangeRecNode *changeToRedo, *changeToPrint;

    /* Check if current points to NULL (in case we are redoing when current points to NULL
     * (since all moves have been undone) bu head points to a move (since the moves that have been done can still be redone)*/
    if (game->movesList->currentMove == NULL) { /* current points to NULL */
        if (game->movesList->head != NULL) {
            game->movesList->currentMove = game->movesList->head;
        } else {
            printf("Error: no moves to redo\n");
            return FALSE;
        }
    } else { /* current doesn't point to NULL - check if there is next nove to redo */
        if (game->movesList->currentMove->next == NULL) {
            printf("Error: no moves to redo\n");
            return FALSE;
        }
        game->movesList->currentMove = game->movesList->currentMove->next;
    }
    /* Getting here means that game->movesList->currentMove points to a valid (not NULL) move to be redone */
    changeToRedo = game->movesList->currentMove->change;
    changeToPrint = changeToRedo;

    while (changeToRedo != NULL) {
        free(game->userBoard[changeToRedo->y - 1][changeToRedo->x - 1]);
        game->userBoard[changeToRedo->y - 1][changeToRedo->x - 1] = createCell(0);
        /* copying currVal to userBoard */
        copyCell(changeToRedo->currVal, game->userBoard[changeToRedo->y - 1][changeToRedo->x - 1]);
        if (changeToRedo->prevVal->value == 0) {
            if (changeToRedo->currVal->value != 0) {
                game->counter++;
            }

        } else if (changeToRedo->currVal->value == 0) {
            game->counter--;
        }
        changeToRedo = changeToRedo->next;

    }

    updateErrors(game);
    printBoard(game);
    printChanges(changeToPrint, 1);

    return TRUE;
}

int save(gameParams *game, char *filePath) {
    FILE *file;

    /* In EDIT mode we require that no erroneous cells exist and that board is solvable: */
    if (game->mode == EDIT_MODE) {
        if (hasErrCells(game) == TRUE) {
            printf("Error: board contains erroneous values\n");
            return FALSE;
        }
        if (solveUsingILP(game, ILP_COMMAND_VALIDATE) == FALSE) {
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
    } else if ((game->userBoard[y - 1][x - 1]->isFixed) == TRUE) {
        printf("Error: cell is fixed\n");
        return FALSE;
    } else if ((game->userBoard[y - 1][x - 1]->value != EMPTY)) {
        printf("Error: cell already contains a value\n");
        return FALSE;
    }

    isSolvable = solveUsingILP(game,
                               ILP_COMMAND_HINT); /* returns boolean indication of solvability of board. solution is at game->solution */
    if (!isSolvable) {
        printf("Error: board is unsolvable\n");
        return FALSE;
    }
    hint = game->solution[x - 1][y - 1]->value;
    printf("Hint: set cell to %d\n", hint);
    return 1;
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
    setValuesByChangeListHead(game, changeListHead);
    game->counter += numOfChanges;
    updateErrors(game);
    printBoard(game);

    if ((game->mode == SOLVE_MODE) && (game->counter == game->N * game->N)) {
        if (solveUsingILP(game, ILP_COMMAND_VALIDATE) == TRUE) {
            printf("Puzzle solved successfully\n");
            game->mode = INIT_MODE;
        } else {
            printf("Puzzle solution erroneous\n");
        }
    }

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

    while (game->movesList->currentMove != NULL) {
        undoEnveloped(game, 1);
    }

    /*boardTofFree = game->userBoard;
    game->userBoard = allocateCellMatrix(game->N);
    freeCellMatrix(boardTofFree, game->N);
    freeAllUserMoveNodes(game->movesList->head);
    free(game->movesList);
    game->counter = 0;
     */

    game->movesList = allocateMoveList();
    printf("Board reset\n");
    printBoard(game);
    return 1;
}

/* Exits the game */
void exitGame(gameParams *game) {

    printf("Exiting...\n");
    freeSudokuGame(game);
}










