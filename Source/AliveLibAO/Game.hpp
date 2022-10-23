#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/DynamicArray.hpp"


class BaseGameObject;
class AnimationBase;

extern u32 sGnFrame; // NOTE: Points to AE copy till moved to common lib

namespace AO {


extern s16 gAttract_507698;
extern DynamicArrayT<BaseGameObject>* gLoadingFiles;
extern s16 sBreakGameLoop;
extern s8 gDDCheatOn;

extern s16 gRestartRuptureFarmsKilledMuds_5076C4;
extern s16 gRestartRuptureFarmsSavedMuds_5076C8;

extern s16 gOldKilledMuds_5076D0;
extern s16 gOldSavedMuds_5076D4;

extern s16 gbKillUnsavedMudsDone_5076CC;


extern DynamicArrayT<::BaseGameObject>* gPlatformsArray;

void Init_GameStates();

void Game_Main(const char_type* pCommandLine);

} // namespace AO
