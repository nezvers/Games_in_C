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

//Sounds & music are converted using bi2header - https://github.com/AntumDeluge/bin2header
// SFX from sfxcellar.accusonus.com
#include "SFX_Cellar_UI_Button_14.wav.h"
#include "SFX_Cellar_UI_Button_41.wav.h"
#include "SFX_Cellar_UI_Button_43.wav.h"
#include "SFX_Cellar_UI_Button_67.wav.h"
// music - https://modarchive.org/index.php?request=view_by_moduleid&query=172898
#include "MUSIC_4_rndd.xm.h"


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
#define TSIZE 1280/20
#define WIDTH 20
#define HEIGHT 10
int tileSize = TSIZE;
int screenWidth = WIDTH * TSIZE;
int screenHeight = HEIGHT * TSIZE;
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



#define MAXVOLUME 11
#define MUSIC 0
#define SFX 1
int volume[] = { MAXVOLUME - 1,MAXVOLUME - 1 };

int optionsmenuCount = 3;
int optionsIndex;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

void (*Screen)(void);
void InitTitle(void);
void TitleScreen(void);
void InitOptions(void);
void OptionsScreen(void);
void InitDifficulity(void);
void DifficulityScreen(void);
void InitLevel(void);
void LevelScreen(void);
void Start(void);
void GameScreen(void);

int quadratic_easing_out(int start, int end, float position);

Color ColorBlend(Color a, Color b, float percent);


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------


void Init(void){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Near Miss Snake");
	
	// Set up audio
	InitAudioDevice();
	SetMasterVolume(100.0);
	
	w_accept = LoadWaveFromMemory(".wav", (const unsigned char *)&SFX_Cellar_UI_Button_43_wav, sizeof SFX_Cellar_UI_Button_43_wav);
	w_cancel = LoadWaveFromMemory(".wav", (const unsigned char *)&SFX_Cellar_UI_Button_14_wav, sizeof SFX_Cellar_UI_Button_14_wav);
	w_menu = LoadWaveFromMemory(".wav", (const unsigned char *)&SFX_Cellar_UI_Button_41_wav, sizeof SFX_Cellar_UI_Button_41_wav);
	w_apple = LoadWaveFromMemory(".wav", (const unsigned char *)&SFX_Cellar_UI_Button_67_wav, sizeof SFX_Cellar_UI_Button_67_wav);
	
	s_accept = LoadSoundFromWave(w_accept);
	s_cancel = LoadSoundFromWave(w_cancel);
	s_menu = LoadSoundFromWave(w_menu);
	s_apple = LoadSoundFromWave(w_apple);
	music = LoadMusicStreamFromMemory(".xm", (unsigned char *)&rndd_xm, sizeof rndd_xm);
	SetMusicVolume(music, 0.1);
	
	PlayMusicStream(music);
	InitTitle();
}

// Main game loop
void GameLoop(void) {
	while (!WindowShouldClose() && !quit) {    // Detect window close button or ESC key
		UpdateFrame();
	}
}

void Cleanup(void)
{
	CloseAudioDevice();
	CloseWindow();
}

void UpdateFrame(void){
	if (IsWindowResized())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
			
			// Original text size made for 2/1 aspect ratio
			float aspectRatio = (float)screenWidth / screenHeight;
			if (aspectRatio < 2.0){
				textSizeRatio = aspectRatio/ 2.0;
			}
			tileSize = screenWidth/2 < screenHeight ? screenWidth/20 : screenHeight/10;
			// Offset to keep game centered
			originX = (screenWidth - 20*tileSize) /2;
			originY = (screenHeight - 10*tileSize) /2;
        }
	UpdateMusicStream(music);
    Screen();
}

// palettes created using colorhunt.co
Color palette[] = { { 255, 255, 255, 255 }, { 200, 200, 200, 255 }, { 130, 130, 130, 255 }, { 0, 0, 0, 255 } };
Color prevPalette[4];
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
int paletteCount = 5;
void SetPalette(void){
	int index = GetRandomValue(0, paletteCount-1) * 4;
	for(int i=0; i < 4; i++){
		prevPalette[i] = palette[i];
		palette[i] = paletteList[index+i];
	}
}


int titleInTime = 70;
int titleCurrentTime;
int blinkInterval = 60;
int blinkTime;
int mainMenuIndex = 0;
#define START 	0
#define OPTIONS 1
#define QUIT 	2

void InitTitle(){
	Screen = TitleScreen;
	SetExitKey(KEY_ESCAPE);
	SetPalette();
	
	titleCurrentTime = 0;
	blinkTime = 0;
	
	mainMenuIndex = 0;
}

void TitleScreen(void){
	int mainMenuCount = 3;
	
	if(IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)){
		mainMenuIndex = (mainMenuIndex -1 +mainMenuCount) % mainMenuCount;
		PlaySound(s_menu);
	}
	else if(IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)){
		mainMenuIndex = (mainMenuIndex +1) % mainMenuCount;
		PlaySound(s_menu);
	}
	else if (IsKeyPressed(KEY_ENTER)){
		if (titleCurrentTime == titleInTime){
			if (mainMenuIndex == START){
				InitDifficulity();
				PlaySound(s_accept);
			}
			else if (mainMenuIndex == OPTIONS){
				InitOptions();
				PlaySound(s_accept);
			}
			else if (mainMenuIndex == QUIT){
				quit = true;
			}
		}
		else{
			titleCurrentTime = titleInTime;
		}
	}
	
	
	if (titleCurrentTime < titleInTime){titleCurrentTime++;}
	else{
		blinkTime = (blinkTime +1) % blinkInterval;
	}
	
	// TITLE
	const char *title = "Near Miss Snake";
	int titleFontH = (int)(screenHeight /5 *textSizeRatio);
	int titleFontW = MeasureText(title, titleFontH) /2;
	int midW = screenWidth /2;
	int titleH = quadratic_easing_out(screenHeight + titleFontH/2, screenHeight /3, (float)titleCurrentTime/titleInTime);
	
	// MENU
	const char * menu[] = {
		"Start",
		"Options",
		"Quit",
	};
	
	int fontBigH = (int)(screenHeight /10 *textSizeRatio);
	int fontSmallH = (int)(screenHeight /12 *textSizeRatio);
	int totalH = (fontBigH + fontSmallH *(mainMenuCount-2));
	int originH = screenHeight - screenHeight/3 - totalH/2;
	
	//----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(palette[0]);
        DrawText(title, midW -titleFontW, titleH -(titleFontH /2), titleFontH, palette[2]);
		if (titleCurrentTime == titleInTime){
			for(int i = 0; i < mainMenuCount; i++){
				int fontH = (i == mainMenuIndex) ? fontBigH : fontSmallH; // if condition ? value_1 else value_2
				int fontW = MeasureText(menu[i], fontH);
				Color col = (i == mainMenuIndex) ? palette[3] : palette[2];
				
				DrawText(menu[i], midW -fontW /2, originH -fontH /2, fontH, col);
				originH += (fontBigH + fontSmallH) /2;
			}
		}

    EndDrawing();
    //----------------------------------------------------------------------------------
	
}



void InitOptions(void){
	Screen = OptionsScreen;
	SetExitKey(0);
	optionsIndex = optionsmenuCount-1;
}

void OptionsScreen(void){
	if(IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)){
		optionsIndex = (optionsIndex -1 +optionsmenuCount) % optionsmenuCount;
		PlaySound(s_menu);
	}
	else if(IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)){
		optionsIndex = (optionsIndex +1) % optionsmenuCount;
		PlaySound(s_menu);
	}
	else if((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && optionsIndex != optionsmenuCount-1){
		volume[optionsIndex] = (volume[optionsIndex] +1) % MAXVOLUME;
		if (optionsIndex == MUSIC){SetMusicVolume(music, 0.1 * (double)volume[MUSIC]/(MAXVOLUME -1));}
		else if (optionsIndex == SFX){
			float vol = (float)volume[SFX]/(MAXVOLUME -1);
			SetSoundVolume(s_accept, vol);
			SetSoundVolume(s_cancel, vol);
			SetSoundVolume(s_menu, vol);
			SetSoundVolume(s_apple, vol);
			PlaySound(s_menu);
		}
	}
	else if((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && optionsIndex != optionsmenuCount-1){
		volume[optionsIndex] = (volume[optionsIndex] -1 +MAXVOLUME) % MAXVOLUME;
		if (optionsIndex == MUSIC){SetMusicVolume(music, 0.1 * (double)volume[MUSIC]/(MAXVOLUME -1));}
		else if (optionsIndex == SFX){
			float vol = (float)volume[SFX]/(MAXVOLUME -1);
			SetSoundVolume(s_accept, vol);
			SetSoundVolume(s_cancel, vol);
			SetSoundVolume(s_menu, vol);
			SetSoundVolume(s_apple, vol);
			PlaySound(s_menu);
		}
	}
	else if(IsKeyPressed(KEY_ENTER)  && optionsIndex == optionsmenuCount-1){
		InitTitle();
		PlaySound(s_cancel);
	}
	else if(IsKeyPressed(KEY_ESCAPE)){
		InitTitle();
		PlaySound(s_cancel);
	}
	
	
	const char * menu[] = {
		TextFormat("Music volume: %d", volume[MUSIC]),
		TextFormat("SFX volume: %d", volume[SFX]),
		"Back",
	};
	
	int fontBigH = (int)(screenHeight /7 *textSizeRatio);
	int fontSmallH = (int)(screenHeight /12 *textSizeRatio);
	int totalH = (fontBigH + fontSmallH *(optionsmenuCount-2));
	int originH = screenHeight/2 - totalH/2;
	int midW = screenWidth/2;
	
	//----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(palette[0]);
		
		
		for(int i = 0; i < optionsmenuCount; i++){
			int fontH = (i == optionsIndex) ? fontBigH : fontSmallH; // if condition ? value_1 else value_2
			int fontW = MeasureText(menu[i], fontH);
			Color col = (i == optionsIndex) ? palette[3] : palette[2];
			
			DrawText(menu[i], midW -fontW /2, originH -fontH /2, fontH, col);
			originH += (fontBigH + fontSmallH) /2;
		}
		

    EndDrawing();
    //----------------------------------------------------------------------------------
}


int difMenuCount = 10;
int diffMenuIndex = 0;

void InitDifficulity(){
	Screen = DifficulityScreen;
	SetExitKey(0);
	diffMenuIndex = 0;
}

void DifficulityScreen(void){
	if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)){
		diffMenuIndex = (diffMenuIndex +1) % difMenuCount;
		PlaySound(s_menu);
	}
	else if(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)){
		diffMenuIndex = (diffMenuIndex -1 +difMenuCount) % difMenuCount;
		PlaySound(s_menu);
	}
	else if(IsKeyPressed(KEY_ENTER)){
		InitLevel();
		PlaySound(s_accept);
	}
	else if(IsKeyPressed(KEY_ESCAPE)){
		InitTitle();
		PlaySound(s_cancel);
	}
	
	const char *text = "Game speed: ";
	const char *diffText = TextFormat("%d", diffMenuIndex+1);
	int midW = screenWidth /2;
	int midH = screenHeight /3;
	int fontH = (int)(screenHeight /5 *textSizeRatio);
	int fontW = MeasureText(text, fontH);
	
	const char *hintText = "Use WASD or direction keys for navigation\nTo feed the snake, press apple against a solid surface";
	int hintH = (int)(screenHeight /17 *textSizeRatio);;
	int hintW = MeasureText(hintText, hintH);
	
	//----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(palette[0]);
        DrawText(text, midW -fontW/2, midH -(fontH /2), fontH, palette[2]);
        DrawText(diffText, midW +fontW/2, midH -(fontH /2), fontH, palette[3]);
        DrawText(hintText, midW -hintW/2, screenHeight - (screenHeight/4) -(fontH /2), hintH, palette[2]);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

#define FREE 0
#define APPLE 1
#define SNAKE 2
#define SOLID 3
typedef struct{
	int x;
	int y;
}vec2;

int level[WIDTH*HEIGHT];

// Solid block layout for a level
int lvl_1[WIDTH*HEIGHT] = {
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
int freeTiles[WIDTH*HEIGHT];
int freeCount;



int length = 3;
int snake[WIDTH*HEIGHT];
int revealSegment = -1;
vec2 dir = {1,0};
vec2 nextDir = {1,0};
int idleTime;
int time;
void SetLevel(int *lvl);
void GetFreeTiles(void);
void SpawnApple(void);
bool InputUpdate(void);
void DrawLevel(Color *pal);

void InitLevel(void){
	Screen = LevelScreen;
	SetLevel(lvl_1);
	idleTime = 6 + (difMenuCount -1 -diffMenuIndex) * 5;
	time = -1;
	length = 3;
	revealSegment = -1;
	dir = (vec2){1,0};
	nextDir = (vec2){1,0};

	// Place snake in level
	snake[0] = WIDTH*4 + 5;
	snake[1] = WIDTH*4 + 4;
	snake[2] = WIDTH*4 + 3;
	
	// Get free tile count
	freeCount = 0;
	for (int i=0; i<WIDTH*HEIGHT; i++){
		if (level[i] == FREE){
			freeCount++;
		}
	}
	
	GetFreeTiles();	
	SpawnApple();
}

void LevelScreen(void){
	if(IsKeyPressed(KEY_ESCAPE)){
		InitDifficulity();
		PlaySound(s_cancel);
	}
	time++;
	if (time % idleTime == 0){
		revealSegment++;
		if (revealSegment > 0){
			level[snake[length -1 -revealSegment+1]] = SNAKE;
			PlaySound(s_apple);
		}
		if (revealSegment == length){
			// Start moving
			Start();
			prevPalette[0] = palette[0];
			prevPalette[1] = palette[1];
			prevPalette[2] = palette[2];
			prevPalette[3] = palette[3];
		}
	}
	if (revealSegment > -1 && revealSegment < length){
		if (time < idleTime/2){
			level[snake[length -1 -revealSegment]] = SNAKE;
		}else{
			level[snake[length -1 -revealSegment]] = FREE;
		}
	}
	
	InputUpdate();
	
	// Color interpolation from prevPalette to palette
	float percent = (float)time / (idleTime * length);
	Color changedColor[] = {
		ColorBlend(prevPalette[0], palette[0], percent),
		ColorBlend(prevPalette[1], palette[1], percent),
		ColorBlend(prevPalette[2], palette[2], percent),
		ColorBlend(prevPalette[3], palette[3], percent),
		};
	
	
	//----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(palette[0]);
		DrawLevel(changedColor);
	EndDrawing();
    //----------------------------------------------------------------------------------
}

void SetLevel(int* lvl){
	for (int i=0; i < WIDTH*HEIGHT; i++){
		level[i] = lvl[i];
	}
}

void GetFreeTiles(void){
	// add free tile index to array
	int j = 0;
	for (int i=0; i<WIDTH*HEIGHT; i++){
		if (level[i] == FREE){
			freeTiles[j] = i;
			j++;
		}
	}
}

void SpawnApple(){
	int n = GetRandomValue(0, freeCount-1);
	level[freeTiles[n]] = APPLE;
}

bool isDead = false;

void Start(){
	Screen = GameScreen;
	time = 0;
	isDead = false;
}

int NextTile(int pos);
void Move(void);
void Food(int t);
void Died(void);

void GameScreen(){
	if(IsKeyPressed(KEY_ESCAPE)){
		InitDifficulity();
		PlaySound(s_cancel);
	}
	time = (time+1) % idleTime;
	if(InputUpdate()){
		time = 0;
	}
	if (time == 0){
		dir = nextDir;
		int tile = NextTile(snake[0]);
		if (tile == snake[0]){return;}
		int type = level[tile];
		switch (type){
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
	//----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(palette[0]);
		DrawLevel(palette);
	EndDrawing();
    //----------------------------------------------------------------------------------
	// do InitLevel after draw
	if(isDead){
		Died();
	}
}

void DrawLevel(Color * pal){
	for (int y=0; y < HEIGHT; y++){
		for (int x=0; x < WIDTH; x++){
			int tile = level[y*WIDTH + x];
			DrawRectangle(x*tileSize +originX, y*tileSize +originY, tileSize, tileSize, pal[tile]);
		}
	}
	
	// Black bars
	Color barCol = {0,0,20, 255};
	if (originX > 0){
		DrawRectangle(0.0f, 0.0f, originX, screenHeight, barCol);
		DrawRectangle(screenWidth -originX, 0.0f, originX, screenHeight, barCol);
	}
	if (originY > 0){
		DrawRectangle(0.0f, 0.0f, screenWidth, originY, barCol);
		DrawRectangle(0.0f, screenHeight -originY, screenWidth, originY, barCol);
	}
}

bool InputUpdate(void){
	vec2 d = (vec2){0,0};
	if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)){
		d = (vec2){1,0};
	}else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)){
		d = (vec2){-1,0};
	}else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)){
		d = (vec2){0,-1};
	}else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)){
		d = (vec2){0,1};
	}
	if ((d.x != 0 || d.y != 0) && ((d.x == 0 && d.y != -dir.y) || (d.y == 0 && d.x != -dir.x))){
		nextDir = d;
		if (d.x == dir.x && d.y == dir.y){
			return true;	// can be used to instantly do the move
		}
	}
	return false;
}


int PosWrap(vec2 pos);
vec2 int2vec(int p);
int vec2int(vec2 p);

int NextTile(int pos){
	vec2 p = int2vec(pos);
	return PosWrap((vec2){p.x+dir.x, p.y+dir.y});
}

void Move(void){
	level[snake[length-1]] = FREE;
	for(int i=length-1; i > 0; i--){
		snake[i] = snake[i-1];
	}
	vec2 head = int2vec(snake[0]);
	snake[0] = PosWrap((vec2){head.x+dir.x, head.y+dir.y});
	level[snake[0]] = SNAKE;
}

void Died(void){
	SetPalette();
	InitLevel();
}

void Push(int from, int to);
void Eat(int t);
void Food(int t){
	int tile = NextTile(t);
	int type = level[tile];
	switch (type){
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

void Push(int from, int to){
	level[from] = FREE;
	level[to] = APPLE;
	Move();
}

void Eat(int t){
	length++;
	for(int i=length-1; i > 0; i--){
		snake[i] = snake[i-1];
	}
	snake[0] = t;
	level[t] = SNAKE;
	freeCount--;
	GetFreeTiles();
	SpawnApple();
}

int PosWrap(vec2 pos){
	pos.x = (pos.x +WIDTH) % WIDTH;
	pos.y = (pos.y + HEIGHT) % HEIGHT;
	return pos.x + WIDTH * pos.y;
}


vec2 int2vec(int p){
	return (vec2){p % WIDTH, p / WIDTH};
}

int vec2int(vec2 p){
	return p.x + p.y * WIDTH;
}

int quadratic_easing_out(int start, int end, float position){
	return (-(end - start) * position * (position - 2) + start);
}


Color ColorBlend(Color a, Color b, float percent){
	return (Color){
		a.r + (int)((b.r-a.r)*percent),
		a.g + (int)((b.g-a.g)*percent),
		a.b + (int)((b.b-a.b)*percent),
		a.a + (int)((b.a-a.a)*percent),
	};
}










