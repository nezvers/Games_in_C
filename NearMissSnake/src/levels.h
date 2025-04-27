#ifndef LEVELS_H
#define LEVELS_H

#define FREE 0
#define APPLE 1
#define SNAKE 2
#define SOLID 3
typedef struct {
    int x;
    int y;
}Vector2Int;

#define TSIZE 1280/20
#define COLUMNS 20
#define ROWS 10

extern int tileSize;
extern int levelTiles[];
extern int freeTiles[];
extern int freeCount;
extern int snakeTiles[];
extern int snakeLength;
extern int revealSegment;
extern Vector2Int moveDir;
extern Vector2Int nextDir;
extern int waitInterval;
extern int moveTimer;

void UpdateFreeTiles(void);
void SpawnApple(void);

/*Can't use current palette, because colors can be interpolated between palettes*/
void DrawLevel(Color pal[4]);

#endif // LEVELS_H