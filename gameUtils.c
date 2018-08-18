//
// Created by eran on 31/07/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <printf.h>
#include "gameUtils.h"
#include "commands.h"


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

/* returns the line separator for print_board
 * consists 4N+m+1 dashes ('-')
 * exits with exit(0) if failed to malloc */
char *getLineSeparator(gameParams *game) {
    char *separator;
    int N, n, m, i;
    n = game->n;
    m = game->m;
    N = m * n;

    separator = malloc(sizeof(char) * (4 * N + m + 1));
    if (separator == NULL) {
        printf("Error: malloc has failed\n");
        exit(0);
    }

    for (i = 0; i < 4 * N + m + 1; i++) {
        separator[i] = '-';
    }

    return separator;
}


/* Allocates memory for cell matrix mat with NxN values */
cell ***allocateCellMatrix(cell ***mat, int N) {

    int i;
    mat = (cell ***) malloc(N * sizeof(cell ***));
    if (mat == NULL) {
        printf("Error: calloc has failed\n");
        exit(0);
    }
    for (i = 0; i < N; i++) {
        mat[i] = (cell **) malloc(N * sizeof(cell **));
        if (mat[i] == NULL) {
            printf("Error: calloc has failed\n");
            exit(0);
        }
    }

    return mat;
}


int find_first_empty_cell(cell **board, int *row, int *col) {
    /* to implement */
    return 1;
}


/* Allocates memory to new nodes
 * sets the curr and prev pointers
 * -- no data is added -- */
void getNewCurrentMove(gameParams *game) {

    userMoveNode *prev = game->movesList->currentMove;
    userMoveNode *curr = (userMoveNode *) malloc(sizeof(userMoveNode *));
    prev->next = curr;
    curr->prev = prev;
    curr->change = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode *));
    game->movesList->currentMove = curr;
    game->movesList->size++;
}


/* Checks if Z is a valid value for non-fixed cell <X,Y> */
int checkIfValid(int x, int y, int z, gameParams *game) {

    if (z == 0) return 1; /* always legal to set a non-fixed cell to 0 */

    if (!(checkIfSquareValid(x, y, z, userBoard))) {
        return 0;
    }

    if (!(checkIfRowValid(x, y, z, userBoard))) {
        return 0;
    }

    if (!(checkIfColumnValid(x, y, z, userBoard))) {
        return 0;
    }

    return 1;

}

/* Called by undo
 * implemented recursively for printing in thr right order:
 * make changes -> print board -> print changes
 * prints at the opposite order, MIGHT NOT BE USED!!
 * */
int makeRecChanges(gameParams *game, cellChangeRecNode *moveToUndo) {

    if (moveToUndo == NULL) {
        printBoard(game);
        return 1;
    }

    game->userBoard[moveToUndo->x - 1][moveToUndo->y - 1] = moveToUndo->prevVal;
    makeRecChanges(game, moveToUndo->next);
    printf("Undo %d,%d: from %d to %d\n", moveToUndo->x, moveToUndo->y, moveToUndo->currVal->value, moveToUndo->prevVal->value);

    return 1;
}


#if 0


typedef struct cellChangeRecNode {
    int x; /*x coordinate of cell*/
    int y; /*y coordinate of cell*/
    int prevVal;
    int currVal;
    struct cellChangeRecNode *next; /*pointer to next node*/
} cellChangeRecNode;


/* a node of the doubly linked list listOfMoves
 * each node's "data" is a pointer to a cellChangeRecNode
 * (which is the first node of a linked list of cellChangeRecNode) */
typedef struct userMoveNode {
    cellChangeRecNode *change;
    struct userMoveNode *next;
    struct userMoveNode *prev;
} userMoveNode;


/* a doubly linked list of nodes of type userMoveNode
 * head is pointer to the head of the list
 * currentMove is a pointer to the last move made by the user*/
typedef struct listOfMoves {
    userMoveNode *head;
    userMoveNode *currentMove;
    int size; /* maybe unnecessary - to be decided later */
} listOfMoves;
#endif
