//
// Created by eran on 01/09/18.
//

#include "gurobi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "gameUtils.h"



/* Solves sudoku using ILP
 * res will hold the solved board values */
int ILP(int ** board, int ** res, int n ,int m) {

    int N, flag, error, zero, i, j, v, ig, jg, count;
    GRBenv *env = NULL;
    GRBmodel *model = NULL;
    N = n*m;

    //int **board = (int **) malloc(N * N * sizeof(int));
    double *sol = (double *) malloc(N * N * N * sizeof(double));
    int *obj = (int *) malloc(N * N * N * sizeof(int));
    char inputline[100];
    int *ind = (int *) malloc(N * sizeof(int));
    double *val = (double *) malloc(N * sizeof(double));
    double *lb = (double *) malloc(N * N * N * sizeof(double));
    char *vtype = (char *) malloc(N * N * N * sizeof(char));
    char *names = (char *) malloc(N * N * N * sizeof(char));
    char *namestorage = (char *) malloc(10 * N * N * N * sizeof(char));
    char *cursor;
    int optimstatus;
    double objval;
    zero = 0;
    error = 0;
    flag = 0;


    /* Create an empty model */
    cursor = namestorage;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                //TODO : check if v+1 is the correct way
                if (board[i][j] == v + 1)
                    lb[i * N * N + j * N + v] = 1;
                else
                    lb[i * N * N + j * N + v] = 0;
                vtype[i * N * N + j * N + v] = GRB_BINARY;
                names[i * N * N + j * N + v] = cursor[0];
                cursor += strlen(names + (i * N * N + j * N + v)) + 1;
            }
        }
    }

    /* Create environment */

    error = GRBloadenv(&env, "sudoku.log");
    if (error) {
        //TODO : handle error
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        printf("error");
        return 0;
    }

    /* Create new model */

    error = GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, lb, NULL,
                        vtype, &names);
    if (error) {
        //TODO : handle error
        printf("error");
        return 0;
    }

    /* Each cell gets a value */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                ind[v] = i * N * N + j * N + v;
                val[v] = 1.0;
            }
            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                //TODO : handle error
                printf("error");
                return 0;
            }
        }
    }

    /* Each value must appear once in each row */

    for (v = 0; v < N; v++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = i * N * N + j * N + v;
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                //TODO : handle error
                printf("error");
                return 0;
            }
        }
    }

    /* Each value must appear once in each column */

    for (v = 0; v < N; v++) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                ind[j] = i * N * N + j * N + v;
                val[j] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                //TODO : handle error
                printf("error");
                return 0;
            }
        }
    }

    /* Each value must appear once in each subgrid */

    for (v = 0; v < N; v++) {

        // defines what block are we at
        for (ig = 0; ig < n; ig++) {
            // number of blocks vertically (rows)

            for (jg = 0; jg < m; jg++) {
                // number of block horizontally (cols)

                count = 0;

                // iterates over the cells in that block
                for (i = ig * m; i < (ig + 1) * m; i++) {
                    for (j = jg * n; j < (jg + 1) * n; j++) {
                        ind[count] = i * N * N + j * N + v;
                        val[count] = 1.0;
                        count++;
                    }
                }
                error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    //TODO : handle error
                    printf("error");
                    return 0;
                }
            }
        }
    }

    /* Optimize model */

    error = GRBoptimize(model);
    if (error) {
        //TODO : handle error
        printf("error");
        return 0;
    }
    /* Write model to 'sudoku.lp' */

    error = GRBwrite(model, "sudoku.lp");
    if (error) {
        //TODO : handle error
        printf("error");
        return 0;
    }
    /* Capture solution information */

    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        //TODO : handle error
        printf("error");
        return 0;
    }

    /* get the objective -- the optimal result of the function */
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        //TODO : handle error
        printf("error");
        return 0;
    }

    /* get the solution - the assignment to each variable */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, sol);
    if (error) {
        printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
        return -1;
    }


    if (optimstatus == GRB_OPTIMAL) {
        updateSolved(sol, res, N);
    }


    /* Free model */

    GRBfreemodel(model);

    /* Free environment */

    GRBfreeenv(env);

    return 0;
}


void updateSolved(double *sol ,int ** res, int N) {
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                if (sol[i * N * N + j * N + k] == 1) {
                    res[i][j] = k + 1;
                }
            }
        }
    }
}


int **fromCellMatToIntMat(cell ***src, int N) {

    int **dst, i, j;
    dst = allocateIntMatrix(N);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            dst[i][j] = src[i][j]->value;
        }
    }

    return dst;
}


cell ***fromIntMatToCellMat(int **src, int N) {

    int i, j;
    cell ***dst;
    dst = allocateCellMatrix(N);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {

            dst[i][j]->value = src[i][j];
        }
    }

    return dst;
}