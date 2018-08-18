#include <stdio.h>
#include <stdlib.h>
#include "parser.h" /* gives access to the parser functions */
#include "gameUtils.h" /* gives access to functions required to initialize game
 * (initGame, createNewBoard) */

#include "commands.h"
#include "etc.h"


int main() {
    int i;
    gameParams *game = getExampleGame(3, 3);


    printBoard(game);


    printf("next");
    scanf("%d", &i);


    printf("%d\n", game->movesList->currentMove->change->prevVal->value);


    printf("next");
    scanf("%d", &i);


    undo(game);


    printf("next");
    scanf("%d", &i);


    redo(game);

    return 1;
}