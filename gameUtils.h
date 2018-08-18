/* this header includes all struct definitions used by the program \
 * as well as utility functions */

#ifndef FINAL_GAMEUTILS_H
#define FINAL_GAMEUTILS_H

/* part 1 - game structs and defines: */

#define TRUE 1
#define FALSE 0
#define VALID 1
#define INVALID 0

/* cellChangeRecNode is a node of a single-linked list of all set operations
 * made by the user - in case of a "SET" operation the list includes only one node
 * in case of a "AUTOFILL" operation the list includes a cellChangeRecNode for each
 * cell that has been automatically set, with its previous value and current value*/
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

/* the struct that represents a cell in the Sudoku board
 * each cell contains information regarding its value, whether it is fixed and whether it is valid*/
typedef struct cell {
    int value;
    int isFixed;
    int isValid; /* isValid == 1 means it's not erroneous, isValid == 0 means value is erroneous */
} cell;

enum gameMode {init, solve, edit};
/* the main struct that represents the Sudoku game */
typedef struct gameParams {
    enum gameMode mode;
    int markErrors;
    int n;
    int m;
    cell **userBoard;
    cell **solution;
    int counter;
    listOfMoves movesList;

} gameParams;


/* part 2 - function declarations: */

/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int check_err_cells(gameParams *game);

/* allocates memory for a new board and copies values of
 * board_to_be_copied. returns pointer to the new board struct*/
cell **copy_board(cell **board_to_be_copied);

/* frees memory of a given board */
void free_board(cell **board_to_be_freed);

int find_first_empty_cell(cell **board, int *, int *);



#endif //FINAL_GAMEUTILS_H