
#include <printf.h>
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>

#include "gameParams.h"

/* userBoard is a 9x9 matrix (sudoku board), row and col are pointers to ints
 * finds first empty cell and assigns its coordinates to row and col
 * */
int findEmptyCell2(int **userBoard, int N, int *row, int *col) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (userBoard[i][j] == 0) {
                *row = i;
                *col = j;
                return 1;
            }
        }
    }
    return 0;
}


/* Used to deep copy a matrix */

int **copyMatrix(int **oldMat) {

    int i, j;
    int **newMat;

    newMat = (int **) calloc(9, sizeof(int *));
    if (newMat == NULL) {
        printf("Error: calloc has failed\n");
        return 0;
    }
    for (i = 0; i < 9; i++) {
        newMat[i] = (int *) calloc(9, sizeof(int));
        if (newMat[i] == NULL) {
            printf("Error: calloc has failed\n");
            return 0;
        }
    }

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            newMat[i][j] = oldMat[i][j];
        }
    }


    return newMat;

}


/* Checks if value z does not appear his 3x3 square in the matrix */
int checkIfSquareValid2(int x, int y, int z, int n, int m, int **userBoard) {

    int i;
    int j;

    for (i = x - x % m; i < x - x % m + m; i++) {
        for (j = y - y % n; j < y - y % n + n; j++) {

            if (userBoard[i][j] == z) {
                if (!((i == x) && (j == y))) { /* exclude cell (x,y) from the square check */
                    return 0;
                }
            }
        }
    }
    return 1;
}

/* Checks if value z does not appear in row x */
int checkIfRowValid2(int x, int y, int z, int N, int **userBoard) {

    int j;

    for (j = 0; j < N; j++) {
        if (j != y) { /* exclude cell (x,y) from the square check */
            if (userBoard[x][j] == z) {
                return 0;
            }
        }
    }
    return 1;
}

/* Checks if value z does not appear in column y */
int checkIfColumnValid2(int x, int y, int z, int N, int **userBoard) {

    int i;

    for (i = 0; i < N; i++) {
        if (i != x) { /* exclude cell (x,y) from the square check */
            if (userBoard[i][y] == z) {
                return 0;
            }
        }
    }
    return 1;
}

/* Checks if Z is a valid value for non-fixed cell <X,Y> */
int checkIfValid2(int x, int y, int z, int n, int m, int **userBoard) {

    if (z == 0) return 1; /* always legal to set a non-fixed cell to 0 */

    if (!(checkIfSquareValid2(x, y, z, n, m, userBoard))) {
        return 0;
    }

    if (!(checkIfRowValid2(x, y, z, n * m, userBoard))) {
        return 0;
    }

    if (!(checkIfColumnValid2(x, y, z, n * m, userBoard))) {
        return 0;
    }

    return 1;

}

/* solves a sudoku board using the deterministic Backtracking algorithm (if solvable)
 * returns "1" if solvable, "0" otherwise
 * */
int solveUsingDetBacktrackingRecursion(int **newSol, int n, int m) {
    int row, col, N, val;
    N = n * m;

    /* if no empty cells exist then board is legally full - return 1 (true)
     * otherwise - (row, col) holds the first unassigned cell from (left to right and top to bottom) */
    if (findEmptyCell2(newSol, N, &row, &col) == 0) {
        return 1;
    }

    /* check 1-N values for (row,col) */
    for (val = 1; val <= N; val++) {
        /* use checkIfValid function to check validity of assignment */
        if (checkIfValid2(row, col, val, n, m, newSol)) {
            newSol[row][col] = val;
            /* if assigning (row, col) = val resulted in success return 1 (true), otherwise - remove val */
            if (solveUsingDetBacktrackingRecursion(newSol, n, m)) {
                return 1;
            } else {
                newSol[row][col] = 0;
            }

        }
    }
    /* return 0 (false) if 1-9 assignments to (row,col) returned false (an unsolvable board)
     * and backtrack (this return serves as one piece of the if (solveUsingDetBacktrackting(userBoard)) recursion chain */
    return 0;
}
