#include "stdafx.h"
#include "Path.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "AmbientSound.hpp"
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

void Path::Init_4DB200(const PathData* pPathData, LevelIds level, s16 path, s16 cameraId, u8** ppPathRes)
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

void Path::Loader_4DB800(s16 xpos, s16 ypos, LoadMode loadMode, TlvTypes typeToLoad)
{
    // Get a pointer to the array of index table offsets
    const s32* indexTable = reinterpret_cast<const s32*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);

    // Calculate the index of the offset we want for the given camera at x/y
    s32 objectTableIdx = indexTable[xpos + (ypos * field_6_cams_on_x)];
    if (objectTableIdx == -1)
    {
        // -1 means there are no objects for the given camera
        return;
    }

    u8* ptr = &(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + objectTableIdx];
    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    while (pPathTLV)
    {
        if (typeToLoad == TlvTypes::None_m1 || typeToLoad == pPathTLV->field_4_type.mType)
        {
            if (loadMode != LoadMode::Mode_0 || !(pPathTLV->field_0_flags.Get(TLV_Flags::eBit1_Created) || pPathTLV->field_0_flags.Get(TLV_Flags::eBit2_Unknown)))
            {
                if (loadMode == LoadMode::Mode_0)
                {
                    pPathTLV->field_0_flags.Set(TLV_Flags::eBit1_Created);
                    pPathTLV->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
                }

                TlvItemInfoUnion data;
                data.parts.tlvOffset = static_cast<u16>(objectTableIdx);
                data.parts.levelId = static_cast<u8>(field_0_levelId);
                data.parts.pathId = static_cast<u8>(field_2_pathId);

                // Call the factory to construct the item
                field_C_pPathData->field_1E_object_funcs.object_funcs[static_cast<s32>(pPathTLV->field_4_type.mType)](pPathTLV, this, data, loadMode);
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

Path_TLV* Path::Get_First_TLV_For_Offsetted_Camera_4DB610(s16 cam_x_idx, s16 cam_y_idx)
{
    const s32 camY = cam_y_idx + gMap_5C3030.field_D2_cam_y_idx;
    const s32 camX = cam_x_idx + gMap_5C3030.field_D0_cam_x_idx;
    if (camX >= field_6_cams_on_x || camX < 0 || camY >= field_8_cams_on_y || camY < 0)
    {
        return nullptr;
    }

    const s32* indexTable = reinterpret_cast<const s32*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);
    const s32 indexTableEntry = indexTable[(camX + (camY * field_6_cams_on_x))];
    if (indexTableEntry == -1)
    {
        return nullptr;
    }

    return reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + indexTableEntry]);
}

Path_TLV* CCSTD Path::Next_TLV_4DB6A0(Path_TLV* pTlv)
{
    if (pTlv->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
    {
        return nullptr;
    }

    // Skip length bytes to get to the start of the next TLV
    u8* ptr = reinterpret_cast<u8*>(pTlv);
    u8* pNext = ptr + pTlv->field_2_length;
    return reinterpret_cast<Path_TLV*>(pNext);
}

Path_TLV* Path::TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes objectType, s16 camX)
{
    Path_TLV* pTlv = Get_First_TLV_For_Offsetted_Camera_4DB610(camX, 0);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->field_4_type.mType != objectType)
    {
        pTlv = Next_TLV_4DB6A0(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }
    return pTlv;
}

Path_TLV* Path::TLV_Get_At_4DB4B0(s16 xpos, s16 ypos, s16 width, s16 height, TlvTypes objectType)
{
    // TODO: Can be refactored to use min/max
    s16 right = 0;
    s16 left = 0;

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

    s16 top = 0;
    s16 bottom = 0;

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

    const s32 grid_cell_y = (top + bottom) / (2 * field_C_pPathData->field_C_grid_height);
    const s32 grid_cell_x = (right + left) / (2 * field_C_pPathData->field_A_grid_width);

    // Check within map bounds
    if (grid_cell_x >= field_6_cams_on_x)
    {
        return nullptr;
    }

    if (grid_cell_y >= field_8_cams_on_y)
    {
        return nullptr;
    }

    // Get the offset to where the TLV list starts for this camera cell
    const s32* indexTable = reinterpret_cast<const s32*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);
    const s32 indexTableEntry = indexTable[(grid_cell_x + (grid_cell_y * field_6_cams_on_x))];
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

Path_TLV* Path::TLV_Get_At_4DB290(Path_TLV* pTlv, FP xpos, FP ypos, FP w, FP h)
{
    const s32 xpos_converted = FP_GetExponent(xpos);
    const s32 ypos_converted = FP_GetExponent(ypos);
    s32 width_converted = FP_GetExponent(w);
    s32 height_converted = FP_GetExponent(h);

    s32 xyPosValid = 1;
    if (xpos_converted < 0 || ypos_converted < 0)
    {
        xyPosValid = 0;
    }

    if (width_converted < 0 || height_converted < 0)
    {
        width_converted = xpos_converted;
        height_converted = ypos_converted;
    }

    if (!pTlv)
    {
        const PathData* pPathData = field_C_pPathData;
        const s32 camX = (xpos_converted + width_converted) / (2 * pPathData->field_A_grid_width);
        const s32 camY = (ypos_converted + height_converted) / (2 * pPathData->field_C_grid_height);

        if (camX >= field_6_cams_on_x || camY >= field_8_cams_on_y)
        {
            return nullptr;
        }

        if (camX < 0 || camY < 0)
        {
            return nullptr;
        }

        u8* pPathRes = *field_10_ppRes;
        const s32* pIndexTable = reinterpret_cast<const s32*>(pPathRes + pPathData->field_16_object_indextable_offset);
        const s32 indexTableEntry = pIndexTable[camX + (field_6_cams_on_x * camY)];

        if (indexTableEntry == -1)
        {
            return nullptr;
        }

        pTlv = reinterpret_cast<Path_TLV*>(&pPathRes[pPathData->field_12_object_offset + indexTableEntry]);
        if (!xyPosValid || (xpos_converted <= pTlv->field_C_bottom_right.field_0_x && width_converted >= pTlv->field_8_top_left.field_0_x && height_converted >= pTlv->field_8_top_left.field_2_y && ypos_converted <= pTlv->field_C_bottom_right.field_2_y))
        {
            return pTlv;
        }
    }

    if (pTlv->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
    {
        return nullptr;
    }

    while (1)
    {
        pTlv = Path::Next_TLV_4DB6A0(pTlv);
        if (!xyPosValid || (xpos_converted <= pTlv->field_C_bottom_right.field_0_x && width_converted >= pTlv->field_8_top_left.field_0_x && height_converted >= pTlv->field_8_top_left.field_2_y && ypos_converted <= pTlv->field_C_bottom_right.field_2_y))
        {
            break;
        }

        if (pTlv->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
        {
            return nullptr;
        }
    }

    return pTlv;
}

Path_TLV* Path::TLV_From_Offset_Lvl_Cam_4DB770(u32 tlvOffset_levelId_PathId)
{
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<s32>(field_0_levelId) && data.parts.pathId == field_2_pathId)
    {
        return reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + data.parts.tlvOffset]);
    }
    else
    {
        return nullptr;
    }
}

u32 Path::TLVInfo_From_TLVPtr_4DB7C0(Path_TLV* pTlv)
{
    TlvItemInfoUnion data;
    data.parts.levelId = static_cast<u8>(field_0_levelId);
    data.parts.pathId = static_cast<u8>(field_2_pathId);

    // Num bytes into the path res block
    const std::size_t diff = reinterpret_cast<u8*>(pTlv) - (*field_10_ppRes);

    // Sub off the offset from the start of the path block to TLV data
    data.parts.tlvOffset = static_cast<u16>(diff - field_C_pPathData->field_12_object_offset);
    return data.all;
}

Path_TLV* CCSTD Path::TLV_Next_Of_Type_4DB720(Path_TLV* pTlv, TlvTypes type)
{
    pTlv = Path::Next_TLV_4DB6A0(pTlv);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->field_4_type.mType != type)
    {
        pTlv = Path::Next_TLV_4DB6A0(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }

    return pTlv;
}

EXPORT void CCSTD Path::TLV_Reset_4DB8E0(u32 tlvOffset_levelId_PathId, s16 hiFlags, s8 bSetCreated, s8 bBit2)
{
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<s32>(gMap_5C3030.field_0_current_level))
    {
        const PathBlyRec* pBlyRec = Path_Get_Bly_Record_460F30(static_cast<LevelIds>(data.parts.levelId), data.parts.pathId);
        u8** ppPathRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, data.parts.pathId, TRUE, FALSE);
        if (ppPathRes)
        {
            const s32 tlvOffset = data.parts.tlvOffset + pBlyRec->field_4_pPathData->field_12_object_offset;
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
                // Seems to be a blob per TLV specific bits
                pTlv->field_1_tlv_state = static_cast<u8>(hiFlags);
            }
            ResourceManager::FreeResource_49C330(ppPathRes);
        }
    }
}

EXPORT void CC Path::Start_Sounds_For_Objects_In_Camera_4CBAF0(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx)
{
    Path_TLV* pTlv = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(cam_x_idx, cam_y_idx);
    while (pTlv)
    {
        if (!(pTlv->field_0_flags.Get(TLV_Flags::eBit1_Created) || (pTlv->field_0_flags.Get(TLV_Flags::eBit2_Unknown))))
        {
            Start_Sounds_for_TLV_4CB530(direction, pTlv);
        }
        pTlv = Next_TLV_4DB6A0(pTlv);
    }
}

EXPORT void CCSTD Path::Reset_TLVs_4DBCF0(u16 pathId)
{
    const PathData* pPathData = Path_Get_Bly_Record_460F30(gMap_5C3030.field_0_current_level, pathId)->field_4_pPathData;
    const s32 camsX = (pPathData->field_4_bTop - pPathData->field_0_bLeft) / pPathData->field_A_grid_width;
    const s32 camsY = (pPathData->field_6_bBottom - pPathData->field_2_bRight) / pPathData->field_C_grid_height;
    u8** ppPath = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, pathId, TRUE, FALSE);
    if (ppPath)
    {
        const s32 totalCams = camsX * camsY;
        const s32* pIdx = reinterpret_cast<s32*>(&(*ppPath)[pPathData->field_16_object_indextable_offset]);
        for (s32 i = 0; i < totalCams; i++)
        {
            if (pIdx[i] != -1)
            {
                Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(&(*ppPath[pIdx[i] + pPathData->field_12_object_offset]));
                while (pTlv)
                {
                    pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                    pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Unknown);
                    pTlv = Path::Next_TLV_4DB6A0(pTlv);
                }
            }
        }
        ResourceManager::FreeResource_49C330(ppPath);
    }
}
