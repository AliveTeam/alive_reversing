#include "stdafx.h"
#include "Path.hpp"
#include "Function.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "AmbientSound.hpp"
#include "../relive_lib/BinaryPath.hpp"
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

}

void Path::Free()
{
    mPathData = 0;
    field_10_ppRes = nullptr;
    mCamsOnY = 0;
    mCamsOnX = 0;
    mCameraId = 0;
    mPathId = 0;
    mLevelId = EReliveLevelIds::eMenu;
}

void Path::Init(const PathData* pPathData, EReliveLevelIds level, s16 path, s16 cameraId, BinaryPath* ppPathRes)
{
    field_10_ppRes = ppPathRes;

    mCameraId = cameraId;
    mLevelId = level;
    mPathId = path;

    mPathData = pPathData;
    mCamsOnX = (mPathData->field_4_bTop - mPathData->field_0_bLeft) / mPathData->field_A_grid_width;
    mCamsOnY = (mPathData->field_6_bBottom - mPathData->field_2_bRight) / mPathData->field_C_grid_height;
}

void Path::Loader_4DB800(s16 xpos, s16 ypos, LoadMode loadMode, ReliveTypes typeToLoad)
{
    relive::Path_TLV* pPathTLV = field_10_ppRes->TlvsForCamera(xpos, ypos);
    while (pPathTLV)
    {
        if (typeToLoad == ReliveTypes::eNone || typeToLoad == pPathTLV->mTlvType)
        {
            if (loadMode != LoadMode::ConstructObject_0 || !(pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit1_Created) || pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed)))
            {
                if (loadMode == LoadMode::ConstructObject_0)
                {
                    pPathTLV->mTlvFlags.Set(relive::TlvFlags::eBit1_Created);
                    pPathTLV->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);
                }

                // Call the factory to construct the item
                ConstructTLVObject(pPathTLV, this, pPathTLV->mId, loadMode);
            }
        }

        // End of TLV list for current camera
        if (pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
        {
            break;
        }

        pPathTLV = Next_TLV(pPathTLV);
    }
}

relive::Path_TLV* Path::Get_First_TLV_For_Offsetted_Camera(s16 cam_x_idx, s16 cam_y_idx)
{
    const s32 camY = cam_y_idx + gMap.mCamIdxOnY;
    const s32 camX = cam_x_idx + gMap.mCamIdxOnX;

    if (camX >= mCamsOnX || camX < 0 || camY >= mCamsOnY || camY < 0)
    {
        return nullptr;
    }

    BinaryPath* pPathData = gMap.GetPathResourceBlockPtr(gMap.mCurrentPath);
    return pPathData->TlvsForCamera(camX, camY);
}

relive::Path_TLV* Path::Next_TLV(relive::Path_TLV* pTlv)
{
    return Next_TLV_Impl(pTlv);
}

relive::Path_TLV* Path::TLV_First_Of_Type_In_Camera(ReliveTypes objectType, s16 camX)
{
    relive::Path_TLV* pTlv = Get_First_TLV_For_Offsetted_Camera(camX, 0);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->mTlvType != objectType)
    {
        pTlv = Next_TLV(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }
    return pTlv;
}

relive::Path_TLV* Path::TLV_Get_At_4DB4B0(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes objectType)
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

    // Get the offset to where the TLV list starts for this camera cell
    BinaryPath* pBinPath = gMap.GetPathResourceBlockPtr(gMap.mCurrentPath);
    relive::Path_TLV* pTlvIter = pBinPath->TlvsForCamera(grid_cell_x, grid_cell_y);
    if (!pTlvIter)
    {
        return nullptr;
    }

    while (pTlvIter)
    {
        if (pTlvIter->mTlvType == objectType
            && right <= pTlvIter->mBottomRightX
            && left >= pTlvIter->mTopLeftX
            && bottom >= pTlvIter->mTopLeftY
            && top <= pTlvIter->mBottomRightY)
        {
            return pTlvIter;
        }
        pTlvIter = Path::Next_TLV(pTlvIter);
    }

    return pTlvIter;
}

relive::Path_TLV* Path::TlvGetAt(relive::Path_TLV* pTlv, FP xpos, FP ypos, FP w, FP h)
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

        BinaryPath* pBinPath = gMap.GetPathResourceBlockPtr(gMap.mCurrentPath);
        pTlv = pBinPath->TlvsForCamera(camX, camY);
        if (!pTlv)
        {
            return nullptr;
        }

        if (!xyPosValid || (xpos_converted <= pTlv->mBottomRightX && width_converted >= pTlv->mTopLeftX && height_converted >= pTlv->mTopLeftY && ypos_converted <= pTlv->mBottomRightY))
        {
            return pTlv;
        }
    }

    if (pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
    {
        return nullptr;
    }

    while (1)
    {
        pTlv = Path::Next_TLV(pTlv);
        if (!xyPosValid || (xpos_converted <= pTlv->mBottomRightX && width_converted >= pTlv->mTopLeftX && height_converted >= pTlv->mTopLeftY && ypos_converted <= pTlv->mBottomRightY))
        {
            break;
        }

        if (pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
        {
            return nullptr;
        }
    }

    return pTlv;
}

relive::Path_TLV* Path::TLV_From_Offset_Lvl_Cam(const Guid& tlvId)
{
    return field_10_ppRes->TlvsById(tlvId);
}

Guid Path::TLVInfo_From_TLVPtr(relive::Path_TLV* pTlv)
{
    return pTlv->mId;
}

relive::Path_TLV* Path::TLV_Next_Of_Type(relive::Path_TLV* pTlv, ReliveTypes type)
{
    pTlv = Path::Next_TLV(pTlv);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->mTlvType != type)
    {
        pTlv = Path::Next_TLV(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }

    return pTlv;
}

void Path::TLV_Reset(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    BinaryPath* pBinPath = gMap.GetPathResourceBlockPtr(gMap.mCurrentPath);
    if (pBinPath)
    {
        relive::Path_TLV* pTlv = pBinPath->TlvsById(tlvId);
        if (pTlv)
        {
            pTlv->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed, bSetDestroyed & 1);
            pTlv->mTlvFlags.Set(relive::TlvFlags::eBit1_Created, bSetCreated & 1);

            if (hiFlags != -1)
            {
                // Seems to be a blob per TLV specific bits
                pTlv->mTlvSpecificMeaning = static_cast<u8>(hiFlags);
            }
        }
    }
}

void Path::Start_Sounds_For_Objects_In_Camera(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx)
{
    relive::Path_TLV* pTlv = sPathInfo->Get_First_TLV_For_Offsetted_Camera(cam_x_idx, cam_y_idx);
    while (pTlv)
    {
        if (!(pTlv->mTlvFlags.Get(relive::TlvFlags::eBit1_Created) || (pTlv->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed))))
        {
            Start_Sounds_for_TLV(direction, pTlv);
        }
        pTlv = Next_TLV(pTlv);
    }
}

void Path::Reset_TLVs(u16 pathId)
{
    BinaryPath* pPath = gMap.GetPathResourceBlockPtr(pathId);
    if (pPath)
    {
        for (auto& cam : pPath->GetCameras())
        {
            auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
            while (pTlv)
            {
                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
                pTlv = Path::Next_TLV(pTlv);
            }
        }
    }
}
