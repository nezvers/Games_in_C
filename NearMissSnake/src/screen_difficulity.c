#include "raylib.h"
#include "game.h"
#include "screens.h"


int diffMenuIndex = 0;

void InitDifficulity() {
    CurrentUpdateCallback = DifficulityScreen;
    SetExitKey(0);
    diffMenuIndex = DIFFICULITY_MENU_COUNT / 2 -1;

    // Don't do palette transition on first time entering a level
    prevPalette[0] = palette[0];
    prevPalette[1] = palette[1];
    prevPalette[2] = palette[2];
    prevPalette[3] = palette[3];
}

void DifficulityScreen(void) {
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
        diffMenuIndex = (diffMenuIndex + 1) % DIFFICULITY_MENU_COUNT;
        PlaySound(s_menu);
    }
    else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
        diffMenuIndex = (diffMenuIndex - 1 + DIFFICULITY_MENU_COUNT) % DIFFICULITY_MENU_COUNT;
        PlaySound(s_menu);
    }
    else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        InitLevelStart();
        PlaySound(s_accept);
    }
    else if (IsKeyPressed(KEY_ESCAPE)) {
        InitTitle();
        PlaySound(s_cancel);
    }

    const char* text = "Game speed: ";
    const char* diffText = TextFormat("%d", diffMenuIndex + 1);
    int midW = screenWidth / 2;
    int midH = screenHeight / 3;
    int fontH = (int)(screenHeight / 5 * textSizeRatio);
    int fontW = MeasureText(text, fontH);

    const char* hintText = "Use WASD or direction keys for navigation.\nTo eat an apple, press apple against a solid surface.";
    int hintH = (int)(screenHeight / 17 * textSizeRatio);;
    int hintW = MeasureText(hintText, hintH);

    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(palette[0]);
    DrawText(text, midW - fontW / 2, midH - (fontH / 2), fontH, palette[2]);
    DrawText(diffText, midW + fontW / 2, midH - (fontH / 2), fontH, palette[3]);
    DrawText(hintText, midW - hintW / 2, screenHeight - (screenHeight / 4) - (fontH / 2), hintH, palette[2]);

    EndDrawing();
    //----------------------------------------------------------------------------------
}