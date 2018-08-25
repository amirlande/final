#include <stdio.h>
#include "parser.h" /* gives access to the parser functions */
#include "gameUtils.h" /* gives access to functions required to initialize game
 * (initGame, createNewBoard) */


#include "parser.c"
int main() {

    printf("%d", checkIfNumericString("345"));
    printf("%d", checkIfNumericString("345.5"));
    printf("%d", checkIfNumericString("4555a5"));
    printf("%d", checkIfNumericString(" "));

}