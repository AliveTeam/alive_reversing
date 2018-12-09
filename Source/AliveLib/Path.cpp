#include "stdafx.h"
#include "Path.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include <assert.h>

ALIVE_VAR(1, 0xbb47c0, Path*, sPath_dword_BB47C0, nullptr);

void Path::ctor_4DB170()
{
    field_C_pPathData = nullptr;
    field_10_ppRes = 0;
    field_8_cams_on_y = 0;
    field_6_cams_on_x = 0;
    field_4_cameraId = 0;
    field_2_pathId = 0;
    field_0_levelId = LevelIds::eMenu_0;
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
    field_0_levelId = LevelIds::eMenu_0;
}

void Path::Init_4DB200(const PathData* pPathData, LevelIds level, __int16 path, __int16 cameraId, BYTE** ppPathRes)
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

void Path::Loader_4DB800(__int16 xpos, __int16 ypos, __int16 loadMode, __int16 typeToLoad)
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
        if (typeToLoad == -1 || typeToLoad == pPathTLV->field_4_type) 
        {
            if (loadMode || !(pPathTLV->field_0_flags.Get(TLV_Flags::eBit1_Created) || pPathTLV->field_0_flags.Get(TLV_Flags::eBit2_Unknown)))
            {
                if (!loadMode)
                {
                    pPathTLV->field_0_flags.Set(TLV_Flags::eBit1_Created);
                    pPathTLV->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
                }

                TlvItemInfoUnion data;
                data.parts.tlvOffset = static_cast<WORD>(objectTableIdx);
                data.parts.levelId = static_cast<BYTE>(field_0_levelId);
                data.parts.pathId = static_cast<BYTE>(field_2_pathId);

                // Call the factory to construct the item
                field_C_pPathData->field_1E_object_funcs.object_funcs[pPathTLV->field_4_type](pPathTLV, this, data, loadMode);
            }
        }

        // End of TLV list for current camera
        if (pPathTLV->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
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
    if (pTlv->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
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

Path_TLV* Path::TLV_Get_At_4DB4B0(__int16 xpos, __int16 ypos, __int16 width, __int16 height, unsigned __int16 objectType)
{
    // TODO: Can be refactored to use min/max
    __int16 right = 0;
    __int16 left = 0;

    if (xpos >= width)
    {
        right = width;
        left = xpos;
    }
    else
    {
        right = xpos;
        left = width;
    }

    __int16 top = 0;
    __int16 bottom = 0;

    if (ypos >= height)
    {
        top = height;
        bottom = ypos;
    }
    else
    {
        top = ypos;
        bottom = height;
    }

    const int gird_cell_y = (top + bottom) / (2 * field_C_pPathData->field_C_grid_height);
    const int grid_cell_x = (right + left) / (2 * field_C_pPathData->field_A_grid_width);
    
    // Check within map bounds
    if (grid_cell_x >=field_6_cams_on_x)
    {
        return nullptr;
    }

    if (gird_cell_y >= field_8_cams_on_y)
    {
        return nullptr;
    }

    // Get the offset to where the TLV list starts for this camera cell
    const int* indexTable = reinterpret_cast<const int*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);
    const int indexTableEntry = indexTable[(grid_cell_x + (gird_cell_y * field_6_cams_on_x))];
    if (indexTableEntry == -1)
    {
        return nullptr;
    }

    // Iterate all TLVs for this cell till we find one that matches the type and is within the TLV bounding rect
    Path_TLV* pTlvIter = reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + indexTableEntry]);
    while (pTlvIter)
    {
        if (pTlvIter->field_4_type == objectType
            && right <= pTlvIter->field_C_bottom_right.field_0_x
            && left >= pTlvIter->field_8_top_left.field_0_x
            && bottom >= pTlvIter->field_8_top_left.field_2_y
            && top <= pTlvIter->field_C_bottom_right.field_2_y)
        {
            return pTlvIter;
        }
        pTlvIter = Path::Next_TLV_4DB6A0(pTlvIter);
    }

    return pTlvIter;
}

Path_TLV* Path::TLV_Get_At_4DB290(Path_TLV* /*pTlv*/, FP /*xpos*/, FP /*ypos*/, FP /*w*/, FP /*h*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

Path_TLV * Path::TLV_From_Offset_Lvl_Cam_4DB770(unsigned int tlvOffset_levelId_PathId)
{
    NOT_IMPLEMENTED();
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<int>(field_0_levelId) && data.parts.pathId == field_2_pathId)
    {
        return reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + data.parts.tlvOffset]);
    }
    else
    {
        return nullptr;
    }
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

EXPORT void __stdcall Path::TLV_Reset_4DB8E0(unsigned int tlvOffset_levelId_PathId, __int16 hiFlags, char bSetCreated, char bBit2)
{
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030))
    {
        const PathBlyRec* pBlyRec = Path_Get_Bly_Record_460F30(static_cast<LevelIds>(data.parts.levelId), data.parts.pathId);
        BYTE** ppPathRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, data.parts.pathId, TRUE, FALSE);
        if (ppPathRes)
        {
            const int tlvOffset = data.parts.tlvOffset + pBlyRec->field_4_pPathData->field_12_object_offset;
            Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(&(*ppPathRes)[tlvOffset]);
            
            if (bBit2 & 1)
            {
                pTlv->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
            }
            else
            {
                pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Unknown);
            }

            if (bSetCreated & 1)
            {
                pTlv->field_0_flags.Set(TLV_Flags::eBit1_Created);
            }
            else
            {
                pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
            }

            if (hiFlags != -1)
            {
                // NOTE: Real game seems to have this as top 8 flags.. but only used as a byte ??
                // Seems to be a blob per TLV specific bits
                pTlv->field_1_unknown = static_cast<BYTE>(hiFlags);
            }
            ResourceManager::FreeResource_49C330(ppPathRes);
        }
    }
}

EXPORT void CC Start_Sounds_for_TLV_4CB530(__int16 /*type*/, Path_TLV* /*pTlv*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(__int16 type, __int16 cam_x_idx, __int16 cam_y_idx)
{
    Path_TLV* pTlv = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(cam_x_idx, cam_y_idx);
    while (pTlv)
    {
        if (!(pTlv->field_0_flags.Get(TLV_Flags::eBit1_Created) || (pTlv->field_0_flags.Get(TLV_Flags::eBit2_Unknown))))
        {
            Start_Sounds_for_TLV_4CB530(type, pTlv);
        }
        pTlv = Next_TLV_4DB6A0(pTlv);
    }
}
