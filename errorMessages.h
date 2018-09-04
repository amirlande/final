//
// Created by amirlande on 8/23/2018.
//

#ifndef FINAL_ERRORMESSAGES_H
#define FINAL_ERRORMESSAGES_H

#include <stdio.h>
#include "gameParams.h"


/* serves as a "shouldn't get here" print function with function and module name */
void printErrorInCodeFlow(char *functionName, char *moduleName);

void printNotImplementedMessage(char *nameOfFunction);

void printMallocFailed();

void printCallocFailed();

void printErrorOpeningFile(enum gameMode mode);

#endif //FINAL_ERRORMESSAGES_H
