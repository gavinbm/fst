#include "ibp.h"

/*
110 105 118 97 71 LTR POP LTR POP LTR POP LTR POP LTR POP RTN
Prints my name, Gavin with a newline at the end
*/

int main() {
    char buffer[MAXLEN];

    struct Stack *stack = init_stack(STACKCAP);
    struct Token *tokens = NULL, *curr_tok;
    struct DictEntry *dict = NULL;
    char *substr, *ins_name;
    int tmp1, tmp2, tmp3, tmp4;
    int vars[12] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    while(1) {
        printf(">> "); // print the prompt
        
        // Get a line of input, make sure it isn't null, tokenize and process it
        if(fgets(buffer, MAXLEN, stdin) != NULL) {
            buffer[strlen(buffer) - 1] = '\0';  // set the null terminator
           
            tokens = lex(buffer); // lex the buffer into tokens that we can analyze
            //print_tokens(tokens); // print the tokens
            repl(stack, tokens, dict);

        } else {
            printf("Error getting input...\n");
            break;
        }
    }

    free_stack(stack);
    return 0;
}

void repl(struct Stack *stack, struct Token *tokens, struct DictEntry *dict) {
    struct Token *curr_tok, *tmp_tok = NULL;
    struct DictEntry *entry;
    char *substr, *ins_name;
    int tmp1, tmp2, tmp3, tmp4;

    // iterate over and process our tokens
    for(curr_tok = tokens; curr_tok != NULL; curr_tok = curr_tok->next) {
        // pass our token type into a switch so we can execute the right instructions
        switch(curr_tok->type) {
            case 3: // it's a number
                tmp1 = atoi(curr_tok->src);
                if(tmp1) {
                    push(stack, tmp1);
                } else {
                    printf("invalid number...\n");
                }
                break;
            case 4: // it's the VAR keyword
                break;
            case 5: // it's a string
                printf("%s", curr_tok->src);
                break;
            /* --- Stack operations --- */
            case 6: // the DUP stack operation
                if(stack->head != NULL) {
                    push(stack, stack->head->value);
                } else {
                    printf("dup: needs at least 1 node on the stack, currently only %d...\n", stack->len);
                }
                break;
            case 7: // the POP stack operation
                pop(stack);
                break;
            case 8: // the SWP stack operation
                if(stack->len >= 2) {
                    tmp1 = stack->head->value;
                    pop(stack);
                    tmp2 = stack->head->value;
                    pop(stack);
                    push(stack, tmp1);
                    push(stack, tmp2);
                } else {
                    printf("swp: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
                }
                break;
            case 9: // the OVR stack operation
                if(stack->len >= 2) {
                    tmp1 = stack->head->next->value;
                    push(stack, tmp1);
                } else {
                    printf("ovr: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
                }
                break;
            case 10: //the ROT stack operation
                if(stack->len >= 3) {
                    tmp1 = stack->head->value;
                    tmp2 = stack->head->next->value;
                    tmp3 = stack->head->next->next->value;
                    pop(stack);
                    pop(stack);
                    pop(stack);
                    push(stack, tmp1);
                    push(stack, tmp2);
                    push(stack, tmp3);
                } else {
                    printf("ovr: needs at least 3 nodes on the stack, currently only %d...\n", stack->len);
                }
                break;
            /* --- Math operations --- */
            case 11: // +
                if(stack->len >= 2) {
                    stack->head->next->value = stack->head->next->value + stack->head->value;
                    pop(stack);
                } else {
                    printf("+: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
                }
                break;
            case 12: // -
                if(stack->len >= 2) {
                    stack->head->next->value = stack->head->next->value - stack->head->value;
                    pop(stack);
                } else {
                    printf("-: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
                }
                break;
            case 13: // *
                if(stack->len >= 2) {
                    stack->head->next->value = stack->head->next->value * stack->head->value;
                    pop(stack);
                } else {
                    printf("*: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
                }
                break;
            case 14: // / (division)
                if(stack->len >= 2) {
                    stack->head->next->value = stack->head->next->value / stack->head->value;
                    pop(stack);
                } else {
                    printf("/: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
                }
                break;
            case 15: // % (modulo)
                if(stack->len >= 2) {
                    stack->head->next->value = stack->head->next->value % stack->head->value;
                    pop(stack);
                } else {
                    printf("%%: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
                }
                break;
            /* --- Boolean operations --- */
            case 16: // =
                if(stack->len >= 2) {
                    if(stack->head->value == stack->head->next->value) {
                        pop(stack);
                        pop(stack);
                        push(stack, 1);
                    } else {
                        pop(stack);
                        pop(stack);
                        push(stack, 0);
                    }
                } else {
                    push(stack, 1);
                }
                break;
            case 17: // !
                if(stack->len >= 2) {
                    if(stack->head->value != stack->head->next->value) {
                        pop(stack);
                        pop(stack);
                        push(stack, 1);
                    } else {
                        pop(stack);
                        pop(stack);
                        push(stack, 0);
                    }
                } else {
                    push(stack, 1);
                }
                break;
            case 18: // >
                if(stack->len >= 2) {
                    if(stack->head->value > stack->head->next->value) {
                        pop(stack);
                        pop(stack);
                        push(stack, 1);
                    } else {
                        pop(stack);
                        pop(stack);
                        push(stack, 0);
                    }
                } else {
                    push(stack, 1);
                }
                break;
            case 19: // <
                if(stack->len >= 2) {
                    if(stack->head->value < stack->head->next->value) {
                        pop(stack);
                        pop(stack);
                        push(stack, 1);
                    } else {
                        pop(stack);
                        pop(stack);
                        push(stack, 0);
                    }
                } else {
                    push(stack, 1);
                }
                break;
            /* --- Bitwise operations --- */
            case 20: // AND
                if(stack->len >= 2) {
                    tmp1 = stack->head->value & stack->head->next->value;
                    pop(stack);
                    push(stack, tmp1);
                } else {
                    push(stack, 1);
                }
                break;
            case 21: // ORR
                if(stack->len >= 2) {
                    tmp1 = stack->head->value | stack->head->next->value;
                    pop(stack);
                    push(stack, tmp1);
                } else {
                    push(stack, 1);
                }
                break;
            case 22: // INV
                if(stack->len >= 2) {
                    tmp1 = ~stack->head->value;
                    pop(stack);
                    push(stack, tmp1);
                } else {
                    push(stack, 1);
                }
                break;
            /* --- I/O operations --- */
            case 23: // DGT
                printf("%d", stack->head->value);
                break;
            case 24: // LTR
                printf("%c", stack->head->value);
                break;
            case 25: // STK
                print_stack(stack);
                break;
            case 26: // RTN
                printf("\n");
                break;
            /* --- New op creator --- */
            case 27: // :
                if(curr_tok->next != NULL) {
                    curr_tok = curr_tok->next;

                    ins_name = malloc((strlen(curr_tok->src) + 1) * sizeof(char));
                    strcpy(ins_name, curr_tok->src);

                    if(curr_tok->next != NULL) {
                        curr_tok = curr_tok->next;
                        substr = malloc((strlen(curr_tok->src) + 1) * sizeof(char));
                        strcpy(substr, curr_tok->src);
                        substr = append_line(substr, " ");
                        
                        while(curr_tok->next != NULL) {
                            curr_tok = curr_tok->next;
                            substr = append_line(substr, curr_tok->src);
                            substr = append_line(substr, " ");
                        }
                        create_entry(&dict, ins_name, substr);
                        free(substr);
                    } else {
                        printf("Missing definition of new word...\n");
                    }
                    free(ins_name);
                } else {
                    printf("Must give new word and it's definition...\n");
                }
                break;
            /* --- Exit the program --- */
            case 28: // BYE
                printf("Exiting program... Goodbye...\n");
                free_dict(dict);
                free_tokens(tokens);
                free_stack(stack);
                exit(0);
                break;
            /* --- dict entry or error --- */
            default:
                print_dict(dict);
                printf("%s\n", curr_tok->src);
                if((entry = indict(dict, curr_tok->src))) {
                    tmp_tok = lex(entry->instructions);
                    repl(stack, tmp_tok, dict);
                } else {
                    printf("wtf is %s?\n", curr_tok->src);
                }
                break;
        }
    }

    free_tokens(tokens);  // free the tokens so we can tokenize the next line of input
}