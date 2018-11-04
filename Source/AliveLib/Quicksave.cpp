#include "stdafx.h"
#include "Quicksave.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "Map.hpp"

#define Quicksave_IMPL true

void Quicksave_ForceLink() { }

ALIVE_VAR(1, 0xBAF7F8, Quicksave, sActiveQuicksaveData_BAF7F8, {});
ALIVE_VAR(1, 0x5C1BF4, int, sAccumulatedObjectCount_5C1BF4, 0);

EXPORT void CC Quicksave_LoadFromMemory_4C95A0(Quicksave *quicksaveData)
{
    sAccumulatedObjectCount_5C1BF4 = quicksaveData->field_200_accumulated_obj_count;
    DestroyObjects_4A1F20();
    Events_Reset_422D70();
    word_5C2FA0 = 1;
    Quicksave_ReadWorldInfo_4C9490(&quicksaveData->field_204_world_info);
    memcpy(&sSwitchStates_5C1A28, &quicksaveData->field_45C_switch_states, sizeof(SwitchStates));
    gMap_5C3030.field_D8_restore_quick_save = reinterpret_cast<BYTE*>(quicksaveData->field_55C_objects_state_data);
    gMap_5C3030.SetActiveCam_480D30(
        quicksaveData->field_204_world_info.field_4_level,
        quicksaveData->field_204_world_info.field_6_path,
        quicksaveData->field_204_world_info.field_8_cam,
        CameraSwapEffects::eEffect0_InstantChange,
        0,
        1);
    gMap_5C3030.field_8 = 1;
}

EXPORT void CC Quicksave_LoadActive_4C9170()
{
    Game_ShowLoadingIcon_482D80();
    Quicksave_LoadFromMemory_4C95A0(&sActiveQuicksaveData_BAF7F8);
}

void CC Quicksave_4C90D0()
{
    NOT_IMPLEMENTED();
}

void CC Quicksave_ReadWorldInfo_4C9490(Quicksave_WorldInfo * a1)
{
    NOT_IMPLEMENTED();
}

void CC Quicksave_FindSaves_4D4150()
{
    NOT_IMPLEMENTED();
}
