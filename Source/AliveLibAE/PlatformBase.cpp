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
    mXPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    mTlvInfo = tlvInfo;

    mVelX = FP_FromInteger(0);
    mVelY = FP_FromInteger(0);

    mCount = 0;
    Animation_Init(
        frameTableOffset,
        maxW,
        maxH,
        ppAnimData,
        1,
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
    mXPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);

    mCollisionLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
        pTlv->field_8_top_left.field_0_x,
        pTlv->field_8_top_left.field_2_y,
        pTlv->field_C_bottom_right.field_0_x,
        pTlv->field_8_top_left.field_2_y,
        32);

    mXOffset = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - mXPos);
    mWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - mXPos);
    mYOffset = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - mYPos);
    mHeightOffset = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - mYPos);

    if (!ObjList_5C1B78->Push_Back(this))
    {
        mGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

PlatformBase::~PlatformBase()
{
    ObjList_5C1B78->Remove_Item(this);

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
    mCollisionLine->field_0_rect.x = FP_GetExponent(FP_FromInteger(mXOffset) + mXPos);
    mCollisionLine->field_0_rect.w = FP_GetExponent(FP_FromInteger(mWidthOffset) + mXPos);
    mCollisionLine->field_0_rect.y = FP_GetExponent(mYPos + FP_FromInteger(mYOffset));
    mCollisionLine->field_0_rect.h = FP_GetExponent(mYPos + FP_FromInteger(mHeightOffset));
}

void PlatformBase::vRemoveCount(BaseAliveGameObject* /*pObj*/)
{
    --mCount;
}

void PlatformBase::vAddCount(BaseAliveGameObject* /*pObj*/)
{
    ++mCount;
    if (mCollisionLine)
    {
        SyncCollisionLinePosition();
    }
}
