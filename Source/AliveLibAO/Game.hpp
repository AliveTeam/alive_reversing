#pragma once

#include "../relive_lib/DynamicArray.hpp"

class BaseGameObject;
class AnimationBase;

extern u32 sGnFrame; // NOTE: Points to AE copy till moved to common lib

extern bool gDDCheatOn;
extern bool gBreakGameLoop; // AE var
extern u16 gAttract; // AE var

namespace AO {



extern s16 gRestartRuptureFarmsKilledMuds;
extern s16 gRestartRuptureFarmsSavedMuds;


void Init_GameStates();

void Game_Main();

} // namespace AO
