#include <stdio.h>
#include <stdlib.h>
#include "gameUtils.h" /* access to functions required to initialize game
 * (initGame, createNewBoard) */
#include "etc.h"
#include "mainAux.h"

/* TODO main module should eventually just include mainAux.h and use its playTurn function (as an interface to the parser)
 * */

 void playSudoku() {
     gameParams *game;
     int continueGame;

     printf("Sudoku\n------\n");
     fflush(stdout); /* TODO check on fflush*/
     game = initSudokuGame();

     continueGame = TRUE;
     while(continueGame) {
         continueGame = playTurn(game); /* playTurn returns TRUE (1) on all commands except EXIT */
     }

     freeSudokuGame(game);
 }

 int main() {
    /* Set srand and seed stuff and call play() */

     playSudoku();
     return  1;
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