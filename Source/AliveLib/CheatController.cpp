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

InputCommands sCheatKeyArray_MovieSelect_5515C0[8] = { eUp, eLeft, eRight, eLeft, eRight, eLeft, eRight, eDown };
InputCommands sCheatKeyArray_LevelSelect_5515D0[8] = { eDown, eRight, eLeft, eRight, eLeft, eRight, eLeft, eUp };
InputCommands sCheatKeyArray_PathSkip_5515E8[6] = { eLeft, eRight, eUp, eDown, eLeft, eRight };

EXPORT void CC CheatController_Cheat_FMV_421AD0()
{
    if (gMap_5C3030.sCurrentCamId_5C3034 == 1)
    {
        sEnableCheatFMV_5C1BEC = !sEnableCheatFMV_5C1BEC;
    }
}

EXPORT void CC CheatController_Cheat_LevelSelect_421B00()
{
    if (gMap_5C3030.sCurrentCamId_5C3034 == 1)
    {
        sEnableCheatLevelSelect_5C1BEE = !sEnableCheatLevelSelect_5C1BEE;
    }
}

EXPORT void CC CheatController_Cheat_PathSkip_421B30()
{
    char nameBuffer[20];

    DestroyObjects_4A1F20();
    ResourceManager::Reclaim_Memory_49C470(0);
    sprintf(nameBuffer, "NXTP%04d.SAV", sActiveQuicksaveData_BAF7F8.field_204_world_info.field_A_unknown_1);
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

CheatEntry sCheatArray_5515F8[4] =
{
    { 1u, (sizeof(sCheatKeyArray_MovieSelect_5515C0) / sizeof(InputCommands)), sCheatKeyArray_MovieSelect_5515C0, 0, &CheatController_Cheat_FMV_421AD0},
    { 1u, (sizeof(sCheatKeyArray_LevelSelect_5515D0) / sizeof(InputCommands)), sCheatKeyArray_LevelSelect_5515D0, 0, &CheatController_Cheat_LevelSelect_421B00 },
    { 0xFFFFFFFE, (sizeof(sCheatKeyArray_PathSkip_5515E8) / sizeof(InputCommands)), sCheatKeyArray_PathSkip_5515E8, 0, &CheatController_Cheat_PathSkip_421B30 },
    { 0u, 0, NULL, 0, NULL }
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
        Mem_Free_495540(this);
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
            for (auto i = sCheatArray_5515F8; i->field_0_level_mask; i++)
            {
                // Bit shift current level for level mask.
                if ((1 << static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)) & i->field_0_level_mask)
                {
                    if (held == i->field_8_cheat_code_ary[i->field_C_success_idx])
                    {
                        i->field_C_success_idx++;

                        // Check if we've successfully entered all cheat code keys.
                        if (i->field_C_success_idx >= i->field_4_cheat_code_length)
                        {
                            i->field_C_success_idx = 0;
                            i->field_10_callback();
                        }
                    }
                    else
                    {
                        i->field_C_success_idx = 0;
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
