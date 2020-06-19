#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Primitives.hpp"
#include "PathData.hpp"

void Game_ForceLink();

EXPORT void CC static_init_set_default_hero_43EC10();

namespace Test
{
    void GameTests();
}

EXPORT void CC Game_Main_4949F0();
EXPORT void CC SYS_EventsPump_494580();
EXPORT void DestroyObjects_4A1F20();
EXPORT void CC Init_GameStates_43BF40();
EXPORT FP CC ScaleToGridSize_4498B0(FP scale);

ALIVE_VAR_EXTERN(unsigned int, sGnFrame_5C1B84);
ALIVE_VAR_EXTERN(bool, sCommandLine_DDCheatEnabled_5CA4B5);
ALIVE_VAR_EXTERN(bool, sCommandLine_ShowFps_5CA4D0);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjList_drawables_5C1124);
ALIVE_VAR_EXTERN(DynamicArrayT<AnimationBase>*, gObjList_animations_5C1A24);
ALIVE_ARY_EXTERN(char, 30, sCdRomDrives_5CA488);
ALIVE_VAR_EXTERN(WORD, gAttract_5C1BA0);
ALIVE_VAR_EXTERN(short, bSkipGameObjectUpdates_5C2FA0);
ALIVE_VAR_EXTERN(short, sNum_CamSwappers_5C1B66);
ALIVE_VAR_EXTERN(int, dword_5CA4D4);
ALIVE_VAR_EXTERN(int, k1_dword_55EF90);
ALIVE_VAR_EXTERN(short, sBreakGameLoop_5C2FE0);
ALIVE_VAR_EXTERN(class Abe*, spAbe_554D5C);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, ObjList_5C1B78);
class ShadowZone;
ALIVE_VAR_EXTERN(DynamicArrayT<ShadowZone>*, sShadowZone_dArray_5C1B80);

extern bool gDebugHelpersEnabled;