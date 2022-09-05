#include "stdafx_ao.h"
#include "RollingBallStopper.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "ResourceManager.hpp"
#include "FixedPoint.hpp"
#include "Sfx.hpp"
#include "Grid.hpp"

namespace AO {

RollingBallStopper::RollingBallStopper(relive::Path_RollingBallStopper* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eRollingBallStopper;
    
    const AnimRecord rec = AO::AnimRec(AnimId::Stone_Ball_Stopper);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Stone_Ball_Stopper, ppRes);
    mAnim.mRenderLayer = Layer::eLayer_FG1_37;

    mStopperSwitchId = pTlv->mStopperSwitchId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }

    mBallSwitchId = pTlv->mBallSwitchId;

    if (pTlv->mStopDirection == relive::reliveXDirection::eLeft)
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mVelX = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(22) : FP_FromInteger(-22);
    mVelY = FP_FromInteger(0);

    mTlvInfo = tlvId;

    // Check its enabled ?
    if (pTlv->mTlvSpecificMeaning)
    {
        mYPos += mSpriteScale * FP_FromInteger(70);
        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            mState = States::eMovingDone;
            mXPos += mSpriteScale * FP_FromInteger(35);
        }
        else
        {
            mState = States::eMovingDone;
            mXPos -= mSpriteScale * FP_FromInteger(35);
        }
    }
    else
    {
        mState = States::eWaitForTrigger;
        SwitchStates_Set(pTlv->mBallSwitchId, 0);
        SwitchStates_Set(pTlv->mStopperSwitchId, 0);
    }

    const auto oldXPos = mXPos;
    MapFollowMe_401D30(TRUE);
    mXPos = oldXPos;

    FP lineXPos = {};
    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        lineXPos = (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2)) + FP_NoFractional(oldXPos);
    }
    else
    {
        lineXPos = FP_NoFractional(oldXPos) - (ScaleToGridSize(mSpriteScale) / FP_FromInteger(2));
    }

    const auto x1 = FP_GetExponent(lineXPos);
    const auto y1 = FP_GetExponent(mYPos);
    if (mSpriteScale == FP_FromInteger(1))
    {
        mCollisionLine = sCollisions->Add_Dynamic_Collision_Line(x1, y1 - 70, x1, y1, eLineTypes::eWallLeft_1);
    }
    else
    {
        mCollisionLine = sCollisions->Add_Dynamic_Collision_Line(x1, y1 - 35, x1, y1, eLineTypes::eBackgroundWallLeft_5);
    }
}

RollingBallStopper::~RollingBallStopper()
{
    if (mState != States::eWaitForTrigger)
    {
        Path::TLV_Reset(mTlvInfo, 1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, 0, 0, 0);
    }

    if (mCollisionLine)
    {
        Rect_Clear(&mCollisionLine->mRect);
    }
}

void RollingBallStopper::VScreenChanged()
{
    if (mState == States::eMoveStopper)
    {
        SwitchStates_Set(mBallSwitchId, 1);
    }
    mBaseGameObjectFlags.Set(Options::eDead);
}

void RollingBallStopper::VUpdate()
{
    switch (mState)
    {
        case States::eWaitForTrigger:
            if (SwitchStates_Get(mStopperSwitchId))
            {
                Rect_Clear(&mCollisionLine->mRect);
                mCollisionLine = nullptr;
                mState = States::eMoveStopper;
                SFX_Play_Pitch(relive::SoundEffects::PickupItem, 100, -2400, 0);
                SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -800, 0);
            }
            break;

        case States::eMoveStopper:
            mVelY += (mSpriteScale * FP_FromInteger(25));
            if (mVelY <= (mSpriteScale * FP_FromInteger(70)))
            {
                mXPos += mVelX;
                mYPos += (mSpriteScale * FP_FromInteger(25));
            }
            else
            {
                mState = States::eMovingDone;
                SwitchStates_Set(mBallSwitchId, 1);
            }
            break;
    }
}

} // namespace AO
