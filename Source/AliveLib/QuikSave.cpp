#include "stdafx.h"
#include "Quicksave.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "Path.hpp"
#include "PathData.hpp"
#include "Map.hpp"

struct QuickSaveRestoreTable
{
    int(CC* mFns[180])(const BYTE *);
};
ALIVE_VAR(1, 0x560c34, QuickSaveRestoreTable, sQuicksaveLoadFunctionTable_560C34, {}); // TODO: Rip table

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
