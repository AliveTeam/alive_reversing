#include "stdafx.h"
#include "Quicksave.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "Path.hpp"
#include "PathData.hpp"
#include "Map.hpp"

int CC LiftMover__CreateFromSaveState_40D180(const BYTE*) { return 12; }
int CC CreateFromSaveState_412C10(const BYTE*) { return 60; }
int CC CreateFromSaveState_417740(const BYTE*) { return 8; }
int CC NakedSlig__CreateFromSaveState_41AE80(const BYTE*) { return 128; }
int CC Grinder__CreateFromSaveState_421600(const BYTE*) { return 20; }
int CC EvilFart__CreateFromSaveState_4281C0(const BYTE*) { return 60; }
int CC Fleech__CreateFromSaveState_42DD50(const BYTE*) { return 180; }
int CC FlyingSlig__CreateFromSaveState_437E40(const BYTE*) { return 172; }
int CC FlyingSligSpawner__CreateFromSaveState_43B690(const BYTE*) { return 16; }
int CC CreateFromSaveState_43BD10(const BYTE*) { return 16; }
int CC CreateFromSaveState_43DC20(const BYTE*) { return 4; }
int CC SlapLock__CreateFromSaveState_43EA00(const BYTE*) { return 24; }
int CC Greeter__CreateFromSaveState_446040(const BYTE*) { return 80; }
int CC Grenade__CreateFromSaveState_449410(const BYTE*) { return 60; }
int CC Glukkon__CreateFromSaveState_442830(const BYTE*) { return 144; }
int CC Abe__CreateFromSaveState_44D4F0(const BYTE*) { return 216; }
int CC SligSpawner__CreateFromSaveState_409B10(const BYTE*) { return 16; }
int CC LiftPoint__CreateFromSaveState_4630F0(const BYTE*) { return 28; }
int CC Mudokon__CreateFromSaveState_4717C0(const BYTE*) { return 136; }
int CC CreateFromSaveState_46A9E0(const BYTE*) { return 60; }
int CC MineCar__CreateFromSaveState_467740(const BYTE*) { return 104; }
int CC Paramite__CreateFromSaveState_4855A0(const BYTE*) { return 120; }
int CC BirdPortal__CreateFromSaveState_499C90(const BYTE*) { return 8; }
int CC CreateFromSaveState_49B200(const BYTE*) { return 4; }
int CC AbilityRing__CreateFromSaveState_49DF90(const BYTE*) { return 40; }
int CC Rock__CreateFromSaveState_49F720(const BYTE*) { return 56; }
int CC Scrab__CreateFromSaveState_4A70A0(const BYTE*) { return 160; }
int CC ScrabSpawner__CreateFromSaveState_4ABEB0(const BYTE*) { return 16; }
int CC SlamDoor__CreateFromSaveState_4C08B0(const BYTE*) { return 8; }
int CC Slig__CreateFromSaveState_4B3B50(const BYTE*) { return 164; }
int CC Slog__CreateFromSaveState_4C54F0(const BYTE*) { return 120; }
int CC Slurg__CreateFromSaveState_4C8DF0(const BYTE*) { return 44; }
int CC TimerTrigger__CreateFromSaveState_4CDF70(const BYTE*) { return 16; }
int CC TrapDoor__CreateFromSaveState_4DDED0(const BYTE*) { return 12; }
int CC Uxb__CreateFromSaveState_4DFAE0(const BYTE*) { return 24; }
int CC WorkWheel__CreateFromSaveState_4E3B10(const BYTE*) { return 16; }

struct QuickSaveRestoreTable
{
    int(CC* mFns[180])(const BYTE *);
};

QuickSaveRestoreTable sQuicksaveLoadFunctionTable =
{
    NULL,
    NULL,
    &SligSpawner__CreateFromSaveState_409B10,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &LiftMover__CreateFromSaveState_40D180,
    NULL,
    &CreateFromSaveState_412C10,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &CreateFromSaveState_417740,
    &NakedSlig__CreateFromSaveState_41AE80,
    NULL,
    NULL,
    NULL,
    &Grinder__CreateFromSaveState_421600,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &EvilFart__CreateFromSaveState_4281C0,
    NULL,
    NULL,
    NULL,
    NULL,
    &Fleech__CreateFromSaveState_42DD50,
    NULL,
    NULL,
    NULL,
    &FlyingSlig__CreateFromSaveState_437E40,
    &FlyingSligSpawner__CreateFromSaveState_43B690,
    NULL,
    &CreateFromSaveState_43BD10,
    NULL,
    NULL,
    &CreateFromSaveState_43DC20,
    &SlapLock__CreateFromSaveState_43EA00,
    NULL,
    NULL,
    &Greeter__CreateFromSaveState_446040,
    &Grenade__CreateFromSaveState_449410,
    NULL,
    &Glukkon__CreateFromSaveState_442830,
    NULL,
    &Abe__CreateFromSaveState_44D4F0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &LiftPoint__CreateFromSaveState_4630F0,
    NULL,
    NULL,
    &Mudokon__CreateFromSaveState_4717C0,
    NULL,
    NULL,
    &CreateFromSaveState_46A9E0,
    NULL,
    NULL,
    NULL,
    NULL,
    &MineCar__CreateFromSaveState_467740,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &Paramite__CreateFromSaveState_4855A0,
    NULL,
    NULL,
    &BirdPortal__CreateFromSaveState_499C90,
    NULL,
    NULL,
    &CreateFromSaveState_49B200,
    NULL,
    &AbilityRing__CreateFromSaveState_49DF90,
    &Rock__CreateFromSaveState_49F720,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &Scrab__CreateFromSaveState_4A70A0,
    &ScrabSpawner__CreateFromSaveState_4ABEB0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &SlamDoor__CreateFromSaveState_4C08B0,
    NULL,
    NULL,
    &Slig__CreateFromSaveState_4B3B50,
    &Slog__CreateFromSaveState_4C54F0,
    NULL,
    NULL,
    &Slurg__CreateFromSaveState_4C8DF0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &TimerTrigger__CreateFromSaveState_4CDF70,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &TrapDoor__CreateFromSaveState_4DDED0,
    &Uxb__CreateFromSaveState_4DFAE0,
    NULL,
    NULL,
    NULL,
    NULL,
    &WorkWheel__CreateFromSaveState_4E3B10,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
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

void QuikSave_RestoreObjectStates_D481890_4C9BE0(const BYTE* pSaveData)
{
    const WORD* pSaveData2 = reinterpret_cast<const WORD*>(pSaveData);

    while (*reinterpret_cast<const DWORD*>(pSaveData2) != 0)
    {
        pSaveData2 += sQuicksaveLoadFunctionTable_560C34.mFns[*pSaveData2](reinterpret_cast<const BYTE*>(pSaveData2)) / sizeof(WORD);
    }

    // Skip the 2 zero entries, the saved flag words come after the object save state data
    const WORD* pSrcFlags = pSaveData2 += 2;
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
                            const BYTE tableValue = kObjectTypeAttributesTable_byte_547794.mTypes[pTlv->field_4_type];
                            if (tableValue)
                            {
                                if (tableValue <= 2)
                                {
                                    pTlv->field_0_flags = *pSrcFlags;
                                    ++pSrcFlags;
                                }
                            }

                            if (pTlv->field_0_flags & 4)
                            {
                                // End of restoring flags for this camera?
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
