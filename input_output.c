//
// Created by amirlande on 8/2/2018.
//

#include "input_output.h"

int saveGameParamsToFile(gameParams *game, FILE *destFile, enum gameMode mode) {
    int row, col;

    fprintf(destFile, "%d %d\n", game->m, game->n);
    for (row = 0; row < game->N; row++) {
        for (col = 0; col < game->N; col++) {
            fprintf(destFile, "%d", game->userBoard[row][col]->value);
            /* If in EDIT mode or cell is fixed - write an extra '.' next to digit */
            if ((game->userBoard[row][col]->isFixed == TRUE) || (mode == EDIT_MODE)) {
                fprintf(destFile, ".");
            }
            /* If haven't reached end of row write space */
            if (col < (game->N - 1)) {
                fprintf(destFile, " ");
            }
        }
        fprintf(destFile, "\n");
    }
    return TRUE;

}

/* Should explicitly update all game fields! */
int loadGameParamsFromFile(gameParams *game, FILE *sourceFile, enum gameMode mode) {
    int row, col, m, n, N;
    int c;

    /* Get new parameters for gameParams fields: */
    /* First get m, n (ignoring white spaces) */
    while (isspace(c = fgetc(destFile))) {}
    m = c - '0';
    while (isspace(c = fgetc(destFile))) {}
    n = c - '0';
    N = n * m;

    /* Allocate new matrix */
    cell ***loadedUserBoard = allocateCellMatrix(N);

    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            /* TODO implement */
        }
    }

    game->mode = mode;
    game->m = m;
    game->n = n;
    game->N = N;
    game->markErrors = TRUE;


    return TRUE;
}
