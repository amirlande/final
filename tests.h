

#ifndef FINAL_TESTS_H
#define FINAL_TESTS_H


/* userBoard is a 9x9 matrix (sudoku board), row and col are pointers to ints
 * finds first empty cell and assigns its coordinates to row and col
 * */
int findEmptyCell2(int **userBoard, int N, int *row, int *col);


/* Used to deep copy a matrix */

int **copyMatrix(int **oldMat);

/* solves a sudoku board using the deterministic Backtracking algorithm (if solvable)
 * returns "1" if solvable, "0" otherwise
 * */
int solveUsingDetBacktrackingRecursion(int **newSol, int n, int m);


/* finding a new solution using deterministic backtracking */



#endif //FINAL_TESTS_H
