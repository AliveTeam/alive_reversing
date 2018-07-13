#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Primitives.hpp"

void Game_ForceLink();

namespace Test
{
    void GameTests();
}

EXPORT void CC Game_Main_4949F0();
EXPORT void CC sub_494580();
EXPORT void CC Init_SetTPage_4F5B60(Prim_SetTPage* pPrim, int /*notUsed1*/, int /*notUsed2*/, int tpage);
EXPORT int CC PSX_getTPage_4F60E0(char tp, char abr, int x, __int16 y);
EXPORT int CC PSX_getClut_4F6350(int x, int y);

ALIVE_VAR_EXTERN(unsigned int, sGnFrame_5C1B84);
ALIVE_VAR_EXTERN(bool, sCommandLine_DDCheatEnabled_5CA4B5);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjList_drawables_5C1124);
ALIVE_VAR_EXTERN(DynamicArrayT<Animation>*, gObjList_animations_5C1A24);
ALIVE_VAR_EXTERN(BOOL, sIOSyncReads_BD2A5C);
ALIVE_ARY_EXTERN(char, 30, sCdRomDrives_5CA488);
