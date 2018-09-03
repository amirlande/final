//
// Created by amirlande on 9/1/2018.
//

#include "mainAux.h"

/* for testing */
void printCallingFunc(char *functionName) {
    printf("Calling %s\n", functionName);
}

/* reads next command from user using getCommandFromUser and invokes the relevant function from "commands.h"
 * return value: FALSE (0) if user enters EXIT or completes the board, TRUE (1) otherwise (game continues)*/
int playTurn(gameParams *game) {
    commandStruct *commandToPlay = getCommandFromUser(game);
    enum commandType type;
    if (commandToPlay->isValid == TRUE) {
        type = commandToPlay->type;
        switch (type) {
            case SET:
                printCallingFunc("SET");
                /* set(commandToPlay->x, commandToPlay->y, commandToPlay->z, game); */
                break;
            case HINT:
                printCallingFunc("HINT");
                /* hint(commandToPlay->x, commandToPlay->y); */
                break;
            case VALIDATE:
                printCallingFunc("VALIDATE");
                /* validate(game); */
                break;
            case RESET:
                printCallingFunc("RESET");
                /* reset(game); */
                break;
            case EXIT:
                /* exitGame(game); */
                printCallingFunc("EXIT");
                freeCommand(commandToPlay);
                return FALSE;
            case SOLVE:
                printCallingFunc("SOLVE");
                solve(game, commandToPlay->fileName);
                break;
            case EDIT:
                printCallingFunc("EDIT");
                /* edit(commandToPlay->fileName, game); */
                break;
            case MARK_ERRORS:
                printCallingFunc("MARK_ERRORS");
                /* mark_errors(game, commandToPlay->markError); */
                break;
            case PRINT_BOARD:
                printCallingFunc("PRINT_BOARD");
                /* print_board(game); */
                break;
            case GENERATE:
                printCallingFunc("GENERATE");
                /* generate(commandToPlay->x, commandToPlay->y, game); */
                break;
            case UNDO:
                /* undo(game); */
                printCallingFunc("UNDO");
                break;
            case REDO:
                /* redo(game); */
                printCallingFunc("REDO");
                break;
            case SAVE:
                /* save(commandToPlay->fileName, game); */
                printCallingFunc("SAVE");
                break;
            case NUM_SOLS:
                /* num_solutions(game); */
                printCallingFunc("NUM_SOLS");
                break;
            case AUTO_FILL:
                /* autoFill(game); */
                printCallingFunc("AUTO_FILL");
                break;
            default:
                printErrorInCodeFlow("playTurn", "parser.c");
        }
        printBoard(game);
        freeCommand(commandToPlay);
        return TRUE;
    }
}