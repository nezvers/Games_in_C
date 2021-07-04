
#include "tetromino.h"
#include <stdio.h>

int cellSize = 16;
int gridWidth = 10;
int gridHeight = 20;
int grid[10*24];
v2i shape[] = {
{ 0,0},{-1, 0},{ 1, 0},{ 0,-1},	{0, 0},{0,-1},{1,0},{0, 1},	{ 0,0},{-1, 0},{1,0},{ 0, 1},	{ 0, 0},{ 0,-1},{-1,0},{ 0, 1},	// T
{ 0,0},{-1, 0},{ 0,-1},{ 1,-1},	{0, 0},{0,-1},{1,0},{1, 1},	{ 0,1},{-1, 1},{0,0},{ 1, 0},	{-1, 0},{-1,-1},{ 0,0},{ 0, 1},	// S
{ 0,0},{ 1, 0},{ 0,-1},{-1,-1},	{0, 0},{0, 1},{1,0},{1,-1},	{ 0,1},{ 1, 1},{0,0},{-1, 0},	{-1, 0},{-1, 1},{ 0,0},{ 0,-1},	// Z
{-1,0},{ 0, 0},{ 1, 0},{ 1,-1},	{0,-1},{0, 0},{0,1},{1, 1},	{-1,0},{ 0, 0},{1,0},{-1, 1},	{ 0,-1},{ 0, 0},{ 0,1},{-1,-1},	// L
{-1,0},{ 0, 0},{ 1, 0},{-1,-1},	{0,-1},{0, 0},{0,1},{1,-1},	{-1,0},{ 0, 0},{1,0},{ 1, 1},	{ 0,-1},{ 0, 0},{ 0,1},{-1, 1},	// J
{-1,0},{ 0, 0},{ 1, 0},{ 2, 0},	{1,-1},{1, 0},{1,1},{1, 2},	{-1,1},{ 0, 1},{1,1},{ 2, 1},	{ 0,-1},{ 0, 0},{ 0,1},{ 0, 2},	// I
{ 0,0},{ 0,-1},{ 1, 0},{ 1,-1},	{0, 0},{0,-1},{1,0},{1,-1},	{ 0,0},{ 0,-1},{1,0},{ 1,-1},	{ 0, 0},{ 0,-1},{ 1,0},{ 1,-1},	// O
};

// all except "I". 5 positions. [rFrom*10 + (int)(rDirection > 0)*5]
v2i rotation[] = {
{ 0, 0},{ 1, 0},{ 1, 1},{ 0,-2},{ 1,-2},	{ 0, 0},{-1, 0},{-1, 1},{ 0,-2},{-1,-2},
{ 0, 0},{ 1, 0},{ 1,-1},{ 0, 2},{ 1, 2},	{ 0, 0},{ 1, 0},{ 1,-1},{ 0, 2},{ 1, 2},
{ 0, 0},{-1, 0},{-1, 1},{ 0,-2},{-1,-2},	{ 0, 0},{ 1, 0},{ 1, 1},{ 0,-2},{ 1,-2},
{ 0, 0},{-1, 0},{-1,-1},{ 0, 2},{-1, 2},	{ 0, 0},{-1, 0},{-1,-1},{ 0, 2},{-1, 2},
};

v2i rotation_I[] = {
{ 0, 0},{-1, 0},{ 2, 0},{-1, 2},{ 2,-1},	{ 0, 0},{-2, 0},{ 1, 0},{-2,-1},{ 1, 2},
{ 0, 0},{ 2, 0},{-1, 0},{ 2, 1},{-1,-2},	{ 0, 0},{-1, 0},{ 2, 0},{-1, 2},{ 2,-1},
{ 0, 0},{ 1, 0},{-2, 0},{ 1,-2},{-2, 1},	{ 0, 0},{ 2, 0},{-1, 0},{ 2, 1},{-1,-2},
{ 0, 0},{-2, 0},{ 1, 0},{-2,-1},{ 1, 2},	{ 0, 0},{ 1, 0},{-2, 0},{ 1,-2},{-2, 1},
};

//					T, 		S, 	 Z, 	L, 		J, 		I, 					O, 	solid, BG,
Color colors[] = { { 200, 122, 255, 255 }, { 0, 228, 48, 255 }, { 230, 41, 55, 255 }, { 255, 161, 0, 255 }, { 0, 121, 241, 255 }, {0,255,255,255}, { 253, 249, 0, 255 }, { 130, 130, 130, 255 }, { 0, 0, 0, 255 } };

int levelSpeed[] = {60, 54, 48, 41, 37, 31, 28, 21, 16, 11, 8, 6, 4, 3, 2, 1};
int levelCount = 16;

/*
J, L, S, T, Z Tetromino Wall Kick Data
Test 1	Test 2	Test 3	Test 4	Test 5
0>>3	( 0, 0)	( 1, 0)	( 1, 1)	( 0,-2)	( 1,-2)
0>>1	( 0, 0)	(-1, 0)	(-1, 1)	( 0,-2)	(-1,-2)
1>>0	( 0, 0)	( 1, 0)	( 1,-1)	( 0, 2)	( 1, 2)
1>>2	( 0, 0)	( 1, 0)	( 1,-1)	( 0, 2)	( 1, 2)
2>>1	( 0, 0)	(-1, 0)	(-1, 1)	( 0,-2)	(-1,-2)
2>>3	( 0, 0)	( 1, 0)	( 1, 1)	( 0,-2)	( 1,-2)
3>>2	( 0, 0)	(-1, 0)	(-1,-1)	( 0, 2)	(-1, 2)
3>>0	( 0, 0)	(-1, 0)	(-1,-1)	( 0, 2)	(-1, 2)

I Tetromino Wall Kick Data
Test 1	Test 2	Test 3	Test 4	Test 5
0>>3	( 0, 0)	(-1, 0)	( 2, 0)	(-1, 2)	( 2,-1)
0>>1	( 0, 0)	(-2, 0)	( 1, 0)	(-2,-1)	( 1, 2)
1>>0	( 0, 0)	( 2, 0)	(-1, 0)	( 2, 1)	(-1,-2)
1>>2	( 0, 0)	(-1, 0)	( 2, 0)	(-1, 2)	( 2,-1)
2>>1	( 0, 0)	( 1, 0)	(-2, 0)	( 1,-2)	(-2, 1)
2>>3	( 0, 0)	( 2, 0)	(-1, 0)	( 2, 1)	(-1,-2)
3>>2	( 0, 0)	(-2, 0)	( 1, 0)	(-2,-1)	( 1, 2)
3>>0	( 0, 0)	( 1, 0)	(-2, 0)	( 1,-2)	(-2, 1)

*/


void ClearGrid(){
	for(int i=0; i<10*24; i++){
		grid[i] = -1;
	}
}

void DrawGridLines(int x, int y, int _col, int _row, int _size, Color color){
	int lenX = _col*_size;
    int lenY = _row*_size;
    // horizontal lines
    for (int i = 0; i < _row+1; i++){
        DrawLine(x, y + _size*i, x+lenX, y + _size*i, color);
    }
    // vertical lines
    for (int i = 0; i < _col+1; i++){
        DrawLine(x + _size*i, y, x + _size*i, y+lenY, color);
    }
}

void DrawLevel(int x, int y){
	for(int i=gridWidth*4; i<gridWidth*(gridHeight+4); i++){
		if(grid[i] > -1){
			int _x = i % gridWidth;
			int _y = i / gridWidth;
			DrawRectangle(x +_x *cellSize, y +(_y -4) *cellSize, cellSize, cellSize, colors[grid[i]]);
		}
	}
}

void DrawTetromino(Tetromino *t, int x, int y){
	for (int i=0; i<4; i++){
		v2i pos = VecAdd(t->b[i], t->p);
		if(pos.y > 3){
			pos.y -= 4;
			DrawRectangle(pos.x*cellSize +x, pos.y*cellSize +y, cellSize, cellSize, colors[t->s]);
		}
	}
}

void DrawTetrominoGhost(Tetromino *t, int x, int y){
	int _y = 0;
	for(int i=0; i<24; i++){
		if(!CheckCollision(VecAdd(t->p, (v2i){0,i}), t->s, t->r)){
			_y = i;
		}
		else{break;}
	}
	Tetromino ghost = *t;
	ghost.p.y += _y;
	
	for (int i=0; i<4; i++){
		v2i pos = VecAdd(ghost.b[i], ghost.p);
		if(pos.y > 3){
			pos.y -= 4;
			DrawRectangle(pos.x*cellSize +x, pos.y*cellSize +y, cellSize, cellSize, (Color){255,255,255,75});
		}
	}
}

void DrawTetrominoEx(Tetromino *t, int x, int y){
	for (int i=0; i<4; i++){
		v2i pos = t->b[i];
		DrawRectangle(pos.x*cellSize +x, pos.y*cellSize +y, cellSize, cellSize, colors[t->s]);
	}
}


Tetromino GetRandomTetromino(){
	Tetromino t = {0};
	
	t.s = GetRandomValue(0, shapeCount-1);
	t.b = &shape[t.s*16 + r0*4];
	return t;
}

void PlaceTetromino(Tetromino *t){
	for(int i=0; i<4; i++){
		v2i p = VecAdd(t->p, t->b[i]);
		grid[p.x + p.y *gridWidth] = t->s;
	}
}

v2i VecAdd(v2i a, v2i b){
	return (v2i){a.x+b.x, a.y+b.y};
}

int GetCellId(int x, int y){
	if (x<0 || x>=gridWidth || y<0 || y>=gridHeight+4){
		return 1;
	}
	return grid[x +y *gridWidth];
}

int GetTetrominoLowest(Tetromino *t){
	int line = 0;
	for (int i=0; i<4; i++){
		int y = t->p.y +t->b[i].y;
		if(y > line){line = y;}
	}
	return line;
}

bool CheckCollision(v2i pos, int _shape, int rot){
	v2i *blocks = &shape[_shape*16 +rot*4];
	for(int i=0; i<4; i++){
		v2i p = VecAdd(pos, blocks[i]);
		if (GetCellId(p.x, p.y) > -1){
			return true;
		}
	}
	return false;
}

int CheckLineCleared(int line){
	for(int i=0; i<gridWidth; i++){
		int value = grid[line*gridWidth +i];
		if(value < 0){
			return 0;
		}
	}
	return 1;
}

int GetLineClearBitmask(int line){
	int bitmask = 0;
	for(int i=0; i<4; i++){
		int b = CheckLineCleared(line -i);
		bitmask += b << i;
	}
	return bitmask;
}

bool CheckLineEmpty(int line){
	for(int i=0; i<gridWidth; i++){
		int value = grid[line*gridWidth +i];
		if(value > -1){
			return false;
		}
	}
	return true;
}

void TetrominoMove(Tetromino *t, v2i dir){
	t->p = VecAdd(t->p, dir);
}

void TetrominoRotate(Tetromino *t, int rDir){
	v2i *off;
	
	if (t->s == I){
		off = &rotation_I[t->r*10 + (int)(rDir > 0)*5];
	}else{
		off = &rotation[t->r*10 + (int)(rDir > 0)*5];
	}
	
	int rNew = (t->r +rDir + 4) % 4;
	for(int i=0; i<5; i++){
		if(!CheckCollision(VecAdd(t->p, off[i]), t->s,rNew)){
			t->p = VecAdd(t->p, off[i]);
			t->r = rNew;
			t->b = &shape[t->s*16 + rNew*4];
			break;
		}
	}
}

void TetrominoHardDrop(Tetromino *t){
	v2i d = {0,1};
	for(int i=0; i<21; i++){
		if(!CheckCollision(VecAdd(t->p, d), t->s, t->r)){
			t->p = VecAdd(t->p, d);
		}else{
			break;
		}
	}
}

void GridClearLines(int line, int bitmask){
	int emptyCount = 0;
	for(int y=line; y>-1; y--){
		int b = bitmask >> (line -y);
		if(line -y < 4 && b & 1){
			emptyCount++;
			for(int x=0; x<gridWidth; x++){
				grid[y*gridWidth +x] = -1;
			}
		}
		else if (!CheckLineEmpty(y)){
			if(emptyCount > 0){
				for(int x=0; x<gridWidth; x++){
					grid[(y+emptyCount)*gridWidth +x] = grid[y*gridWidth +x];
					grid[y*gridWidth +x] = -1;
				}
			}
		}
		else{
			return;
		}
	}
}











