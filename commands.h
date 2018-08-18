//
// Created by eran on 31/07/18.
//
#ifndef FINAL_COMMANDS_H
#define FINAL_COMMANDS_H

#include "gameUtils.h" /* gives access to all struct definitions */
#include "solver.h" /* gives access to solving functions */
#include "input_output.h" /* gives access to edit, solve and save commands */

/* prints the Sudoku board */
void printBoard(gameParams *game);

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


/* Pre:
 * command is valid
 * game is at edit or solve mode
 *
 * Post:
 * last command was undone
 * lists and nodes are updated properly */
int undo(gameParams *game);


/* Pre:
 * command is valid
 * game is at edit or solve mode
 *
 * Post:
 * last command that was undone is redone
 * lists and nodes are updated properly */
int redo(gameParams *game);

#endif //FINAL_COMMANDS_H
