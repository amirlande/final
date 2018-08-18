//
// Created by eran on 18/08/18.
//

#include <zconf.h>

#include "etc.h"


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
            game2->userBoard[i][j]->value = i + j;
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

