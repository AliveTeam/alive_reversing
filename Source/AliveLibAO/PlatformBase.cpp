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

void PlatformBase::AddDynamicCollision(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo)
{
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    mPlatformBaseTlvInfo = tlvInfo;

    mPlatformBaseMap = pMap;

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

    field_10C = 0;
    mBaseAnimatedWithPhysicsGameObject_YOffset = 0;

    field_110 = 0;
    mPlatformBaseCount = 0;

    Animation_Init_417FD0(
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

    mPlatformBaseCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
        pTlv->mTopLeft.x,
        pTlv->mTopLeft.y,
        pTlv->mBottomRight.x,
        pTlv->mTopLeft.y,
        eLineTypes::eDynamicCollision_32
    );

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.x) - mBaseAnimatedWithPhysicsGameObject_XPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRight.x) - mBaseAnimatedWithPhysicsGameObject_XPos);
    mPlatformBaseYOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.y) - mBaseAnimatedWithPhysicsGameObject_YPos);
    mPlatformBaseHeightOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.y) - mBaseAnimatedWithPhysicsGameObject_YPos);

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
        if (gMap.mCurrentLevel == mBaseAnimatedWithPhysicsGameObject_LvlNumber && gMap.mCurrentPath == mBaseAnimatedWithPhysicsGameObject_PathNumber)
        {
            Rect_Clear(&mPlatformBaseCollisionLine->field_0_rect);
        }
    }
}

void PlatformBase::SyncCollisionLinePosition()
{
    mPlatformBaseCollisionLine->field_0_rect.x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(mPlatformBaseXOffset));
    mPlatformBaseCollisionLine->field_0_rect.w = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(mPlatformBaseWidthOffset));
    mPlatformBaseCollisionLine->field_0_rect.y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(mPlatformBaseYOffset));
    mPlatformBaseCollisionLine->field_0_rect.h = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(mPlatformBaseHeightOffset));
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
            pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos += xpos;
            pObjIter->mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(mPlatformBaseCollisionLine->field_0_rect.y);
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
        mPlatformBaseCollisionLine->field_0_rect.x = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(mPlatformBaseXOffset));
        mPlatformBaseCollisionLine->field_0_rect.w = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(mPlatformBaseWidthOffset));
        mPlatformBaseCollisionLine->field_0_rect.y = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(mPlatformBaseYOffset));
        mPlatformBaseCollisionLine->field_0_rect.h = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(mPlatformBaseHeightOffset));
    }
}

} // namespace AO
