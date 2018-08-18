#include <stdio.h>
#include "parser.h" /* gives access to the parser functions */
#include "gameUtils.h" /* gives access to functions required to initialize game
 * (initGame, createNewBoard) */

#include "commands.h"

int main() {
    gameParams *game = getExampleGame(5,2);
    print_board(game);
}