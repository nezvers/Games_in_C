all: build

build:
	gcc main.c game.c tetromino.c -o main.exe Raylib/raylib.rc.data -Wall -std=c99 -Wno-missing-braces -IRaylib -LRaylib -lraylib -lopengl32 -lgdi32 -lwinmm

