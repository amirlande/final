//
// Created by amirlande on 8/2/2018.
//


#ifndef FINAL_SOLVER_H
#define FINAL_SOLVER_H



/* precondition: board has no erroneous values (to be checked before calling this function)
 * the function solves the board using ILP algorithm
 * returns TRUE (1) if solvable or FALSE (0) if unsolvable*/
int solveUsingILP(gameParams *game);


/* precondition: board has no erroneous values (to be checked before calling this function)
 * the function counts the number of solutions for a given board using exhaustive backtracking
 * it is called by num_solutions (in commands.h) */
int count_solutions(gameParams *game);

#endif //FINAL_SOLVER_H