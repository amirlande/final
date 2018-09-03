//
// Created by amirlande on 8/2/2018.
//

#ifndef FINAL_INPUT_OUTPUT_H
#define FINAL_INPUT_OUTPUT_H

#include <stdio.h>
#include <ctype.h>
#include "gameParams.h"
#include "gameUtils.h"

int saveGameParamsToFile(gameParams *game, FILE *destfile, enum gameMode mode);

int loadGameParamsFromFile(gameParams *game, FILE *sourcefile, enum gameMode mode);


#endif //FINAL_INPUT_OUTPUT_H
