//
// Created by eran on 31/07/18.
//
#ifndef FINAL_COMMANDS_H
#define FINAL_COMMANDS_H

#include "gameUtils.h"

/* prints the Sudoku board */
void print_board(gameParams *game);

/* preconditions: 1. called only in SOLVE mode 2. X is either 0 or 1
 * (preconditions should be verified in parser module) */
void mark_errors(gameParams *game, int X);

/* preconditions: 1. called only on EDIT or SOLVE modes
 * the function first checks whether there are erroneous values
 * if no erroneous cells where found - uses ILP to determine whether the board is solvable */
int validate(gameParams *game);

/* preconditions: 1. called only on EDIT or SOLVE modes
 * prints the number of solutions for the current board
 * the function first checks whether there are erroneous values
 * if no erroneous cells where found - counts the number of possible solutions */
int num_solutions(gameParams *game);



#endif //FINAL_COMMANDS_H
