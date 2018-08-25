#include <stdio.h>
#include <stdlib.h>
#include "parser.h" /* gives access to the parser functions */
#include "gameUtils.h" /* gives access to functions required to initialize game
 * (initGame, createNewBoard) */



#include "parser.h"
#include "commands.h"
#include "etc.h"

int main() {


    /* testing - merged from Amir's branch
    printf("%d", checkIfNumericString("345"));
    printf("%d", checkIfNumericString("345.5"));
    printf("%d", checkIfNumericString("4555a5"));
    printf("%d", checkIfNumericString(" "));
    */
    /* testing - merged from Eran's branch */

    int i;
    gameParams *game = getExampleGame(5, 4);
    printBoard(game);
    undo(game);
    redo(game);
    return 1;
}