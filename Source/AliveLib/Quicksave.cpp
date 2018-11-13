#include "stdafx.h"
#include "Quicksave.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "StatsSign.hpp"
#include "DDCheat.hpp"

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

ALIVE_VAR(1, 0x5c1bbc, WORD, word_5C1BBC, 0);
ALIVE_VAR(1, 0x5c1bfa, BYTE, byte_5C1BFA, 0); // Related to boiler meter colours
ALIVE_VAR(1, 0x5c1bf8, WORD, word_5C1BF8, 0); // Ditto

ALIVE_VAR(1, 0x5c1be8, int, dword_5C1BE8, 0); // Gas counter?

void CC Quicksave_ReadWorldInfo_4C9490(const Quicksave_WorldInfo* pInfo)
{
    word_5C1BBC = pInfo->field_2E;
    sSavedKilledMudsPerPath_5C1B50 = pInfo->field_18_saved_killed_muds_per_path;
    byte_5C1B64 = pInfo->field_17;
    sActiveHero_5C1B68->field_114_flags |= 100u;
    sStatsSignCurrentArea_5C1A20 = pInfo->field_2C;
    sKilledMudokons_5C1BC0 = pInfo->field_14_killed_muds;
    sRescuedMudokons_5C1BC2 = pInfo->field_12_saved_muds;
    sMudokonsInArea_5C1BC4 = pInfo->field_16_muds_in_area; // TODO: Check types
    byte_5C1BFA = pInfo->field_2D;
    word_5C1BF8 = pInfo->field_30;
    dword_5C1BE8 = pInfo->field_38;
    word_5C1BDA = pInfo->field_3C;
    sVisitedBonewerks_5C1C02 = pInfo->field_32_visited_bonewerks;
    sVisitedBarracks_5C1C04 = pInfo->field_34_visited_barracks;
    sVisitedFeecoEnder_5C1C06 = pInfo->field_36_visited_feeco_ender;
    sGnFrame_5C1B84 = pInfo->field_0_gnFrame;
}

void CC Quicksave_SaveWorldInfo_4C9310(Quicksave_WorldInfo* pInfo)
{
    PSX_RECT rect = {};
    sControlledCharacter_5C1B8C->vGetBoundingRect_424FD0(&rect, 1);

    pInfo->field_0_gnFrame = sGnFrame_5C1B84;
    pInfo->field_4_level = gMap_5C3030.sCurrentLevelId_5C3030;
    pInfo->field_6_path = gMap_5C3030.sCurrentPathId_5C3032;
    pInfo->field_8_cam = gMap_5C3030.sCurrentCamId_5C3034;
    pInfo->field_2E = word_5C1BBC;
    pInfo->field_18_saved_killed_muds_per_path = sSavedKilledMudsPerPath_5C1B50;
    pInfo->field_17 = byte_5C1B64;
    pInfo->field_2C = sStatsSignCurrentArea_5C1A20;
    pInfo->field_12_saved_muds = sRescuedMudokons_5C1BC2;
    pInfo->field_14_killed_muds = sKilledMudokons_5C1BC0;
    pInfo->field_16_muds_in_area = static_cast<char>(sMudokonsInArea_5C1BC4); // TODO: Check types
    pInfo->field_2D = byte_5C1BFA;
    pInfo->field_30 = word_5C1BF8;
    pInfo->field_3C = word_5C1BDA;
    pInfo->field_32_visited_bonewerks = sVisitedBonewerks_5C1C02;
    pInfo->field_34_visited_barracks = sVisitedBarracks_5C1C04;
    pInfo->field_36_visited_feeco_ender = sVisitedFeecoEnder_5C1C06;
    pInfo->field_38 = dword_5C1BE8;
    pInfo->field_C_controlled_x = sControlledCharacter_5C1B8C->field_B8_xpos.GetExponent();
    pInfo->field_E_controlled_y = rect.h;
    pInfo->field_10_controlled_scale = sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(1.0);
}

void CC Quicksave_FindSaves_4D4150()
{
    NOT_IMPLEMENTED();
}
