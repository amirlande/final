//
// Created by amirlande on 8/23/2018.
//


#include "commandStruct.h"

/* allocated memory for s commandStruct and returns pointer to it
 * struct fields are initialized to default values */
commandStruct *initializeCommand() {
    commandStruct *command = (commandStruct *) malloc(sizeof(commandStruct));
    if (command == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }
    command->type = INVALID_COMMAND;
    command->isValid = FALSE;
    command->x = 0;
    command->y = 0;
    command->z = 0;
    command->markErrors = 0;
    command->fileName = NULL;
    return command;
}

void freeCommand(commandStruct *command) {
    if (command->fileName != NULL) {
        free(command->fileName);
    }
    free(command);
}

