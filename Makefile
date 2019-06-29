CC = g++

# compiler flags
CFLAGS = -Wall -g

# dependencies
DEP = chip8.h
OBJ = main.o chip8.o

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c %.c

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

clean:
	$(RM) *.o *~
