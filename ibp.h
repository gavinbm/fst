#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLEN 128
#define STACKCAP 64

/*
        ------ Token Type Map ------
   Specials      Stack Ops   Math Ops   Bool Ops
  EOF     = 1     DUP = 6     + = 11     =  = 16
  NEWLINE = 2     POP = 7     - = 12     !  = 17
  NUMBER  = 3     SWP = 8     * = 13     >  = 18
  VAR     = 4     OVR = 9     / = 14     <  = 19
  STRING  = 5     ROT = 10    % = 15

  Bitwise Ops     I/O Ops     New Op Creator
   AND = 20       DGT = 23      :   = 27
   ORR = 21       LTR = 24      BYE = 28
   INV = 22       STK = 25
                  RTN = 26
*/

struct StackNode {
    int value;
    struct StackNode *next;
};

struct Stack {
    int capacity;
    int len;
    struct StackNode *head;
};

struct Token {
    char *src;
    int type;
    struct Token *next;
};

struct DictEntry {
    char *word;
    char *instructions;
    struct DictEntry *next;
};

/* ---- repl.c ---- */
void repl(struct Stack *stack, struct Token *tokens, struct DictEntry *dict);

/* ---- dict.c ---- */
void create_entry(struct DictEntry **head, char *name, char *instructions);
void print_dict(struct DictEntry *dict);
void free_dict(struct DictEntry *dict);
struct DictEntry *indict(struct DictEntry *head, char *name);

/* ---- lexer.c ---- */
struct Token *lex(char *input);
void create_token(struct Token **head, char *source, int type);
int iskeyword(char *name);
void makeshorttoken(char letter, int type, struct Token *tokens);
void strupper(char *str);
char *append_line(char *curr_code, char *new_code);
void print_tokens(struct Token *tokens);
void free_tokens(struct Token *tokens);

/* ---- stack.c ---- */
struct Stack *init_stack(int capacity);
void push(struct Stack *stack, int new_value);
void pop(struct Stack *stack);
void print_stack(struct Stack *stack);
void free_stack(struct Stack *stack);
