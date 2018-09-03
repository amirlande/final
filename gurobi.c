//
// Created by eran on 01/09/18.
//

#include "gurobi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gameUtils.h"
#include "memoryAllocation.h"


/* Solves sudoku using ILP
 * res will hold the solved board values */
int ILP(int **board, int **res, int n, int m, ILPCommand command) {

    int N, error, i, j, v, ig, jg, count, *ind, *ind2, optimstatus, result;
    double *sol, *val, *val2, *lb, objval;
    char *vtype;
    GRBenv *env;
    env = NULL;
    GRBmodel *model = NULL;
    N = n * m;
    sol = (double *) malloc(N * N * N * sizeof(double));
    ind = (int *) malloc(N * sizeof(int));
    ind2 = (int *) malloc(N * sizeof(int));
    val = (double *) malloc(N * sizeof(double));
    val2 = (double *) malloc(N * sizeof(double));
    lb = (double *) malloc(N * N * N * sizeof(double));
    vtype = (char *) malloc(N * N * N * sizeof(char));


    result = 0;

    /* Create an empty model */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (v = 0; v < N; v++) {
                if (board[i][j] == v + 1)
                    lb[i * N * N + j * N + v] = 1;
                else
                    lb[i * N * N + j * N + v] = 0;
                vtype[i * N * N + j * N + v] = GRB_BINARY;

            }
        }
    }

    /* Create environment */
    error = GRBloadenv(&env, "sudoku.log");
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* Create new model */
    error = GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, lb, NULL,
                        vtype, NULL);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
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
                printf("ERROR %d GRBaddconstr1(): %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return result;
            }
        }
    }


    /* constrain that if cell had value != 0
     * it will hold the same value */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (board[i][j] != 0) {
                ind2[0] = i * N * N + j * N + (board[i][j] - 1);
                val2[0] = 1;
                error = GRBaddconstr(model, 1, ind2, val2, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    printf("ERROR %d GRBaddconstr2(): %s\n", error, GRBgeterrormsg(env));
                    freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                    return result;
                }
            }
        }
    }


#if OLD
    /* constrain that if cell had value != 0
     * it will hold the same value */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (board[i][j] != 0) {
                for (v = 0; v < N; v++) {
                    if (board[i][j] == v + 1) {
                        ind[v] = i * N * N + j * N + v;
                        val[v] = 1.0;
                    }
                }
                error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
                if (error) {
                    //TODO : add free func
                    printf("ERROR %d value should not change GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
                    return result;
                }
            }
        }
    }

#endif

    /* Each value must appear once in each row */
    for (v = 0; v < N; v++) {
        for (j = 0; j < N; j++) {
            for (i = 0; i < N; i++) {
                ind[i] = i * N * N + j * N + v;
                val[i] = 1.0;
            }

            error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
            if (error) {
                printf("ERROR %d GRBaddconstr3(): %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return result;
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
                printf("ERROR %d GRBaddconstr4(): %s\n", error, GRBgeterrormsg(env));
                freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                return result;
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
                    printf("ERROR %d GRBaddconstr5(): %s\n", error, GRBgeterrormsg(env));
                    freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
                    return result;
                }
            }
        }
    }

    /* Optimize model */
    error = GRBoptimize(model);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* Write model to 'sudoku.lp' */
    error = GRBwrite(model, "sudoku.lp");
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* Capture solution information */
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* get the objective -- the optimal result of the function */
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* get the solution - the assignment to each variable */
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, sol);
    if (error) {
        printf("ERROR: %s\n", GRBgeterrormsg(env));
        freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);
        return result;
    }

    /* board is solved */
    if (optimstatus == GRB_OPTIMAL) {
        if (command != VALIDATE) {
            updateSolved(sol, res, N);
        }
        result = 1;
    }

    freeILP(sol, ind, ind2, val, val2, lb, vtype, env, model);

    return result;
}


void updateSolved(double *sol, int **res, int N) {
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

void freeILP(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
             GRBmodel *model) {

    free(sol);
    free(ind);
    free(ind2);
    free(val);
    free(val2);
    free(lb);
    free(vtype);
    GRBfreemodel(model);
    GRBfreeenv(env);
}