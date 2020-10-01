#include "stdafx.h"
#include "CheatController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "MainMenu.hpp"
#include "QuikSave.hpp"

void CheatController_ForceLink() { }

ALIVE_VAR(1, 0x5BC120, CheatController*, pCheatController_5BC120, nullptr);

const InputCommands sCheatKeyArray_MovieSelect_5515C0[8] = 
{
    eUp,
    eLeft,
    eRight,
    eLeft,
    eRight,
    eLeft,
    eRight,
    eDown
};

const InputCommands sCheatKeyArray_LevelSelect_5515D0[8] =
{
    eDown,
    eRight,
    eLeft,
    eRight,
    eLeft,
    eRight,
    eLeft,
    eUp
};

const InputCommands sCheatKeyArray_PathSkip_5515E8[6] =
{
    eLeft,
    eRight,
    eUp,
    eDown,
    eLeft,
    eRight
};

EXPORT void CC CheatController_Cheat_FMV_421AD0()
{
    if (gMap_5C3030.field_4_current_camera == 1)
    {
        sEnableCheatFMV_5C1BEC = !sEnableCheatFMV_5C1BEC;
    }
}

EXPORT void CC CheatController_Cheat_LevelSelect_421B00()
{
    if (gMap_5C3030.field_4_current_camera == 1)
    {
        sEnableCheatLevelSelect_5C1BEE = !sEnableCheatLevelSelect_5C1BEE;
    }
}

EXPORT void CC CheatController_Cheat_PathSkip_421B30()
{
    char nameBuffer[20];

    DestroyObjects_4A1F20();
    ResourceManager::Reclaim_Memory_49C470(0);
    sprintf(nameBuffer, "NXTP%04d.SAV", sActiveQuicksaveData_BAF7F8.field_204_world_info.field_A_save_num);
    ResourceManager::LoadResourceFile_49C170(nameBuffer, 0); 
    BYTE **resource = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_NxtP, ResourceID::kUnknownResID_0, 1u, 0);
    if (resource)
    {
        memcpy(&sActiveQuicksaveData_BAF7F8, *resource, sizeof(sActiveQuicksaveData_BAF7F8));
        ResourceManager::FreeResource_49C330(resource);
        sActiveQuicksaveData_BAF7F8.field_200_accumulated_obj_count = 1024;
        Quicksave_LoadActive_4C9170();
    }
}

CheatEntry sCheatArray_5515F8[] =
{
    { 1u, ALIVE_COUNTOF(sCheatKeyArray_MovieSelect_5515C0), sCheatKeyArray_MovieSelect_5515C0, 0, &CheatController_Cheat_FMV_421AD0},
    { 1u, ALIVE_COUNTOF(sCheatKeyArray_LevelSelect_5515D0), sCheatKeyArray_LevelSelect_5515D0, 0, &CheatController_Cheat_LevelSelect_421B00 },
    { 0xFFFFFFFE, ALIVE_COUNTOF(sCheatKeyArray_PathSkip_5515E8), sCheatKeyArray_PathSkip_5515E8, 0, &CheatController_Cheat_PathSkip_421B30 }
};


CheatController* CheatController::ctor_421BD0()
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    SetVTable(this, 0x544B44);
    field_4_typeId = Types::eNone_0;
    field_20 = 0;
    return this;
}

BaseGameObject* CheatController::vdtor_421C10(signed int flags)
{
    dtor_421C40();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void CheatController::dtor_421C40()
{
    SetVTable(this, 0x544B44);
    pCheatController_5BC120 = nullptr;
    BaseGameObject_dtor_4DBEC0();
}

void CheatController::Update_421C70()
{
    auto held = sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_C_held;

    if (held)
    {
        // Only do cheat code check if shift is held
        if (sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_0_pressed & eRun)
        {
            for (auto& cheatEntry : sCheatArray_5515F8)
            {
                // Bit shift current level for level mask.
                if ((1 << static_cast<int>(gMap_5C3030.field_0_current_level)) & cheatEntry.field_0_level_mask)
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

BaseGameObject* CheatController::VDestructor(signed int flags)
{
    return vdtor_421C10(flags);
}

void CheatController::VUpdate()
{
    Update_421C70();
}

void CheatController::VRender(int ** /*pOrderingTable*/)
{
    // NULL
}

void CheatController::VScreenChanged()
{
    // NULL
}
