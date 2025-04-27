#include "raylib.h"
#include "game.h"
#include "screens.h"
#include "levels.h"

int tileSize = TSIZE;
int levelTiles[COLUMNS * ROWS];

// TODO: use custom struct for data extendability
// Solid block layout for a level
int lvl_1[COLUMNS * ROWS] = {
3,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0,3,
};
int freeTiles[COLUMNS * ROWS];
int freeCount;
int snakeLength = 3;
int revealSegment = -1;
int snakeTiles[COLUMNS * ROWS];
Vector2Int moveDir = { 1,0 };
Vector2Int nextDir = { 1,0 };
int waitInterval;
int moveTimer;


Color ColorBlend(Color a, Color b, float percent) {
    return (Color) {
        a.r + (int)((b.r - a.r) * percent),
            a.g + (int)((b.g - a.g) * percent),
            a.b + (int)((b.b - a.b) * percent),
            a.a + (int)((b.a - a.a) * percent),
    };
}

// TODO: Add more level layouts
/* Populate tile data for set level*/
void SetLevel(int* lvl) {
    for (int i = 0; i < COLUMNS * ROWS; i++) {
        levelTiles[i] = lvl[i];
    }
}

void UpdateFreeTiles(void) {
    // add free tile index to array
    int j = 0;
    for (int i = 0; i < COLUMNS * ROWS; i++) {
        if (levelTiles[i] == FREE) {
            freeTiles[j] = i;
            j++;
        }
    }
}

void SpawnApple() {
    int n = GetRandomValue(0, freeCount - 1);
    levelTiles[freeTiles[n]] = APPLE;
}

void InitLevelStart(void) {
    CurrentUpdateCallback = LevelStartScreen;
    SetLevel(lvl_1);
    waitInterval = 6 + (DIFFICULITY_MENU_COUNT - 1 - diffMenuIndex) * 5;
    moveTimer = -1;
    snakeLength = 3;
    revealSegment = -1;
    moveDir = (Vector2Int){ 1,0 };
    nextDir = (Vector2Int){ 1,0 };

    // Place snake in level
    // TODO: place it in level data
    snakeTiles[0] = COLUMNS * 4 + 5;
    snakeTiles[1] = COLUMNS * 4 + 4;
    snakeTiles[2] = COLUMNS * 4 + 3;

    // Get free tile count
    freeCount = 0;
    for (int i = 0; i < COLUMNS * ROWS; i++) {
        if (levelTiles[i] == FREE) {
            freeCount++;
        }
    }

    UpdateFreeTiles();
    SpawnApple();
}

void LevelStartScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        InitDifficulity();
        PlaySound(s_cancel);
    }
    moveTimer++;
    if (moveTimer % waitInterval == 0) {
        revealSegment++;
        if (revealSegment > 0) {
            levelTiles[snakeTiles[snakeLength - 1 - revealSegment + 1]] = SNAKE;
            PlaySound(s_apple);
        }
        if (revealSegment == snakeLength) {
            // Start moving
            InitGameplay();
            prevPalette[0] = palette[0];
            prevPalette[1] = palette[1];
            prevPalette[2] = palette[2];
            prevPalette[3] = palette[3];
        }
    }
    if (revealSegment > -1 && revealSegment < snakeLength) {
        if (moveTimer < waitInterval / 2) {
            levelTiles[snakeTiles[snakeLength - 1 - revealSegment]] = SNAKE;
        }
        else {
            levelTiles[snakeTiles[snakeLength - 1 - revealSegment]] = FREE;
        }
    }

    InputUpdate();

    // Color interpolation from prevPalette to palette
    float percent = (float)moveTimer / (waitInterval * snakeLength);
    Color changedColor[] = {
      ColorBlend(prevPalette[0], palette[0], percent),
      ColorBlend(prevPalette[1], palette[1], percent),
      ColorBlend(prevPalette[2], palette[2], percent),
      ColorBlend(prevPalette[3], palette[3], percent),
    };


    BeginDrawing();

    ClearBackground(palette[0]);
    DrawLevel(changedColor);
    EndDrawing();
}


void DrawLevel(Color pal[4]) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLUMNS; x++) {
            int tile = levelTiles[y * COLUMNS + x];
            DrawRectangle(x * tileSize + originX, y * tileSize + originY, tileSize, tileSize, pal[tile]);
        }
    }

    // Black bars outside games aspect ratio
    Color barCol = { 0,0,20, 255 };
    if (originX > 0) {
        DrawRectangle(0.0f, 0.0f, originX, screenHeight, barCol);
        DrawRectangle(screenWidth - originX, 0.0f, originX, screenHeight, barCol);
    }
    if (originY > 0) {
        DrawRectangle(0.0f, 0.0f, screenWidth, originY, barCol);
        DrawRectangle(0.0f, screenHeight - originY, screenWidth, originY, barCol);
    }
}