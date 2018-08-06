#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Primitives.hpp"

void Game_ForceLink();

EXPORT void CC static_init_set_default_hero_43EC10();

namespace Test
{
    void GameTests();
}

EXPORT void CC Game_Main_4949F0();
EXPORT void CC SYS_EventsPump_494580();


ALIVE_VAR_EXTERN(unsigned int, sGnFrame_5C1B84);
ALIVE_VAR_EXTERN(bool, sCommandLine_DDCheatEnabled_5CA4B5);
ALIVE_VAR_EXTERN(bool, sCommandLine_ShowFps_5CA4D0);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjList_drawables_5C1124);
ALIVE_VAR_EXTERN(DynamicArrayT<Animation>*, gObjList_animations_5C1A24);
ALIVE_VAR_EXTERN(BOOL, sIOSyncReads_BD2A5C);
ALIVE_ARY_EXTERN(char, 30, sCdRomDrives_5CA488);
ALIVE_VAR_EXTERN(WORD, word_5C1BA0);
ALIVE_VAR_EXTERN(class Abe *, sActiveHero_5C1B68);
ALIVE_VAR_EXTERN(short, word_5C1B66);
ALIVE_VAR_EXTERN(class Collisions*, sCollisions_DArray_5C1128);
