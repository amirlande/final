//
// Created by amirlande on 8/23/2018.
//

#ifndef FINAL_COMMANDSTRUCT_H
#define FINAL_COMMANDSTRUCT_H

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

enum commandType {
    INVALID_COMMAND = 0,
    SET = 1,
    HINT = 2,
    VALIDATE = 3,
    RESET = 4,
    EXIT = 5,
    SOLVE = 6,
    EDIT = 7,
    MARK_ERRORS = 8,
    PRINT_BOARD = 9,
    GENERATE = 10,
    UNDO = 11,
    REDO = 12,
    SAVE = 13,
    NUM_SOLS = 14,
    AUTO_FILL = 15,
};

/* definition of a struct used to represent a command
 * it holds type of command, arguments and indication of validity */
typedef struct commandStruct {
    int isValid;
    enum commandType type;
    int x;
    int y;
    int z;
    char *fileName; /* used by solve and edit commands */
    int markError; /* used by mark_errors command */
} commandStruct;

/* allocated memory for s commandStruct and returns pointer to it
 * struct fields are initialized to default values */
commandStruct *initializeCommand();

void freeCommand(commandStruct *command);

#endif //FINAL_COMMANDSTRUCT_H
