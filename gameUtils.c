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
