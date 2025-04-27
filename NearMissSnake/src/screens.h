#ifndef SCREENS_H
#define SCREENS_H

void (*CurrentUpdateCallback)(void);
void InitTitle(void);
void TitleScreen(void);

void InitOptions(void);
void OptionsScreen(void);

void InitDifficulity(void);
void DifficulityScreen(void);

void InitLevelStart(void);
void LevelStartScreen(void);

void InitGameplay(void);
void GameplayScreen(void);


#endif // SCREENS_H