//
// Created by amirlande on 8/2/2018.
//

#include "gameUtils.h" /* gives access to all struct definitions and utility functions
 * used by the solving algorithms */


/* precondition: board has no erroneous values (to be checked before calling this function)
 * the function solves the board using ILP algorithm
 * returns TRUE (1) if solvable or FALSE (0) if unsolvable*/
int solveUsingILP(gameParams *game) {
    /* to be implemented */
}

/* called by count_solutions (which has the precondition of no erroneous cells!)
 * operates as a recursive function, but uses an explicit stack of calls instead of actual recursion
 * when finished - counter holds the number of solutions*/
void count_solutions_rec(cell **board, int *counter) {
    int row, col;
    int val;

    if (find_first_empty_cell(board, &row, &col) == INVALID) {
        /* no empty cell found - board is full with valid cells (assuming precondition of validity of all cells!) */
        *counter = 1; /* only 1 solution */
        return;
    }
    /* getting here means (row, col) hold the indexes of the first empty cell */
    /* continue from here */


}

/* precondition: board has no erroneous values (to be checked before calling this function)
 * the function returns the number of solutions for a given board using exhaustive backtracking
 * it is called by num_solutions (in commands.h) */
int count_solutions(gameParams *game) {
    int num_of_sols;
    cell **temp_board;
    temp_board = copy_board(game->userBoard);

    num_of_sols = 0;
    count_solutions_rec(temp_board, &num_of_sols);
    free_board(temp_board); /* free memory before returning */
    return num_of_sols;
}
