#include "stdafx.h"
#include "PathData.hpp"
#include "Map.hpp"
#include "AmbientSound.hpp"
#include "../relive_lib/BinaryPath.hpp"
#include <assert.h>
#include "Path.hpp"
#include "Factory.hpp"
#include "MapWrapper.hpp"

Path* gPathInfo = nullptr;

Path::Path()
{
    mPathData = nullptr;
    mBinaryPath = nullptr;
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
    mBinaryPath = nullptr;
    mCamsOnY = 0;
    mCamsOnX = 0;
    mCameraId = 0;
    mPathId = 0;
    mLevelId = EReliveLevelIds::eMenu;
}

void Path::Init(const PathData* pPathData, EReliveLevelIds level, s16 path, s16 cameraId, BinaryPath* ppPathRes)
{
    mBinaryPath = ppPathRes;

    mCameraId = cameraId;
    mLevelId = level;
    mPathId = path;

    mPathData = pPathData;
    mCamsOnX = (mPathData->field_4_bTop - mPathData->field_0_bLeft) / mPathData->field_A_grid_width;
    mCamsOnY = (mPathData->field_6_bBottom - mPathData->field_2_bRight) / mPathData->field_C_grid_height;
}

void Path::Loader_4DB800(s16 xpos, s16 ypos, relive::LoadMode loadMode, ReliveTypes typeToLoad)
{
    const TlvList& tlvs = mBinaryPath->TlvsForCamera(xpos, ypos);
    for (auto& pPathTLV : tlvs.mTlvs)
    {
        if (typeToLoad == ReliveTypes::eNone || typeToLoad == pPathTLV->mTlvType)
        {
            if (loadMode != relive::LoadMode::ConstructObject_0 || !(pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit1_Created) || pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed)))
            {
                if (loadMode == relive::LoadMode::ConstructObject_0)
                {
                    pPathTLV->mTlvFlags.Set(relive::TlvFlags::eBit1_Created);
                    pPathTLV->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);
                }

                // Call the factory to construct the item
                relive::ConstructTLVObject(pPathTLV.get(), pPathTLV->mId, loadMode);
            }
        }

        // End of TLV list for current camera
        if (pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
        {
            break;
        }
    }
}

TlvIterator Path::Get_First_TLV_For_Offsetted_Camera(s16 cam_x_idx, s16 cam_y_idx)
{
    const s32 camY = cam_y_idx + gMap.mCamIdxOnY;
    const s32 camX = cam_x_idx + gMap.mCamIdxOnX;

    if (camX >= mCamsOnX || camX < 0 || camY >= mCamsOnY || camY < 0)
    {
        ALIVE_FATAL("Camera out of bounds");
    }

    BinaryPath* pPathData = gMap.GetPathResourceBlockPtr(gMap.mCurrentPath);
    return pPathData->TlvsForCamera(camX, camY).FirstIterator();
}

TlvIterator Path::TLV_First_Of_Type_In_Camera(ReliveTypes objectType, s16 camX)
{
    TlvIterator tlvIterator = Get_First_TLV_For_Offsetted_Camera(camX, 0);
    while(tlvIterator.GetTlv())
    {
        if (tlvIterator.GetTlv()->mTlvType == objectType)
        {
            return tlvIterator;
        }
    }
    return TlvIterator::Invalid();
}

TlvIterator Path::VTLV_Get_At_Of_Type(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes objectType)
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
    const TlvList& tlvs = pBinPath->TlvsForCamera(grid_cell_x, grid_cell_y);

    u32 idx = 0;
    for (auto& pTlvIter : tlvs.mTlvs)
    {
        if (pTlvIter->mTlvType == objectType
            && right <= pTlvIter->mBottomRightX
            && left >= pTlvIter->mTopLeftX
            && bottom >= pTlvIter->mTopLeftY
            && top <= pTlvIter->mBottomRightY)
        {
            return TlvIterator(pTlvIter.get(), &tlvs, idx);
        }
        idx++;
    }
    return TlvIterator::Invalid();
}

TlvIterator Path::TLV_Get_At(TlvIterator tlvIterator, FP xpos, FP ypos, FP width, FP height)
{
    const s32 xpos_converted = FP_GetExponent(xpos);
    const s32 ypos_converted = FP_GetExponent(ypos);
    s32 width_converted = FP_GetExponent(width);
    s32 height_converted = FP_GetExponent(height);

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

    if (!tlvIterator.IsValid())
    {
        const PathData* pPathData = mPathData;
        const s32 camX = (xpos_converted + width_converted) / (2 * pPathData->field_A_grid_width);
        const s32 camY = (ypos_converted + height_converted) / (2 * pPathData->field_C_grid_height);

        if (camX >= mCamsOnX || camY >= mCamsOnY)
        {
            return TlvIterator::Invalid();
        }

        if (camX < 0 || camY < 0)
        {
            return TlvIterator::Invalid();
        }

        BinaryPath* pBinPath = gMap.GetPathResourceBlockPtr(gMap.mCurrentPath);
        const TlvList& tlvs = pBinPath->TlvsForCamera(camX, camY);
        tlvIterator = tlvs.FirstIterator();

        if (!xyPosValid || (xpos_converted <= tlvIterator.GetTlv()->mBottomRightX && width_converted >= tlvIterator.GetTlv()->mTopLeftX && height_converted >= tlvIterator.GetTlv()->mTopLeftY && ypos_converted <= tlvIterator.GetTlv()->mBottomRightY))
        {
            return tlvIterator;
        }
    }

    if (tlvIterator.GetTlv()->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
    {
        return TlvIterator::Invalid();
    }

    while (1)
    {
        tlvIterator = tlvIterator.Next_TLV();
        if (!xyPosValid || (xpos_converted <= tlvIterator.GetTlv()->mBottomRightX && width_converted >= tlvIterator.GetTlv()->mTopLeftX && height_converted >= tlvIterator.GetTlv()->mTopLeftY && ypos_converted <= tlvIterator.GetTlv()->mBottomRightY))
        {
            break;
        }

        if (tlvIterator.GetTlv()->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
        {
            return TlvIterator::Invalid();
        }
    }

    return tlvIterator;
}

TlvIterator Path::TLV_From_Offset_Lvl_Cam(const Guid& tlvId)
{
    return mBinaryPath->TlvById(tlvId);
}

Guid Path::TLVInfo_From_TLVPtr(relive::Path_TLV* pTlv)
{
    return pTlv->mId;
}

TlvIterator Path::TLV_Next_Of_Type(TlvIterator tlvIterator, ReliveTypes type)
{
    // Skip current which is already of type
    tlvIterator = tlvIterator.Next_TLV();
    while (tlvIterator.GetTlv())
    {
        // Got the next of type
        if (tlvIterator.GetTlv()->mTlvType == type)
        {
            return tlvIterator;
        }
        tlvIterator = tlvIterator.Next_TLV();
    }
    return TlvIterator::Invalid();
}

void Path::TLV_Reset(const Guid& tlvId, s16 hiFlags)
{
    Path::Set_TLVData(tlvId, hiFlags, 0, 0);
}

void Path::TLV_Persist(const Guid& tlvId, s16 hiFlags)
{
    Path::Set_TLVData(tlvId, hiFlags, 1, 0);
}

void Path::TLV_Delete(const Guid& tlvId, s16 hiFlags)
{
    Path::Set_TLVData(tlvId, hiFlags, 0, 1);
}

void Path::Set_TLVData(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    auto& paths = gMap.GetLoadedPaths();
    for (std::unique_ptr<BinaryPath>& pBinPath : paths)
    {
        if (pBinPath)
        {
            relive::Path_TLV* pTlv = pBinPath->TlvById(tlvId).GetTlv();
            if (pTlv)
            {
                pTlv->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed, bSetDestroyed & 1);
                pTlv->mTlvFlags.Set(relive::TlvFlags::eBit1_Created, bSetCreated & 1);

                if (hiFlags != -1)
                {
                    // Seems to be a blob per TLV specific bits
                    pTlv->mTlvSpecificMeaning = static_cast<u8>(hiFlags);
                }
                break;
            }
        }
    }
}

void Path::Start_Sounds_For_Objects_In_Camera(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx)
{
    TlvIterator tlvIterator = gPathInfo->Get_First_TLV_For_Offsetted_Camera(cam_x_idx, cam_y_idx);
    while(tlvIterator.GetTlv())
    {
        if (!(tlvIterator.GetTlv()->mTlvFlags.Get(relive::TlvFlags::eBit1_Created) || (tlvIterator.GetTlv()->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed))))
        {
            Start_Sounds_for_TLV(direction, tlvIterator.GetTlv());
        }
        tlvIterator = tlvIterator.Next_TLV();
    }
}

void Path::Reset_TLVs(u16 pathId)
{
    BinaryPath* pPath = gMap.GetPathResourceBlockPtr(pathId);
    if (pPath)
    {
        for (auto& cam : pPath->GetCameras())
        {
            for (auto& pTlv : cam->mTlvs.mTlvs)
            {
                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
            }
        }
    }
}
