#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
// MAYBE: #include <stdint.h> convert most ints to uint8_t

void Init(void);
void Cleanup(void);
void UpdateFrame(void);

/*breaks the game loop*/
extern bool quit;

extern int screenWidth;
extern int screenHeight;
extern float textSizeRatio;
/*To keep games aspect ratio, elements are drawn with an offset*/
extern int originX;
extern int originY;

extern Color palette[4];
extern Color prevPalette[4];
void SetPalette(void);

extern Sound s_accept;
extern Sound s_cancel;
extern Sound s_menu;
extern Sound s_apple;
extern Music music;
void SetGameMusicVolume(float value);
void SetGameSfxVolume(float value);

#define MAXVOLUME 11
#define MUSIC 0
#define SFX 1
/*Music, SFX*/
extern int volumeList[2];

#define DIFFICULITY_MENU_COUNT 10
extern int diffMenuIndex;

#endif // GAME_H