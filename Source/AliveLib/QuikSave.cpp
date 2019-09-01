#include "stdafx.h"
#include "QuikSave.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "Path.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "Glukkon.hpp"
#include "UXB.hpp"
#include "ThrowableArray.hpp"
#include "StatsSign.hpp"
#include "DDCheat.hpp"
#include "Events.hpp"
#include "WorkWheel.hpp"
#include "Grinder.hpp"
#include "ScreenManager.hpp"
#include "Io.hpp"
#include "LiftPoint.hpp"
#include "LiftMover.hpp"
#include "TrapDoor.hpp"
#include "TimerTrigger.hpp"
#include "SlamDoor.hpp"
#include "FlyingSlig.hpp"
#include "SlapLock.hpp"
#include "AbilityRing.hpp"
#include "Shadow.hpp"
#include "Slurg.hpp"
#include "GasCountDown.hpp"
#include "Rock.hpp"
#include "Meat.hpp"
#include "Bone.hpp"
#include "MineCar.hpp"
#include "Slig.hpp"
#include "SligSpawner.hpp"
#include "ScrabSpawner.hpp"
#include "GameEnderController.hpp"
#include "Paramite.hpp"
#include "BirdPortal.hpp"
#include "ColourfulMeter.hpp"

EXPORT int CC CreateFromSaveState_417740(const BYTE*) { NOT_IMPLEMENTED(); return 8; }
EXPORT int CC NakedSlig__CreateFromSaveState_41AE80(const BYTE*) { NOT_IMPLEMENTED(); return 128; }
EXPORT int CC EvilFart__CreateFromSaveState_4281C0(const BYTE*) { NOT_IMPLEMENTED(); return 60; }
EXPORT int CC Fleech__CreateFromSaveState_42DD50(const BYTE*) { NOT_IMPLEMENTED(); return 180; }
EXPORT int CC FlyingSlig__CreateFromSaveState_437E40(const BYTE*) { NOT_IMPLEMENTED(); return 172; }
EXPORT int CC Greeter__CreateFromSaveState_446040(const BYTE*) { NOT_IMPLEMENTED(); return 80; }
EXPORT int CC Grenade__CreateFromSaveState_449410(const BYTE*) { NOT_IMPLEMENTED(); return 60; }
EXPORT int CC Mudokon__CreateFromSaveState_4717C0(const BYTE*) { NOT_IMPLEMENTED(); return 136; }
EXPORT int CC Scrab__CreateFromSaveState_4A70A0(const BYTE*) { NOT_IMPLEMENTED(); return 160; }
EXPORT int CC Slog__CreateFromSaveState_4C54F0(const BYTE*) { NOT_IMPLEMENTED(); return 120; }

struct QuickSaveRestoreTable
{
    int(CC* mFns[180])(const BYTE *);
};

QuickSaveRestoreTable sQuicksaveLoadFunctionTable =
{
    nullptr,
    nullptr,
    &SligSpawner::CreateFromSaveState_409B10,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &LiftMover::CreateFromSaveState_40D180,
    nullptr,
    &Bone::CreateFromSaveState_412C10,
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
    &Grinder::CreateFromSaveState_421600,
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
    &FlyingSligSpawner::CreateFromSaveState_43B690,
    nullptr,
    &GameEnderController::CreateFromSaveState_43BD10,
    nullptr,
    nullptr,
    &SlapLockWhirlWind::CreateFromSaveState_43DC20,
    &SlapLock::CreateFromSaveState_43EA00,
    nullptr,
    nullptr,
    &Greeter__CreateFromSaveState_446040,
    &Grenade__CreateFromSaveState_449410,
    nullptr,
    &Glukkon::CreateFromSaveState_442830,
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
    &LiftPoint::CreateFromSaveState_4630F0,
    nullptr,
    nullptr,
    &Mudokon__CreateFromSaveState_4717C0,
    nullptr,
    nullptr,
    &Meat::CreateFromSaveState_46A9E0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &MineCar::CreateFromSaveState_467740,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &Paramite::CreateFromSaveState_4855A0,
    nullptr,
    nullptr,
    &BirdPortal::CreateFromSaveState_499C90,
    nullptr,
    nullptr,
    &ThrowableArray::CreateFromSaveState_49B200,
    nullptr,
    &AbilityRing::CreateFromSaveState_49DF90,
    &Rock::CreateFromSaveState_49F720,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &Scrab__CreateFromSaveState_4A70A0,
    &ScrabSpawner::CreateFromSaveState_4ABEB0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &SlamDoor::CreateFromSaveState_4C08B0,
    nullptr,
    nullptr,
    &Slig::CreateFromSaveState_4B3B50,
    &Slog__CreateFromSaveState_4C54F0,
    nullptr,
    nullptr,
    &Slurg::CreateFromSaveState_4C8DF0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &TimerTrigger::CreateFromSaveState_4CDF70,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &TrapDoor::CreateFromSaveState_4DDED0,
    &UXB::CreateFromSaveState_4DFAE0,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &WorkWheel::CreateFromSaveState_4E3B10,
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
ALIVE_VAR(1, 0x5c1bf8, WORD, word_5C1BF8, 0); // Ditto

ALIVE_VAR(1, 0xbb234c, WORD, sQuickSave_saved_switchResetters_count_BB234C, 0);

void QuikSave_RestoreObjectStates_D481890_4C9BE0(const BYTE* pSaveData)
{
    const WORD* pSaveData2 = reinterpret_cast<const WORD*>(pSaveData);

    while (*reinterpret_cast<const DWORD*>(pSaveData2) != 0)
    {
        pSaveData2 += sQuicksaveLoadFunctionTable_560C34.mFns[*pSaveData2](reinterpret_cast<const BYTE*>(pSaveData2)) / sizeof(WORD);
    }

    // Skip the 2 zero entries, the saved flag words come after the object save state data
    const BYTE* pSrcFlags = reinterpret_cast<const BYTE*>(pSaveData2 + 2);
    for (short i = 1; i <= sPathData_559660.paths[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_1A_num_paths; i++)
    {
        const PathBlyRec* pPathRec = Path_Get_Bly_Record_460F30(gMap_5C3030.sCurrentLevelId_5C3030, i);
        if (pPathRec->field_0_blyName)
        {
            const PathData* pPathData = pPathRec->field_4_pPathData;
            const int widthCount = (pPathData->field_4_bTop - pPathData->field_0_bLeft) / pPathData->field_A_grid_width;
            const int heightCount = (pPathData->field_6_bBottom - pPathData->field_2_bRight) / pPathData->field_C_grid_height;
            BYTE** ppPathRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, TRUE, FALSE);
            if (ppPathRes)
            {
                const int totalCameraCount = widthCount * heightCount;
                const int* indexTable = reinterpret_cast<const int*>(*ppPathRes + pPathData->field_16_object_indextable_offset);
                for (int j = 0; j < totalCameraCount; j++)
                {
                    const int tlvOffset = indexTable[j];
                    if (tlvOffset != -1)
                    {
                        BYTE* ptr = &(*ppPathRes)[pPathData->field_12_object_offset + tlvOffset];
                        Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(ptr);
                        while (pTlv)
                        {
                            // TODO: Convert table to strongly typed flags
                            const BYTE tableValue = kObjectTypeAttributesTable_byte_547794.mTypes[pTlv->field_4_type];
                            if (tableValue == 1 || tableValue == 2) // Type 0 ignored - actually it should never be written here anyway
                            {
                                pTlv->field_0_flags.Raw().all = *pSrcFlags;
                                pSrcFlags++;

                                pTlv->field_1_unknown = *pSrcFlags;
                                pSrcFlags++;
                            }
                            pTlv = Path::Next_TLV_4DB6A0(pTlv);
                        }
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
ALIVE_ARY(1, 0xBB31D8, SaveFileRec, 128, sSaveFileRecords_BB31D8, {});
ALIVE_VAR(1, 0xBB43FC, int, sSavedGameToLoadIdx_BB43FC, 0);
ALIVE_VAR(1, 0xBB43E0, signed int, sTotalSaveFilesCount_BB43E0, 0);

EXPORT void CC Quicksave_LoadFromMemory_4C95A0(Quicksave *quicksaveData)
{
    sAccumulatedObjectCount_5C1BF4 = quicksaveData->field_200_accumulated_obj_count;
    DestroyObjects_4A1F20();
    Events_Reset_422D70();
    bSkipGameObjectUpdates_5C2FA0 = 1;
    Quicksave_ReadWorldInfo_4C9490(&quicksaveData->field_204_world_info);
    sSwitchStates_5C1A28 = quicksaveData->field_45C_switch_states;
    gMap_5C3030.field_D8_restore_quick_save = reinterpret_cast<BYTE*>(quicksaveData->field_55C_objects_state_data);
    gMap_5C3030.SetActiveCam_480D30(
        quicksaveData->field_204_world_info.field_4_level,
        quicksaveData->field_204_world_info.field_6_path,
        quicksaveData->field_204_world_info.field_8_cam,
        CameraSwapEffects::eEffect0_InstantChange,
        0,
        1);
    gMap_5C3030.field_8_force_load = 1;
}

EXPORT void CC Quicksave_LoadActive_4C9170()
{
    Game_ShowLoadingIcon_482D80();
    Quicksave_LoadFromMemory_4C95A0(&sActiveQuicksaveData_BAF7F8);
}

static void WriteChars(char*& pDst, BYTE v1, BYTE v2)
{
    *pDst = v1;
    pDst++;

    *pDst = v2;
    pDst++;
}

static void WriteFlags(BYTE*& pSaveBuffer, const Path_TLV* pTlv, const BitField8<TLV_Flags>& flags)
{
    *pSaveBuffer = flags.Raw().all;
    pSaveBuffer++;

    *pSaveBuffer = pTlv->field_1_unknown;
    pSaveBuffer++;
}

EXPORT void CCSTD Quicksave_SaveBlyData_4C9660(BYTE* pSaveBuffer)
{
    for (short i = 1; i <= sPathData_559660.paths[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_1A_num_paths; i++)
    {
        const PathBlyRec* pPathRec = Path_Get_Bly_Record_460F30(gMap_5C3030.sCurrentLevelId_5C3030, i);
        if (pPathRec->field_0_blyName)
        {
            const PathData* pPathData = pPathRec->field_4_pPathData;
            const int widthCount = (pPathData->field_4_bTop - pPathData->field_0_bLeft) / pPathData->field_A_grid_width;
            const int heightCount = (pPathData->field_6_bBottom - pPathData->field_2_bRight) / pPathData->field_C_grid_height;
            BYTE** ppPathRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, TRUE, FALSE);
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
                        while (pTlv)
                        {
                            if (kObjectTypeAttributesTable_byte_547794.mTypes[pTlv->field_4_type] == 1)
                            {
                                BitField8<TLV_Flags> flags = pTlv->field_0_flags;
                                if (flags.Get(TLV_Flags::eBit1_Created))
                                {
                                    flags.Clear(TLV_Flags::eBit1_Created);
                                    flags.Clear(TLV_Flags::eBit2_Unknown);
                                }
                                WriteFlags(pSaveBuffer, pTlv, flags);
                            }
                            else if (kObjectTypeAttributesTable_byte_547794.mTypes[pTlv->field_4_type] == 2)
                            {
                                WriteFlags(pSaveBuffer, pTlv, pTlv->field_0_flags);
                            }
                            else
                            {
                                // Type 0 ignored
                            }
                            pTlv = Path::Next_TLV_4DB6A0(pTlv);
                        }
                    }
                }
                ResourceManager::FreeResource_49C330(ppPathRes);
            }
        }
    }

    // NOTE: Some values with things like total save size written here, but they are never used
}

struct SaveFlagsAndData
{
    BitField8<TLV_Flags> flags;
    BYTE data;
};
ALIVE_ARY(1, 0xBB233C, SaveFlagsAndData, 8, sSwitchReset_Saved_States_BB233C, {});

EXPORT void CC Quicksave_SaveSwitchResetterStates_4C9870()
{
    sQuickSave_saved_switchResetters_count_BB234C = 0;
    for (short i = 1; i <= sPathData_559660.paths[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_1A_num_paths; i++)
    {
        const PathBlyRec* pPathRec = Path_Get_Bly_Record_460F30(gMap_5C3030.sCurrentLevelId_5C3030, i);
        if (pPathRec->field_0_blyName)
        {
            const PathData* pPathData = pPathRec->field_4_pPathData;
            const int widthCount = (pPathData->field_4_bTop - pPathData->field_0_bLeft) / pPathData->field_A_grid_width;
            const int heightCount = (pPathData->field_6_bBottom - pPathData->field_2_bRight) / pPathData->field_C_grid_height;
            BYTE** ppPathRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, TRUE, FALSE);
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
                        while (pTlv)
                        {
                            if (pTlv->field_4_type == TlvTypes::Null_76)
                            {
                                if (sQuickSave_saved_switchResetters_count_BB234C < 8)
                                {
                                    sSwitchReset_Saved_States_BB233C[sQuickSave_saved_switchResetters_count_BB234C].flags = pTlv->field_0_flags;
                                    sSwitchReset_Saved_States_BB233C[sQuickSave_saved_switchResetters_count_BB234C].data = pTlv->field_1_unknown;

                                    sQuickSave_saved_switchResetters_count_BB234C++;
                                }
                                else
                                {
                                    LOG_WARNING("Out of write space !!");
                                }
                            }
                            pTlv = Path::Next_TLV_4DB6A0(pTlv);
                        }
                    }
                }
                ResourceManager::FreeResource_49C330(ppPathRes);
            }
        }
    }
}

EXPORT void CC Quicksave_RestoreSwitchResetterStates_4C9A30()
{
    int idx = 0;
    for (short i = 1; i <= sPathData_559660.paths[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_1A_num_paths; i++)
    {
        const PathBlyRec* pPathRec = Path_Get_Bly_Record_460F30(gMap_5C3030.sCurrentLevelId_5C3030, i);
        if (pPathRec->field_0_blyName)
        {
            const PathData* pPathData = pPathRec->field_4_pPathData;
            const int widthCount = (pPathData->field_4_bTop - pPathData->field_0_bLeft) / pPathData->field_A_grid_width;
            const int heightCount = (pPathData->field_6_bBottom - pPathData->field_2_bRight) / pPathData->field_C_grid_height;
            BYTE** ppPathRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, TRUE, FALSE);
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
                        while (pTlv)
                        {
                            if (pTlv->field_4_type == TlvTypes::Null_76)
                            {
                                if (idx < 8)
                                {
                                    pTlv->field_0_flags = sSwitchReset_Saved_States_BB233C[idx].flags;
                                    pTlv->field_1_unknown = sSwitchReset_Saved_States_BB233C[idx].data ;

                                    idx++;
                                }
                                else
                                {
                                    LOG_WARNING("Out of read space !!");
                                }
                            }
                            pTlv = Path::Next_TLV_4DB6A0(pTlv);
                        }
                    }
                }
                ResourceManager::FreeResource_49C330(ppPathRes);
            }
        }
    }
    sQuickSave_saved_switchResetters_count_BB234C = 0;
}

void CC MEMCARD_Write_SJISC_String_4A2770(char* src, char* dst, int srcLength)
{
    while (srcLength > 0)
    {
        const char srcChar = *src;
        if (*src >= 'A' && srcChar <= 'Z')
        {
            WriteChars(dst, 0x82, *src + 0x1F);
        }
        else if (srcChar >= 'a' && srcChar <= 'z')
        {
            WriteChars(dst, 0x82, *src + 0x20);
        }
        else if (srcChar < '0' || srcChar > '9')
        {
            if (srcChar == ':')
            {
                WriteChars(dst, 0x81, 0x46);
            }
            else
            {
                if (srcChar == '"')
                {
                    WriteChars(dst, 0x81, 0x67);
                }
                else
                {
                    WriteChars(dst, 0x81, 0x40);
                }
            }
        }
        else
        {
            WriteChars(dst, 0x82, *src + 0x1F);
        }
        src++;
        srcLength--;
    }
}

EXPORT void CC MEMCARD_Generate_Timestamp_String_4A21F0(char *pStr)
{
    sprintf(pStr, "%03d:%02d",
        (signed int)(sGnFrame_5C1B84 - sGameStartedFrame_5C1B88) / 30 / 60 / 60 % 1000,
        (signed int)(sGnFrame_5C1B84 - sGameStartedFrame_5C1B88) / 30 / 60 % 60);
}

EXPORT void CC MEMCARD_Write_Timestamp_SJISC_String_4A2290(char* dst)
{
    char gameTimeStr[12] = {};
    MEMCARD_Generate_Timestamp_String_4A21F0(gameTimeStr);

    const size_t len = strlen(gameTimeStr);
    for (size_t idx = 0; idx < len; idx++)
    {
        // More SJISC conversion ?
        const char strChar = gameTimeStr[idx];
        if (strChar == ' ')
        {
            dst[2 * idx] = 0x81u;
            dst[2 * idx + 1] = 0x40;
        }
        else if (strChar == ':')
        {
            dst[2 * idx] = 0x81u;
            dst[2 * idx + 1] = 0x46;
        }
        else
        {
            dst[2 * idx] = 0x82u;
            dst[2 * idx + 1] = strChar + 0x1F;
        }
    }
}
ALIVE_VAR(1, 0xBB19F8, Quicksave_PSX_Header, sSaveHeader2_BB19F8, {});
ALIVE_VAR(1, 0xBB17F8, Quicksave_PSX_Header, sSaveHeader1_BB17F8, {});

EXPORT void CC Quicksave_SaveToMemory_4C91A0(Quicksave* pSave)
{
    if (sActiveHero_5C1B68->field_10C_health > FP_FromInteger(0))
    {
        pSave->field_200_accumulated_obj_count = sAccumulatedObjectCount_5C1BF4;

        // Don't really know what the point of doing this is? Might as well just memset the pSave header?
        Quicksave_PSX_Header* pHeaderToUse = nullptr;
        if (word_5C1BBC == 0)
        {
            pHeaderToUse = &sSaveHeader1_BB17F8;
        }
        else
        {
            pHeaderToUse = &sSaveHeader2_BB19F8;
        }
        pSave->field_0_header = *pHeaderToUse;

        MEMCARD_Write_Timestamp_SJISC_String_4A2290(&pSave->field_0_header.field_0_frame_1_name[50]);

        char src[12] = {};
        sprintf(src, "%2sP%02dC%02d", 
            sPathData_559660.paths[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_18_lvl_name,
            gMap_5C3030.sCurrentPathId_5C3032,
            gMap_5C3030.sCurrentCamId_5C3034);
        MEMCARD_Write_SJISC_String_4A2770(src, &pSave->field_0_header.field_0_frame_1_name[32], 8);
        Quicksave_SaveWorldInfo_4C9310(&pSave->field_204_world_info);
        pSave->field_45C_switch_states = sSwitchStates_5C1A28;

        BYTE* pDataIter = pSave->field_55C_objects_state_data;
        for (int idx = 0; idx < gBaseGameObject_list_BB47C4->Size(); idx++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (!(pObj->field_6_flags.Get(BaseGameObject::eDead)))
            {
                pDataIter += pObj->VGetSaveState(pDataIter);
            }
        }

        // Write a DWORD of 0
        pDataIter[0] = 0;
        pDataIter[1] = 0;
        pDataIter[2] = 0;
        pDataIter[3] = 0;

        Quicksave_SaveBlyData_4C9660(pDataIter + 4);
    }
}

void CC Quicksave_4C90D0()
{
    Game_ShowLoadingIcon_482D80();
    pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, 640, 240, 0);
    pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, 640, 240, 1);
    pScreenManager_5BB5F4->InvalidateRect_40EC90(0, 0, 640, 240, 2);
    Path_Get_Bly_Record_460F30(gMap_5C3030.sCurrentLevelId_5C3030, gMap_5C3030.sCurrentPathId_5C3032);
    Quicksave_SaveToMemory_4C91A0(&sActiveQuicksaveData_BAF7F8);
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
    gTotalMeterBars_5C1BFA = pInfo->field_2D_total_meter_bars;
    word_5C1BF8 = pInfo->field_30;
    dword_5C1BE8 = pInfo->field_38;
    gAbeBulletProof_5C1BDA = pInfo->field_3C_bBulletProof;
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
    pInfo->field_2D_total_meter_bars = gTotalMeterBars_5C1BFA;
    pInfo->field_30 = word_5C1BF8;
    pInfo->field_3C_bBulletProof = gAbeBulletProof_5C1BDA;
    pInfo->field_32_visited_bonewerks = sVisitedBonewerks_5C1C02;
    pInfo->field_34_visited_barracks = sVisitedBarracks_5C1C04;
    pInfo->field_36_visited_feeco_ender = sVisitedFeecoEnder_5C1C06;
    pInfo->field_38 = dword_5C1BE8;
    pInfo->field_C_controlled_x = FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos);
    pInfo->field_E_controlled_y = rect.h;
    pInfo->field_10_controlled_scale = sControlledCharacter_5C1B8C->field_CC_sprite_scale == FP_FromDouble(1.0);
}

EXPORT int CC Sort_comparitor_4D42C0(const void *pSaveRecLeft, const void *pSaveRecRight)
{
    const int leftTime = reinterpret_cast<const SaveFileRec*>(pSaveRecLeft)->field_20_lastWriteTimeStamp;
    const int rightTime = reinterpret_cast<const SaveFileRec*>(pSaveRecRight)->field_20_lastWriteTimeStamp;

    if (leftTime <= rightTime)
    {
        return leftTime < rightTime;
    }
    else
    {
        return -1;
    }
}

void CC Quicksave_FindSaves_4D4150()
{
    sTotalSaveFilesCount_BB43E0 = 0;

    IO_EnumerateDirectory("*.sav", [](const char* fileName, DWORD lastWriteTime)
    {
        if (sTotalSaveFilesCount_BB43E0 < 128)
        {
            size_t fileNameLen = strlen(fileName) - 4;
            if (fileNameLen > 0)
            {
                // Limit length to prevent buffer overflow
                if (fileNameLen > 20)
                {
                    fileNameLen = 20;
                }

                SaveFileRec* pRec = &sSaveFileRecords_BB31D8[sTotalSaveFilesCount_BB43E0];
                memcpy(pRec->field_0_fileName, fileName, fileNameLen);
                pRec->field_0_fileName[fileNameLen] = 0;

                pRec->field_20_lastWriteTimeStamp = lastWriteTime;
                sTotalSaveFilesCount_BB43E0++;
            }
        }

    });

    // Sort all we've found by time stamp, users probably want to load their last save first
    qsort(sSaveFileRecords_BB31D8, sTotalSaveFilesCount_BB43E0, sizeof(SaveFileRec), Sort_comparitor_4D42C0);

    // Underflow
    if (sSavedGameToLoadIdx_BB43FC < 0)
    {
        sSavedGameToLoadIdx_BB43FC = 0;
    }

    // Overflow
    if (sSavedGameToLoadIdx_BB43FC >= sTotalSaveFilesCount_BB43E0)
    {
        sSavedGameToLoadIdx_BB43FC = sTotalSaveFilesCount_BB43E0 - 1;
    }
}

namespace Test
{
    class AbeSave : public Abe
    {
    public:
    };

    static void Compare(Abe::Flags_1AC bitToSet, Abe_SaveState::Flags_D4 bitToExpect)
    {
        Shadow shadow = {};
        AbeSave abe = {};
        abe.field_E0_pShadow = &shadow;
        Abe_SaveState state = {};
        abe.field_1AC_flags.Set(bitToSet);
        abe.VGetSaveState(reinterpret_cast<BYTE*>(&state));
        ASSERT_TRUE(state.wordD4.Get(bitToExpect));
    }

    static void Compare(Abe::Flags_1AE bitToSet, Abe_SaveState::Flags_D4 bitToExpect)
    {
        Shadow shadow = {};
        AbeSave abe = {};
        abe.field_E0_pShadow = &shadow;
        Abe_SaveState state = {};
        abe.field_1AE.Set(bitToSet);
        abe.VGetSaveState(reinterpret_cast<BYTE*>(&state));
        ASSERT_TRUE(state.wordD4.Get(bitToExpect));
    }

    static void Compare(Flags_114 bitToSet, Abe_SaveState::Flags_D4 bitToExpect)
    {
        Shadow shadow = {};
        AbeSave abe = {};
        abe.field_E0_pShadow = &shadow;
        Abe_SaveState state = {};
        abe.field_114_flags.Set(bitToSet);
        abe.VGetSaveState(reinterpret_cast<BYTE*>(&state));
        ASSERT_TRUE(state.wordD4.Get(bitToExpect));
    }

    template<class TComp>
    static void Compare(Abe::Flags_1AC bitToSet, TComp comp)
    {
        Shadow shadow = {};
        AbeSave abe = {};
        abe.field_E0_pShadow = &shadow;
        Abe_SaveState state = {};
        abe.field_1AC_flags.Set(bitToSet);
        abe.VGetSaveState(reinterpret_cast<BYTE*>(&state));
        comp(state);
    }

    template<class TSet>
    static void Compare(TSet set, Abe_SaveState::Flags_D6 bitToExpect)
    {
        Shadow shadow = {};
        AbeSave abe = {};
        abe.field_E0_pShadow = &shadow;
        Abe_SaveState state = {};
        set(abe);
        abe.VGetSaveState(reinterpret_cast<BYTE*>(&state));
        ASSERT_TRUE(state.wordD6.Get(bitToExpect));
    }


    template<class TSet>
    static void Compare(TSet set, Abe_SaveState::Flags_D4 bitToExpect)
    {
        Shadow shadow = {};
        AbeSave abe = {};
        abe.field_E0_pShadow = &shadow;
        Abe_SaveState state = {};
        set(abe);
        abe.VGetSaveState(reinterpret_cast<BYTE*>(&state));
        ASSERT_TRUE(state.wordD4.Get(bitToExpect));
    }

    template<class TSet, class TCompare>
    static void Compare(TSet set, TCompare comp)
    {
        Shadow shadow = {};
        AbeSave abe = {};
        abe.field_E0_pShadow = &shadow;
        Abe_SaveState state = {};
        set(abe);
        abe.VGetSaveState(reinterpret_cast<BYTE*>(&state));
        comp(state);
    }

    static void TestAbeSave()
    {
        Compare(Abe::Flags_1AC::e1AC_Bit1, Abe_SaveState::Flags_D4::eD4_Bit1);
        Compare(Abe::Flags_1AC::e1AC_Bit2, Abe_SaveState::Flags_D4::eD4_Bit2);
        Compare(Abe::Flags_1AC::e1AC_Bit3_Fall_To_Well, Abe_SaveState::Flags_D4::eD4_Bit3);
        Compare(Abe::Flags_1AC::e1AC_Bit4, Abe_SaveState::Flags_D4::eD4_Bit4);
        Compare(Abe::Flags_1AC::e1AC_Bit5_bShrivel, [](Abe_SaveState& state) { ASSERT_EQ(state.bShrivel, 1); });
        Compare(Abe::Flags_1AC::e1AC_Bit6, Abe_SaveState::Flags_D4::eD4_Bit5);
        Compare(Abe::Flags_1AC::e1AC_Bit7, Abe_SaveState::Flags_D4::eD4_Bit6);
        Compare(Abe::Flags_1AC::e1AC_Bit8, Abe_SaveState::Flags_D4::eD4_Bit7);
        Compare(Abe::Flags_1AC::e1AC_Bit9_bLaughAtChantEnd, Abe_SaveState::Flags_D4::eD4_Bit8);
        // Abe::Flags_1AC::e1AC_Bit10 not persisted
        // Abe::Flags_1AC::e1AC_Bit11 not persisted
        Compare(Abe::Flags_1AC::e1AC_Bit12, Abe_SaveState::Flags_D4::eD4_Bit9);
        Compare(Abe::Flags_1AC::e1AC_eBit13, Abe_SaveState::Flags_D4::eD4_Bit10);
        Compare(Abe::Flags_1AC::e1AC_eBit14, Abe_SaveState::Flags_D4::eD4_Bit11);
        Compare(Abe::Flags_1AC::e1AC_eBit15_bHaveHealing, Abe_SaveState::Flags_D4::eD4_Bit12);
        // Flags_114::e114_Bit1 not persisted
        // Flags_114::e114_MotionChanged_Bit2 not persisted
        // Flags_114::e114_Bit3_Can_Be_Possessed not persisted
        // Flags_114::e114_Bit4_bPossesed not persisted
        // Flags_114::e114_Bit5 not persisted
        // Flags_114::e114_Bit6_SetOffExplosives not persisted
        // Flags_114::e114_Bit7_Electrocuted not persisted
        // Flags_114::e114_Bit8 not persisted
        // Flags_114::e114_Bit9 not persisted
        Compare(Flags_114::e114_Bit10, Abe_SaveState::Flags_D4::eD4_eBit13);
        // Flags_114::e114_Bit11 will crash as it attempts to iterate the object list to find the electrocute obj but it is not persisted directly
        Compare(Abe::Flags_1AC::e1AC_eBit16_bIsMudancheeVault_Ender, Abe_SaveState::Flags_D4::eD4_eBit14);
        Compare(Abe::Flags_1AE::e1AE_Bit1_bIsMudancheeVault_Ender, Abe_SaveState::Flags_D4::eD4_eBit15);
        // Abe::Flags_1AE::e1AE_Bit2_bDoQuickSave not persisted
        Compare([](Abe& abe) { abe.field_E0_pShadow->field_14_flags.Set(Shadow::eBit1_ShadowAtBottom); }, Abe_SaveState::Flags_D6::eD6_Bit1);

        Compare([](Abe& abe) { abe.field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX); }, [](Abe_SaveState& state) { ASSERT_EQ(state.bAnimFlipX, 1); });
        Compare([](Abe& abe) { abe.field_6_flags.Set(BaseGameObject::eDrawable); }, [](Abe_SaveState& state) { ASSERT_EQ(state.bDrawable, 1); });
        Compare([](Abe& abe) { abe.field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render); }, [](Abe_SaveState& state) { ASSERT_EQ(state.bAnimRender, 1); });
        Compare([](Abe& abe) { abe.field_114_flags.Set(Flags_114::e114_Bit7_Electrocuted); }, [](Abe_SaveState& state) { ASSERT_EQ(state.bElectrocuted, 1); });
        Compare([](Abe& abe) { abe.field_114_flags.Set(Flags_114::e114_Bit8_bInvisible); }, [](Abe_SaveState& state) { ASSERT_EQ(state.word42, 1); });


        // Needs resource manager and tons of other stuff, requires too many hacks to test this call
        //Abe::CreateFromSaveState_44D4F0(reinterpret_cast<const BYTE*>(&state));
    }

    void QuikSave_Tests()
    {
        TestAbeSave();
    }
}
