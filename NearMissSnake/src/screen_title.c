#include "raylib.h"
#include "game.h"
#include "screens.h"

#define TITLE_ANIM_INTERVAL 70
int titleAnimationTimer;
int blinkInterval = 60;
int blinkTimer = 0;

int mainMenuIndex = 0;
#define START 	0
#define OPTIONS 1
#define QUIT 	2


int QuadraticEasingOut(int start, int end, float position) {
    return (-(end - start) * position * (position - 2) + start);
}

void InitTitle() {
    CurrentUpdateCallback = TitleScreen;
    SetExitKey(KEY_ESCAPE);
    SetPalette();

    titleAnimationTimer = 0;
    blinkTimer = 0;
    mainMenuIndex = 0;
}

void TitleScreen(void) {
    int mainMenuCount = 3;

    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        mainMenuIndex = (mainMenuIndex - 1 + mainMenuCount) % mainMenuCount;
        PlaySound(s_menu);
    }
    else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
        mainMenuIndex = (mainMenuIndex + 1) % mainMenuCount;
        PlaySound(s_menu);
    }
    else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (titleAnimationTimer == TITLE_ANIM_INTERVAL) {
            if (mainMenuIndex == START) {
                InitDifficulity();
                PlaySound(s_accept);
            }
            else if (mainMenuIndex == OPTIONS) {
                InitOptions();
                PlaySound(s_accept);
            }
            else if (mainMenuIndex == QUIT) {
                quit = true;
            }
        }
        else {
            titleAnimationTimer = TITLE_ANIM_INTERVAL;
        }
    }


    if (titleAnimationTimer < TITLE_ANIM_INTERVAL) { titleAnimationTimer++; }
    else {
        blinkTimer = (blinkTimer + 1) % blinkInterval;
    }

    // TITLE
    const char* title = "Near Miss Snake";
    int titleFontH = (int)(screenHeight / 5 * textSizeRatio);
    int titleFontW = MeasureText(title, titleFontH) / 2;
    int midW = screenWidth / 2;
    int titleH = QuadraticEasingOut(screenHeight + titleFontH / 2, screenHeight / 3, (float)titleAnimationTimer / TITLE_ANIM_INTERVAL);

    // MENU
    const char* menu[] = {
      "Start",
      "Options",
      "Quit",
    };

    int fontBigH = (int)(screenHeight / 10 * textSizeRatio);
    int fontSmallH = (int)(screenHeight / 12 * textSizeRatio);
    int totalH = (fontBigH + fontSmallH * (mainMenuCount - 2));
    int originH = screenHeight - screenHeight / 3 - totalH / 2;

    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(palette[0]);

    if (blinkTimer < blinkInterval / 2) {
        DrawText(title, midW - titleFontW, titleH - (titleFontH / 2), titleFontH, palette[2]);
    }
    else {
        DrawText(title, midW - titleFontW, titleH - (titleFontH / 2), titleFontH, palette[1]);
    }
    if (titleAnimationTimer == TITLE_ANIM_INTERVAL) {
        for (int i = 0; i < mainMenuCount; i++) {
            int fontH = (i == mainMenuIndex) ? fontBigH : fontSmallH; // if condition ? value_1 else value_2
            int fontW = MeasureText(menu[i], fontH);
            Color col = (i == mainMenuIndex) ? palette[3] : palette[2];

            DrawText(menu[i], midW - fontW / 2, originH - fontH / 2, fontH, col);
            originH += (fontBigH + fontSmallH) / 2;
        }
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
}