#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "MeatSaw.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "Math.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Path.hpp"

namespace AO {

MeatSaw::~MeatSaw()
{
    if (mResetOffscreen && SwitchStates_Get(mSwitchId) != field_F2_switch_value)
    {
        Path::TLV_Reset(mTlvId, 1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvId, 0, 0, 0);
    }

    mMotorAnim.VCleanUp();
}

void MeatSaw::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MeatSawMotor));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MeatSaw_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MeatSaw_Moving));
}

MeatSaw::MeatSaw(relive::Path_MeatSaw* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eMeatSaw);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::MeatSaw_Idle));
    
    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_0);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_24);
        SetScale(Scale::Fg);
    }

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 8);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    field_F6_switch_min_time_off = pTlv->mSwitchMinTimeOff;
    field_F8_switch_max_time_off = pTlv->mSwitchMaxTimeOff;

    field_E6_max_rise_time = pTlv->mMaxRiseTime;
    mSwitchId = pTlv->mSwitchId;

    mYOffset = 0;
    field_F4 = 0;

    if (pTlv->mType == relive::Path_MeatSaw::Type::eAutomatic)
    {
        mResetOffscreen = true;
        mUsesSwitchId = false;
    }
    else if (pTlv->mType == relive::Path_MeatSaw::Type::eSwitchId)
    {
        mResetOffscreen = true;
        mUsesSwitchId = true;
    }
    else // eAutomaticPersistOffscreen_0
    {
        mResetOffscreen = false;
        mUsesSwitchId = false;
    }

    field_EA_speed1 = pTlv->mSpeed;
    field_E8_speed2 = pTlv->mSpeed;
    if (pTlv->mStartState == relive::Path_MeatSaw::StartState::eOff)
    {
        field_F0_switch_value = SwitchStates_Get(pTlv->mSwitchId) == 0;
    }
    else
    {
        if (pTlv->mSwitchId)
        {
            field_F0_switch_value = SwitchStates_Get(pTlv->mSwitchId);
        }
        else
        {
            field_F0_switch_value = 0;
        }
    }

    if (pTlv->mSwitchId)
    {
        field_F2_switch_value = SwitchStates_Get(pTlv->mSwitchId);
    }
    else
    {
        field_F2_switch_value = 0;
    }

    field_FA_automatic_min_time_off = pTlv->mAutomaticMinTimeOff;

    field_EC_off_speed = pTlv->mOffSpeed;
    field_FC_automatic_max_time_off = pTlv->mAutomaticMaxTimeOff;

    if (pTlv->mTlvSpecificMeaning)
    {
        field_F0_switch_value = field_F0_switch_value == 0;
        field_F2_switch_value = field_F2_switch_value == 0;
    }

    mIdleTimer = 0;
    mState = MeatSawStates::eIdle_0;
    field_10C_FrameCount = 0;
    mYPos -= FP_FromInteger(pTlv->mMaxRiseTime);
    mTlvId = tlvId;

    if (pTlv->mInitialPosition != 0)
    {
        mState = MeatSawStates::eGoingUp_2;
        field_F4 = pTlv->mMaxRiseTime + pTlv->mSpeed - pTlv->mMaxRiseTime % pTlv->mSpeed;
    }

    if (mMotorAnim.Init(
        GetAnimRes(AnimId::MeatSawMotor),
            this))
    {
        mMotorAnim.SetRenderLayer(GetAnimation().GetRenderLayer());
        mMotorAnim.SetSpriteScale(GetSpriteScale());

        mMotorAnim.SetRGB(mRGB.r, mRGB.g, mRGB.b);

        mMotorAnim.SetBlendMode(relive::TBlendModes::eBlend_0);

        mMotorAnim.SetBlending(false);
        mMotorAnim.SetSemiTrans(true);

        CreateShadow();
    }
    else
    {
        SetListAddFailed(true);
    }
}

void MeatSaw::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged() || !sControlledCharacter || // Can be nullptr during the game ender
        FP_Abs(sControlledCharacter->mXPos - mXPos) > FP_FromInteger(1024))
    {
        SetDead(true);
    }
}

void MeatSaw::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    GrindUpObjects_439CD0();

    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    if (!(field_10C_FrameCount % 87))
    {
        SFX_Play_Camera(relive::SoundEffects::MeatsawOffscreen, 45, direction);
    }

    if (!(field_10C_FrameCount % 25))
    {
        SFX_Play_Camera(relive::SoundEffects::MeatsawIdle, 45, direction);
    }

    field_10C_FrameCount++;

    switch (mState)
    {
        case MeatSawStates::eIdle_0:
            if ((mIdleTimer <= static_cast<s32>(sGnFrame) || mUsesSwitchId) &&
                (!mResetOffscreen || SwitchStates_Get(mSwitchId) == field_F0_switch_value))
            {
                mState = MeatSawStates::eGoingDown_1;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSaw_Moving));
                mAutomaticMeatSawIsDown = false;
                field_E8_speed2 = field_EA_speed1;
                mSfxTimer = MakeTimer(2);
            }
            else
            {
                if (mResetOffscreen)
                {
                    if (!mUsesSwitchId)
                    {
                        if (field_EC_off_speed)
                        {
                            if (mIdleTimer <= static_cast<s32>(sGnFrame))
                            {
                                mState = MeatSawStates::eGoingDown_1;
                                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSaw_Moving));
                                mAutomaticMeatSawIsDown = true;
                                field_E8_speed2 = field_EC_off_speed;
                                mSfxTimer = MakeTimer(2);
                            }
                        }
                    }
                }
            }
            break;

        case MeatSawStates::eGoingDown_1:
            field_F4 += field_E8_speed2;

            if (!((sGnFrame - mSfxTimer) % 8))
            {
                SFX_Play_Camera(relive::SoundEffects::MeatsawDown, 50, direction);
            }

            if (field_F4 >= field_E6_max_rise_time)
            {
                mState = MeatSawStates::eGoingUp_2;
                mSfxTimer = MakeTimer(2);
            }
            break;

        case MeatSawStates::eGoingUp_2:
            if (!((sGnFrame - mSfxTimer) % 10))
            {
                mSfxTimer = sGnFrame;
                SFX_Play_Camera(relive::SoundEffects::MeatsawUp, 50, direction);
            }

            field_F4 -= field_E8_speed2;

            if (field_F4 <= 0)
            {
                mState = MeatSawStates::eIdle_0;
                s16 minRnd = 0;
                s16 maxRnd = 0;
                if (mAutomaticMeatSawIsDown)
                {
                    maxRnd = field_FC_automatic_max_time_off;
                    minRnd = field_FA_automatic_min_time_off;
                }
                else
                {
                    maxRnd = field_F8_switch_max_time_off;
                    minRnd = field_F6_switch_min_time_off;
                }

                mIdleTimer = MakeTimer(Math_RandomRange(minRnd, maxRnd));
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSaw_Idle));
                if (mUsesSwitchId)
                {
                    SwitchStates_Set(mSwitchId, field_F0_switch_value == 0 ? 1 : 0);
                }
            }
            break;
    }
}

void MeatSaw::GrindUpObjects_439CD0()
{
    PSX_RECT ourRect = VGetBoundingRect();

    ourRect.y += mYOffset;
    ourRect.h += mYOffset;

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->GetIsBaseAliveGameObject())
        {
            if (pObjIter->GetDrawable())
            {
                // Can't grind meat with a meat saw, that would be grindception
                if (pObjIter->Type() != ReliveTypes::eMeat)
                {
                    const PSX_RECT objRect = pObjIter->VGetBoundingRect();

                    if (RectsOverlap(ourRect, objRect) && pObjIter->GetSpriteScale() == GetSpriteScale() && pObjIter->mHealth > FP_FromInteger(0))
                    {
                        if (pObjIter->mXPos >= FP_FromInteger(ourRect.x) && pObjIter->mXPos <= FP_FromInteger(ourRect.w))
                        {
                            if (!pObjIter->VTakeDamage(this))
                            {
                                return;
                            }

                            relive_new Blood(
                                pObjIter->mXPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(-5),
                                FP_FromInteger(5),
                                GetSpriteScale(),
                                50);

                            relive_new Blood(
                                pObjIter->mXPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(0),
                                FP_FromInteger(5),
                                GetSpriteScale(),
                                50);

                            relive_new Blood(
                                pObjIter->mXPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(5),
                                FP_FromInteger(5),
                                GetSpriteScale(),
                                50);

                            SfxPlayMono(relive::SoundEffects::KillEffect, 127);
                            SFX_Play_Pitch(relive::SoundEffects::KillEffect, 127, -700);
                            return;
                        }
                    }
                }
            }
        }
    }
}

void MeatSaw::VRender(OrderingTable& ot)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mYOffset = field_F4;
        BaseAnimatedWithPhysicsGameObject::VRender(ot);

        mMotorAnim.VRender(
            FP_GetExponent(mXPos
                           + FP_FromInteger(gScreenManager->mCamXOff)
                           - gScreenManager->mCamPos->x),
            FP_GetExponent(mYPos
                           + (FP_FromInteger(gScreenManager->mCamYOff + field_E6_max_rise_time))
                           - gScreenManager->mCamPos->y),
            ot,
            0,
            0);
    }
}

} // namespace AO
