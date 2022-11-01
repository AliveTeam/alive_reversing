#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/DynamicArray.hpp"


class BaseGameObject;
class AnimationBase;

extern u32 sGnFrame; // NOTE: Points to AE copy till moved to common lib

extern DynamicArrayT<BaseGameObject>* gPlatformsArray;

namespace AO {


extern s16 gAttract;
extern s16 sBreakGameLoop;
extern s8 gDDCheatOn;

extern s16 gRestartRuptureFarmsKilledMuds_5076C4;
extern s16 gRestartRuptureFarmsSavedMuds_5076C8;

extern s16 gbKillUnsavedMudsDone_5076CC;



void Init_GameStates();

void Game_Main(const char_type* pCommandLine);

} // namespace AO
