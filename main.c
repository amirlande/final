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
    }
}


int main() {
    int seed;
    seed = time(NULL);
    srand(seed);

    playSudoku();
    return 1;
}