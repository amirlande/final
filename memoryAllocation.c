
#include "memoryAllocation.h"


/* Allocates memory for a new gameParams struct
 * Initializes its fields to default values
 * Called once by the play() function in the beginning of the program */
gameParams *initSudokuGame() {
    gameParams *newGame;

    newGame = (gameParams *) malloc(sizeof(gameParams));
    if (newGame == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }

    newGame->mode = INIT_MODE;
    newGame->markErrors = 1;
    newGame->m = 0;
    newGame->n = 0;
    newGame->N = 0;
    newGame->solution = NULL;
    newGame->userBoard = NULL;
    newGame->counter = 0;
    newGame->movesList = allocateMoveList(); /* TODO - ask Eran how should be initialized here as well as in initializeSudokuGameFields */
    return newGame;
}

void initializeSudokuGameFields(gameParams *game, int m, int n) {
    game->m = m;
    game->n = n;
    game->N = m * n;
    game->mode = INIT_MODE;
    game->markErrors = TRUE;
    game->counter = 0;
    game->userBoard = allocateCellMatrix(game->N);
    game->solution = allocateCellMatrix(game->N);
    /* game->movesList = allocateMoveList(); no required since in cleanSudokuGame we don't free listOfMoves memory */
}

/* Frees all memory allocated to game parameter
 * (This is the complementary free function of initSudokuGame() */
int freeSudokuGame(gameParams *game) {

    if (game->userBoard != NULL) {
        freeCellMatrix(game->userBoard, game->m * game->n);
    }
    if (game->solution != NULL) {
        freeCellMatrix(game->solution, game->m * game->n);
    }
    if (game->movesList != NULL) {
        freeAllUserMoveNodes(game->movesList->head);
        free(game->movesList);
    }
    free(game);

    return 1;
}

/* Frees memory allocated to game fields, and initilizes its fields */
void cleanSudokuGame(gameParams *game) {
    game->markErrors = TRUE;
    game->m = 0;
    game->n = 0;
    game->N = 0;
    game->mode = INIT_MODE;
    game->counter = 0;
    freeCellMatrix(game->userBoard, game->N);
    freeCellMatrix(game->solution, game->N);
    /* Free all memory used by moveList nodes and set head and current to NULL */
    freeAllUserMoveNodes(game->movesList->head); /* TODO ask Eran about this */
    game->movesList->currentMove = NULL;
    game->movesList->head = NULL;
}

/* Frees all memory allocated to the given board
 * (This is the complementary free function of allocateCellMatrix */
void freeCellMatrix(cell ***mat, int N) {

    int i, j;
    if (mat == NULL) {
        return;
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            free(mat[i][j]);
        }
        free(mat[i]);
    }
    free(mat);
}


/* frees all the userMoveNode
 * starting from moveToFree to the end */
void freeAllUserMoveNodes(userMoveNode *moveToFree) {

    if (moveToFree == NULL) {
        return;
    }

    freeAllUserMoveNodes(moveToFree->next);
    freeCellChangeRecNode(moveToFree->change);
    free(moveToFree);
}

/* frees all the freeCellChangeRecNode
 * starting from change to the end */
void freeCellChangeRecNode(cellChangeRecNode *changeToFree) {

    if (changeToFree == NULL) {
        return;
    }

    freeCellChangeRecNode(changeToFree->next);

    free(changeToFree->prevVal);
    free(changeToFree->currVal); /* TODO Causes program to crash on specific scenario */
    free(changeToFree);

}

/* Allocates memory for cell matrix mat with NxN values
 * This call allocated memory for all cells, and it initializes each cell's fields to:
 * cell->value = 0
 * cell->isValid = TRUE (1)
 * cell-isFixed = FALSE (0)*/
cell ***allocateCellMatrix(int N) {
    int i, j;
    cell ***mat;

    mat = (cell ***) malloc(N * sizeof(cell **));
    if (mat == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < N; i++) {
        mat[i] = (cell **) malloc(N * sizeof(cell *));
        if (mat[i] == NULL) {
            printMallocFailed();
            exit(EXIT_FAILURE);
        }
        for (j = 0; j < N; j++) {
            mat[i][j] = createCell(0);
        }
    }
    return mat;
}

/* "Constructor" - creates a cell with the passed value. By default new cells are valid and no fixed TODO */
cell *createCell(int value) {
    cell *newCell = (cell *) malloc(sizeof(cell));
    if (newCell == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    newCell->value = value;
    newCell->isValid = TRUE;
    newCell->isFixed = FALSE;
    return newCell;
}


/* Allocates memory for int matrix
 * with size N*N */
int **allocateIntMatrix(int N) {

    int i, **mat;
    mat = calloc((size_t)N, sizeof(int *));
    if (mat == NULL) {
        printCallocFailed();
        exit(0);
    }
    for (i = 0; i < N; i++) {
        mat[i] = calloc((size_t)N, sizeof(int));
        if (mat[i] == NULL) {
            printCallocFailed();
            exit(0);
        }
    }

    return mat;
}


/* Frees memory allocated for matrix mat of size n*/
void freeIntMatrix(int **mat, int n) {
    int i;
    for (i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}


listOfMoves *allocateMoveList() {
    listOfMoves *list;
    list = (listOfMoves *) malloc(sizeof(listOfMoves));
    if (list == NULL) {
        printMallocFailed();
        return NULL;
    }
    list->head = NULL;
    list->currentMove = NULL;
    /* list->size = 0; */
    return list;
}
