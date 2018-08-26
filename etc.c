//
// Created by eran on 18/08/18.
//

#define NULL 0
#include "etc.h"


/* get an instance of a game if needed for tests */
gameParams *getExampleGame(int n, int m) {

    userMoveNode *head, *first, *second;
    gameParams *game2 = NULL;
    int i, j;
    game2 = (gameParams *) malloc(sizeof(gameParams));
    if (game2 == NULL) {
        printf("Error: malloc has failed\n");
        free(game2);
        exit(0);
    }
    game2->n = n;
    game2->m = m;
    game2->N = m*n;
    game2->mode = INIT_MODE;
    game2->markErrors = 1;
    game2->counter = 0;
    game2->userBoard = allocateCellMatrix(game2->userBoard, game2->n * game2->m);

    for (i = 0; i < m * n; i++) {
        for (j = 0; j < m * n; j++) {
            game2->userBoard[i][j] = (cell *) malloc(sizeof(cell *));
            game2->userBoard[i][j]->value = i + j;
            game2->userBoard[i][j]->isValid = 1;

            if (i % 3 == 0) {
                game2->userBoard[i][j]->isFixed = 1;
            }
            if (j % 3 == 0) {
                game2->userBoard[i][j]->isValid = 0;
            }
        }
    }


    game2->solution = allocateCellMatrix(game2->solution, m * n);
    game2->counter = 6;
    game2->movesList = (listOfMoves *)malloc(sizeof(listOfMoves *));
    game2->movesList->size = 1;
    game2->movesList->head = (userMoveNode *) malloc(sizeof(userMoveNode*));
    head = game2->movesList->head;
    head->change = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode *));


    head->prev = NULL;
    head->next = (userMoveNode *) malloc(sizeof(userMoveNode*));

    first = head -> next;
    first->next = NULL;
    first->prev = head;
    first->change = (cellChangeRecNode *) malloc(sizeof(cellChangeRecNode *));
    first->change->currVal = (cell*)malloc(sizeof(cell*));
    first->change->prevVal = (cell*)malloc(sizeof(cell*));
    first->change->next = NULL;
    first->change->x = 1;
    first->change->y = 1;

    first->change->currVal = game2->userBoard[3][3];
    first->change->prevVal = game2->userBoard[0][0];

    game2->movesList->currentMove = first;



    return game2;

}

#if 0

typedef struct cellChangeRecNode {
    int x; /*x coordinate of cell*/
    int y; /*y coordinate of cell*/
    cell *prevVal;
    cell *currVal;
    struct cellChangeRecNode *next; /*pointer to next node*/
} cellChangeRecNode;

#endif