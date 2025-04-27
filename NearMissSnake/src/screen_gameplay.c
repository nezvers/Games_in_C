#include "raylib.h"
#include "game.h"
#include "screens.h"
#include "levels.h"

bool isDead = false;

bool InputUpdate(void);

int NextTile(int pos);
void Move(void);
void Food(int t);
void Died(void);
void Push(int from, int to);
void Eat(int t);

int PosWrap(Vector2Int pos) {
    pos.x = (pos.x + COLUMNS) % COLUMNS;
    pos.y = (pos.y + ROWS) % ROWS;
    return pos.x + COLUMNS * pos.y;
}

Vector2Int index2vec(int p) {
    return (Vector2Int) { p% COLUMNS, p / COLUMNS };
}

int vec2index(Vector2Int p) {
    return p.x + p.y * COLUMNS;
}


void InitGameplay() {
    CurrentUpdateCallback = GameplayScreen;
    moveTimer = 0;
    isDead = false;
}

void GameplayScreen() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        InitDifficulity();
        PlaySound(s_cancel);
    }

    moveTimer = (moveTimer + 1) % waitInterval;
    if (InputUpdate()) {
        moveTimer = 0;
    }

    if (moveTimer == 0) {
        moveDir = nextDir;
        int tile = NextTile(snakeTiles[0]);
        if (tile == snakeTiles[0]) { return; }
        int type = levelTiles[tile];
        switch (type) {
        case FREE:
            Move();
            break;
        case APPLE:
            Food(tile);
            break;
        case SNAKE:
            isDead = true;
            PlaySound(s_cancel);
            break;
        case SOLID:
            isDead = true;
            PlaySound(s_cancel);
            break;
        }
    }

    BeginDrawing();

    ClearBackground(palette[0]);
    DrawLevel(palette);
    EndDrawing();

  // do InitLevel after draw
    if (isDead) {
        Died();
    }
}


bool InputUpdate(void) {
    Vector2Int d = (Vector2Int){ 0,0 };
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        d = (Vector2Int){ 1,0 };
    }
    else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
        d = (Vector2Int){ -1,0 };
    }
    else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        d = (Vector2Int){ 0,-1 };
    }
    else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        d = (Vector2Int){ 0,1 };
    }
    if ((d.x != 0 || d.y != 0) && ((d.x == 0 && d.y != -moveDir.y) || (d.y == 0 && d.x != -moveDir.x))) {
        nextDir = d;
        if (d.x == moveDir.x && d.y == moveDir.y) {
            return true;	// can be used to instantly do the move
        }
    }
    return false;
}


int NextTile(int pos) {
    Vector2Int p = index2vec(pos);
    return PosWrap((Vector2Int) { p.x + moveDir.x, p.y + moveDir.y });
}

void Move(void) {
    levelTiles[snakeTiles[snakeLength - 1]] = FREE;
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeTiles[i] = snakeTiles[i - 1];
    }
    Vector2Int head = index2vec(snakeTiles[0]);
    snakeTiles[0] = PosWrap((Vector2Int) { head.x + moveDir.x, head.y + moveDir.y });
    levelTiles[snakeTiles[0]] = SNAKE;
}

void Died(void) {
    SetPalette();
    InitLevelStart();
}

void Food(int t) {
    int tile = NextTile(t);
    int type = levelTiles[tile];
    switch (type) {
    case FREE:
        Push(t, tile);
        break;
    case SOLID:
        Eat(t);
        PlaySound(s_apple);
        break;
    case SNAKE:
        Eat(t);
        PlaySound(s_apple);
        break;
    }
}

void Push(int from, int to) {
    levelTiles[from] = FREE;
    levelTiles[to] = APPLE;
    Move();
}

void Eat(int t) {
    snakeLength++;
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeTiles[i] = snakeTiles[i - 1];
    }
    snakeTiles[0] = t;
    levelTiles[t] = SNAKE;
    freeCount--;
    UpdateFreeTiles();
    SpawnApple();
}

