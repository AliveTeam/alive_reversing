#include "stdafx.h"
#include "PlatformBase.hpp"
#include "Function.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Map.hpp"

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

void PlatformBase::AddDynamicCollision(AnimId animId, relive::Path_TLV* pTlv, const Guid& tlvId)
{
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mPlatformBaseTlvInfo = tlvId;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    mPlatformBaseCount = 0;
    Animation_Init(GetAnimRes(animId));

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
        SetScale(Scale::Fg);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        SetScale(Scale::Bg);
    }

   const PerFrameInfo* pFrameHeader = GetAnimation().Get_FrameHeader(0);
   // TODO: Check field_8_data.points[1].y
    mYPos += FP_NoFractional(FP_FromInteger(-pFrameHeader->mBoundMin.y) * GetSpriteScale());
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);

    field_124_pCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
        pTlv->mTopLeftX,
        pTlv->mTopLeftY,
        pTlv->mBottomRightX,
        pTlv->mTopLeftY,
        eLineTypes::eDynamicCollision_32);

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftX) - mXPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRightX) - mXPos);
    mPlatformBaseYOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftY) - mYPos);
    mPlatformBaseHeightOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftY) - mYPos);

    if (!gPlatformsArray->Push_Back(this))
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

PlatformBase::~PlatformBase()
{
    gPlatformsArray->Remove_Item(this);

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
