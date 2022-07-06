#include "stdafx.h"
#include "Path.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "AmbientSound.hpp"
#include <assert.h>

ALIVE_VAR(1, 0xbb47c0, Path*, sPathInfo, nullptr);

Path::Path()
{
    mPathData = nullptr;
    field_10_ppRes = nullptr;
    mCamsOnY = 0;
    mCamsOnX = 0;
    mCameraId = 0;
    mPathId = 0;
    mLevelId = EReliveLevelIds::eMenu;
}

Path::~Path()
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
}

void Path::Free()
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
    mPathData = 0;
    field_10_ppRes = nullptr;
    mCamsOnY = 0;
    mCamsOnX = 0;
    mCameraId = 0;
    mPathId = 0;
    mLevelId = EReliveLevelIds::eMenu;
}

void Path::Init(const PathData* pPathData, EReliveLevelIds level, s16 path, s16 cameraId, u8** ppPathRes)
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
    field_10_ppRes = ppPathRes;
    ResourceManager::Inc_Ref_Count_49C310(ppPathRes);

    mCameraId = cameraId;
    mLevelId = level;
    mPathId = path;

    mPathData = pPathData;
    mCamsOnX = (mPathData->field_4_bTop - mPathData->field_0_bLeft) / mPathData->field_A_grid_width;
    mCamsOnY = (mPathData->field_6_bBottom - mPathData->field_2_bRight) / mPathData->field_C_grid_height;
}

void Path::Loader_4DB800(s16 xpos, s16 ypos, LoadMode loadMode, TlvTypes typeToLoad)
{
    // Get a pointer to the array of index table offsets
    const s32* indexTable = reinterpret_cast<const s32*>(*field_10_ppRes + mPathData->field_16_object_indextable_offset);

    // Calculate the index of the offset we want for the given camera at x/y
    s32 objectTableIdx = indexTable[xpos + (ypos * mCamsOnX)];
    if (objectTableIdx == -1)
    {
        // -1 means there are no objects for the given camera
        return;
    }

    u8* ptr = &(*field_10_ppRes)[mPathData->field_12_object_offset + objectTableIdx];
    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    while (pPathTLV)
    {
        if (typeToLoad == TlvTypes::None_m1 || typeToLoad == pPathTLV->mTlvType32.mType)
        {
            if (loadMode != LoadMode::ConstructObject_0 || !(pPathTLV->mTlvFlags.Get(TlvFlags::eBit1_Created) || pPathTLV->mTlvFlags.Get(TlvFlags::eBit2_Destroyed)))
            {
                if (loadMode == LoadMode::ConstructObject_0)
                {
                    pPathTLV->mTlvFlags.Set(TlvFlags::eBit1_Created);
                    pPathTLV->mTlvFlags.Set(TlvFlags::eBit2_Destroyed);
                }

                TlvItemInfoUnion data;
                data.parts.tlvOffset = static_cast<u16>(objectTableIdx);
                data.parts.levelId = static_cast<u8>(MapWrapper::ToAE(mLevelId));
                data.parts.pathId = static_cast<u8>(mPathId);

                // Call the factory to construct the item
                mPathData->field_1E_object_funcs.object_funcs[static_cast<s32>(pPathTLV->mTlvType32.mType)](pPathTLV, this, data, loadMode);
            }
        }

        // End of TLV list for current camera
        if (pPathTLV->mTlvFlags.Get(TlvFlags::eBit3_End_TLV_List))
        {
            break;
        }

        objectTableIdx += pPathTLV->mLength;
        pPathTLV = Next_TLV(pPathTLV);
    }
}

Path_TLV* Path::Get_First_TLV_For_Offsetted_Camera(s16 cam_x_idx, s16 cam_y_idx)
{
    const s32 camY = cam_y_idx + gMap.mCamIdxOnY;
    const s32 camX = cam_x_idx + gMap.mCamIdxOnX;
    if (camX >= mCamsOnX || camX < 0 || camY >= mCamsOnY || camY < 0)
    {
        return nullptr;
    }

    const s32* indexTable = reinterpret_cast<const s32*>(*field_10_ppRes + mPathData->field_16_object_indextable_offset);
    const s32 indexTableEntry = indexTable[(camX + (camY * mCamsOnX))];
    if (indexTableEntry == -1)
    {
        return nullptr;
    }

    return reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[mPathData->field_12_object_offset + indexTableEntry]);
}

Path_TLV* Path::Next_TLV(Path_TLV* pTlv)
{
    return Next_TLV_Impl(pTlv);
}

Path_TLV* Path::TLV_First_Of_Type_In_Camera(TlvTypes objectType, s16 camX)
{
    Path_TLV* pTlv = Get_First_TLV_For_Offsetted_Camera(camX, 0);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->mTlvType32.mType != objectType)
    {
        pTlv = Next_TLV(pTlv);
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

    const s32 grid_cell_y = (top + bottom) / (2 * mPathData->field_C_grid_height);
    const s32 grid_cell_x = (right + left) / (2 * mPathData->field_A_grid_width);

    // Check within map bounds
    if (grid_cell_x >= mCamsOnX)
    {
        return nullptr;
    }

    if (grid_cell_y >= mCamsOnY)
    {
        return nullptr;
    }

    // Get the offset to where the TLV list starts for this camera cell
    const s32* indexTable = reinterpret_cast<const s32*>(*field_10_ppRes + mPathData->field_16_object_indextable_offset);
    const s32 indexTableEntry = indexTable[(grid_cell_x + (grid_cell_y * mCamsOnX))];
    if (indexTableEntry == -1)
    {
        return nullptr;
    }

    // Iterate all TLVs for this cell till we find one that matches the type and is within the TLV bounding rect
    Path_TLV* pTlvIter = reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[mPathData->field_12_object_offset + indexTableEntry]);
    while (pTlvIter)
    {
        if (pTlvIter->mTlvType32 == objectType
            && right <= pTlvIter->mBottomRight.x
            && left >= pTlvIter->mTopLeft.x
            && bottom >= pTlvIter->mTopLeft.y
            && top <= pTlvIter->mBottomRight.y)
        {
            return pTlvIter;
        }
        pTlvIter = Path::Next_TLV(pTlvIter);
    }

    return pTlvIter;
}

Path_TLV* Path::TlvGetAt(Path_TLV* pTlv, FP xpos, FP ypos, FP w, FP h)
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
        const PathData* pPathData = mPathData;
        const s32 camX = (xpos_converted + width_converted) / (2 * pPathData->field_A_grid_width);
        const s32 camY = (ypos_converted + height_converted) / (2 * pPathData->field_C_grid_height);

        if (camX >= mCamsOnX || camY >= mCamsOnY)
        {
            return nullptr;
        }

        if (camX < 0 || camY < 0)
        {
            return nullptr;
        }

        u8* pPathRes = *field_10_ppRes;
        const s32* pIndexTable = reinterpret_cast<const s32*>(pPathRes + pPathData->field_16_object_indextable_offset);
        const s32 indexTableEntry = pIndexTable[camX + (mCamsOnX * camY)];

        if (indexTableEntry == -1)
        {
            return nullptr;
        }

        pTlv = reinterpret_cast<Path_TLV*>(&pPathRes[pPathData->field_12_object_offset + indexTableEntry]);
        if (!xyPosValid || (xpos_converted <= pTlv->mBottomRight.x && width_converted >= pTlv->mTopLeft.x && height_converted >= pTlv->mTopLeft.y && ypos_converted <= pTlv->mBottomRight.y))
        {
            return pTlv;
        }
    }

    if (pTlv->mTlvFlags.Get(TlvFlags::eBit3_End_TLV_List))
    {
        return nullptr;
    }

    while (1)
    {
        pTlv = Path::Next_TLV(pTlv);
        if (!xyPosValid || (xpos_converted <= pTlv->mBottomRight.x && width_converted >= pTlv->mTopLeft.x && height_converted >= pTlv->mTopLeft.y && ypos_converted <= pTlv->mBottomRight.y))
        {
            break;
        }

        if (pTlv->mTlvFlags.Get(TlvFlags::eBit3_End_TLV_List))
        {
            return nullptr;
        }
    }

    return pTlv;
}

Path_TLV* Path::TLV_From_Offset_Lvl_Cam(u32 tlvOffset_levelId_PathId)
{
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<s32>(MapWrapper::ToAE(mLevelId)) && data.parts.pathId == mPathId)
    {
        return reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[mPathData->field_12_object_offset + data.parts.tlvOffset]);
    }
    else
    {
        return nullptr;
    }
}

u32 Path::TLVInfo_From_TLVPtr(Path_TLV* pTlv)
{
    TlvItemInfoUnion data;
    data.parts.levelId = static_cast<u8>(MapWrapper::ToAE(mLevelId));
    data.parts.pathId = static_cast<u8>(mPathId);

    // Num bytes into the path res block
    const std::size_t diff = reinterpret_cast<u8*>(pTlv) - (*field_10_ppRes);

    // Sub off the offset from the start of the path block to TLV data
    data.parts.tlvOffset = static_cast<u16>(diff - mPathData->field_12_object_offset);
    return data.all;
}

Path_TLV* Path::TLV_Next_Of_Type(Path_TLV* pTlv, TlvTypes type)
{
    pTlv = Path::Next_TLV(pTlv);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->mTlvType32.mType != type)
    {
        pTlv = Path::Next_TLV(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }

    return pTlv;
}

void Path::TLV_Reset(u32 tlvOffset_levelId_PathId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel)))
    {
        const PathBlyRec* pBlyRec = Path_Get_Bly_Record(MapWrapper::FromAE(static_cast<LevelIds>(data.parts.levelId)), data.parts.pathId);
        u8** ppPathRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Path, data.parts.pathId, TRUE, FALSE);
        if (ppPathRes)
        {
            const s32 tlvOffset = data.parts.tlvOffset + pBlyRec->field_4_pPathData->field_12_object_offset;
            Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(&(*ppPathRes)[tlvOffset]);

            if (bSetDestroyed & 1)
            {
                pTlv->mTlvFlags.Set(TlvFlags::eBit2_Destroyed);
            }
            else
            {
                pTlv->mTlvFlags.Clear(TlvFlags::eBit2_Destroyed);
            }

            if (bSetCreated & 1)
            {
                pTlv->mTlvFlags.Set(TlvFlags::eBit1_Created);
            }
            else
            {
                pTlv->mTlvFlags.Clear(TlvFlags::eBit1_Created);
            }

            if (hiFlags != -1)
            {
                // Seems to be a blob per TLV specific bits
                pTlv->mTlvState = static_cast<u8>(hiFlags);
            }
            ResourceManager::FreeResource_49C330(ppPathRes);
        }
    }
}

void Path::Start_Sounds_For_Objects_In_Camera(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx)
{
    Path_TLV* pTlv = sPathInfo->Get_First_TLV_For_Offsetted_Camera(cam_x_idx, cam_y_idx);
    while (pTlv)
    {
        if (!(pTlv->mTlvFlags.Get(TlvFlags::eBit1_Created) || (pTlv->mTlvFlags.Get(TlvFlags::eBit2_Destroyed))))
        {
            Start_Sounds_for_TLV(direction, pTlv);
        }
        pTlv = Next_TLV(pTlv);
    }
}

void Path::Reset_TLVs(u16 pathId)
{
    const PathData* pPathData = Path_Get_Bly_Record(gMap.mCurrentLevel, pathId)->field_4_pPathData;
    const s32 camsX = (pPathData->field_4_bTop - pPathData->field_0_bLeft) / pPathData->field_A_grid_width;
    const s32 camsY = (pPathData->field_6_bBottom - pPathData->field_2_bRight) / pPathData->field_C_grid_height;
    u8** ppPath = ResourceManager::GetLoadedResource(ResourceManager::Resource_Path, pathId, TRUE, FALSE);
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
                    pTlv->mTlvFlags.Clear(TlvFlags::eBit1_Created);
                    pTlv->mTlvFlags.Clear(TlvFlags::eBit2_Destroyed);
                    pTlv = Path::Next_TLV(pTlv);
                }
            }
        }
        ResourceManager::FreeResource_49C330(ppPath);
    }
}
