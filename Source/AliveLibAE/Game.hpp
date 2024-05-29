#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

void Game_Main();
void DestroyObjects();
void Init_GameStates();
s32 Game_End_Frame(u32 flags);

extern u32 sGnFrame;
extern bool gDDCheatOn;
extern u16 gAttract;
extern bool gSkipGameObjectUpdates;
extern s16 gNumCamSwappers;
extern bool gBreakGameLoop;
