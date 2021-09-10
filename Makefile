all: build

build: stack.o repl.o lexer.o
	cc -o tinyforth stack.o repl.o lexer.o -ggdb

repl.o: repl.c tinyforth.h
	cc -c repl.c tinyforth.h -ggdb

lexer.o: lexer.c tinyforth.h
	cc -c lexer.c tinyforth.h -ggdb

stack.o: stack.c tinyforth.h
	cc -c stack.c tinyforth.h -ggdb

clean:
	rm tinyforth tinyforth.h.gch *.o 
	