#include "raylib.h"
#include "game.h"
#include "screens.h"


#define OPTIONS_MENU_COUNT 3
int optionsIndex;

void InitOptions(void) {
    CurrentUpdateCallback = OptionsScreen;
    SetExitKey(0);
    optionsIndex = OPTIONS_MENU_COUNT - 1;
}

void OptionsScreen(void) {
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        optionsIndex = (optionsIndex - 1 + OPTIONS_MENU_COUNT) % OPTIONS_MENU_COUNT;
        PlaySound(s_menu);
    }
    else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        optionsIndex = (optionsIndex + 1) % OPTIONS_MENU_COUNT;
        PlaySound(s_menu);
    }
    else if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && optionsIndex != OPTIONS_MENU_COUNT - 1) {
        volumeList[optionsIndex] = (volumeList[optionsIndex] + 1) % MAXVOLUME;
        if (optionsIndex == MUSIC) {
            SetGameMusicVolume((float)volumeList[MUSIC] / (MAXVOLUME - 1));
        }
        else if (optionsIndex == SFX) {
            SetGameSfxVolume((float)volumeList[SFX] / (MAXVOLUME - 1));
            PlaySound(s_menu);
        }
    }
    else if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && optionsIndex != OPTIONS_MENU_COUNT - 1) {
        volumeList[optionsIndex] = (volumeList[optionsIndex] - 1 + MAXVOLUME) % MAXVOLUME;
        if (optionsIndex == MUSIC) {
            SetGameMusicVolume((float)volumeList[MUSIC] / (MAXVOLUME - 1));
        }
        else if (optionsIndex == SFX) {
            SetGameSfxVolume((float)volumeList[SFX] / (MAXVOLUME - 1));
            PlaySound(s_menu);
        }
    }
    else if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) && optionsIndex == OPTIONS_MENU_COUNT - 1) {
        InitTitle();
        PlaySound(s_cancel);
    }
    else if (IsKeyPressed(KEY_ESCAPE)) {
        InitTitle();
        PlaySound(s_cancel);
    }


    const char* menu[] = {
      TextFormat("Music volume: %d", volumeList[MUSIC]),
      TextFormat("SFX volume: %d", volumeList[SFX]),
      "Back",
    };

    int fontBigH = (int)(screenHeight / 7 * textSizeRatio);
    int fontSmallH = (int)(screenHeight / 12 * textSizeRatio);
    int totalH = (fontBigH + fontSmallH * (OPTIONS_MENU_COUNT - 2));
    int originH = screenHeight / 2 - totalH / 2;
    int midW = screenWidth / 2;

    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(palette[0]);


    for (int i = 0; i < OPTIONS_MENU_COUNT; i++) {
        int fontH = (i == optionsIndex) ? fontBigH : fontSmallH; // if condition ? value_1 else value_2
        int fontW = MeasureText(menu[i], fontH);
        Color col = (i == optionsIndex) ? palette[3] : palette[2];

        DrawText(menu[i], midW - fontW / 2, originH - fontH / 2, fontH, col);
        originH += (fontBigH + fontSmallH) / 2;
    }


    EndDrawing();
    //----------------------------------------------------------------------------------
}