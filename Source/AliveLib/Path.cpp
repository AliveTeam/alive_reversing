#include "stdafx.h"
#include "Path.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "PathData.hpp"
#include "Map.hpp"

const unsigned short Path_Door::kType = 5;
const unsigned short Path_Teleporter::kType = 88;

void Path::ctor_4DB170()
{
    field_C_pPathData = nullptr;
    field_10_ppRes = 0;
    field_8_cams_on_y = 0;
    field_6_cams_on_x = 0;
    field_4_cameraId = 0;
    field_2_pathId = 0;
    field_0_levelId = 0;
}

void Path::dtor_4DB1A0()
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
}

void Path::Free_4DB1C0()
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
    field_C_pPathData = 0;
    field_10_ppRes = 0;
    field_8_cams_on_y = 0;
    field_6_cams_on_x = 0;
    field_4_cameraId = 0;
    field_2_pathId = 0;
    field_0_levelId = 0;
}

void Path::Init_4DB200(const PathData* pPathData, __int16 level, __int16 path, __int16 cameraId, BYTE** ppPathRes)
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
    field_10_ppRes = ppPathRes;
    ResourceManager::Inc_Ref_Count_49C310(ppPathRes);

    field_4_cameraId = cameraId;
    field_0_levelId = level;
    field_2_pathId = path;

    field_C_pPathData = pPathData;
    field_6_cams_on_x = (field_C_pPathData->field_4_bTop - field_C_pPathData->field_0_bLeft) / field_C_pPathData->field_A_grid_width;
    field_8_cams_on_y = (field_C_pPathData->field_6_bBottom - field_C_pPathData->field_2_bRight) / field_C_pPathData->field_C_grid_height;
}

void Path::Loader_4DB800(__int16 xpos, __int16 ypos, int loadMode, __int16 typeToLoad)
{
    // Get a pointer to the array of index table offsets
    const int* indexTable = reinterpret_cast<const int*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);

    // Calculate the index of the offset we want for the given camera at x/y
    int objectTableIdx = indexTable[xpos + (ypos * field_6_cams_on_x)];
    if (objectTableIdx == -1)
    {
        // -1 means there are no objects for the given camera
        return;
    }

    BYTE* ptr = &(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + objectTableIdx];
    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    while (pPathTLV)
    {
        if ((typeToLoad == -1 || typeToLoad == pPathTLV->field_4_type) && ((WORD)loadMode || !(pPathTLV->field_0_flags & 3)))
        {
            void(__cdecl *pPathFnTable)(Path_TLV*, Path*, DWORD, __int16);
            pPathFnTable = (decltype(pPathFnTable))field_C_pPathData->field_1E_object_funcs.object_funcs[pPathTLV->field_4_type];

            if (!(WORD)loadMode)
            {
                pPathTLV->field_0_flags |= 3u;
            }

            pPathFnTable(
                pPathTLV,
                this,
                objectTableIdx | ((field_0_levelId | (field_2_pathId << 8)) << 16),
                static_cast<short>(loadMode));
        }

        // End of TLV list for current camera
        if (pPathTLV->field_0_flags & 4)
        {
            break;
        }

        objectTableIdx += pPathTLV->field_2_length;
        pPathTLV = Next_TLV_4DB6A0(pPathTLV);
    }
}

Path_TLV* Path::Get_First_TLV_For_Offsetted_Camera_4DB610(__int16 cam_x_idx, __int16 cam_y_idx)
{
    const int camY = cam_y_idx + gMap_5C3030.field_D2_cam_y_idx;
    const int camX = cam_x_idx + gMap_5C3030.field_D0_cam_x_idx;
    if (camX >= field_6_cams_on_x || camX < 0 || camY >= field_8_cams_on_y || camY < 0)
    {
        return nullptr;
    }

    const int* indexTable = reinterpret_cast<const int*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);
    const int indexTableEntry = indexTable[(camX + (camY * field_6_cams_on_x))];
    if (indexTableEntry == -1)
    {
        return nullptr;
    }

    return reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + indexTableEntry]);
}

Path_TLV* __stdcall Path::Next_TLV_4DB6A0(Path_TLV* pTlv)
{
    if (pTlv->field_0_flags & 4)
    {
        return nullptr;
    }

    // Skip length bytes to get to the start of the next TLV
    BYTE* ptr = reinterpret_cast<BYTE*>(pTlv);
    BYTE* pNext = ptr + pTlv->field_2_length;
    return reinterpret_cast<Path_TLV*>(pNext);
}

Path_TLV* Path::TLV_First_Of_Type_In_Camera_4DB6D0(unsigned __int16 objectType, __int16 camX)
{
    Path_TLV* pTlv = Path::Get_First_TLV_For_Offsetted_Camera_4DB610(camX, 0);
    if (!pTlv)
    {
        return 0;
    }

    while (pTlv->field_4_type != objectType)
    {
        pTlv = Path::Next_TLV_4DB6A0(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }
    return pTlv;
}

Path_TLV* __stdcall Path::TLV_Next_Of_Type_4DB720(Path_TLV* pTlv, unsigned __int16 type)
{
    pTlv = Path::Next_TLV_4DB6A0(pTlv);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->field_4_type != type)
    {
        pTlv = Path::Next_TLV_4DB6A0(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }

    return pTlv;
}
