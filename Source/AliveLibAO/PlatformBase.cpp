#include "stdafx_ao.h"
#include "Function.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

namespace AO {

void PlatformBase::VRemove(BaseAliveGameObject* pObj)
{
    mPlatformBaseCount--;
    LOG_INFO("Remove " << (u64) pObj << " count " << mPlatformBaseCount << " gnFrame " << sGnFrame);
}

void PlatformBase::AddDynamicCollision(AnimId animId, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo)
{
    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    mPlatformBaseTlvInfo = tlvInfo;

    mPlatformBaseMap = pMap;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_10C = 0;
    mYOffset = 0;

    field_110 = 0;
    mPlatformBaseCount = 0;

    Animation_Init(animId, ppAnimData);

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

    FrameInfoHeader* pFrameHeader = mAnim.Get_FrameHeader(0);
    mYPos += FP_NoFractional(FP_FromInteger(-pFrameHeader->field_8_data.points[1].y) * mSpriteScale);
    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);

    mPlatformBaseCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
        pTlv->mTopLeft.x,
        pTlv->mTopLeft.y,
        pTlv->mBottomRight.x,
        pTlv->mTopLeft.y,
        eLineTypes::eDynamicCollision_32
    );

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.x) - mXPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRight.x) - mXPos);
    mPlatformBaseYOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.y) - mYPos);
    mPlatformBaseHeightOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.y) - mYPos);

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
    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
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
