//
// Created by amirlande on 8/2/2018.
//

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
gameParams *wrapInGameParamsStruct(BOARD*board, int N, int n, int m) {
    gameParams *partialGameParams;
    partialGameParams = (gameParams *) malloc(1 * sizeof(gameParams));
    if (partialGameParams == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    partialGameParams->N = N;
    partialGameParams->n = n;
    partialGameParams->m = m;
    partialGameParams->userBoard = *board;
    return partialGameParams;
}

void markAsFixed(BOARD*board, int N) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if ((*board)[i][j]->value != EMPTY) {
                (*board)[i][j]->isFixed = TRUE;
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
        if (checkIfValid(row, col, value,
                         partialGameParams)) { /* TODO adapt checkIfValid function to take cell *** rather than only gameParams */
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
                    (board)[i][j] = EMPTY;
                }
            }
        }
    }
    board[row][col]->value = value;
}

void countWithBacktracking(gameParams *partialGameParams, int *numOfSols) {
    Stack *stack;
    element *stackElement;
    int row, col, value, N;

    stack = initializeStack();
    N = partialGameParams->N;

    /* Set (row, col) to hold first empty cell */
    if (findFirstEmptyCell(partialGameParams->userBoard, partialGameParams->N, &row, &col) == INVALID) {
        printf("I thought should never get here - should have been caught at if ((game->counter) == (game->N * game->N))");
        *numOfSols = 1;
        return;
    }
    /* Push onto stack an element for each valid (row, col, value)
     * row and col are fixed, value ranges from 1 to N inclusive */
    pushValidValues(partialGameParams, row, col, stack);

    while (!isEmpty(stack)) {
        stackElement = pop(stack);
        setBoard(partialGameParams->userBoard, N, stackElement->row, stackElement->col, stackElement->value);

        if (findFirstEmptyCell(partialGameParams->userBoard, N, &row, &col) == VALID) {
            pushValidValues(partialGameParams, row, col, stack);
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
    BOARD*tempBoard; /* This is a pointer to a *copy* of the board (cell ***) that we'll work on */
    gameParams *partialGameParams; /* This wraps tempBoard, N, n, m fields and passed to other functions */

    /* First check if all N*N cells are assigned - if they are then the board is legally (precondition) full
     * and we return numOfSols == 1 TODO check carefully that count is incremented and decremented on changes */
    if ((game->counter) == (game->N * game->N)) {
        numOfSols = 1;
        return numOfSols;
    }

    /* Prepare gameParams object to pass to countWithBacktracking:
     * 1 - Copy current game board and mark ALL its assigned values as fixed
     * 2 - Wrap the tempBoard inside a gameParams struct */
    tempBoard = copyBoard(game->userBoard,
                          game->N); /* TODO implementation of copyBoard waits for allocateMatrix to be fixed */
    markAsFixed(tempBoard, game->N);
    partialGameParams = wrapInGameParamsStruct(tempBoard, game->N, game->n,
                                               game->m); /* TODO remember this may cause problems check while debugging */

    /* Call the main Backtracking Iterative function: */
    countWithBacktracking(partialGameParams, &numOfSols);

    /* Free memory and return: */
    freeSudokuGame(partialGameParams);
    return numOfSols;
}


#ifdef oldImplementation

/* Recursive version - to be altered to iterative version with a stack */
int countSolutionsRec(gameParams *game, cell ***copyOfBoard, int N) {
    int row, col;
    int val;
    int counter;

    counter = 0;
    if (findFirstEmptyCell(copyOfBoard, N, &row, &col) == INVALID) {
        /* no empty cell found - board is full with valid cells (assuming precondition of validity of all cells!) */
       counter = 1; /* only 1 solution */
    }

    else {
        /* getting here means (row, col) hold the indexes of the first empty cell */
        for (val = 1; val < N; val++) {
            if (checkIfValid(row, col, val, game)) { /* TODO - problematic - uses userBoard instead of the copy of the board */
                copyOfBoard[row][col]->value = val;
                counter = counter + countSolutionsRec(game, copyOfBoard, N);
            }
        }
    }
    return counter;
}

void countSolutionsTailRec(gameParams *game, cell ***copyOfBoard, int N, int counter) {
    int row, col;
    int val;
    Stack *stack;
    cell ***currentBoard;

    initializeStack(stack); /* TODO free memory */
    push(stack, copyOfBoard); /* push first snapshot onto stack */

    while (!isEmpty(stack)) {
        currentBoard = pop(stack);
        /* At this point we have all the variables we need for a regular recursive call:
         * game, N, counter - intact. currentBoard holds the snapshot value */

        if (findFirstEmptyCell(currentBoard, N, &row, &col) == INVALID) {
            /* no empty cell found - board is full with valid cells (assuming precondition of validity of all cells!) */
            counter = counter + 1; /* only 1 solution */
            /* TODO should we free the current snapshot memory here? */
        }
        else {
            /* getting here means (row, col) hold the indexes of the first empty cell at current copyOfBoard*/
            for (val = 1; val < N; val++) {
                currentBoard = copyBoard(currentBoard);
                if (checkIfValid(row, col, val,
                                 game)) { /* TODO - problematic - uses userBoard instead of the copy of the board */
                    currentBoard[row][col]->value = val;
                    /* Instead of a recursive call we push the new snapshot onto the stack */
                    push(stack, currentBoard);
                    /* countSolutionsTailRec(game, copyOfBoard, N, &counter); */
                }
            }
        }
    }
}

/* precondition: board has no erroneous values (to be checked before calling this function)
 * the function returns the number of solutions for a given board using exhaustive backtracking
 * it is called by num_solutions (in commands.h) */
int countSolutions(gameParams *game) {
    int numOfSols;
    cell ***tempBoard;
    tempBoard = copyBoard(game->userBoard); /* TODO implementation of copyBoard */

    numOfSols = 0;
    numOfSols = countSolutionsRec(game, tempBoard, game->N);
    freeBoard(tempBoard, game->N); /* free memory before returning */
    return numOfSols;
}

#endif