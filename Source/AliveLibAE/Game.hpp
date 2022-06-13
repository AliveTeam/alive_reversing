#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Animation.hpp"
#include "Primitives.hpp"
#include "PathData.hpp"

void Game_Main_4949F0();
void DestroyObjects_4A1F20();
void Init_GameStates_43BF40();
s32 Game_End_Frame_4950F0(u32 flags);

ALIVE_VAR_EXTERN(u32, sGnFrame);
ALIVE_VAR_EXTERN(bool, sCommandLine_DDCheatEnabled_5CA4B5);
ALIVE_VAR_EXTERN(bool, sCommandLine_ShowFps_5CA4D0);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjListDrawables);
ALIVE_VAR_EXTERN(DynamicArrayT<AnimationBase>*, gAnimations);
ALIVE_ARY_EXTERN(s8, 30, sCdRomDrives_5CA488);
ALIVE_VAR_EXTERN(u16, gAttract_5C1BA0);
ALIVE_VAR_EXTERN(s16, bSkipGameObjectUpdates_5C2FA0);
ALIVE_VAR_EXTERN(s16, sNum_CamSwappers_5C1B66);
ALIVE_VAR_EXTERN(s32, dword_5CA4D4);
ALIVE_VAR_EXTERN(s32, k1_dword_55EF90);
ALIVE_VAR_EXTERN(s16, sBreakGameLoop_5C2FE0);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, ObjList_5C1B78);
class ShadowZone;
ALIVE_VAR_EXTERN(DynamicArrayT<ShadowZone>*, sShadowZone_dArray_5C1B80);

extern bool gDebugHelpersEnabled;
