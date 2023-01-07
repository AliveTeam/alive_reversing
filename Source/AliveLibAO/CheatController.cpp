#include "stdafx_ao.h"
#include "CheatController.hpp"
#include "../relive_lib/Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "Input.hpp"

namespace AO {

CheatController* gCheatController = nullptr;

bool gEnableFartGasCheat = false;
bool gVoiceCheat = false;
bool gEnableCheatFMV = false;
bool gEnableCheatLevelSelect = false;

void CheatController_Cheat_FartGas()
{
    gEnableFartGasCheat = !gEnableFartGasCheat;
}

void CheatController_Cheat_VoiceLocks()
{
    gVoiceCheat = !gVoiceCheat;
}

void CheatController_Cheat_LevelSelect()
{
    if (gMap.mCurrentCamera == 1)
    {
        gEnableCheatLevelSelect = !gEnableCheatLevelSelect;
    }
}

void CheatController_Cheat_FMV()
{
    if (gMap.mCurrentCamera == 1)
    {
        gEnableCheatFMV = !gEnableCheatFMV;
    }
}

struct CheatEntry final
{
    s32 mLevelMask;
    s32 mCheatCodeLength;
    const InputCommands* mCheatCodeAry;
    s32 mSuccessIdx;
    void (*mCallBack)(void);
};

const InputCommands sCheatKeyArray_FartGas[] = {
    eThrowItem,
    eThrowItem,
    eThrowItem,
    eThrowItem,
    eThrowItem,
    eThrowItem,
};

const InputCommands sCheatKeyArray_VoiceLocks[] = {
    eCrouchOrRoll,
    eDown,
    eThrowItem,
    eDoAction,
    eDown,
    eCrouchOrRoll,
};

const InputCommands sCheatKeyArray_FMV[] = {
    eUp,
    eLeft,
    eRight,
    eLeft,
    eRight,
    eLeft,
    eRight,
    eDown,
};

const InputCommands sCheatKeyArray_LevelSelect[] = {
    eDown,
    eRight,
    eLeft,
    eRight,
    eLeft,
    eRight,
    eLeft,
    eUp,
};

static CheatEntry sCheatArray[4] = {
    {~0, ALIVE_COUNTOF(sCheatKeyArray_FartGas), sCheatKeyArray_FartGas, 0, &CheatController_Cheat_FartGas},
    {~0, ALIVE_COUNTOF(sCheatKeyArray_VoiceLocks), sCheatKeyArray_VoiceLocks, 0, &CheatController_Cheat_VoiceLocks},
    {1, ALIVE_COUNTOF(sCheatKeyArray_FMV), sCheatKeyArray_FMV, 0, &CheatController_Cheat_FMV},
    {1, ALIVE_COUNTOF(sCheatKeyArray_LevelSelect), sCheatKeyArray_LevelSelect, 0, &CheatController_Cheat_LevelSelect}};

CheatController::CheatController()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true);
    SetType(ReliveTypes::eNone);
}

void CheatController::VScreenChanged()
{
    // Empty
}

CheatController::~CheatController()
{
    gCheatController = nullptr;
}

void CheatController::VUpdate()
{
    const u16 held = Input().GetPressed();
    if (held)
    {
        // Only do cheat code check if shift is held
        if (Input().IsAnyPressed(InputCommands::eRun))
        {
            for (auto& cheatEntry : sCheatArray)
            {
                // Bit shift current level for level mask.
                if ((1 << static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))) & cheatEntry.mLevelMask)
                {
                    if (held == cheatEntry.mCheatCodeAry[cheatEntry.mSuccessIdx])
                    {
                        cheatEntry.mSuccessIdx++;

                        // Check if we've successfully entered all cheat code keys.
                        if (cheatEntry.mSuccessIdx >= cheatEntry.mCheatCodeLength)
                        {
                            cheatEntry.mSuccessIdx = 0;
                            cheatEntry.mCallBack();
                        }
                    }
                    else
                    {
                        cheatEntry.mSuccessIdx = 0;
                    }
                }
            }
        }
    }
}

} // namespace AO
