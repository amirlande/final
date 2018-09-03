//
// Created by amirlande on 8/28/2018.
//

#ifndef FINAL_MEMORYALLOCATION_H
#define FINAL_MEMORYALLOCATION_H

#include "gameUtils.h" /* gives access to all struct definitions */
#include "solver.h" /* gives access to solving functions */
#include "input_output.h" /* gives access to edit, solve and save commands */


/* frees all game components */
int freeGame(gameParams *game);

/* frees all memory allocated to the given board */
void freeBoard(cell ***mat, int N);

/* frees all the userMoveNode
 * starting from moveToFree to the end */
void freeAllUserMoveNodes(userMoveNode *moveToFree);

/* frees all the freeCellChangeRecNode
 * starting from change to the end */
void freeCellChangeRecNode(cellChangeRecNode *changeToFree);

/* Allocates memory for cell matrix mat with NxN values */
cell ***allocateCellMatrix(int N);

/* Allocates memory for int matrix
 * with size N*N */
int **allocateIntMatrix(int N);

/* Frees memory allocated for matrix mat of size n*/
void freeIntMatrix(int **mat, int n) ;

#endif //FINAL_MEMORYALLOCATION_H
