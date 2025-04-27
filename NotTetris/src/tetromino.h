#ifndef TETRIS_H
#define TETRIS_H

#include "raylib.h"

typedef struct{ //v2i
	int x;
	int y;
}v2i;

typedef struct{ // Tetromino
	v2i p;	// position
	int s;	// shape id
	int r;	// rotation id
	v2i *b;	// pointer to first shape[] block for shape and rotation
}Tetromino;


enum {// ROTATION POSITION
	r0,
	r1,
	r2,
	r3,
};

enum {// SHAPES
	T,
	S,
	Z,
	L,
	J,
	I,
	O,
	shapeCount, // for ranomization with modulo
};

// block positions as an offset from Tetromino x & y. 2d array in 1D [shapeEnum*16 + rotationEnum*4]
v2i shape[16* 7];


extern int cellSize; 	// grid size
extern int gridWidth;
extern int gridHeight;

extern Color colors[];
extern int levelSpeed[];
extern int levelCount;


v2i VecAdd(v2i a, v2i b);

Tetromino GetRandomTetromino();
void ClearGrid(void);
void PlaceTetromino(Tetromino *t);
void DrawTetromino(Tetromino *t, int x, int y);
void DrawTetrominoGhost(Tetromino *t, int x, int y);
void DrawTetrominoEx(Tetromino *t, int x, int y);
void DrawGridLines(int x, int y, int col, int row, int size, Color color);
void DrawLevel(int x, int y);
int GetCellId(int x, int y);
int GetTetrominoLowest(Tetromino *t);
bool CheckCollision(v2i pos, int _shape, int rot);
int CheckLineCleared(int line);
bool CheckLineEmpty(int line);
int GetLineClearBitmask(int line);

void TetrominoMove(Tetromino *t, v2i dir);
void TetrominoRotate(Tetromino *t, int rDir);
void TetrominoHardDrop(Tetromino *t);
void GridClearLines(int line, int bitmask);

#endif // TETRIS_H







