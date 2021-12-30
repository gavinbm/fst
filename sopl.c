#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 128
#define STACKCAP 64

/*
=== instruction set ===
 - Arithmetic operators for integers  (DONE)
    - +     ( n m -- (n + m) )
    - -     ( n m -- (n - m) )
    - *     ( n m -- (n * m) )
    - /     ( n m -- (n / m) )
    - %     ( n m -- (n % m) )
 - Boolean operators for comparisons   (DONE)
    - =     checks equality of top two items on the stack
    - !     checks for inequality
    - >     checks for greater than
    - <     checks for less than
    - and   bitwise AND
    - orr   bitwise OR
    - inv   bitwise NOT
 - Stack manipulation instructions     (DONE)
    - dup   ( n -- n n )
    - pop   ( n -- )
    - swp   ( n m -- m n ) 
    - ovr   ( n m -- n m n )
    - rot   ( n m o -- o m n )
 - Simple I/O instructions             (DONE)
    - dgt   ( n -- )
    - ltr   ( c -- )
    - stk   ( -- )
    - rtn   ( -- )
 - New instruction creation             (WiP)
    - def   creates a new instruction using the ones you have

110 105 118 97 71 ltr pop ltr pop ltr pop ltr pop ltr pop rtn
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
// === the stack ===
int stack[STACKCAP];
int spr = -1;

void push(int value) {
    if(spr < STACKCAP) {
        spr++;
        stack[spr] = value;
    } else
        puts("Stack overflow...");
}

void pop() {
    if(spr > 0)
        spr--;
    else
        puts("Stack underflow...");
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
    char keys[STR][4] = { "dup", "pop", "swp", "ovr", "rot",
                          "+", "-", "*", "/", "%%", "=", "!",
                          ">", "<", "and", "orr", "inv", "dgt",
                          "ltr", "stk", "rtn", "def", "bye"};
    
    for(int i = 0; i < STR; i++) {
        if(strcmp(keys[i], src) == 0)
            return i;
    }

    return -1;
}

void updatetok(lex **lexer, char *tok, int type, int size) {
    if((*lexer)->tok)
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
        case EOF: (*curr)->type = -1; break;
        case '\n': peek++; break;
        case ' ': peek++; break;
        case '\0': (*curr)->type = -2; break;
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
void exec(lex *lexer) {

    int tmp1, tmp2, tmp3, tmp4;

    switch(lexer->type) {
        // === stack ops ===
        case DUP:
            push(stack[spr]);
            break;
        case POP:
            pop();
            break;
        case SWP:
            tmp1 = stack[spr];
            pop();
            tmp2 = stack[spr];
            pop();
            push(tmp1);
            push(tmp2);
            break;
        case OVR:
            tmp1 = stack[spr - 1];
            push(tmp1);
            break;
        case ROT:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            tmp3 = stack[spr - 2];
            pop(); pop(); pop();
            push(tmp1); push(tmp2); push(tmp3);
            break;
        // === arithmetic ops ===
        case PLS:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            push(tmp1 + tmp2);
            break;
        case MIN:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            push(tmp1 - tmp2);
            break;
        case MUL:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            push(tmp1 * tmp2);
            break;
        case DIV:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            push(tmp1 / tmp2);
            break;
        case MOD:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            push(tmp1 % tmp2);
            break;
        // === boolean ops ===
        case EQL:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            if(tmp1 == tmp2)
                push(1);
            else
                push(0);
            break;
        case NOT:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            if(tmp1 != tmp2)
                push(1);
            else
                push(0);
            break;
        case GRT:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            if(tmp1 > tmp2)
                push(1);
            else
                push(0);
            break;
        case LES:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            if(tmp1 < tmp2)
                push(1);
            else
                push(0);
            break;
        // === bitwise ops ===
        case AND:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            push(tmp1 & tmp2);
            break;
        case ORR:
            tmp1 = stack[spr];
            tmp2 = stack[spr - 1];
            pop(); pop();
            push(tmp1 | tmp2);
            break;
        case INV:
            tmp1 = stack[spr];
            pop();
            push(~tmp1);
            break;
        // === i/o ops ===
        case DGT:
            printf("%d", stack[spr]);
            break;
        case LTR:
            printf("%c", stack[spr]);
            break;
        case STK:
            for(int i = 0; i < spr; i++)
                printf("%d ", stack[i]);
            break;
        case RTN:
            printf("\n");
            break;
        // === make new op ===
        case DEF:
            break;
        // === exit op ===
        case BYE:
            free(lexer->tok);
            free(lexer);
            exit(0);
            break;
        // === literals ===
        case STR:
            printf("%s", lexer->tok);
            break;
        case NUM:
            push(atoi(lexer->tok));
            break;
        default:
            printf("wtf is %s?\n", lexer->tok);
            break;
    }
}

// ===================================================
// === driver code ===
int main() {
    char buffer[MAXLEN];

    lex *lexer = malloc(sizeof(lex));
    lexer->tok = NULL;

    while(fgets(buffer, MAXLEN, stdin) != NULL) {
        buffer[strlen(buffer) - 1] = '\0';
        lexer->pos = buffer;
        next(&lexer);
        while(lexer->type != -2) {
            exec(lexer);
            next(&lexer);
        }
    }

    return 0;
}
