
#ifndef FINAL_COMMANDS_H
#define FINAL_COMMANDS_H


#include <stdio.h>
#include <stdlib.h>

#include "gameUtils.h"
#include "input_output.h"
#include "solver.h"
#include "gurobi.h"
#include "memoryAllocation.h"


/* Starts a puzzle at SOLVE mode
 * Pre:
 * game is at SOLVE, EDIT or INIT mode (available in all modes)
 * If the file exists - it is contains valid data ans is well formatted
 * filePath is a well-formatted file path (not NULL)
 *
 * Post:
 * If file does not exist or cannot be opened - print error
 * If file exists - starts puzzle in solve mode
 * All fields of game struct are updated according to new board (loaded from file)
*/
int solve(gameParams *game, char *filePath);


int edit(gameParams *game, char *filePath);

/* preconditions: 1. called only in SOLVE mode 2. X is either 0 or 1
 * (preconditions should be verified in parser module) */
void mark_errors(gameParams *game, int X);

/* prints the Sudoku board */
void printBoard(gameParams *game);

/* Sets new value Z for cell X Y
 *
 * Preconditions:
 * command is valid
 * x,y,z are valid and integers
 *
 * Post:
 * value z is set to [x][y], currentMove holds the right data with the change.
 * lists and nodes are updated properly */
int set(int x, int y, int z, gameParams *game);

/* Preconditions: 1. called only on EDIT or SOLVE modes
 * the function first checks whether there are erroneous values
 * if no erroneous cells where found - uses ILP to determine whether the board is solvable */
int validate(gameParams *game);

/* Pre:
 * Available in EDIT mode only
 * x, y are valid integers */
int generate(gameParams *game, int x, int y);

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

int save(gameParams *game, char *filePath);

int hint(int x, int y, gameParams *game);

/* preconditions: 1. called only on EDIT or SOLVE modes
 * prints the number of solutions for the current board
 * the function first checks whether there are erroneous values
 * if no erroneous cells were found - counts the number of possible solutions */
int numSolutions(gameParams *game);

/* Automatically fill "obvious" values
 * cells which contain a single legal value
 *
 * Pre:
 * game is at Solve mode */
int autoFill(gameParams *game);

/* resets all moves
 *
 * Pre:
 * game is at Solve mode
 *
 * Post:
 * all moves are undone
 * all move nodes are freed except head node
 * */
int reset(gameParams *game);

/* Exits the game */
void exitGame(gameParams *game);


#endif
