#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"

void Game_ForceLink();

START_NS_AO

class BaseGameObject;
class AnimationBase;

ALIVE_VAR_EXTERN(unsigned int, gnFrameCount_507670);
ALIVE_VAR_EXTERN(short, gAttract_507698);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjList_drawables_504618);
ALIVE_VAR_EXTERN(DynamicArrayT<AnimationBase>*, gObjList_animations_505564);
ALIVE_VAR_EXTERN(short, sBreakGameLoop_507B78);

ALIVE_VAR_EXTERN(short, gRestartRuptureFarmsKilledMuds_5076C4);
ALIVE_VAR_EXTERN(short, gRestartRuptureFarmsSavedMuds_5076C8);

ALIVE_VAR_EXTERN(short, gOldKilledMuds_5076D0);
ALIVE_VAR_EXTERN(short, gOldSavedMuds_5076D4);

ALIVE_VAR_EXTERN(short, gbKillUnsavedMudsDone_5076CC);


ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, ObjListPlatforms_50766C);

EXPORT void SYS_EventsPump_44FF90();

EXPORT void CC Init_GameStates_41CEC0();

END_NS_AO
