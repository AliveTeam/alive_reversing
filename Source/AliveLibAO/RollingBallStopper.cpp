#include "stdafx_ao.h"
#include "RollingBallStopper.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Collisions.hpp"
#include "ResourceManager.hpp"
#include "FixedPoint.hpp"
#include "Sfx.hpp"

namespace AO {

RollingBallStopper::RollingBallStopper(Path_RollingBallStopper* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eRollingBallStopper;
    
    const AnimRecord rec = AO::AnimRec(AnimId::Stone_Ball_Stopper);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Stone_Ball_Stopper, ppRes);
    mAnim.mRenderLayer = Layer::eLayer_FG1_37;

    field_114_release_switch_id = pTlv->field_18_stopper_switch_id;

    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }

    field_116_ball_switch_id = pTlv->field_1C_ball_switch_id;

    if (pTlv->field_1E_direction == XDirection_short::eLeft_0)
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    mVelX = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(22) : FP_FromInteger(-22);
    mVelY = FP_FromInteger(0);

    field_10C_tlvInfo = tlvInfo;

    // Check its enabled ?
    if (pTlv->field_1_unknown)
    {
        mYPos += mSpriteScale * FP_FromInteger(70);
        if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
        {
            field_112_state = States::eMovingDone_2;
            mXPos += mSpriteScale * FP_FromInteger(35);
        }
        else
        {
            field_112_state = States::eMovingDone_2;
            mXPos -= mSpriteScale * FP_FromInteger(35);
        }
    }
    else
    {
        field_112_state = States::eWaitForTrigger_0;
        SwitchStates_Set(pTlv->field_1C_ball_switch_id, 0);
        SwitchStates_Set(pTlv->field_18_stopper_switch_id, 0);
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
        field_118_pLine = sCollisions->Add_Dynamic_Collision_Line(x1, y1 - 70, x1, y1, eLineTypes::eWallLeft_1);
    }
    else
    {
        field_118_pLine = sCollisions->Add_Dynamic_Collision_Line(x1, y1 - 35, x1, y1, eLineTypes::eBackgroundWallLeft_5);
    }
}

RollingBallStopper::~RollingBallStopper()
{
    if (field_112_state != States::eWaitForTrigger_0)
    {
        Path::TLV_Reset(field_10C_tlvInfo, 1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_10C_tlvInfo, 0, 0, 0);
    }

    if (field_118_pLine)
    {
        Rect_Clear(&field_118_pLine->mRect);
    }
}

void RollingBallStopper::VScreenChanged()
{
    if (field_112_state == States::eMoveStopper_1)
    {
        SwitchStates_Set(field_116_ball_switch_id, 1);
    }
    mBaseGameObjectFlags.Set(Options::eDead);
}

void RollingBallStopper::VUpdate()
{
    switch (field_112_state)
    {
        case States::eWaitForTrigger_0:
            if (SwitchStates_Get(field_114_release_switch_id))
            {
                Rect_Clear(&field_118_pLine->mRect);
                field_118_pLine = nullptr;
                field_112_state = States::eMoveStopper_1;
                SFX_Play_Pitch(SoundEffect::PickupItem_33, 100, -2400, 0);
                SFX_Play_Pitch(SoundEffect::LiftStop_35, 80, -800, 0);
            }
            break;

        case States::eMoveStopper_1:
            mVelY += (mSpriteScale * FP_FromInteger(25));
            if (mVelY <= (mSpriteScale * FP_FromInteger(70)))
            {
                mXPos += mVelX;
                mYPos += (mSpriteScale * FP_FromInteger(25));
            }
            else
            {
                field_112_state = States::eMovingDone_2;
                SwitchStates_Set(field_116_ball_switch_id, 1);
            }
            break;
    }
}

} // namespace AO
