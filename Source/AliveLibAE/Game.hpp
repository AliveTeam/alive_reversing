#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/Primitives.hpp"
#include "PathData.hpp"

void Game_Main();
void DestroyObjects_4A1F20();
void Init_GameStates();
s32 Game_End_Frame_4950F0(u32 flags);

extern u32 sGnFrame;
extern bool gDDCheatOn;
extern u16 gAttract;
extern s16 bSkipGameObjectUpdates;
extern s16 gNumCamSwappers;
extern s16 sBreakGameLoop;
extern DynamicArrayT<BaseGameObject>* gPlatformsArray;
