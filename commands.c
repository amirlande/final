//
// Created by eran on 31/07/18.
//

#include "commands.h"

/* preconditions: 1. called only in SOLVE mode 2. X is either 0 or 1
 * (preconditions should be verified in parser module) */
void mark_errors(gameParams *game, int X) {
    game->markErrors = X;

    /* need to implement the change in the appearance of the table */
};