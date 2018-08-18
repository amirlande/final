//
// Created by eran on 31/07/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include "gameUtils.h"


/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int check_err_cells(gameParams *game) {
    /* to be implemented */
}


/* allocates memory for a new board and copies values of
 * board_to_be_copied. returns pointer to the new board struct*/
cell **copy_board(cell **board_to_be_copied) {
    /* to be implemented */
}

/* frees memory of a given board */
void free_board(cell **board_to_be_freed) {
    /* to be implemented */
}

/* returns the line separator for print_board
 * consists 4N+m+1 dashes ('-')
 * exits with exit(0) if failed to malloc */
char *getLineSeparator(gameParams *game) {
    char *separator;
    int N, n, m, i;
    n = game->n;
    m = game->m;
    N = m * n;

    separator = malloc(sizeof(char) * (4 * N + m + 1));
    if (separator == NULL) {
        printf("Error: malloc has failed\n");
        exit(0);
    }

    for (i = 0; i < 4 * N + m + 1; i++) {
        separator[i] = '-';
    }

    return separator;
}

/* get an instance of a game if needed for tests */
gameParams *getExampleGame() {
    gameParams *game2 = null;
    int i, j;
    game2 = (gameParams *) malloc(sizeof(gameParams));
    if (game2 == NULL) {
        printf("Error: malloc has failed\n");
        free(game2);
        exit(0);
    }


    game2->n = 5;
    game2->m = 2;
    game2->gameMode = edit;
    game2->markErrors = 1;
    game2->movesList = null;
    game2->counter = 0;
    game2->userBoard = allocateMatrix(game2->userBoard, game2->n * game2->m);

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            game2->userBoard[i][j] = malloc(sizeof(cell));
            game2->userBoard[i][j].value = i * j;
            game2->userBoard[i][j].isValid = 1;

            if (i % 3 == 0) {
                game2->userBoard[i][j].isFixed = 1;
            }
            if (j % 3 == 0) {
                game2->userBoard[i][j].isValid = 0;
            }
        }
    }


    /*
     *
     * typedef struct cell {
    int value;
    int isFixed;
    int isValid;  isValid == 1 means it's not erroneous,
} cell;
     *
     *
     *
     *
     *     enum gameMode mode;
    int markErrors;
    int n;
    int m;
    cell **userBoard;
    cell **solution;
    int counter;
    listOfMoves movesList;
     */


    game2->solution = allocateMatrix(game2->solution);
    game2->counter = 0;


}


/* Allocates memory for matrix mat with NxN values */
int **allocateMatrix(int **mat, int N) {

    int i;
    mat = calloc(N, sizeof(int *));
    if (mat == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
    for (i = 0; i < N; i++) {
        mat[i] = calloc(N, sizeof(int));
        if (mat[i] == NULL) {
            printf("Error: calloc has failed\n");
            exit(0);
        }
    }

    return mat;
}
