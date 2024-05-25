#include "stdafx_ao.h"
#include "RollingBallStopper.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Grid.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

RollingBallStopper::RollingBallStopper(relive::Path_RollingBallStopper* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eRollingBallStopper);
    
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Stone_Ball_Stopper));
    Animation_Init(GetAnimRes(AnimId::Stone_Ball_Stopper));

    GetAnimation().SetRenderLayer(Layer::eLayer_FG1_37);

    mStopperSwitchId = pTlv->mStopperSwitchId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }

    mBallSwitchId = pTlv->mBallSwitchId;

    if (pTlv->mStopDirection == relive::reliveXDirection::eLeft)
    {
        GetAnimation().SetFlipX(true);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mVelX = GetAnimation().GetFlipX() ? FP_FromInteger(22) : FP_FromInteger(-22);
    mVelY = FP_FromInteger(0);

    mTlvInfo = tlvId;

    // Check its enabled ?
    if (pTlv->mTlvSpecificMeaning)
    {
        mYPos += GetSpriteScale() * FP_FromInteger(70);
        if (GetAnimation().GetFlipX())
        {
            mState = States::eMovingDone;
            mXPos += GetSpriteScale() * FP_FromInteger(35);
        }
        else
        {
            mState = States::eMovingDone;
            mXPos -= GetSpriteScale() * FP_FromInteger(35);
        }
    }
    else
    {
        mState = States::eWaitForTrigger;
        SwitchStates_Set(pTlv->mBallSwitchId, 0);
        SwitchStates_Set(pTlv->mStopperSwitchId, 0);
    }

    const auto oldXPos = mXPos;
    MapFollowMe(true);
    mXPos = oldXPos;

    FP lineXPos = {};
    if (GetAnimation().GetFlipX())
    {
        lineXPos = (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2)) + FP_NoFractional(oldXPos);
    }
    else
    {
        lineXPos = FP_NoFractional(oldXPos) - (ScaleToGridSize(GetSpriteScale()) / FP_FromInteger(2));
    }

    const auto x1 = FP_GetExponent(lineXPos);
    const auto y1 = FP_GetExponent(mYPos);
    if (GetSpriteScale() == FP_FromInteger(1))
    {
        mCollisionLine = gCollisions->Add_Dynamic_Collision_Line(x1, y1 - 70, x1, y1, eLineTypes::eWallLeft_1);
    }
    else
    {
        mCollisionLine = gCollisions->Add_Dynamic_Collision_Line(x1, y1 - 35, x1, y1, eLineTypes::eBackgroundWallLeft_5);
    }
}

RollingBallStopper::~RollingBallStopper()
{
    if (mState != States::eWaitForTrigger)
    {
        Path::TLV_Reset(mTlvInfo, 1);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, 0);
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
    SetDead(true);
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
                SFX_Play_Pitch(relive::SoundEffects::PickupItem, 100, -2400);
                SFX_Play_Pitch(relive::SoundEffects::LiftStop, 80, -800);
            }
            break;

        case States::eMoveStopper:
            mVelY += (GetSpriteScale() * FP_FromInteger(25));
            if (mVelY <= (GetSpriteScale() * FP_FromInteger(70)))
            {
                mXPos += mVelX;
                mYPos += (GetSpriteScale() * FP_FromInteger(25));
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
