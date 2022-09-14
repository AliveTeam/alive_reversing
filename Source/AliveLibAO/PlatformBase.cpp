#include "stdafx_ao.h"
#include "Function.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

void PlatformBase::VRemove(BaseAliveGameObject* pObj)
{
    mPlatformBaseCount--;
    LOG_INFO("Remove " << (u64) pObj << " count " << mPlatformBaseCount << " gnFrame " << sGnFrame);
}

void PlatformBase::AddDynamicCollision(AnimId animId, relive::Path_TLV* pTlv, Map* pMap, const Guid& tlvId)
{
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mPlatformBaseTlvInfo = tlvId;

    mPlatformBaseMap = pMap;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_10C = 0;
    mYOffset = 0;

    field_110 = 0;
    mPlatformBaseCount = 0;

    Animation_Init(GetAnimRes(animId));

    if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mScale = Scale::Fg;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mScale = Scale::Bg;
    }

    const PerFrameInfo* pFrameHeader = mAnim.Get_FrameHeader(0);
    // TODO: Check field_8_data.points[1].y
    mYPos += FP_NoFractional(FP_FromInteger(-pFrameHeader->mBoundMax.y) * mSpriteScale);
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);

    mPlatformBaseCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
        pTlv->mTopLeftX,
        pTlv->mTopLeftY,
        pTlv->mBottomRightX,
        pTlv->mTopLeftY,
        eLineTypes::eDynamicCollision_32
    );

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftX) - mXPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRightX) - mXPos);
    mPlatformBaseYOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftY) - mYPos);
    mPlatformBaseHeightOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftY) - mYPos);

    if (!ObjListPlatforms_50766C->Push_Back(this))
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

PlatformBase::~PlatformBase()
{
    ObjListPlatforms_50766C->Remove_Item(this);

    if (mPlatformBaseCollisionLine)
    {
        if (gMap.mCurrentLevel == mCurrentLevel && gMap.mCurrentPath == mCurrentPath)
        {
            Rect_Clear(&mPlatformBaseCollisionLine->mRect);
        }
    }
}

void PlatformBase::SyncCollisionLinePosition()
{
    mPlatformBaseCollisionLine->mRect.x = FP_GetExponent(mXPos + FP_FromInteger(mPlatformBaseXOffset));
    mPlatformBaseCollisionLine->mRect.w = FP_GetExponent(mXPos + FP_FromInteger(mPlatformBaseWidthOffset));
    mPlatformBaseCollisionLine->mRect.y = FP_GetExponent(mYPos + FP_FromInteger(mPlatformBaseYOffset));
    mPlatformBaseCollisionLine->mRect.h = FP_GetExponent(mYPos + FP_FromInteger(mPlatformBaseHeightOffset));
}

void PlatformBase::KeepThingsOnPlatform(FP xpos)
{
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mLiftPoint == this)
        {
            pObjIter->mXPos += xpos;
            pObjIter->mYPos = FP_FromInteger(mPlatformBaseCollisionLine->mRect.y);
        }
    }
}

void PlatformBase::VAdd(BaseAliveGameObject* pObj)
{
    mPlatformBaseCount++;
    LOG_INFO("Add " << (u64) pObj << " count " << mPlatformBaseCount << " gnFrame " << sGnFrame);
    field_110 = 1;

    if (mPlatformBaseCollisionLine)
    {
        mPlatformBaseCollisionLine->mRect.x = FP_GetExponent(mXPos + FP_FromInteger(mPlatformBaseXOffset));
        mPlatformBaseCollisionLine->mRect.w = FP_GetExponent(mXPos + FP_FromInteger(mPlatformBaseWidthOffset));
        mPlatformBaseCollisionLine->mRect.y = FP_GetExponent(mYPos + FP_FromInteger(mPlatformBaseYOffset));
        mPlatformBaseCollisionLine->mRect.h = FP_GetExponent(mYPos + FP_FromInteger(mPlatformBaseHeightOffset));
    }
}

} // namespace AO
