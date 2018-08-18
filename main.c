#include <stdio.h>
#include <stdlib.h>
#include "parser.h" /* gives access to the parser functions */
#include "gameUtils.h" /* gives access to functions required to initialize game
 * (initGame, createNewBoard) */

#include "commands.h"
#include "etc.h"


int main() {
    gameParams *game = getExampleGame(5, 4);
    printBoard(game);
}