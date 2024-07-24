/*******************************************************************************************


********************************************************************************************/

#include "raylib.h"





#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define LERP(a, b, c) (a + (b - a) * c)


void GameInit();
void GameUpdate();
void GameDraw();
void GameLoop(){GameUpdate(); GameDraw();}
void Reset();



void UpdateScreen();



int screenWidth = 320 * 3;
int screenHeight = 180*3;
const int gameWidth = 320;
const int gameHeight = 180;
RenderTexture viewport;
int scale = 1;
Vector2 vpOffset = {0.0f, 0.0f};


/**********************/
float delta;
int score1;
int score2;
float ballW = 4;
float ballSpd = 1.0f * 60;
Vector2 ball;
Vector2 dir = {1.0f, 0.0f};
Vector2 center;
float padX1;
float padX2;
float padY1;
float padY2;
float padW = 8;
float padH = 32;
float margin = 4.0f;

void UpdatePads();
void UpdateBall();

void DrawBall();
void DrawPaddles();
void DrawLevel();
void DrawScore();


int main(void){
    GameInit();
    
    SetTargetFPS(60);
    while (!WindowShouldClose()){
        GameLoop();
    }
    UnloadRenderTexture(viewport);
    CloseWindow();
    return 0;
}

void GameInit() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "classic game: platformer");
    viewport = LoadRenderTexture(gameWidth, gameHeight);
    UpdateScreen();
    
    Reset();
}

void Reset(){
    score1 = 0;
    score2 = 0;
    center = (Vector2){gameWidth / 2, gameHeight / 2};
    ball = center;
    dir = (Vector2){1.0f, 1.0f};
    padX1 = margin + padW + ballW;
    padX2 = gameWidth - margin - padW - ballW;
    padY1 = padY2 = gameHeight / 2 - padH / 2;
}

void GameUpdate(){
    if (IsWindowResized()){
        UpdateScreen();
    }
    delta = GetFrameTime();
    UpdatePads();
    UpdateBall();
}

void UpdateScreen(){
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    scale = MAX(1, MIN((screenWidth/gameWidth), (screenHeight/gameHeight)));
    vpOffset.x = (screenWidth - (gameWidth * scale)) / 2;
    vpOffset.y = (screenHeight - (gameHeight * scale)) / 2;
}

void MovePad(float *x, float *y, float *spd, int dir){
    float maxSpd = 2.0f * 60;
    float acc = 0.05f * 60;
    
    if (dir != 0){
        *spd = LERP(*spd, maxSpd * dir, acc * delta);
    }
    else{
        *spd = LERP(*spd, 0.0f, acc * delta);
    }
    *y += *spd * delta;
    
    if (*y < margin){
        *y = margin;
        *spd = 0.0f;
    }
    if (*y > gameHeight - margin - padH){
        *y = gameHeight - margin - padH;
        *spd = 0.0f;
    }
}

void UpdatePads(){
    static float padS1;
    static float padS2;
    
    int padD1 = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
    int padD2 = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);
    
    MovePad(&padX1, &padY1, &padS1, padD1);
    MovePad(&padX2, &padY2, &padS2, padD2);
}

void UpdateBall(){
    static Vector2 vel;
    vel.x = dir.x * ballSpd * delta;
    vel.y = dir.y * ballSpd * delta;
    
    float y0 = ball.y + ballW;
    float y1 = ball.y - ballW;
    // Left paddle
    if (vel.x < 0.0f){
        if (y0 > padY1 && y1 < padY1 + padH){
            if (ball.x - ballW > padX1 && ball.x - ballW + vel.x < padX1){
                ball.x = padX1 + ballW;
                vel.x = 0.0f;
                dir.x *= -1;
            }
        }
    }
    // Right paddle
    else if (vel.x > 0.0f){
        if (y0 > padY2 && y1 < padY2 + padH){
            if (ball.x + ballW < padX2 && ball.x + ballW + vel.x > padX2){
                ball.x = padX2 - ballW;
                vel.x = 0.0f;
                dir.x *= -1;
            }
        }
    }
    
    ball.x += vel.x;
    ball.y += vel.y;
    
    // Top & bottom bounce
    if (ball.y > gameHeight - margin - ballW){
        ball.y = gameHeight - margin - ballW;
        dir.y *= -1;
    }
    else if (ball.y < margin + ballW){
        ball.y = margin + ballW;
        dir.y *= -1;
    }
    
    //Scoring
    if (ball.x < 0.0f){
        ball = center;
        score1 += 1;
    }
    else if (ball.x > gameWidth){
        ball = center;
        score2 += 1;
    }
}

void GameDraw(){
    // Viewport scaling
    const Vector2 origin = (Vector2){0.0f, 0.0f};
    const Rectangle vp_r = (Rectangle){0.0f,gameHeight,gameWidth, -gameHeight}; // flip vertically: position = left-bottom
    Rectangle out_r = (Rectangle){vpOffset.x, vpOffset.y, gameWidth * scale, gameHeight * scale};
    
    // Render game's viewport
    BeginTextureMode(viewport);
        DrawRectangle(0, 0, gameWidth, gameHeight, DARKGRAY); // Background
        DrawLevel();
        DrawScore();
        DrawPaddles();
        DrawBall();
    EndTextureMode();
    
    // Draw the viewport
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(viewport.texture, vp_r, out_r, origin, 0.0f, WHITE);
    EndDrawing();
}

void DrawLevel(){
    DrawLine(0, margin, gameWidth, margin, GRAY);
    DrawLine(0, gameHeight - margin, gameWidth, gameHeight - margin, GRAY);
    //DrawLine(0, gameHeight /2, gameWidth, gameHeight / 2, GRAY);
}

void DrawScore(){
    const int fontSize = 32;
    const Color col = GRAY;
    static const char *s0 = ":";
    const char *s1 = TextFormat("%d", score1);
    const char *s2 = TextFormat("%d", score2);
    
    int x = gameWidth / 2;
    int y = gameHeight / 2  - fontSize / 2;
    DrawText(s0, x - MeasureText(s0, fontSize) / 2, y, fontSize, col);
    DrawText(s1, x - MeasureText(s1, fontSize) - 8, y, fontSize, col);
    DrawText(s2, x + 8, y, fontSize, col);
}

void DrawPaddles(){
    DrawRectangle (padX1 - padW, padY1, padW, padH, BLUE);
    DrawRectangle (padX2, padY2, padW, padH, RED);
}

void DrawBall(){
    DrawRectangle (ball.x - ballW, ball.y - ballW, ballW * 2, ballW * 2, WHITE);
}