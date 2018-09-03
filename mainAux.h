//
// Created by amirlande on 9/1/2018.
//

#ifndef FINAL_MAINAUX_H
#define FINAL_MAINAUX_H

#include "parser.h"

/* reads next command from user using getCommandFromUser and invokes the relevant function from "commands.h"
 * return value: FALSE (0) if user enters EXIT or completes the board, TRUE (1) otherwise (game continues)*/
int playTurn(gameParams *game);

#endif //FINAL_MAINAUX_H
