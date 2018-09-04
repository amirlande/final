//
// Created by amirlande on 8/23/2018.
//

#include "errorMessages.h"


/* serves as a "shouldn't get here" print function with function and module name */
void printErrorInCodeFlow(char *functionName, char *moduleName) {
    printf("Error in code flow - shouldn't have got here\n"
           "Location: %s function at %s module\n");
}

void printNotImplementedMessage(char *nameOfFunction) {
    printf("%s function hasn't been implemented yet!\n", nameOfFunction);
}

void printMallocFailed() {
    printf("Error: malloc has failed\n");
}

void printCallocFailed() {
    printf("Error: calloc has failed\n");
}

void printErrorOpeningFile(gameMode mode) {
    if (mode == SOLVE_MODE) {
        printf("Error: File doesn't exist or cannot be opened\n");
    } else if (mode == EDIT_MODE) {
        printf("Error: File cannot be opened\n");
    } else {
        printErrorInCodeFlow("printErrorOpeningFile", "input_output.c");
    }
}