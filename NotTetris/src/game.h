#ifndef GAME_H
#define GAME_H



typedef struct{ // StateMachine
	void (*enter)(void);
	void (*update)(void);
}StateMachine;


extern int screenWidth;
extern int screenHeight;
extern StateMachine sm;
extern void (*Screen)(void);

void StateSpawnEnter(void);
void StateFallEnter(void);
void StateFallUpdate(void);
void StateClearEnter(void);
void StateClearUpdate(void);
void StateGameOverEnter(void);
void StateGameOverUpdate(void);


void InitGame(void);
void GameLoop(void);

void InitGameScreen(void);
void GameScreen(void);
void SpawnNewPiece(void);

void DrawPreview(void);

#endif // GAME_H









