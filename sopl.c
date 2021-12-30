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
int spr = 0;

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
        printf("[%s, %d] ", keys[i], i);
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
        case EOF: break;
        case '\0': peek++; break;
        case '\n': break;
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
void exec(lex *lexer) {

    int tmp1, tmp2, tmp3, tmp4;

    switch(lexer->type) {
        
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
    lexer->pos = buffer;
    lexer->tok = NULL;

    while(1) {
        printf(">> "); // print the prompt
        
        // Get a line of input, make sure it isn't null, tokenize and process it
        if(fgets(buffer, MAXLEN, stdin) != NULL) {
            buffer[strlen(buffer) - 1] = '\0';  // set the null terminator
            
            next(&lexer);
            exec(lexer);
            printf("[%s] -- [%d]\n", lexer->tok, lexer->type);

        } else {
            printf("Error getting input...\n");
            break;
        }
    }

    return 0;
}
