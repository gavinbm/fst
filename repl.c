#include "fst.h"

int main() {
    char buffer[MAXLEN];

    struct Stack *stack = init_stack();
    
    while(1) {
        printf(">> ");
        if(fgets(buffer, MAXLEN, stdin) != NULL) {
        buffer[strlen(buffer) - 1] = '\0';

        
            if(strcmp(buffer, "exit") == 0) {
                printf("exiting input loop...\n");
                break;
            }
            else if(strcmp(buffer, "print") == 0) {
                print_stack(stack);
            }
            else if(strcmp(buffer, "pop") == 0) {
                pop(stack);
            } else {
                int val;
                if((val = atoi(buffer)))
                    push(stack, val);
                else
                    printf("Invalid literal...\n");
            }
        }
    }

    free_stack(stack);
    return 0;
}