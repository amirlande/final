
#ifndef FINAL_PARSER_H
#define FINAL_PARSER_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "commands.h" /* access to all user commands the parser invokes after parsing input */
#include "errorMessages.h"
#include "commandStruct.h"

/* parses command from user console input into a userCommand struct and then invokes relevant function from "commands.h"
 * returns a commandStruct with a "isValid" indication. Calling function must check the "isValid" field of the returned value
 * important: this function allocates memory for the commandStruct it returns
 * therefore after resolving the command (i.e. calling SET function with the right args)
 * the resolveCommand function should free this memory*/
commandStruct *getCommandFromUser(gameParams *game);

#endif
