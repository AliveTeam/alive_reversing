#include "stdafx_ao.h"
#include "CheatController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"

START_NS_AO

ALIVE_VAR(1, 0x4FF958, CheatController*, pCheatController_4FF958, nullptr);

ALIVE_VAR(1, 0x507704, short, sEnableFartGasCheat_507704, 0);
ALIVE_VAR(1, 0x507708, short, sVoiceCheat_507708, 0);
ALIVE_VAR(1, 0x50770C, short, sEnableCheatFMV_50770C, 0);
ALIVE_VAR(1, 0x507710, short, sEnableCheatLevelSelect_507710, 0);

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
    if (gMap_507BA8.field_4_current_camera == 1)
    {
        sEnableCheatLevelSelect_507710 = sEnableCheatLevelSelect_507710 == 0;
    }
}

EXPORT void CC CheatController_Cheat_FMV_40FBB0()
{
    if (gMap_507BA8.field_4_current_camera == 1)
    {
        sEnableCheatFMV_50770C = sEnableCheatFMV_50770C == 0;
    }
}

CheatController* CheatController::ctor_40FBF0()
{
    ctor_487E10(1);
    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    SetVTable(this, 0x4BA8A8);
    field_4_typeId = Types::eNone_0;
    field_10 = 0;
    return this;
}

BaseGameObject* CheatController::VDestructor(signed int flags)
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

CheatController* CheatController::Vdtor_40FCD0(signed int flags)
{
    dtor_40FC20();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
