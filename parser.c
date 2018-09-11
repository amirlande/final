
#include "parser.h"

#define COMMAND_LEN 257


void printInvalidCommand() {
    printf("ERROR: invalid command\n");
}

/* @param: low and high - inclusive */
void printErrorInRange(int low, int high) {
    printf("Error: value not in range %d-%d\n", low, high);
}

void printNotZeroOrOne() {
    printf("Error: the value should be 0 or 1\n");
}

/* reads input's chars one by one and checks that all are valid digits
 * uses "ctype.h" library function isdigit
 * returns TRUE (1) or FALSE (0) */
int checkIfNumericString(char *string) {
    int i = 0, isNumeric = TRUE;
    if (string == NULL) return FALSE;

    while (string[i] != '\0') {
        if (isdigit(string[i]) == FALSE) { /* isdigit returns 0 = FALSE if the passed char isn't an int */
            isNumeric = FALSE;
            break;
        }
        i++;
    }
    return isNumeric;
}

/* checks if string represents a non-negative integer in the range of from-to (inclusive)
 * return value: if string represents a valid int - its int value, otherwise -1 (error indicator) */
int checkIfInRange(char *string, int low, int high) {
    int number, isInRange;
    if (checkIfNumericString(string)) {
        number = atoi(string); /* string is a numeric string - safe to use atoi for conversion */
        isInRange = (number >= low && number <= high) ? (number) : (-1);
        return isInRange;
    }
    return -1;
}

int checkIfZeroOrOne(char *string) {
    int number, isZeroOrOne;
    if (checkIfNumericString(string)) {
        number = atoi(string);
        isZeroOrOne = (number == 0 || number == 1);
        return (isZeroOrOne == TRUE) ? (number) : (-1);
    }
    return (-1);
}

int commandAvailable(commandType type, enum gameMode mode) {
    switch (type) {
        case SET:
            return ((mode == SOLVE_MODE) || (mode == EDIT_MODE));
        case HINT:
            return (mode == SOLVE_MODE);
        case VALIDATE:
            return ((mode == SOLVE_MODE) || (mode == EDIT_MODE));
        case RESET:
            return ((mode == SOLVE_MODE) || (mode == EDIT_MODE));
        case EXIT:
        case SOLVE:
        case EDIT:
            return TRUE;
        case MARK_ERRORS:
            return (mode == SOLVE_MODE);
        case PRINT_BOARD:
            return ((mode == SOLVE_MODE) || (mode == EDIT_MODE));
        case GENERATE:
            return (mode == EDIT_MODE);
        case UNDO:
            return ((mode == SOLVE_MODE) || (mode == EDIT_MODE));
        case REDO:
            return ((mode == SOLVE_MODE) || (mode == EDIT_MODE));
        case SAVE:
            return ((mode == SOLVE_MODE) || (mode == EDIT_MODE));
        case NUM_SOLS:
            return ((mode == SOLVE_MODE) || (mode == EDIT_MODE));
        case AUTO_FILL:
            return (mode == SOLVE_MODE);
        default:
            printErrorInCodeFlow("commandAvailable", "parser.c");
            return TRUE;
    }
}

/* @params: command - pointer to the command to which this function gets parameters
 * typeOfCommand - enum representing the command type
 * copyOfInput - a copy of the original string that the user entered as a command (including the first token, such as "set")
 * N - the N parameter of the sudoku board (sudoku has 1-N digits) */
void getParams(commandStruct *command, commandType typeOfCommand, char *copyOfInput, int N) {
    int number;
    char *token;

    token = strtok(copyOfInput, " \t\r\n"); /* gets rid of first word (already read before, in getCommandFromUser) */

    switch (typeOfCommand) {
        case SET: /* read 3 space-separated integers */
            /* first argument */
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* If token is NULL the command is invalid */
                printInvalidCommand();
                return;
            }
            number = checkIfInRange(token, 1, N);
            if (number != -1) {
                command->x = number;
            } else {
                printErrorInRange(0, N);
                command->isValid = FALSE;
                return;
            }
            /* second argument */
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* If token is NULL the command is invalid */
                printInvalidCommand();
                return;
            }
            number = checkIfInRange(token, 1, N);
            if (number != -1) {
                command->y = number;
            } else {
                printErrorInRange(0, N);
                command->isValid = FALSE;
                return;
            }
            /* third argument */
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* If token is NULL the command is invalid */
                printInvalidCommand();
                return;
            }
            number = checkIfInRange(token, 0, N);
            if (number != -1) {
                command->z = number;
            } else {
                printErrorInRange(0, N);
                command->isValid = FALSE;
                return;
            }
            command->isValid = TRUE; /* successfully read 3 valid integers from user - SET command is valid */
            break;

        case HINT:
            /* first argument */
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* If token is NULL the command is invalid */
                printInvalidCommand();
                return;
            }
            number = checkIfInRange(token, 1, N);
            if (number != -1) {
                command->x = number;
            } else {
                printErrorInRange(1, N);
                command->isValid = FALSE;
                return;
            }
            /* second argument */
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* If token is NULL the command is invalid */
                printInvalidCommand();
                return;
            }
            number = checkIfInRange(token, 1, N);
            if (number != -1) {
                command->y = number;
            } else {
                printErrorInRange(1, N);
                command->isValid = FALSE;
                return;
            }
            command->isValid = TRUE; /* successfully read 2 valid integers from user - HINT command is valid */
            break;

        case GENERATE:
            /* first argument - must be between 0 to N*N */
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* If token is NULL the command is invalid */
                printInvalidCommand();
                return;
            }
            number = checkIfInRange(token, 0, N * N); /* TODO check if the upper bound is N*N */
            if (number != -1) {
                command->x = number;
            } else {
                printErrorInRange(0, N * N);
                command->isValid = FALSE;
                return;
            }
            /* second argument - must be between 0 to N*N */
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* If token is NULL the command is invalid */
                printInvalidCommand();
                return;
            }
            number = checkIfInRange(token, 0, N * N); /* TODO check if the upper bound is N*N */
            if (number != -1) {
                command->y = number;
            } else {
                printErrorInRange(0, N * N);
                command->isValid = FALSE;
                return;
            }
            command->isValid = TRUE; /* successfully read 2 valid integers from user - GENERATE command is valid */
            break;

        /* deal with SOLVE and SAVE identically - read file path */
        case SOLVE:
        case SAVE:
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* Not enough arguments (file path wasn't provided) invalid command */
                printInvalidCommand();
                return;
            }
            command->fileName = (char *) malloc(strlen(token) + 1); /* allocate memory for fileName TODO free this memory */
            if (command->fileName == NULL) {
                printMallocFailed();
                exit(EXIT_FAILURE);
            }
            strcpy(command->fileName, token);
            command->isValid = TRUE;
            break;

        case EDIT: /* The user may enter "edit" without a file path */
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* If no file path provided temp will point to NULL */
                command->fileName = NULL;
            }
            else { /* Path was provided by the user */
                command->fileName = (char *) malloc(strlen(token) + 1);
                if (command->fileName == NULL) {
                    printMallocFailed();
                    exit(EXIT_FAILURE);
                }
                strcpy(command->fileName, token);
            }
            command->isValid = TRUE;
            break;

        case MARK_ERRORS:
            token = strtok(NULL, " \t\r\n");
            if (token == NULL) { /* Not enough arguments - invalid command */
                printInvalidCommand();
                return;
            }
            number = checkIfZeroOrOne(token);
            if (number != -1) {
                command->markErrors = number;
                command->isValid = TRUE;
            } else {
                printNotZeroOrOne();
                command->isValid = FALSE;
                return;
            }
            break;
        default:
            printErrorInCodeFlow("getParams", "parser.c");
    }
}

/* parses command from user console input into a userCommand struct and then invokes relevant function from "commands.h"
 * returns a commandStruct with a "isValid" indication. Calling function must check the "isValid" field of the returned value
 * important: this function allocates memory for the commandStruct it returns
 * therefore after resolving the command (i.e. calling SET function with the right args)
 * the resolveCommand function should free this memory*/

commandStruct *getCommandFromUser(gameParams *game) {
    commandStruct *command;
    char input[COMMAND_LEN];
    char copyOfInput[COMMAND_LEN]; /* holds a copy of input string, to be passed to getParams function (since strtok alters input) */
    char *token;
    int digit;

    command = initializeCommand(); /* memory allocation - Freed in the end of playSudoku() */
    token = (char *) malloc(COMMAND_LEN * sizeof(char) + 1); /* allocate memory using the COMMAND_LEN upper bound (memory freed in the end of the function)*/
    if (token == NULL) {
        printMallocFailed();
        exit(EXIT_FAILURE);
    }

    do {
        /* read command from console (at most 256 chars) */
        printf("Enter your command:\n");
        if (fgets(input, COMMAND_LEN, stdin) == NULL) {
            /* exitCleanly(); */
            freeCommand(command);
            free(token);
            freeSudokuGame(game);
            exit(EXIT_FAILURE);
        } /* TODO reached EOF - exit cleanly - to be implemented */
        strcpy(copyOfInput, input);
        strcpy(token, strtok(input, " \t\r\n")); /* Read first token and copy it to the allocated memory */
    } while (token == NULL); /* while input == empty line (ignoring empty lines) */

    /* classify command according to first token */
    if (strcmp(token, "set") == 0) {
        if (commandAvailable(SET, game->mode)) {
            command->type = SET;
            getParams(command, SET, copyOfInput,
                      game->N); /* getParams parses x,y and z and sets isValid tu TRUE if all are valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "hint") == 0) { /* hint command */
        if (commandAvailable(HINT, game->mode)) {
            command->type = HINT;
            getParams(command, HINT, copyOfInput,
                      game->N); /* getParams parses x,y and sets isValid tu TRUE if all are valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "validate") == 0) { /* validate command */
        if (commandAvailable(VALIDATE, game->mode)) {
            command->type = VALIDATE;
            command->isValid = TRUE; /* no need for further parameters, therefore the command is valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "reset") == 0) { /* restart command */
        if (commandAvailable(RESET, game->mode)) {
            command->type = RESET;
            command->isValid = TRUE; /* no need for further parameters, therefore the command is valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "exit") == 0) { /* exit command */
        if (commandAvailable(EXIT, game->mode)) {
            command->type = EXIT;
            command->isValid = TRUE; /* no need for further parameters, therefore the command is valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "solve") == 0) { /* available modes : ALL */
        if (commandAvailable(SOLVE, game->mode)) {
            command->type = SOLVE;
            getParams(command, SOLVE, copyOfInput, game->N);
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "edit") == 0) { /* available modes : ALL */
        if (commandAvailable(EDIT, game->mode)) {
            command->type = EDIT;
            getParams(command, EDIT, copyOfInput, game->N);
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "mark_errors") == 0) { /* available modes: SOLVE */
        if (commandAvailable(MARK_ERRORS, game->mode)) {
            command->type = MARK_ERRORS;
            getParams(command, MARK_ERRORS, copyOfInput, game->N);
            command->isValid = TRUE; /* no need for further parameters, therefore the command is valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "print_board") == 0) {
        if (commandAvailable(PRINT_BOARD, game->mode)) {
            command->type = PRINT_BOARD;
            command->isValid = TRUE; /* no need for further parameters, therefore the command is valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "generate") == 0) {
        if (commandAvailable(GENERATE, game->mode)) {
            command->type = GENERATE;
            getParams(command, GENERATE, copyOfInput, game->N);
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "undo") == 0) {
        if (commandAvailable(UNDO, game->mode)) {
            command->type = UNDO;
            command->isValid = TRUE; /* no need for further parameters, therefore the command is valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "redo") == 0) {
        if (commandAvailable(REDO, game->mode)) {
            command->type = REDO;
            command->isValid = TRUE; /* no need for further parameters, therefore the command is valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "save") == 0) {
        if (commandAvailable(SAVE, game->mode)) {
            command->type = SAVE;
            getParams(command, SAVE, copyOfInput, game->N);
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "num_solutions") == 0) {
        if (commandAvailable(NUM_SOLS, game->mode)) {
            command->type = NUM_SOLS;
            command->isValid = TRUE; /* no need for further parameters, therefore the command is valid */
        }
        else {
            printInvalidCommand();
        }
    } else if (strcmp(token, "autofill") == 0) {
        if (commandAvailable(AUTO_FILL, game->mode)) {
            command->type = AUTO_FILL;
            command->isValid = TRUE;
        }
        else {
            printInvalidCommand();
        }
    }
        /* anything else is an invalid command: */
    else {
        command->isValid = FALSE;
        printInvalidCommand();
    }
    free(token);
    return command;
}