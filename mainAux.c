


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
                set(commandToPlay->x, commandToPlay->y, commandToPlay->z, game);
                break;
            case HINT:
                hint(commandToPlay->x, commandToPlay->y, game);
                break;
            case VALIDATE:
                validate(game);
                break;
            case RESET:
                reset(game);
                break;
            case EXIT:
                freeCommand(commandToPlay);
                exitGame(game);
                return FALSE;
            case SOLVE:
                solve(game, commandToPlay->fileName);
                break;
            case EDIT:
                edit(game, commandToPlay->fileName);
                break;
            case MARK_ERRORS:
                mark_errors(game, commandToPlay->markErrors);
                break;
            case PRINT_BOARD:
                printBoard(game);
                break;
            case GENERATE:
                generate(game, commandToPlay->x, commandToPlay->y);
                break;
            case UNDO:
                undo(game);
                break;
            case REDO:
                redo(game);
                break;
            case SAVE:
                save(game, commandToPlay->fileName);
                break;
            case NUM_SOLS:
                numSolutions(game);
                break;
            case AUTO_FILL:
                autoFill(game);
                break;
            default:
                printErrorInCodeFlow("playTurn", "parser.c");
        }
    }

    if (commandToPlay->type != (UNDO) && commandToPlay->type != AUTO_FILL) {
        printBoard(game);
    }
    freeCommand(commandToPlay);
    return TRUE;
}