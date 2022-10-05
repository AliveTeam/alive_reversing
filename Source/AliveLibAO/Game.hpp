#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "DynamicArray.hpp"

void Game_ForceLink();

namespace AO {

class BaseGameObject;
class AnimationBase;

ALIVE_VAR_EXTERN(u32, gnFrameCount_507670);
ALIVE_VAR_EXTERN(s16, gAttract_507698);
ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gObjList_drawables_504618);
ALIVE_VAR_EXTERN(DynamicArrayT<AnimationBase>*, gObjList_animations_505564);
extern DynamicArrayT<BaseGameObject>* gLoadingFiles;
ALIVE_VAR_EXTERN(s16, sBreakGameLoop_507B78);
ALIVE_VAR_EXTERN(s8, gDDCheatMode_508BF8);

ALIVE_VAR_EXTERN(s16, gRestartRuptureFarmsKilledMuds_5076C4);
ALIVE_VAR_EXTERN(s16, gRestartRuptureFarmsSavedMuds_5076C8);

ALIVE_VAR_EXTERN(s16, gOldKilledMuds_5076D0);
ALIVE_VAR_EXTERN(s16, gOldSavedMuds_5076D4);

ALIVE_VAR_EXTERN(s16, gbKillUnsavedMudsDone_5076CC);


ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, ObjListPlatforms_50766C);

EXPORT void CC Init_GameStates_41CEC0();

EXPORT void Game_Main_450050();

} // namespace AO
