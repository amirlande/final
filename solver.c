
#include "solver.h"


/* precondition: board has no erroneous values (to be checked before calling this function)
 * the function solves the board using ILP algorithm
 * returns TRUE (1) if solvable or FALSE (0) if unsolvable*/
int solveUsingILP(gameParams *game, ILPCommand cmd) {

    int **board, **sol, result;
    cell ***oldSol;

    oldSol = game->solution;
    sol = allocateIntMatrix(game->N);
    board = fromCellMatToIntMat(game->userBoard, game->N);
    result = ILP(board, sol, game->n, game->m, cmd);
    if (cmd != ILP_COMMAND_VALIDATE) {
        game->solution = fromIntMatToCellMat(sol, game->N);
        freeCellMatrix(oldSol, game->N);
    }
    freeIntMatrix(sol, game->N);
    freeIntMatrix(board, game->N);
    return result;
}

/* This function is for module-internal use only.
 * It creates and returns a gameParams object to wrap the board, N, n, m struct fields,
 * so we can pass it to helper functions at gameUtils (that accept gameParams objects).
 * Notice: the following fields aren't assigned and are ignored by the helper functions
 * (the helper functions only use N, n, m and userBoard fields) */
gameParams *wrapInGameParamsStruct(cell ***board, int N, int n, int m) {
    gameParams *temporaryGameParams;
    temporaryGameParams = (gameParams *) malloc(sizeof(gameParams));
    if (temporaryGameParams == NULL) {
        printMallocFailed();
        exit(0);
    }
    temporaryGameParams->N = N;
    temporaryGameParams->n = n;
    temporaryGameParams->m = m;
    temporaryGameParams->userBoard = board; /* No need to allocate memory for userBoard, it points to an already-allocated struct */
    temporaryGameParams->solution = allocateCellMatrix(N);
    temporaryGameParams->markErrors = TRUE;
    temporaryGameParams->movesList = allocateMoveList();
    return temporaryGameParams;
}

void markAsFixed(cell ***board, int N) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if ((board)[i][j]->value != EMPTY) {
                (board)[i][j]->isFixed = TRUE;
            }
        }
    }
}

/* Helper function used by countSolutionRec function.
 * Sets row and col to hold indexes of the first empty cell on the board.
 * Returns VALID (1) on success (if an empty cell exists), or INVALID on failure (if all cells are filled) */
int findFirstEmptyCell(cell ***board, int N, int *row, int *col) {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (board[i][j]->value == 0) {
                *row = i;
                *col = j;
                return VALID;
            }
        }
    }
    /* Getting here means all cells are filled - therefore no first empty cell */
    return INVALID;
}

void pushValidValues(gameParams *partialGameParams, int row, int col, Stack *stack) {
    int value, N;

    N = partialGameParams->N;
    /* Iterate value from N down to 1 to check validity of assignment of value to (row, col) cell
     * If assignment is valid - push onto stack en element with row, col and val */
    for (value = N; value > 0; value--) {
        if (checkIfValid(row, col, value, partialGameParams)) {
            push(stack, row, col, value);
        }
    }
}

/* Removes all assignments of non-fixed cells "after" (row, col) cell,
 * and assigns value to (row, cell) */
void setBoard(BOARD board, int N, int row, int col, int value) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i > row || (i == row && j > col)) { /* Only cells that come "after" (row, col) */
                if (!(board[i][j]->isFixed)) {
                    (board)[i][j]->value = EMPTY;
                }
            }
        }
    }
    board[row][col]->value = value;
}

void countWithBacktracking(gameParams *gameParams, int *numOfSols) {
    Stack *stack;
    element *stackElement;
    int row, col, N;

    stack = initializeStack();
    N = gameParams->N;

    /* Set (row, col) to hold first empty cell */
    if (findFirstEmptyCell(gameParams->userBoard, gameParams->N, &row, &col) == INVALID) {
        printf("I thought should never get here - should have been caught at if ((game->counter) == (game->N * game->N))");
        *numOfSols = 1;
        return;
    }
    /* Push onto stack an element for each valid (row, col, value)
     * row and col are fixed, value ranges from 1 to N inclusive */
    pushValidValues(gameParams, row, col, stack);

    while (!isEmpty(stack)) {
        stackElement = pop(stack);
        setBoard(gameParams->userBoard, N, stackElement->row, stackElement->col, stackElement->value);

        if (findFirstEmptyCell(gameParams->userBoard, N, &row, &col) == VALID) {
            pushValidValues(gameParams, row, col, stack);
        } else { /* All cells are full - found a valid solution */
            (*numOfSols)++;
        }
        /* Done dealing with this stackElement - free memory: */
        free(stackElement);
    }
    free(stack);
}

/* Precondition: board has no erroneous values (to be checked before calling this function).
 * This function returns the number of solutions for a given board using exhaustive backtracking,
 * it is called by num_solutions (defined in commands.h) */
int countSolutions(gameParams *game) {
    int numOfSols;
    cell ***tempBoard; /* This is a pointer to a *copy* of the board (cell ***) that we'll work on */
    gameParams *temporaryGameParams; /* This wraps tempBoard, N, n, m fields and passed to other functions */

    numOfSols = 0;
    /* First check if all N*N cells are assigned - if they are then the board is legally (precondition) full
     * and we return numOfSols == 1 */
    if ((game->counter) == (game->N * game->N)) {
        numOfSols = 1;
        return numOfSols;
    }

    /* Prepare gameParams object to pass to countWithBacktracking:
     * 1 - Copy current game board and mark ALL its assigned values as fixed
     * 2 - Wrap the tempBoard inside a gameParams struct */
    tempBoard = copyBoard(game->userBoard, game->N);
    markAsFixed(tempBoard, game->N);
    temporaryGameParams = wrapInGameParamsStruct(tempBoard, game->N, game->n,
                                                 game->m);
    /* Call the main Backtracking Iterative function: */
    countWithBacktracking(temporaryGameParams, &numOfSols);

    /* Free memory and return: */
    freeSudokuGame(temporaryGameParams);
    return numOfSols;
}

