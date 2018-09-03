//
// Created by eran on 01/09/18.
//

#ifndef FINAL_GUROBI_H
#define FINAL_GUROBI_H

#include "gurobi_c.h"
#include "gameParams.h"

typedef enum ILPCommand {
    VALIDATE = 1,
    GENERATE = 2,
    HINT = 3,


} ILPCommand;


void updateSolved(double *sol, int **res, int N);

int ILP(int **board, int **res, int n, int m, ILPCommand command);

int **fromCellMatToIntMat(cell ***src, int N);

cell ***fromIntMatToCellMat(int **src, int N);

void freeILP(double *sol, int *ind, int *ind2, double *val, double *val2, double *lb, char *vtype, GRBenv *env,
             GRBmodel *model);

#endif //FINAL_GUROBI_H
