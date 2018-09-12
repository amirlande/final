

#ifndef FINAL_MEMORYALLOCATION_H
#define FINAL_MEMORYALLOCATION_H

#include "gameParams.h"
#include "errorMessages.h"

/* 1. gameParams (the whole game): */

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

/* Frees memory allocated to game fields, and initializes its fields */
void cleanSudokuGame(gameParams *game);

/* 2. Board (cell ***) and (int **) */

/* Allocates memory for cell matrix mat with NxN values */
cell ***allocateCellMatrix(int N);

/* Frees all memory allocated to the given board
 * (This is the complementary free function of allocateCellMatrix */
void freeCellMatrix(cell ***mat, int N);


/* "Constructor" - creates a cell with the passed value. By default new cells are valid and no fixed TODO */
cell *createCell(int value);

/* Allocates memory for int matrix
 * with size N*N */
int **allocateIntMatrix(int N);

/* Frees memory allocated for matrix mat of size n*/
void freeIntMatrix(int **mat, int n);


/* 3. List of moves: */

/* frees all the userMoveNode
 * starting from moveToFree to the end */
void freeAllUserMoveNodes(userMoveNode *moveToFree);

/* frees all the freeCellChangeRecNode
 * starting from change to the end */
void freeCellChangeRecNode(cellChangeRecNode *changeToFree);

listOfMoves *allocateMoveList();

#endif
