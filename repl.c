#include "ibp.h"

int main() {
    char buffer[MAXLEN];

    struct Stack *stack = init_stack();
    struct Token *tokens = NULL, *curr_tok;

    while(1) {
        printf(">> ");
        if(fgets(buffer, MAXLEN, stdin) != NULL) {
            buffer[strlen(buffer) - 1] = '\0';

            tokens = lex(buffer); // lex the buffer into tokens that we can analyze
            print_tokens(tokens); // print the tokens

            free_tokens(tokens); // free the tokens so we can tokenize the next line of input
        } else {
            printf("Error getting input...\n");
            break;
        }
    }

    free_stack(stack);
    return 0;
}