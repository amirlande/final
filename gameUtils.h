/* this header includes all struct definitions used by the program \
 * as well as utility functions */

#ifndef FINAL_GAMEUTILS_H
#define FINAL_GAMEUTILS_H

#include <stdlib.h>
#include <stdio.h>
#include "gameParams.h"
#include "errorMessages.h"
#include "memoryAllocation.h"

/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int hasErrCells(gameParams *game);

int boardIsEmpty(gameParams *game);

/* Allocates memory for a new board and copies values of
 * board_to_be_copied.
 * Returns pointer to the new board struct (Notice - it is a cell ****) */
cell ***copyBoard(cell ***board_to_be_copied, int N);

/* returns the line separator for print_board
 * consists 4N+m+1 dashes ('-')
 * exits with exit(0) if failed to malloc */
char *getLineSeparator(gameParams *game);


int checkIfValid(int x, int y, int z, gameParams *game);


/* prints the changes after undo/redo */
int printChanges(cellChangeRecNode *moveToPrint, int isRedo);


/* Called by undo
 * implemented recursively for printing in thr right order:
 * make changes -> print board -> print changes
 * prints at the opposite order, MIGHT NOT BE USED!!
 * */
int makeRecChanges(gameParams *game, cellChangeRecNode *moveToUndo);

/* Checks if value z does not appear his 3x3 square in the matrix */
int checkIfSquareValid(int x, int y, int z, gameParams *game);

/* Checks if value z does not appear in row x */
int checkIfRowValid(int x, int y, int z, gameParams *game);

/* Checks if value z does not appear in column y */
int checkIfColumnValid(int x, int y, int z, gameParams *game);

/* Returns the only legal value
 * for the empty Cell [x][y]
 * returns FALSE - iff has 0, or more than 1 values */
int doesCellHaveASingleLegalValue(gameParams *game, int x, int y);


/* sets a new value z to cell [x][y] */
void setValue(gameParams *game, int x, int y, int z);

/* Called by autoFill
 * returns the list of changes */
cellChangeRecNode *getAutoFillChangeList(gameParams *game, int *numOfChanges);


void setValuesBychangeListHead(gameParams *game, cellChangeRecNode *changeListNode);


/* Called by autoFill */
void setNewChangeListToGame(gameParams *game, cellChangeRecNode *changeListHead);

/* "Constructor" - creates a cell with the passed value. By default new cells are valid and no fixed TODO */
cell *createCell(int value);


/* Allocates memory to new nodes
 * sets the curr and prev pointers
 * -- no data is added -- */
void getNewCurrentMove(gameParams *game);


/* Cleans game->userBoard and game->solution to zero's */
void cleanUserBoardAndSolution(gameParams *game);

/* Used by randomlyFillXCells */
void setToEmpty(int **matrix, int N);

void markFullCellsAsFixed(cell ***board, int N);


/* Iterates over each cell
 * and check if it is valid
 * or erroneous */
void updateErrors(gameParams *game);

void copyCell(cell *src, cell *dst);

#endif
