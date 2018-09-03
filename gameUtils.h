/* this header includes all struct definitions used by the program \
 * as well as utility functions */

#ifndef FINAL_GAMEUTILS_H
#define FINAL_GAMEUTILS_H

#include <stdlib.h>
#include <stdio.h>
#include "gameParams.h"

#define TRUE 1
#define FALSE 0
#define VALID 1
#define INVALID 0
#define EMPTY 0
#define BOARD cell ***


/* Allocates memory for a new gameParams struct
 * Initializes its fields to default values
 * Called once by the play() function in the beginning of the program */
gameParams *initSudokuGame();

/* Pre:
 * 1. Memory for game already allocated
 * 2. Memory for game internal fields *not* allocated
 * Post: Initializes game fields to a (m*n)*(m*n) sudoku game */
void initializeSudokuGameFields(gameParams *game, int m, int n);

/* Frees all memory allocated to game, including game itself
 * (This is the complementary free function of initSudokuGame() */
int freeSudokuGame(gameParams *game);

/* Frees memory allocated to game fields, and initilizes its fields */
void cleanSudokuGame(gameParams *game);


/* Allocates memory for cell matrix mat with NxN values
 * This call allocated memory for all cells, and it initializes each cell's fields to:
 * cell->value = 0
 * cell->isValid = TRUE (1)
 * cell-isFixed = FALSE (0)*/
cell ***allocateCellMatrix(int N);

/* Frees all memory allocated to the given board
 * (This is the complementary free function of allocateCellMatrix */
void freeCellMatrix(cell ***mat, int N);

/* TODO - this function may need changes - when is it used? */
/* gets a gameParams instance after one malloc */
int createNewGame(gameParams *game, int n, int m) ;


/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int hasErrCells(gameParams *game);

int boardIsEmpty(gameParams *game);

/* Allocates memory for a new board and copies values of
 * board_to_be_copied.
 * Returns pointer to the new board struct (Notice - it is a cell ****) */
BOARD*copyBoard(cell ***board_to_be_copied, int N);

/* returns the line separator for print_board
 * consists 4N+m+1 dashes ('-')
 * exits with exit(0) if failed to malloc */
char *getLineSeparator(gameParams *game);


int checkIfValid(int x, int y, int z, gameParams *game);

/* prints the changes after undo/redo */
int printChanges(gameParams *game, cellChangeRecNode *moveToPrint, int isRedo);


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

/* Called by autoFill */
void setNewChangeListToGame(gameParams *game, cellChangeRecNode *changeListHead);

/* "Constructor" - creates a cell with the passed value. By default new cells are valid and no fixed TODO */
cell *createCell(int value);


/* the REAL undo.
 * enveloped by the func named "undo".
 * made this change for the reset func */
int undoEnveloped(gameParams *game, int isReset);

int randomlyFillXCells(gameParams *game, int x);

/* Return TRUE (1) on success, FALSE (0) on failure */
int randomlyFillXCellsAndSolve(gameParams *game, int x);

void randomlyClearYCells(gameParams *game, int y);





#endif //FINAL_GAMEUTILS_H
