all: build

build: stack.o repl.o lexer.o
	cc -o ibp stack.o repl.o lexer.o -ggdb

repl.o: repl.c ibp.h
	cc -c repl.c ibp.h -ggdb

lexer.o: lexer.c ibp.h
	cc -c lexer.c ibp.h -ggdb

stack.o: stack.c ibp.h
	cc -c stack.c ibp.h -ggdb

clean:
	rm ibp ibp.h.gch *.o 
	