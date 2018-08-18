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
gameParams *getExampleGame(int n, int m) {
    gameParams *game2 = NULL;
    int i, j;
    game2 = (gameParams *) malloc(sizeof(gameParams));
    if (game2 == NULL) {
        printf("Error: malloc has failed\n");
        free(game2);
        exit(0);
    }


    game2->n = n;
    game2->m = m;
    game2->mode = edit;
    game2->markErrors = 1;
    game2->counter = 0;
    game2->userBoard = allocateCellMatrix(game2->userBoard, game2->n * game2->m);

    for (i = 0; i < m * n; i++) {
        for (j = 0; j < m * n; j++) {
            game2->userBoard[i][j] = (cell *) malloc(sizeof(cell *));
            game2->userBoard[i][j]->value = i * j;
            game2->userBoard[i][j]->isValid = 1;

            if (i % 3 == 0) {
                game2->userBoard[i][j]->isFixed = 1;
            }
            if (j % 3 == 0) {
                game2->userBoard[i][j]->isValid = 0;
            }
        }
    }


    game2->solution = allocateCellMatrix(game2->solution, m * n);
    game2->counter = 0;

    return game2;

}


/* Allocates memory for cell matrix mat with NxN values */
cell ***allocateCellMatrix(cell ***mat, int N) {

    int i;
    mat = (cell ***) malloc(N * sizeof(cell ***));
    if (mat == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
    for (i = 0; i < N; i++) {
        mat[i] = (cell **) malloc(N * sizeof(cell **));
        if (mat[i] == NULL) {
            printf("Error: calloc has failed\n");
            exit(0);
        }
    }

    return mat;
}
