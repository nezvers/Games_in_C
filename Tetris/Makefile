OUTPUT=main
CC=gcc
RAYLIB=raylib
CFLAGS=-Wall -std=c99 -Wno-missing-braces
LFLAGS=-IRaylib -L$(RAYLIB) -l$(RAYLIB) -lopengl32 -lgdi32 -lwinmm

all: main

hardcoded: main.c game.c tetromino.c
	$(CC) main.c game.c tetromino.c -o $(OUTPUT).exe Raylib/raylib.rc.data $(CFLAGS) $(LFLAGS)


main: main.c game.o tetromino.o
	$(CC) $(CFLAGS) -o $(OUTPUT).exe $^ Raylib/raylib.rc.data $(LFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

clean:
	del *.o $(OUTPUT).exe