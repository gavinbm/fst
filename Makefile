all: build

build: stack.o repl.o
	cc -o fst stack.o repl.o -ggdb

repl.o: repl.c fst.h
	cc -c repl.c fst.h -ggdb

stack.o: stack.c fst.h
	cc -c stack.c fst.h -ggdb

clean:
	rm fst *.o
	