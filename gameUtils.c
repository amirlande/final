//
// Created by eran on 31/07/18.
//

#include "gameUtils.h"


/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int check_err_cells(gameParams *game) {
    /* to be implemented */
}



/* allocates memory for a new board and copies values of
 * board_to_be_copied. returns pointer to the new board struct*/
cell **copy_board(cell **board_to_be_copied) {
    /* to be implemented */
}

/* frees memory of a given board */
void free_board(cell **board_to_be_freed) {
    /* to be implemented */
}