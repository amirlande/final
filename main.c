#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "gameParams.h"
#include "mainAux.h"
#include "memoryAllocation.h"
#include "tests.h"

void playSudoku() {
    gameParams *game;
    int continueGame;

    printf("Sudoku\n------\n");
    fflush(stdout);
    game = initSudokuGame();

    continueGame = TRUE;
    while (continueGame) {
        continueGame = playTurn(game); /* playTurn returns TRUE (1) on all commands except EXIT */
        fflush(stdout);
       /* printf("game counter: %d\n", game->counter);*/
    }
}

/* Only for testing - this function randomly picks m, n between 1 and 5
 * and saves an N*N empty board under the passed filePath
 * This board can be then used to generate a random sudoku board using the generate command in EDIT mode */
int emptyBoardGenerator(char *filePath) {
    int n, m, N;
    int row, col;
    FILE *file;

    file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Failed to open file at emptyBoardGenerator\n");
        exit(0);
    }

    n = 3;
    m = 1;

    while (n == 0) { n = rand() % 6; }
    while (m == 0) { m = rand() % 6; }

    N = n * m;
    printf("N = %d, n = %d, m = %d\n", N, n, m);

    fprintf(file, "%d %d\n", m, n);
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            fprintf(file, "%d", 0);
            /* If haven't reached end of row write space */
            if (col < (N - 1)) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
    return TRUE;
}

int main() {
    int seed;
    seed = time(NULL);
    srand(seed);

   emptyBoardGenerator("xxx");

    playSudoku();

/*
    int x, y, i, j, k, **board;
    gameParams *game;
    game = (gameParams *) malloc(sizeof(gameParams));
    game->userBoard = allocateCellMatrix(2);
    game->n = 1;
    game->m = 2;
    game->counter = 0;
    game->solution = allocateCellMatrix(2);
    game->N = 2;
    game->markErrors = 1;
    game->movesList = allocateMoveList();
    for (x = 0; x < 2; x++) {
        for (y = 0; y < 2; y++) {
            if (rand() % 2 == 1) {
                game->userBoard[x][y]->value = rand() % 2 + 1;
            } else {
                game->userBoard[x][y]->value = 0;
            }
        }
    }


    board = fromCellMatToIntMat(game->userBoard, 2);
    printBoard(game);

    for (x = 0; x < 2; x++) {
        for (y = 0; y < 2; y++) {
            for (k = 1; k < 3; k++) {
                if (checkIfValid(x, y, k, game)) {
                    printf("cell <%d,%d> (%d) can have %d\n", x + 1, y + 1, game->userBoard[y][x]->value, k);
                }
            }
        }
    }

*/
    return 1;
}