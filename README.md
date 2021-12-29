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
In accordance with it's stack-based nature, sopl uses reverse polish notation to perform arithmetic and has a collection of instructions, all of which use the stack to get their arguments and many of which offer exciting ways to manipulate the values on the stack. an example of reverse polish notation is 
```
2 3 +
```
Which is equivalent to
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

# What is a FORTH and what is Sopl?
FORTH is a lot of things but to sum it up, it's a stack-based virtual machine that typically runs on a system without an OS and performs all interpretation, compilation, and assembly of it's language on it's own. This isn't what I'm doing here as I'd have to write a USB stack and familiarize myself with processor interrupts to do that properly and I don't have the time to commit to that these days.

Sopl is like a higher-level FORTH that runs inside an OS. It's purely an exercise in C programming, interpreter design, and (most importantly) fun for me.

If you're technically savvy and want to learn more about FORTH, [the wikipedia page](https://www.cs.mcgill.ca/~rwest/wikispeedia/wpcd/wp/f/Forth.htm#:~:text=Forth%20parsing%20is%20simple%2C%20as,systems%20recognise%20additional%20whitespace%20characters.) has plenty of details to learn the inner machinations of it. If you're less technically inclined, no problem, the book [Starting FORTH](https://1scyem2bunjw1ghzsf1cjwwn-wpengine.netdna-ssl.com/wp-content/uploads/2018/01/Starting-FORTH.pdf) is a great read that's not nearly as dry as you might think a book about an esoteric programming language/OS/VM/assembler/interpreter would be.
