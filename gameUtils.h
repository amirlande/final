/* this header includes all struct definitions used by the program \
 * as well as utility functions */

#ifndef FINAL_GAMEUTILS_H
#define FINAL_GAMEUTILS_H

/* part 1 - game structs and defines: */

#define TRUE 1
#define FALSE 0
#define VALID 1
#define INVALID 0
#define EMPTY 0
#define BOARD cell ***


/* the struct that represents a cell in the Sudoku board
 * each cell contains information regarding its value, whether it is fixed and whether it is valid*/
typedef struct cell {
    int value;
    int isFixed;
    int isValid; /* isValid == 1 means it's not erroneous, isValid == 0 means value is erroneous */
} cell;


/* cellChangeRecNode is a node of a single-linked list of all set operations
 * made by the user - in case of a "SET" operation the list includes only one node
 * in case of a "AUTOFILL" operation the list includes a cellChangeRecNode for each
 * cell that has been automatically set, with its previous value and current value*/
typedef struct cellChangeRecNode {
    int x; /*x coordinate of cell*/
    int y; /*y coordinate of cell*/
    cell *prevVal;
    cell *currVal;
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
 * head of list is an empty node which is not counter at size
 * currentMove is a pointer to the last move made by the user*/
typedef struct listOfMoves {
    userMoveNode *head;
    userMoveNode *currentMove;
    int size; /* TODO maybe unnecessary - to be decided later */
} listOfMoves;


enum gameMode {
    INIT_MODE,
    SOLVE_MODE,
    EDIT_MODE
};

/* the main struct that represents the Sudoku game */
typedef struct gameParams {
    enum gameMode mode;
    int markErrors;
    int n;
    int m;
    int N;
    cell ***userBoard;
    cell ***solution;
    int counter;
    listOfMoves *movesList;
} gameParams;


/* part 2 - function declarations: */

/* preconditions:
 * checks whether board has any erroneous cells
 * returns TRUE (1) if an erroneous cell was found, and FALSE (0) otherwise
 * (used by validate command) */
int checkErrCells(gameParams *game);

/* Allocates memory for a new board and copies values of
 * board_to_be_copied.
 * Returns pointer to the new board struct (Notice - it is a cell ****) */
BOARD*copyBoard(cell ***board_to_be_copied, int N);

/* returns the line separator for print_board
 * consists 4N+m+1 dashes ('-')
 * exits with exit(0) if failed to malloc */
char *getLineSeparator(gameParams *game);

/* Allocates memory to new nodes
 * sets the curr and prev pointers
 * -- no data is added -- */
void getNewCurrentMove(gameParams *game);

int checkIfValid(int x, int y, int z, gameParams *game);

/* prints the changes after undo/redo */
int printChanges(gameParams *game, cellChangeRecNode *moveToPrint, int isRedo);


/* Called by undo
 * implemented recursively for printing in thr right order:
 * make changes -> print board -> print changes
 * prints at the opposite order, MIGHT NOT BE USED!!
 * */
int makeRecChanges(gameParams *game, cellChangeRecNode *moveToUndo);

/* Checks if value z does not appear his 3x3 square in the matrix */
int checkIfSquareValid(int x, int y, int z, gameParams *game);

/* Checks if value z does not appear in row x */
int checkIfRowValid(int x, int y, int z, gameParams *game);

/* Checks if value z does not appear in column y */
int checkIfColumnValid(int x, int y, int z, gameParams *game);

/* Returns the only legal value
 * for the empty Cell [x][y]
 * returns FALSE - iff has 0, or more than 1 values */
int doesCellHasASingleLegalValue(gameParams *game, int x, int y);


/* sets a new value z to cell [x][y] */
void setValue(gameParams *game, int x, int y, int z);

/* Called by autoFill
 * returns the list of changes */
cellChangeRecNode *getAutoFillChangeList(gameParams *game, int *numOfChanges);

/* Called by autoFill */
void setNewChangeListToGame(gameParams *game, cellChangeRecNode *changeListHead);


/* gets a gameParams instance after one malloc */
int createNewGame(gameParams *game, int n, int m);

/* "Constructor" - creates a cell with the passed value. By default new cells are valid and no fixed TODO */
cell *createCell(int value);

#endif //FINAL_GAMEUTILS_H
