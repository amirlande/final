//
// Created by eran on 31/07/18.
//
#ifndef FINAL_COMMANDS_H
#define FINAL_COMMANDS_H

#include "gameUtils.h" /* gives access to all struct definitions */
#include "solver.h" /* gives access to solving functions */
#include "input_output.h" /* gives access to edit, solve and save commands */

#define MAX_NUMBER_OF_ATTEMPTS 1000

/* prints the Sudoku board */
void printBoard(gameParams *game);

/* preconditions: 1. called only in SOLVE mode 2. X is either 0 or 1
 * (preconditions should be verified in parser module) */
void mark_errors(gameParams *game, int X);

/* preconditions: 1. called only on EDIT or SOLVE modes
 * the function first checks whether there are erroneous values
 * if no erroneous cells where found - uses ILP to determine whether the board is solvable */
int validate(gameParams *game);

/* Pre:
 * Available in EDIT mode only
 * x, y are valid integers */
int generate(gameParams *game, int x, int y);

/* preconditions: 1. called only on EDIT or SOLVE modes
 * prints the number of solutions for the current board
 * the function first checks whether there are erroneous values
 * if no erroneous cells where found - counts the number of possible solutions */
int numSolutions(gameParams *game);

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

int hint(int x, int y, gameParams *game);

/* Automatically fill "obvious" values
 * cells which contain a single legal value
 *
 * Pre:
 * game is at Solve mode */
int autoFill(gameParams *game);

/* Pre:
 * command is valid
 * game is at edit or solve mode
 *
 * Post:
 * last command was undone
 * lists and nodes are updated properly */
int undo(gameParams *game);


/* the REAL undo.
 * enveloped by the func named "undo".
 * made this change for the reset func */
int undoEnveloped(gameParams *game, int isReset);

/* Pre:
 * command is valid
 * game is at edit or solve mode
 *
 * Post:
 * last command that was undone is redone
 * lists and nodes are updated properly */
int redo(gameParams *game);

/* Starts a puzzle at SOLVE mode
 * Pre:
 * game is at SOLVE, EDIT or INIT mode (available in all modes)
 * If the file exists - it is contains valid data ans is well formatted
 *
 * Post:
 * If file does not exist or cannot be opened - print error
 * If file exists - starts puzzle in solve mode
*/
int solve(gameParams *game, char *filePath);


int edit(gameParams *game, char *filePath);

int save(gameParams *game, char *filePath);

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

#endif //FINAL_COMMANDS_H
