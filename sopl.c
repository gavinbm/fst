#include "ibp.h"

/*
110 105 118 97 71 LTR POP LTR POP LTR POP LTR POP LTR POP RTN
Prints my name with a newline at the end
*/

// === Utility Functions ===
char *readfile(char *filename, int *len) {
    FILE *fp = fopen(filename, "r");
    char *buffer = NULL;
    long length;

    if(fp) {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = malloc(length + 1);
        if(buffer) {
            fread(buffer, 1, length, fp);
        }
        fclose(fp);
    }
    *len = length;
    return buffer;
} 
// ============================================
// === Lexer ===

enum {DUP, POP, SWP, OVR, ROT, PLS, MIN, MUL, DIV, MOD, 
      EQL, NOT, GRT, LES, AND, ORR, INV, DGT, LTR, STK, RTN,
      DEF, BYE, STR, NUM};

struct lex {
    int type;
    char *tok;
    char *pos;
} typedef lex;

int iskey(char *src) {
    char keys[BYE][4] = { "dup", "pop", "swp", "ovr", "rot"
                          "+", "-", "*", "/", "%%", "=", "!",
                          ">", "<", "and", "orr", "inv", "dgt",
                          "ltr", "stk", "rtn", "def", "bye"};
    
    for(int i = 0; i < BYE; i++) {
        if(strcmp(keys[i], src) == 0)
            return i;
    }

    return -1;
}

void updatetok(lex **lexer, char *tok, int type, int size) {
    free((*lexer)->tok);
    (*lexer)->tok = malloc(size + 1);
    memcpy((*lexer)->tok, tok, size);
    (*lexer)->tok[size] = '\0';
    (*lexer)->type = type;
}

void next(lex **curr) {

    char *peek = (*curr)->pos, *sub;
    int len = 0, pos, key;

    switch(*peek) {
        case EOF: break;
        case ' ': peek++; break;
        case '+': updatetok(curr, peek, PLS, 1); peek++; break;
        case '-': updatetok(curr, peek, MIN, 1); peek++; break;
        case '*': updatetok(curr, peek, MUL, 1); peek++; break;
        case '/': updatetok(curr, peek, DIV, 1); peek++; break;
        case '%': updatetok(curr, peek, MOD, 1); peek++; break;
        case '=': updatetok(curr, peek, EQL, 1); peek++; break;
        case '!': updatetok(curr, peek, NOT, 1); peek++; break;
        case '>': updatetok(curr, peek, GRT, 1); peek++; break;
        case '<': updatetok(curr, peek, LES, 1); peek++; break;
        case '\"':
            peek++;
            while(peek[len] != '\"') {
                if(peek[len] == '\n' || peek[len] == '\t' || peek[len] == '\r' || peek[len] == '\\' || peek[len] == '%') {
                    printf("illegal char in string...\n");
                    exit(10);
                }
                len++;
            }

            sub = malloc(len + 1);
            memcpy(sub, peek, len);
            sub[len] = '\0';
            updatetok(curr, peek, STR, len);
            free(sub);

            peek = peek + len + 1;
            break;
        default:
            if(*peek >= 'a' && *peek <= 'z') {
                while(peek[len] >= 'a' && peek[len] <= 'z')
                    len++;
                
                sub = malloc(len + 1);
                memcpy(sub, peek, len);
                sub[len] = '\0';
                key = iskey(sub);
                if(key == -1) {
                    printf("invalid token [%s]...\n", sub);
                    exit(2);
                } else {
                    updatetok(curr, peek, key, len);
                }
                free(sub);
            }
            else if(*peek >= '0' && *peek <= '9') {
                while(peek[len] >= '0' && peek[len] <= '9')
                    len++;
                
                if(peek[len] == '.') {
                    puts("No floats..."); exit(4);
                }

                updatetok(curr, peek, NUM, len);
            } else {
                printf("invalid char [%c]\n", *peek);
                exit(1);
            }

            peek = peek + len + 1;
            break;
    }

    (*curr)->pos = peek;
}
// ===================================================
// === repl ===

// execute the current command
void exec(struct Stack *stack, lex *lexer) {

    int tmp1, tmp2, tmp3, tmp4;
    switch(lexer->type) {
        case NUM: // it's a number
            tmp1 = atoi(lexer->tok);
            if(tmp1) {
                push(stack, tmp1);
            } else {
                printf("invalid number...\n");
            }
            break;
        case STR: // it's a string
            printf("%s", lexer->tok);
            break;
        /* --- Stack operations --- */
        case DUP: // the DUP stack operation
            if(stack->head != NULL) {
                push(stack, stack->head->value);
            } else {
                printf("dup: needs at least 1 node on the stack, currently only %d...\n", stack->len);
            }
            break;
        case POP: // the POP stack operation
            pop(stack);
            break;
        case SWP: // the SWP stack operation
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
        case OVR: // the OVR stack operation
            if(stack->len >= 2) {
                tmp1 = stack->head->next->value;
                push(stack, tmp1);
            } else {
                printf("ovr: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
            }
            break;
        case ROT: //the ROT stack operation
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
        case PLS: // +
            if(stack->len >= 2) {
                stack->head->next->value = stack->head->next->value + stack->head->value;
                pop(stack);
            } else {
                printf("+: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
            }
            break;
        case MIN: // -
            if(stack->len >= 2) {
                stack->head->next->value = stack->head->next->value - stack->head->value;
                pop(stack);
            } else {
                printf("-: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
            }
            break;
        case MUL: // *
            if(stack->len >= 2) {
                stack->head->next->value = stack->head->next->value * stack->head->value;
                pop(stack);
            } else {
                printf("*: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
            }
            break;
        case DIV: // / (division)
            if(stack->len >= 2) {
                stack->head->next->value = stack->head->next->value / stack->head->value;
                pop(stack);
            } else {
                printf("/: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
            }
            break;
        case MOD: // % (modulo)
            if(stack->len >= 2) {
                stack->head->next->value = stack->head->next->value % stack->head->value;
                pop(stack);
            } else {
                printf("%%: needs at least 2 nodes on the stack, currently only %d...\n", stack->len);
            }
            break;
        /* --- Boolean operations --- */
        case EQL: // =
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
        case NOT: // !
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
        case GRT: // >
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
        case LES: // <
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
        case AND: // AND
            if(stack->len >= 2) {
                tmp1 = stack->head->value & stack->head->next->value;
                pop(stack);
                push(stack, tmp1);
            } else {
                push(stack, 1);
            }
            break;
        case ORR: // ORR
            if(stack->len >= 2) {
                tmp1 = stack->head->value | stack->head->next->value;
                pop(stack);
                push(stack, tmp1);
            } else {
                push(stack, 1);
            }
            break;
        case INV: // INV
            if(stack->len >= 2) {
                tmp1 = ~stack->head->value;
                pop(stack);
                push(stack, tmp1);
            } else {
                push(stack, 1);
            }
            break;
        /* --- I/O operations --- */
        case DGT: // DGT
            printf("%d", stack->head->value);
            break;
        case LTR: // LTR
            printf("%c", stack->head->value);
            break;
        case STK: // STK
            print_stack(stack);
            break;
        case RTN: // RTN
            printf("\n");
            break;
        /* --- New op creator --- */
        case DEF: // def
            // TODO...
            break;
        /* --- Exit the program --- */
        case 28: // BYE
            printf("Exiting program... Goodbye...\n");
            free_stack(stack);
            exit(0);
            break;
        /* --- dict entry or error --- */
        default:
            printf("wtf is %s?\n", lexer->tok);
            break;
    }
}

int main() {
    char buffer[MAXLEN];

    struct Stack *stack = init_stack(STACKCAP);
    lex *lexer = malloc(sizeof(lex));
    lexer->pos = buffer;
    lexer->tok = NULL;

    while(1) {
        printf(">> "); // print the prompt
        
        // Get a line of input, make sure it isn't null, tokenize and process it
        if(fgets(buffer, MAXLEN, stdin) != NULL) {
            buffer[strlen(buffer) - 1] = '\0';  // set the null terminator
            
            next(lexer);
            exec(stack, lexer);

        } else {
            printf("Error getting input...\n");
            break;
        }
    }

    free_stack(stack);
    return 0;
}
