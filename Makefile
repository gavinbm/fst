all: build

build: stack.o repl.o lexer.o
	cc -o fst stack.o repl.o lexer.o -ggdb

repl.o: repl.c fst.h
	cc -c repl.c fst.h -ggdb

lexer.o: lexer.c fst.h
	cc -c lexer.c fst.h -ggdb

stack.o: stack.c fst.h
	cc -c stack.c fst.h -ggdb

clean:
	rm fst fst.h.gch *.o 
	