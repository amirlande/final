//
// Created by amirlande on 9/1/2018.
//

#ifndef FINAL_GAMEPARAMS_H
#define FINAL_GAMEPARAMS_H

#include "list.h"

/* the struct that represents a cell in the Sudoku board
 * each cell contains information regarding its value, whether it is fixed and whether it is valid*/
typedef struct cell {
    int value;
    int isFixed;
    int isValid; /* isValid == 1 means it's not erroneous, isValid == 0 means value is erroneous */
} cell;


enum gameMode {
    INIT_MODE,
    SOLVE_MODE,
    EDIT_MODE
};

/* the main struct that represents the Sudoku game */
typedef struct gameParams {
    enum gameMode mode;
    int markErrors;
    int n;
    int m;
    int N;
    cell ***userBoard;
    cell ***solution;
    int counter;
    listOfMoves *movesList;
} gameParams;

#endif //FINAL_GAMEPARAMS_H
