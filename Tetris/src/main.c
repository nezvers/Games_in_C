/*******************************************************************************************
*
*   
*
********************************************************************************************/

#include "raylib.h"
#include "game.h"


//#define PLATFORM_WEB

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


//----------------------------------------------------------------------------------
int main(){
    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);
    
	while (!WindowShouldClose()){
        GameLoop();
    }
#endif

    CloseWindow();
    return 0;
}






