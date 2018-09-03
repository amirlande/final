//
// Created by eran on 01/09/18.
//

#ifndef FINAL_GUROBI_H
#define FINAL_GUROBI_H


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

#endif //FINAL_GUROBI_H
