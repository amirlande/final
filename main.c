#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include "gameParams.h"
#include "mainAux.h"
#include "memoryAllocation.h"

void playSudoku() {
    gameParams *game;
    int continueGame;

    printf("Sudoku\n------\n");
    fflush(stdout); /* TODO check on fflush*/
    game = initSudokuGame();

    continueGame = TRUE;
    while (continueGame) {
        continueGame = playTurn(game); /* playTurn returns TRUE (1) on all commands except EXIT */
        fflush(stdout);
    }

    freeSudokuGame(game);
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
        exit(EXIT_FAILURE);
    }

    n = 0;
    m = 0;
    while (n == 0) {n = rand()%6;}
    while (m == 0) {m = rand()%6;}
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

void testFileOpening() {
    FILE *file;

    /* Reading existing, RELATIVE */
    file = fopen("C:\\Users\\amirlande\\CLionProjects\\final\\cmake-build-debug\\board1", "r");
    if (file == NULL) {
        printf("Error opening ""board1"" for reading\n");
    } else {
        printf("Success\n");
    }
    fclose(file);

    /* Writing to existing, RELATIVE */
    file = fopen("board3", "w");
    if (file == NULL) {
        printf("Error opening ""board1"" for writing\n");
    }
    else {
        printf("Success\n");
    }
    fclose(file);


}

int main() {
    int seed;
    seed = time(NULL);
    srand(seed);

    emptyBoardGenerator("randBoard"); /* TODO delete this before submitting project! */
    //testFileOpening();

    playSudoku();
    return 1;
}


#ifdef OLDMAIN
int main() {


    /* testing - merged from Amir's branch
    printf("%d", checkIfNumericString("345"));
    printf("%d", checkIfNumericString("345.5"));
    printf("%d", checkIfNumericString("4555a5"));
    printf("%d", checkIfNumericString(" "));

    */
    /* testing - merged from Eran's branch */
    int x, y, k;
    gameParams *game = getExampleGame(5, 2);
    printBoard(game);
    /*  undo(game);
      redo(game);*/
    printf("%d\n", game->userBoard[5][5]->value);
    printf("%d all\n", checkIfValid(5, 5, 1, game));
    printf("%d square\n", checkIfSquareValid(5, 5, 5, game));
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++) {
            for (k = 1; k < 11; k++) {
                if (checkIfValid(x, y, k, game)) {
                    printf("cell <%d,%d> (%d) can have %d\n", y, x,game->userBoard[x][y]->value, k);
                }
            }
        }
    }
    return 1;

}
#endif