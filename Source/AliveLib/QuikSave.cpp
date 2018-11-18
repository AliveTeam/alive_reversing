#include "stdafx.h"
#include "QuikSave.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "Path.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "UXB.hpp"
#include "ThrowableArray.hpp"
#include "StatsSign.hpp"
#include "DDCheat.hpp"
#include "Events.hpp"

EXPORT int CC LiftMover__CreateFromSaveState_40D180(const BYTE*) { NOT_IMPLEMENTED(); return 12; }
EXPORT int CC CreateFromSaveState_412C10(const BYTE*) { NOT_IMPLEMENTED(); return 60; }
EXPORT int CC CreateFromSaveState_417740(const BYTE*) { NOT_IMPLEMENTED(); return 8; }
EXPORT int CC NakedSlig__CreateFromSaveState_41AE80(const BYTE*) { NOT_IMPLEMENTED(); return 128; }
EXPORT int CC Grinder__CreateFromSaveState_421600(const BYTE*) { NOT_IMPLEMENTED(); return 20; }
EXPORT int CC EvilFart__CreateFromSaveState_4281C0(const BYTE*) { NOT_IMPLEMENTED(); return 60; }
EXPORT int CC Fleech__CreateFromSaveState_42DD50(const BYTE*) { NOT_IMPLEMENTED(); return 180; }
EXPORT int CC FlyingSlig__CreateFromSaveState_437E40(const BYTE*) { NOT_IMPLEMENTED(); return 172; }
EXPORT int CC FlyingSligSpawner__CreateFromSaveState_43B690(const BYTE*) { NOT_IMPLEMENTED(); return 16; }
EXPORT int CC CreateFromSaveState_43BD10(const BYTE*) { NOT_IMPLEMENTED(); return 16; }
EXPORT int CC CreateFromSaveState_43DC20(const BYTE*) { NOT_IMPLEMENTED(); return 4; }
EXPORT int CC SlapLock__CreateFromSaveState_43EA00(const BYTE*) { NOT_IMPLEMENTED(); return 24; }
EXPORT int CC Greeter__CreateFromSaveState_446040(const BYTE*) { NOT_IMPLEMENTED(); return 80; }
EXPORT int CC Grenade__CreateFromSaveState_449410(const BYTE*) { NOT_IMPLEMENTED(); return 60; }
EXPORT int CC Glukkon__CreateFromSaveState_442830(const BYTE*) { NOT_IMPLEMENTED(); return 144; }
EXPORT int CC SligSpawner__CreateFromSaveState_409B10(const BYTE*) { NOT_IMPLEMENTED(); return 16; }
EXPORT int CC LiftPoint__CreateFromSaveState_4630F0(const BYTE*) { NOT_IMPLEMENTED(); return 28; }
EXPORT int CC Mudokon__CreateFromSaveState_4717C0(const BYTE*) { NOT_IMPLEMENTED(); return 136; }
EXPORT int CC CreateFromSaveState_46A9E0(const BYTE*) { NOT_IMPLEMENTED(); return 60; }
EXPORT int CC MineCar__CreateFromSaveState_467740(const BYTE*) { NOT_IMPLEMENTED(); return 104; }
EXPORT int CC Paramite__CreateFromSaveState_4855A0(const BYTE*) { NOT_IMPLEMENTED(); return 120; }
EXPORT int CC BirdPortal__CreateFromSaveState_499C90(const BYTE*) { NOT_IMPLEMENTED(); return 8; }
EXPORT int CC AbilityRing__CreateFromSaveState_49DF90(const BYTE*) { NOT_IMPLEMENTED(); return 40; }
EXPORT int CC Rock__CreateFromSaveState_49F720(const BYTE*) { NOT_IMPLEMENTED(); return 56; }
EXPORT int CC Scrab__CreateFromSaveState_4A70A0(const BYTE*) { NOT_IMPLEMENTED(); return 160; }
EXPORT int CC ScrabSpawner__CreateFromSaveState_4ABEB0(const BYTE*) { NOT_IMPLEMENTED(); return 16; }
EXPORT int CC SlamDoor__CreateFromSaveState_4C08B0(const BYTE*) { NOT_IMPLEMENTED(); return 8; }
EXPORT int CC Slig__CreateFromSaveState_4B3B50(const BYTE*) { NOT_IMPLEMENTED(); return 164; }
EXPORT int CC Slog__CreateFromSaveState_4C54F0(const BYTE*) { NOT_IMPLEMENTED(); return 120; }
EXPORT int CC Slurg__CreateFromSaveState_4C8DF0(const BYTE*) { NOT_IMPLEMENTED(); return 44; }
EXPORT int CC TimerTrigger__CreateFromSaveState_4CDF70(const BYTE*) { NOT_IMPLEMENTED(); return 16; }
EXPORT int CC TrapDoor__CreateFromSaveState_4DDED0(const BYTE*) { NOT_IMPLEMENTED(); return 12; }
EXPORT int CC WorkWheel__CreateFromSaveState_4E3B10(const BYTE*) { NOT_IMPLEMENTED(); return 16; }

struct QuickSaveRestoreTable
{
    int(CC* mFns[180])(const BYTE *);
};

QuickSaveRestoreTable sQuicksaveLoadFunctionTable =
{
    nullptr,
    nullptr,
    &SligSpawner__CreateFromSaveState_409B10,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &LiftMover__CreateFromSaveState_40D180,
    nullptr,
    &CreateFromSaveState_412C10,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &CreateFromSaveState_417740,
    &NakedSlig__CreateFromSaveState_41AE80,
    nullptr,
    nullptr,
    nullptr,
    &Grinder__CreateFromSaveState_421600,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &EvilFart__CreateFromSaveState_4281C0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &Fleech__CreateFromSaveState_42DD50,
    nullptr,
    nullptr,
    nullptr,
    &FlyingSlig__CreateFromSaveState_437E40,
    &FlyingSligSpawner__CreateFromSaveState_43B690,
    nullptr,
    &CreateFromSaveState_43BD10,
    nullptr,
    nullptr,
    &CreateFromSaveState_43DC20,
    &SlapLock__CreateFromSaveState_43EA00,
    nullptr,
    nullptr,
    &Greeter__CreateFromSaveState_446040,
    &Grenade__CreateFromSaveState_449410,
    nullptr,
    &Glukkon__CreateFromSaveState_442830,
    nullptr,
    &Abe::CreateFromSaveState_44D4F0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &LiftPoint__CreateFromSaveState_4630F0,
    nullptr,
    nullptr,
    &Mudokon__CreateFromSaveState_4717C0,
    nullptr,
    nullptr,
    &CreateFromSaveState_46A9E0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &MineCar__CreateFromSaveState_467740,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &Paramite__CreateFromSaveState_4855A0,
    nullptr,
    nullptr,
    &BirdPortal__CreateFromSaveState_499C90,
    nullptr,
    nullptr,
    &ThrowableArray::CreateFromSaveState_49B200,
    nullptr,
    &AbilityRing__CreateFromSaveState_49DF90,
    &Rock__CreateFromSaveState_49F720,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &Scrab__CreateFromSaveState_4A70A0,
    &ScrabSpawner__CreateFromSaveState_4ABEB0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &SlamDoor__CreateFromSaveState_4C08B0,
    nullptr,
    nullptr,
    &Slig__CreateFromSaveState_4B3B50,
    &Slog__CreateFromSaveState_4C54F0,
    nullptr,
    nullptr,
    &Slurg__CreateFromSaveState_4C8DF0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &TimerTrigger__CreateFromSaveState_4CDF70,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &TrapDoor__CreateFromSaveState_4DDED0,
    &Uxb__CreateFromSaveState_4DFAE0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &WorkWheel__CreateFromSaveState_4E3B10,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

ALIVE_VAR(1, 0x560c34, QuickSaveRestoreTable, sQuicksaveLoadFunctionTable_560C34, sQuicksaveLoadFunctionTable);

struct QuickSaveFlagTypeTable
{
    BYTE mTypes[136];
};

const QuickSaveFlagTypeTable kQuickSaveFlagsTable =
{
    {
        0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01,
        0x00, 0x01, 0x00, 0x00, 0x01, 0x02, 0x02, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x00, 0x02, 0x00,
        0x02, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x02, 0x01, 0x02,
        0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x01,
        0x00, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01, 0x01, 0x02,
        0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
        0x02, 0x01, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01,
        0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
};

ALIVE_VAR(1, 0x547794, QuickSaveFlagTypeTable, kObjectTypeAttributesTable_byte_547794, kQuickSaveFlagsTable);

ALIVE_VAR(1, 0x5c1bbc, WORD, word_5C1BBC, 0);
ALIVE_VAR(1, 0x5c1bfa, BYTE, byte_5C1BFA, 0); // Related to boiler meter colours
ALIVE_VAR(1, 0x5c1bf8, WORD, word_5C1BF8, 0); // Ditto

ALIVE_VAR(1, 0x5c1be8, int, dword_5C1BE8, 0); // Gas counter?

void QuikSave_RestoreObjectStates_D481890_4C9BE0(const BYTE* pSaveData)
{
    const WORD* pSaveData2 = reinterpret_cast<const WORD*>(pSaveData);

    while (*reinterpret_cast<const DWORD*>(pSaveData2) != 0)
    {
        pSaveData2 += sQuicksaveLoadFunctionTable_560C34.mFns[*pSaveData2](reinterpret_cast<const BYTE*>(pSaveData2)) / sizeof(WORD);
    }

    // Skip the 2 zero entries, the saved flag words come after the object save state data
    const BYTE* pSrcFlags = reinterpret_cast<const BYTE*>(pSaveData2 + 2);
    for (short i = 1; i < sPathData_559660.paths[gMap_5C3030.sCurrentLevelId_5C3030].field_1A_num_paths; i++)
    {
        const PathBlyRec* pPathRec = Path_Get_Bly_Record_460F30(gMap_5C3030.sCurrentLevelId_5C3030, i);
        if (pPathRec->field_0_blyName)
        {
            const PathData* pPathData = pPathRec->field_4_pPathData;
            const int widthCount = (pPathData->field_4_bTop - pPathData->field_0_bLeft) / pPathData->field_A_grid_width;
            const int heightCount = (pPathData->field_6_bBottom - pPathData->field_2_bRight) / pPathData->field_C_grid_height;
            BYTE** ppPathRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, 1, 0);
            if (ppPathRes)
            {
                const int totalCameraCount = widthCount * heightCount;
                const int* indexTable = reinterpret_cast<const int*>(*ppPathRes + pPathData->field_16_object_indextable_offset);
                for (int j = 0; j < totalCameraCount; j++)
                {
                    int tlvOffset = indexTable[j];
                    if (tlvOffset != -1)
                    {
                        BYTE* ptr = &(*ppPathRes)[pPathData->field_12_object_offset + tlvOffset];
                        Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(ptr);
                        do
                        {
                            // TODO: Convert table to strongly typed flags
                            const BYTE tableValue = kObjectTypeAttributesTable_byte_547794.mTypes[pTlv->field_4_type];
                            if (tableValue)
                            {
                                if (tableValue <= 2)
                                {
                                    pTlv->field_0_flags.Raw().all = *pSrcFlags;
                                    pSrcFlags++;
                                    
                                    pTlv->field_1_unknown = *pSrcFlags;
                                    pSrcFlags++;
                                }
                            }

                            if (pTlv->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
                            {
                                // End of restoring flags for this camera
                                break;
                            }

                            tlvOffset += pTlv->field_2_length;
                            pTlv = Path::Next_TLV_4DB6A0(pTlv);
                        } while (pTlv->field_2_length);
                    }
                }
                ResourceManager::FreeResource_49C330(ppPathRes);
            }
        }
    }
    pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
}


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

void CC Quicksave_ReadWorldInfo_4C9490(const Quicksave_WorldInfo* pInfo)
{
    word_5C1BBC = pInfo->field_2E;
    sSavedKilledMudsPerPath_5C1B50 = pInfo->field_18_saved_killed_muds_per_path;
    byte_5C1B64 = pInfo->field_17;
    sActiveHero_5C1B68->field_114_flags.Set(Flags_114::e114_Bit9);
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
    pInfo->field_C_controlled_x = FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos);
    pInfo->field_E_controlled_y = rect.h;
    pInfo->field_10_controlled_scale = sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(1.0);
}

void CC Quicksave_FindSaves_4D4150()
{
    NOT_IMPLEMENTED();
}
