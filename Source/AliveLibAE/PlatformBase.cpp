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

void PlatformBase::AddDynamicCollision(s32 frameTableOffset, s32 maxW, u16 maxH, u8** ppAnimData, Path_TLV* pTlv, Map* /*pMap*/, s32 tlvInfo)
{
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    mPlatformBaseTlvInfo = tlvInfo;

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    mPlatformBaseCount = 0;
    Animation_Init(
        frameTableOffset,
        maxW,
        maxH,
        ppAnimData,
        1);

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }

    FrameInfoHeader* pFrameHeader = mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(0);
    mBaseAnimatedWithPhysicsGameObject_YPos += FP_NoFractional(FP_FromInteger(-pFrameHeader->field_8_data.points[1].y) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);

    field_124_pCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
        pTlv->mTopLeft.x,
        pTlv->mTopLeft.y,
        pTlv->mBottomRight.x,
        pTlv->mTopLeft.y,
        32);

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.x) - mBaseAnimatedWithPhysicsGameObject_XPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRight.x) - mBaseAnimatedWithPhysicsGameObject_XPos);
    mPlatformBaseYOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.y) - mBaseAnimatedWithPhysicsGameObject_YPos);
    mPlatformBaseHeightOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.y) - mBaseAnimatedWithPhysicsGameObject_YPos);

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
        if (gMap.mCurrentLevel == mBaseAnimatedWithPhysicsGameObject_LvlNumber && gMap.mCurrentPath == mBaseAnimatedWithPhysicsGameObject_PathNumber)
        {
            Rect_Clear(&field_124_pCollisionLine->field_0_rect);
        }
    }
}

void PlatformBase::SyncCollisionLinePosition()
{
    field_124_pCollisionLine->field_0_rect.x = FP_GetExponent(FP_FromInteger(mPlatformBaseXOffset) + mBaseAnimatedWithPhysicsGameObject_XPos);
    field_124_pCollisionLine->field_0_rect.w = FP_GetExponent(FP_FromInteger(mPlatformBaseWidthOffset) + mBaseAnimatedWithPhysicsGameObject_XPos);
    field_124_pCollisionLine->field_0_rect.y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(mPlatformBaseYOffset));
    field_124_pCollisionLine->field_0_rect.h = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(mPlatformBaseHeightOffset));
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
