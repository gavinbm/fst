# SOPL - Stack Oriented Programming Language

I've found my interest in computer science to be compilers, assemblers, and interpreters (really anything to do with linguistics and automata implementations) and I thought FORTH was an interesting language. So I've set out to design what is essentially my own FORTH, implemented in C which isn't really what FORTHs are supposed to be written in but this isn't FORTH so that doesn't matter. It's quite a bit more than a language really, it's a stack-based VM where the language is that VM's assembly language, you can think of it like programming LLVM or JVM bytecode directly instead of compiling to them. The fun part with these languages is how easy they are to implement, since stack-based languages don't usually have a strict gammar to parse, you can play it fast and loose with things like lexical analysis and parse tree building.

This is an exercise in programming for me, this language is likely not suitable for any sort of commercial use but it's fun and that's what matters to me.

# TODOs and Milestones
This is the very early stages of development and right now only really allows you to interact with a stack that stores integers via pushes and pops. The hope is to add a decent number of built-in instructions that users can use to build even better, higher-level instructions. This TODO list will absolutely change greatly over the next few months as I decide what features to actually include in the finished product.

 - [x] Make the stack store numbers
 - [x] Write an input loop
 - [x] Write the lexer
 - [x] Build the initial dictionary
 - [ ] Allow for expansion of the dictionary

# Current Features and Instructions
In accordance with it's stack-based nature, sopl uses reverse polish notation to perform arithmetic and has a collection of instructions, all of which use the stack to get their arguments and many of which offer exciting ways to manipulate the values on the stack. So a reverse polish notation expression like
```
2 3 +
```
written in the standard infix notation would be
```
2 + 3
```
The actual operations that happen here are sopl will read the 2, recognize it's a number, and push it to the stack. Then it will do the same for 3, and once it hits the + it will add 2 and 3 together since they're on the top of the stack and store the result, 5, at the top of the stack. So the 2 and 3 will be consumed and the only value left on the stack will be the 5.

The built-in instructions include:
```
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
    - :   creates a new instruction using the ones you have
```

# The Virtual Machine (it's just a stack)
We can only put integers on our stack so we can implement the stack as an integer array like so
```C
int stack[STACKCAP]; // the stack itself
int spr = -1;        // the stack pointer, index of the top of the stack
```
STACKCAP is a defined constant (it's 64) so we have that many stack spots which we can fill and manipulate to our hearts content. Obviously a stack can manipulated only via pushes and pops, anything else is just a bunch of pushes and pops wearing a big trench coat pretending to be a unique instruction. The push and pop we functions we use can be seen here
```C
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

```
Simple stuff right? All we really have to do is check that we're within the bounds of our stack and then increment the stack pointer as needed, filling in values when we push. You might notice we don't delete anything when we pop, that's fine, decrementing the stack pointer ensure we simply can't access it and that it will be overwritten once we push again.

# The Interpreter - The Lexer
Reading stack-based languages are really simple if you're using reverse polish notation, especially when we have such a sparse set of features and language constructs. We'll examine the interpreter I've written by chunking it into sections, let's start with the lexer, which is really just one struct and two functions. The meat and potatoes of our lexer is the next function (I'll give the code later) which grabs the next token and all the information needed to figure out what exactly it is and if it's valid.

The lexer is represented as a single struct like so
```C
struct lex {
    int type;  // the type of the current token
    char *tok; // the text of the current token
    char *pos; // the position in the string we're lexing
} typedef lex;
```

This a pretty standard, barebones style lexer that doesn't use any fun bells and whistles. We allocate only one of these and just keep updating the values stored in that single lexer. We update these values with the following function

```C
// lexer - the lexer we're editing
// tok   - the new token text we're looking at
// type  - the lexeme type associated with the token
// size  - the length of the token text
void updatetok(lex **lexer, char *tok, int type, int size) {
    // free the old token text if it's there
    if((*lexer)->tok)
        free((*lexer)->tok);
    
    (*lexer)->tok = malloc(size + 1); // allocate the space for the text
    memcpy((*lexer)->tok, tok, size); // copy in the text
    (*lexer)->tok[size] = '\0';       // set the null terminator
    (*lexer)->type = type;            // set the new type
}
```

I love double pointers, they're magical things really once ya get a hang for em. This function is called everytime we check a token, we can see how it's called in the next function the code for which is here

```C
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
```
You can see that we're once again abusing double pointers, passing in the address to the lexer so we can update it without allocating new pointers. I think reducing the number of mallocs greatly improves the efficiency of your code since malloc works via some massive linked list that exists behind your code and reducing your interactions with that likely improves both the asymptotic time and the real time taken by your code.

# The Interpreter - Parsing
We don't really parse honestly, no need to with a language like this that has no strict grammar. It's just space separated instructions, easy stuff. The tokens we get are either things that go on the stack or instructions that do stuff to the stack (unless it's a string literal which just gets printed to stdout).

# The Interpreter - Instruction Execution
This is pretty fun, I'm not going to include the code because it's a 100+ line switch statement but the entire process I'm going to outline is in the exec function. This function decides what to do based on the current lexeme type. If we get a number, we put it on the stack, if we get a word/character, we check if it's an instruction. If it's an instruction, we execute it, otherwise we throw an error (which I think is rather comically written). Really simple stuff but it was fun to write the C implementation for all of these instructions.

# What is a FORTH and what is Sopl?
FORTH is a lot of things but to sum it up, it's a stack-based virtual machine that typically runs on a system without an OS and performs all interpretation, compilation, and assembly of the language on its own. This isn't what I'm doing here as I'd have to write a USB stack and familiarize myself with processor interrupts to do that properly and I don't have the time to commit to that these days.

Sopl is like a higher-level FORTH that runs inside an OS. It's purely an exercise in C programming, interpreter design, and (most importantly) fun for me.

If you're technically savvy and want to learn more about FORTH, [the wikipedia page](https://www.cs.mcgill.ca/~rwest/wikispeedia/wpcd/wp/f/Forth.htm#:~:text=Forth%20parsing%20is%20simple%2C%20as,systems%20recognise%20additional%20whitespace%20characters.) has plenty of details to learn the inner machinations of it. If you're less technically inclined, no problem, the book [Starting FORTH](https://1scyem2bunjw1ghzsf1cjwwn-wpengine.netdna-ssl.com/wp-content/uploads/2018/01/Starting-FORTH.pdf) is a great read that's not nearly as dry as you might think a book about an esoteric programming language/OS/VM/assembler/interpreter would be.
