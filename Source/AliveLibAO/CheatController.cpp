#include "stdafx_ao.h"
#include "CheatController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Input.hpp"

namespace AO {

ALIVE_VAR(1, 0x4FF958, CheatController*, pCheatController_4FF958, nullptr);

ALIVE_VAR(1, 0x507704, s16, sEnableFartGasCheat_507704, 0);
ALIVE_VAR(1, 0x507708, s16, sVoiceCheat_507708, 0);
ALIVE_VAR(1, 0x50770C, s16, sEnableCheatFMV_50770C, 0);
ALIVE_VAR(1, 0x507710, s16, sEnableCheatLevelSelect_507710, 0);

EXPORT void CC CheatController_Cheat_FartGas_40FB70()
{
    sEnableFartGasCheat_507704 = sEnableFartGasCheat_507704 == 0;
}

EXPORT void CC CheatController_Cheat_VoiceLocks_40FB90()
{
    sVoiceCheat_507708 = sVoiceCheat_507708 == 0;
}

EXPORT void CC CheatController_Cheat_LevelSelect_40FBD0()
{
    if (gMap.field_4_current_camera == 1)
    {
        sEnableCheatLevelSelect_507710 = sEnableCheatLevelSelect_507710 == 0;
    }
}

EXPORT void CC CheatController_Cheat_FMV_40FBB0()
{
    if (gMap.field_4_current_camera == 1)
    {
        sEnableCheatFMV_50770C = sEnableCheatFMV_50770C == 0;
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


const InputCommands sCheatKeyArray_FartGas_4C50B8[] = {
    eThrowItem,
    eThrowItem,
    eThrowItem,
    eThrowItem,
    eThrowItem,
    eThrowItem,
};

const InputCommands sCheatKeyArray_VoiceLocks_4C50C8[] = {
    eCrouchOrRoll,
    eDown,
    eThrowItem,
    eDoAction,
    eDown,
    eCrouchOrRoll,
};

const InputCommands sCheatKeyArray_FMV_4C50D8[] = {
    eUp,
    eLeft,
    eRight,
    eLeft,
    eRight,
    eLeft,
    eRight,
    eDown,
};

const InputCommands sCheatKeyArray_LevelSelect_4C50E8[] = {
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
    {~0, ALIVE_COUNTOF(sCheatKeyArray_FartGas_4C50B8), sCheatKeyArray_FartGas_4C50B8, 0, &CheatController_Cheat_FartGas_40FB70},
    {~0, ALIVE_COUNTOF(sCheatKeyArray_VoiceLocks_4C50C8), sCheatKeyArray_VoiceLocks_4C50C8, 0, &CheatController_Cheat_VoiceLocks_40FB90},
    {1, ALIVE_COUNTOF(sCheatKeyArray_FMV_4C50D8), sCheatKeyArray_FMV_4C50D8, 0, &CheatController_Cheat_FMV_40FBB0},
    {1, ALIVE_COUNTOF(sCheatKeyArray_LevelSelect_4C50E8), sCheatKeyArray_LevelSelect_4C50E8, 0, &CheatController_Cheat_LevelSelect_40FBD0}};

CheatController* CheatController::ctor_40FBF0()
{
    ctor_487E10(1);
    mFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    SetVTable(this, 0x4BA8A8);
    field_4_typeId = Types::eNone_0;
    field_10 = 0;
    return this;
}

BaseGameObject* CheatController::VDestructor(s32 flags)
{
    return Vdtor_40FCD0(flags);
}


void CheatController::VScreenChanged()
{
    // Empty
}

BaseGameObject* CheatController::dtor_40FC20()
{
    SetVTable(this, 0x4BA8A8);
    pCheatController_4FF958 = nullptr;
    return dtor_487DF0();
}

CheatController* CheatController::Vdtor_40FCD0(s32 flags)
{
    dtor_40FC20();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void CheatController::VUpdate()
{
    VUpdate_40FC40();
}

void CheatController::VUpdate_40FC40()
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
                if ((1 << static_cast<s32>(gMap.mCurrentLevel)) & cheatEntry.field_0_level_mask)
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
