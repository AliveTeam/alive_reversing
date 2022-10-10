#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/Primitives.hpp"
#include "PathData.hpp"

void Game_Main_4949F0();
void DestroyObjects_4A1F20();
void Init_GameStates_43BF40();
s32 Game_End_Frame_4950F0(u32 flags);

extern u32 sGnFrame;
extern bool sCommandLine_DDCheatEnabled_5CA4B5;
extern bool sCommandLine_ShowFps_5CA4D0;
extern s8 sCdRomDrives_5CA488[30];
extern u16 gAttract_5C1BA0;
extern s16 bSkipGameObjectUpdates_5C2FA0;
extern s16 sNum_CamSwappers_5C1B66;
extern s32 dword_5CA4D4;
extern s32 k1_dword_55EF90;
extern s16 sBreakGameLoop_5C2FE0;
extern DynamicArrayT<BaseGameObject>* ObjList_5C1B78;

extern bool gDebugHelpersEnabled;
