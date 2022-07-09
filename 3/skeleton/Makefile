CC      = gcc
CFLAGS  = -std=c99 -pedantic -Wall

OBJ = mips.o compiler.o memory.o

main: test.o $(OBJ)
	$(CC) $(CFLAGS) -o test test.o $(OBJ)

test: main
	./test

compile: compile.o $(OBJ)
	$(CC) $(CFLAGS) -o compile compile.o $(OBJ)

clean:
	-rm -f *.o compile test mipsTestFile

%.o: %.c
	$(CC) $(CFLAGS) -c $<
