
#include "game.h"
#include "tetromino.h"
#include "stdio.h"

int screenWidth = 800;
int screenHeight = 450;
StateMachine sm = {0};
int fallTime;
int fallFrames = 30;
int clearBitmask = 0;
int clearLine = 0;
Tetromino pieces[7];
Tetromino *tet; // moving piece

void (*Screen)(void);



void StateMachineTransition(StateMachine *_sm, void (*_enter)(), void (*_update)()){
	_sm->enter = _enter;
	_sm->update = _update;
	_sm->enter();
}

void InitGame(){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	InitGameScreen();
}

void GameLoop(void){
	if (IsWindowResized()){
		screenWidth = GetScreenWidth();
		screenHeight = GetScreenHeight();
		
	}
    Screen();
}

void InitGameScreen(){
	Screen = GameScreen;
	ClearGrid();
	//Prepare preview pieces
	for(int i=1; i<7; i++){
		pieces[i] = GetRandomTetromino();
	}
	
	// ALWAYS LAST
	StateMachineTransition(&sm, StateSpawnEnter, (void*){0});
}

void GameScreen(){
	sm.update();
	
	BeginDrawing();

        ClearBackground(BLACK);
		DrawTetrominoGhost(tet);
		DrawTetromino(tet);
		DrawLevel(0, 0);
		//shaded grid pizzazz
        DrawGridLines(1, 1, gridWidth, gridHeight, cellSize, GRAY);
        DrawGridLines(0, 0, gridWidth, gridHeight, cellSize, LIGHTGRAY);
		
		DrawPreview();

    EndDrawing();
}

void DrawPreview(){
	int xOrigin = (gridWidth+2) *cellSize;
	int yOrigin = (1) *cellSize;
	for (int i=0; i<6; i++){
		DrawTetrominoEx(&pieces[i+1], xOrigin, yOrigin +(cellSize*3 *i));
	}
}

void InputUpdate(){
	static int moveTimer = -5;
	const int moveMod = 2;
	
	if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
		if(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)){
			if(!CheckCollision(VecAdd(tet->p, (v2i){1,0}), tet->s, tet->r)){
				TetrominoMove(tet, (v2i){1,0});
			}
			moveTimer = -moveMod * 5;
		}
		moveTimer++;
		if(moveTimer >= moveMod){moveTimer =0;}
		if(moveTimer ==0){
			if(!CheckCollision(VecAdd(tet->p, (v2i){1,0}), tet->s, tet->r)){
				TetrominoMove(tet, (v2i){1,0});
			}
		}
	}
	if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
		if(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)){
			if(!CheckCollision(VecAdd(tet->p, (v2i){-1,0}), tet->s, tet->r)){
				TetrominoMove(tet, (v2i){-1,0});
			}
			moveTimer = -moveMod *5;
		}
		moveTimer++;
		if(moveTimer >= moveMod){moveTimer =0;}
		if(moveTimer ==0){
			if(!CheckCollision(VecAdd(tet->p, (v2i){-1,0}), tet->s, tet->r)){
				TetrominoMove(tet, (v2i){-1,0});
			}
		}
	}
	
	if(IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)){
		fallTime = -1;
		TetrominoHardDrop(tet);
	}
	
	if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){
		if (fallTime >0){
			fallTime = -moveMod;
		}
	}
	
	if(IsKeyPressed(KEY_M)){
		TetrominoRotate(tet, 1);
	}else if(IsKeyPressed(KEY_N)){
		TetrominoRotate(tet, -1);
	}
}

void SpawnNewPiece(){
	for(int i=0; i<6; i++){
		pieces[i] = pieces[i+1];
	}
	pieces[6] = GetRandomTetromino();
	tet = &pieces[0];
}

void StateSpawnEnter(){
	SpawnNewPiece();
	tet->p = (v2i){4, 2};
	
	
	
	if(CheckCollision(tet->p, tet->s, tet->r)){
		StateMachineTransition(&sm, StateGameOverEnter, StateGameOverUpdate);
	}
	else{
		StateMachineTransition(&sm, StateFallEnter, StateFallUpdate);
	}
}



void StateFallEnter(void){
	fallTime = -1;
}
void StateFallUpdate(void){
	InputUpdate();
	
	fallTime = (fallTime+1) % fallFrames;
	if (fallTime == 0){
		if(!CheckCollision(VecAdd(tet->p, (v2i){0,1}), tet->s, tet->r)){
			TetrominoMove(tet, (v2i){0, 1});
		}
		else{
			PlaceTetromino(tet);
			StateMachineTransition(&sm, StateClearEnter, StateClearUpdate);
		}
	}
}
void StateClearEnter(void){
	clearLine = GetTetrominoLowest(tet);
	//Tetromino hasn't left the spawn zone
	if (clearLine < 4){
		StateMachineTransition(&sm, StateSpawnEnter, (void*){0});
		return;
	}
	
	clearBitmask = GetLineClearBitmask(clearLine);
	// no line has been cleared
	if (clearBitmask == 0){
		StateMachineTransition(&sm, StateSpawnEnter, (void*){0});
		return;
	}
	
	GridClearLines(clearLine, clearBitmask);
	
	StateMachineTransition(&sm, StateSpawnEnter, (void*){0});
}
void StateClearUpdate(void){
	
}
void StateGameOverEnter(void){
	
}
void StateGameOverUpdate(void){
	
}