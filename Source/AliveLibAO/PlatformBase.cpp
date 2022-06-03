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
    mCount--;
    LOG_INFO("Remove " << (u64) pObj << " count " << mCount << " gnFrame " << gnFrameCount_507670);
}

void PlatformBase::AddDynamicCollision(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo)
{
    mXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    mTlvInfo = tlvInfo;

    mMap = pMap;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    field_10C = 0;
    mYOffset = 0;

    field_110 = 0;
    mCount = 0;

    Animation_Init_417FD0(
        frameTableOffset,
        maxW,
        maxH,
        ppAnimData,
        1);

    if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mScale = 1;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mScale = 0;
    }

    FrameInfoHeader* pFrameHeader = mAnim.Get_FrameHeader(0);
    mYPos += FP_NoFractional(FP_FromInteger(-pFrameHeader->field_8_data.points[1].y) * mSpriteScale);
    mXPos = FP_FromInteger((pTlv->field_10_top_left.field_0_x + pTlv->field_14_bottom_right.field_0_x) / 2);

    mCollisionLine = sCollisions_DArray_504C6C->Add_Dynamic_Collision_Line(
        pTlv->field_10_top_left.field_0_x,
        pTlv->field_10_top_left.field_2_y,
        pTlv->field_14_bottom_right.field_0_x,
        pTlv->field_10_top_left.field_2_y,
        eLineTypes::eUnknown_32
    );

    mXOffset = FP_GetExponent(FP_FromInteger(pTlv->field_10_top_left.field_0_x) - mXPos);
    mWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->field_14_bottom_right.field_0_x) - mXPos);
    mYOffset = FP_GetExponent(FP_FromInteger(pTlv->field_10_top_left.field_2_y) - mYPos);
    mHeightOffset = FP_GetExponent(FP_FromInteger(pTlv->field_10_top_left.field_2_y) - mYPos);

    if (!ObjListPlatforms_50766C->Push_Back(this))
    {
        mGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

PlatformBase::~PlatformBase()
{
    ObjListPlatforms_50766C->Remove_Item(this);

    if (mCollisionLine)
    {
        if (gMap.mCurrentLevel == mLvlNumber && gMap.mCurrentPath == mPathNumber)
        {
            Rect_Clear(&mCollisionLine->field_0_rect);
        }
    }
}

void PlatformBase::SyncCollisionLinePosition()
{
    mCollisionLine->field_0_rect.x = FP_GetExponent(mXPos + FP_FromInteger(mXOffset));
    mCollisionLine->field_0_rect.w = FP_GetExponent(mXPos + FP_FromInteger(mWidthOffset));
    mCollisionLine->field_0_rect.y = FP_GetExponent(mYPos + FP_FromInteger(mYOffset));
    mCollisionLine->field_0_rect.h = FP_GetExponent(mYPos + FP_FromInteger(mHeightOffset));
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

        if (pObjIter->field_F8_pLiftPoint == this)
        {
            pObjIter->mXPos += xpos;
            pObjIter->mYPos = FP_FromInteger(mCollisionLine->field_0_rect.y);
        }
    }
}

void PlatformBase::VAdd(BaseAliveGameObject* pObj)
{
    mCount++;
    LOG_INFO("Add " << (u64) pObj << " count " << mCount << " gnFrame " << gnFrameCount_507670);
    field_110 = 1;

    if (mCollisionLine)
    {
        mCollisionLine->field_0_rect.x = FP_GetExponent(mXPos + FP_FromInteger(mXOffset));
        mCollisionLine->field_0_rect.w = FP_GetExponent(mXPos + FP_FromInteger(mWidthOffset));
        mCollisionLine->field_0_rect.y = FP_GetExponent(mYPos + FP_FromInteger(mYOffset));
        mCollisionLine->field_0_rect.h = FP_GetExponent(mYPos + FP_FromInteger(mHeightOffset));
    }
}

} // namespace AO
