#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "DynamicArray.hpp"

void Game_ForceLink();

class BaseGameObject;

namespace AO {

class AnimationBase;

ALIVE_VAR_EXTERN(u32, sGnFrame);
ALIVE_VAR_EXTERN(s16, gAttract_507698);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjListDrawables);
ALIVE_VAR_EXTERN(DynamicArrayT<AnimationBase>*, gAnimations);
ALIVE_VAR_EXTERN(s16, sBreakGameLoop_507B78);
ALIVE_VAR_EXTERN(s8, gDDCheatMode_508BF8);

ALIVE_VAR_EXTERN(s16, gRestartRuptureFarmsKilledMuds_5076C4);
ALIVE_VAR_EXTERN(s16, gRestartRuptureFarmsSavedMuds_5076C8);

ALIVE_VAR_EXTERN(s16, gOldKilledMuds_5076D0);
ALIVE_VAR_EXTERN(s16, gOldSavedMuds_5076D4);

ALIVE_VAR_EXTERN(s16, gbKillUnsavedMudsDone_5076CC);


ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, ObjListPlatforms_50766C);

void Init_GameStates();

void Game_Main();

} // namespace AO
