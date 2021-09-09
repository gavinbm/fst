# FST - A stack-based VM and assembly language
Recntly I've been caught up in writing a compiler and I've decided that I'm about done with that project outside of adding a stdlib or optimizations like constant folding, so now I want to design what is essentially my own FORTH. This will be a much higher-level implementation than what most FORTHs are, and will be done in C instead of assembly because I've been enjoying C quite a bit more than assembly recently.

# TODOs and Milestones
This is the very early stages of development and right now only really allows you to interact with a stack that
stores integers via pushes and pops. The hope is to add a decent number of built-in instructions that users can use to build even better, higher-level instructions. This TODO list will absolutely change greatly over the next few months as I decide what features to actually include in the finished product.

 - [x] Make the stack store numbers
 - [x] Write an input loop
 - [ ] Write the lexer
 - [ ] Build the initial dictionary
 - [ ] Allow for expansion of the dictionary

# What is a FORTH and what is FST?
FORTH is a lot of things but to sum it up, it's a stack-based virtual machine that typically runs on a system without an OS and performs all interpretation, compilation, and assembly of it's language on it's own. This isn't what I'm doing here as I'd have to write a USB stack and familiarize myself with processor interrupts to do that properly and I don't have the time to commit to that these days.

FST will be a higher-level FORTH, likley with a more assembly-esque syntax, that runs inside an OS. It's purely an exercise in C programming, interpreter design, and (most importantly) fun for me.

If you're technically savvy and want to learn more about FORTH, [this article](https://www.cs.mcgill.ca/~rwest/wikispeedia/wpcd/wp/f/Forth.htm#:~:text=Forth%20parsing%20is%20simple%2C%20as,systems%20recognise%20additional%20whitespace%20characters.) is a fantastic technical write up. If you're less technically inclined, no problem, the book [Starting FORTH](https://1scyem2bunjw1ghzsf1cjwwn-wpengine.netdna-ssl.com/wp-content/uploads/2018/01/Starting-FORTH.pdf) is a fantastic read that's not nearly as dry as you might think a book about an esoteric programming language/OS/VM/assembler/interpreter would be.