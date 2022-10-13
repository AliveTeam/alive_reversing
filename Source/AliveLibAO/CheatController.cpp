#include "stdafx_ao.h"
#include "CheatController.hpp"
#include "Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "Input.hpp"

namespace AO {

CheatController* gCheatController = nullptr;

s16 gEnableFartGasCheat = 0;
s16 gVoiceCheat = 0;
s16 gEnableCheatFMV = 0;
s16 gEnableCheatLevelSelect = 0;

void CheatController_Cheat_FartGas()
{
    gEnableFartGasCheat = gEnableFartGasCheat == 0;
}

void CheatController_Cheat_VoiceLocks()
{
    gVoiceCheat = gVoiceCheat == 0;
}

void CheatController_Cheat_LevelSelect()
{
    if (gMap.mCurrentCamera == 1)
    {
        gEnableCheatLevelSelect = gEnableCheatLevelSelect == 0;
    }
}

void CheatController_Cheat_FMV()
{
    if (gMap.mCurrentCamera == 1)
    {
        gEnableCheatFMV = gEnableCheatFMV == 0;
    }
}

struct CheatEntry final
{
    s32 field_0_level_mask;
    s32 field_4_cheat_code_length;
    const InputCommands* field_8_cheat_code_ary;
    s32 field_C_success_idx;
    void (*field_10_callback)(void);
};
ALIVE_ASSERT_SIZEOF(CheatEntry, 0x14);


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

CheatEntry stru_4C50F8[4] = {
    {~0, ALIVE_COUNTOF(sCheatKeyArray_FartGas), sCheatKeyArray_FartGas, 0, &CheatController_Cheat_FartGas},
    {~0, ALIVE_COUNTOF(sCheatKeyArray_VoiceLocks), sCheatKeyArray_VoiceLocks, 0, &CheatController_Cheat_VoiceLocks},
    {1, ALIVE_COUNTOF(sCheatKeyArray_FMV), sCheatKeyArray_FMV, 0, &CheatController_Cheat_FMV},
    {1, ALIVE_COUNTOF(sCheatKeyArray_LevelSelect), sCheatKeyArray_LevelSelect, 0, &CheatController_Cheat_LevelSelect}};

CheatController::CheatController()
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    SetType(ReliveTypes::eNone);
    field_10 = 0;
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
    const u16 held = Input().Held();
    if (held)
    {
        // Only do cheat code check if shift is held
        if (Input().IsAnyPressed(InputCommands::eRun))
        {
            for (auto& cheatEntry : stru_4C50F8)
            {
                // Bit shift current level for level mask.
                if ((1 << static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))) & cheatEntry.field_0_level_mask)
                {
                    if (held == cheatEntry.field_8_cheat_code_ary[cheatEntry.field_C_success_idx])
                    {
                        cheatEntry.field_C_success_idx++;

                        // Check if we've successfully entered all cheat code keys.
                        if (cheatEntry.field_C_success_idx >= cheatEntry.field_4_cheat_code_length)
                        {
                            cheatEntry.field_C_success_idx = 0;
                            cheatEntry.field_10_callback();
                        }
                    }
                    else
                    {
                        cheatEntry.field_C_success_idx = 0;
                    }
                }
            }
        }
    }
}

} // namespace AO
