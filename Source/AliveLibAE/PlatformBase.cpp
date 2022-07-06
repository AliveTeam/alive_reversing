#include "stdafx.h"
#include "PlatformBase.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

PlatformBase::PlatformBase()
    : BaseAliveGameObject(0)
{

}



void PlatformBase::VAdd(BaseAliveGameObject* pObj)
{
    vAddCount(pObj);
}

void PlatformBase::VRemove(BaseAliveGameObject* pObj)
{
    vRemoveCount(pObj);
}

void PlatformBase::AddDynamicCollision(AnimId animId, u8** ppAnimData, Path_TLV* pTlv, s32 tlvInfo)
{
    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    mPlatformBaseTlvInfo = tlvInfo;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

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

    field_124_pCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
        pTlv->mTopLeft.x,
        pTlv->mTopLeft.y,
        pTlv->mBottomRight.x,
        pTlv->mTopLeft.y,
        eLineTypes::eDynamicCollision_32);

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.x) - mXPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRight.x) - mXPos);
    mPlatformBaseYOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.y) - mYPos);
    mPlatformBaseHeightOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.y) - mYPos);

    if (!ObjList_5C1B78->Push_Back(this))
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

PlatformBase::~PlatformBase()
{
    ObjList_5C1B78->Remove_Item(this);

    if (field_124_pCollisionLine)
    {
        if (gMap.mCurrentLevel == mCurrentLevel && gMap.mCurrentPath == mCurrentPath)
        {
            Rect_Clear(&field_124_pCollisionLine->mRect);
        }
    }
}

void PlatformBase::SyncCollisionLinePosition()
{
    field_124_pCollisionLine->mRect.x = FP_GetExponent(FP_FromInteger(mPlatformBaseXOffset) + mXPos);
    field_124_pCollisionLine->mRect.w = FP_GetExponent(FP_FromInteger(mPlatformBaseWidthOffset) + mXPos);
    field_124_pCollisionLine->mRect.y = FP_GetExponent(mYPos + FP_FromInteger(mPlatformBaseYOffset));
    field_124_pCollisionLine->mRect.h = FP_GetExponent(mYPos + FP_FromInteger(mPlatformBaseHeightOffset));
}

void PlatformBase::vRemoveCount(BaseAliveGameObject* /*pObj*/)
{
    --mPlatformBaseCount;
}

void PlatformBase::vAddCount(BaseAliveGameObject* /*pObj*/)
{
    ++mPlatformBaseCount;
    if (field_124_pCollisionLine)
    {
        SyncCollisionLinePosition();
    }
}
