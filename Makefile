CC = g++

# compiler flags
CFLAGS = -Wall -g

# dependencies
DEP = chip8.h
OBJ = main.o chip8.o

%.o: %.c $(DEP)
	$(CC) -c -o $@ $^ $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
