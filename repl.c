#include "tinyforth.h"

int main() {
    char buffer[MAXLEN];

    struct Stack *stack = init_stack();
    struct Token *tokens = NULL, *curr_tok;

    while(1) {
        printf(">> ");
        if(fgets(buffer, MAXLEN, stdin) != NULL) {
            buffer[strlen(buffer) - 1] = '\0';

            // lex the buffer into tokens that we can analyze
            tokens = lex(buffer);
            print_tokens(tokens);
            for(curr_tok = tokens; curr_tok != NULL; curr_tok = curr_tok->next) {

                switch(curr_tok->type) {
                    case 0: // it's a word
                        break;
                    case 1: // it's a literal number or char
                        break;
                    case 2: // it's a literal string
                        break;
                    default: // it's something else
                        break;
                }
            }

            free_tokens(tokens);
        }
    }

    free_stack(stack);
    return 0;
}