CC = g++

# compiler flags
CFLAGS = -Wall -g

# linker flags
LFLAGS = -lSDL2

# dependencies
DEP = chip8.h
OBJ = main.o chip8.o

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c %.c

main: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LFLAGS) -o main

clean:
	$(RM) *.o *~
