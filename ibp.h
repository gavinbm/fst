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
void strupper(char *str);
char *append_line(char *curr_code, char *new_code);

/* ---- stack.c ---- */
struct Stack *init_stack(int capacity);
void push(struct Stack *stack, int new_value);
void pop(struct Stack *stack);
void print_stack(struct Stack *stack);
void free_stack(struct Stack *stack);
