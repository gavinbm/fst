#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLEN 128
#define STACKCAP 64

struct StackNode {
    int value;
    struct StackNode *next;
};

struct Stack {
    int capacity;
    int len;
    struct StackNode *head;
};

struct Dict {
    char *name;
};

struct Token {
    char *src;
    int type;
    struct Token *next;
};

/* ---- repl.c ---- */
/* ---- dict.c ---- */
/* ---- lexer.c ---- */
struct Token *lex(char *input);
void create_token(struct Token **head, char *source, int type);
int iskeyword(char *name);
void print_tokens(struct Token *tokens);
void free_tokens(struct Token *tokens);

/* ---- stack.c ---- */
struct Stack *init_stack();
void push(struct Stack *stack, int new_value);
void pop(struct Stack *stack);
void print_stack(struct Stack *stack);
void free_stack(struct Stack *stack);
