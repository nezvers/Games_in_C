/*******************************************************************************************
*
*   Near Miss Snake
*
*   Sample game developed by Agnis Aldiņš "NeZvers"
*
*   This game has been created using raylib v3.5.0 (www.raylib.com)
*
*
********************************************************************************************/


#include "raylib.h"
#include <stdio.h>
#include "game.h"
#include "screens.h"
#include "levels.h"

//Sounds & music are converted using bi2header - https://github.com/AntumDeluge/bin2header
// SFX from sfxcellar.accusonus.com
#include "SFX_ui_cancel.wav.h"
#include "SFX_ui_menu.wav.h"
#include "SFX_ui_accept.wav.h"
#include "SFX_apple.wav.h"
// music - https://modarchive.org/index.php?request=view_by_moduleid&query=172898
#include "MUSIC_main.xm.h"


#if defined(PLATFORM_WEB) // TODO:
#include <emscripten/emscripten.h>
#endif


int screenWidth = COLUMNS * TSIZE;
int screenHeight = ROWS * TSIZE;
float textSizeRatio = 1.0;
int originX = 0;
int originY = 0;
bool quit = false;

Wave w_accept;
Wave w_cancel;
Wave w_menu;
Wave w_apple;

Sound s_accept;
Sound s_cancel;
Sound s_menu;
Sound s_apple;
Music music;
// Music, SFX
int volumeList[] = { 3, 4 };


Color palette[4];
Color prevPalette[4];
// palettes created using colorhunt.co
Color paletteList[] = {
  { 255, 255, 255, 255 }, { 200, 200, 200, 255 }, { 130, 130, 130, 255 }, { 0, 0, 0, 255 },
  {0xFA,0xF3,0xE0,255}, {0xEA,0xBF,0x9F,255}, {0xB6,0x89,0x73,255}, {0x1E,0x21,0x2D,255},
  {0xEA,0xFF,0xD0,255}, {0xFF,0x75,0xA0,255}, {0xFC,0xE3,0x8A,255}, {0x95,0xE1,0xD3,255},
  {0xF9,0xF8,0x71,255}, {0xFF,0x5E,0x78,255}, {0x84,0x5E,0xC2,255}, {0xFF,0xC7,0x5F,255},
  {0xF8,0xA1,0xD1,255}, {0x82,0x26,0x59,255}, {0xB3,0x41,0x80,255}, {0xE3,0x6B,0xAE,255},
  {0xFF,0xE2,0x27,255}, {0xEB,0x59,0x6E,255}, {0x4D,0x37,0x5D,255}, {0x12,0x10,0x13,255},
  {0xF4,0xE0,0x22,255}, {0xDE,0x1B,0x4A,255}, {0x5A,0x37,0xC3,255}, {0x18,0x22,0x4B,255},
  {0xFF,0xF6,0x00,255}, {0xFF,0x00,0x5C,255}, {0x81,0x00,0x34,255}, {0x26,0x00,0x1B,255},
  {0xf9,0xed,0x69,255}, {0xf0,0x8a,0xd,255}, {0xb8,0x3b,0x5e,255}, {0x6a,0x2c,0x70,255},
};
#define PALETTE_COUNT 5

void SetPalette(void) {
    int index = GetRandomValue(0, PALETTE_COUNT - 1) * 4;
    for (int i = 0; i < 4; i++) {
        prevPalette[i] = palette[i];
        palette[i] = paletteList[index + i];
    }
}


void SetGameMusicVolume(float value) {
    SetMusicVolume(music, 0.1 * value);
}

void SetGameSfxVolume(float value) {
    SetSoundVolume(s_accept, value);
    SetSoundVolume(s_cancel, value);
    SetSoundVolume(s_menu, value);
    SetSoundVolume(s_apple, value);
}


void Init(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Near Miss Snake");

    // Set up audio
    InitAudioDevice();
    SetMasterVolume(100.0);

    w_accept = LoadWaveFromMemory(".wav", (const unsigned char*)&SFX_ui_accept_wav, sizeof SFX_ui_accept_wav);
    w_cancel = LoadWaveFromMemory(".wav", (const unsigned char*)&SFX_ui_cancel_wav, sizeof SFX_ui_cancel_wav);
    w_menu = LoadWaveFromMemory(".wav", (const unsigned char*)&SFX_ui_menu_wav, sizeof SFX_ui_menu_wav);
    w_apple = LoadWaveFromMemory(".wav", (const unsigned char*)&SFX_apple_wav, sizeof SFX_apple_wav);

    s_accept = LoadSoundFromWave(w_accept);
    s_cancel = LoadSoundFromWave(w_cancel);
    s_menu = LoadSoundFromWave(w_menu);
    s_apple = LoadSoundFromWave(w_apple);
    SetGameSfxVolume((float)volumeList[SFX] / (MAXVOLUME - 1));

    music = LoadMusicStreamFromMemory(".xm", (unsigned char*)&MUSIC_main_xm, sizeof MUSIC_main_xm);
    SetGameMusicVolume((float)volumeList[MUSIC] / (MAXVOLUME - 1));
    PlayMusicStream(music);

    InitTitle();
}


void Cleanup(void){
    CloseAudioDevice();
    CloseWindow();
}

void UpdateFrame(void) {
    if (IsWindowResized())
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        // Original text size made for 2/1 aspect ratio
        float aspectRatio = (float)screenWidth / screenHeight;
        if (aspectRatio < 2.0) {
            textSizeRatio = aspectRatio / 2.0;
        }
        tileSize = screenWidth / 2 < screenHeight ? screenWidth / 20 : screenHeight / 10;
        // Offset to keep game centered
        originX = (screenWidth - COLUMNS * tileSize) / 2;
        originY = (screenHeight - ROWS * tileSize) / 2;
    }
    UpdateMusicStream(music);
    CurrentUpdateCallback();
}

















