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
#include "Map.hpp"

namespace AO {

MeatSaw::~MeatSaw()
{
    if (mResetOffscreen && SwitchStates_Get(mSwitchId) != field_F2_switch_value)
    {
        Path::TLV_Reset(mTlvId, 1);
    }
    else
    {
        Path::TLV_Reset(mTlvId, 0);
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

    mSwitchMinTimeOff = pTlv->mSwitchMinTimeOff;
    mSwitchMaxTimeOff = pTlv->mSwitchMaxTimeOff;

    mYTravelDistance = pTlv->mYTravelDistance;
    mSwitchId = pTlv->mSwitchId;

    mYOffset = 0;

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

    mSwitchSpeed = pTlv->mSwitchSpeed;
    mCurrentSpeed = pTlv->mSwitchSpeed;

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

    mAutomaticMinTimeOff = pTlv->mAutomaticMinTimeOff;

    mAutomaticSpeed = pTlv->mAutomaticSpeed;
    mAutomaticMaxTimeOff = pTlv->mAutomaticMaxTimeOff;

    if (pTlv->mTlvSpecificMeaning)
    {
        field_F0_switch_value = field_F0_switch_value == 0;
        field_F2_switch_value = field_F2_switch_value == 0;
    }

    mIdleTimer = 0;
    mState = MeatSawStates::eIdle_0;
    mFrameCountForSfx = 0;
    mYPos -= FP_FromInteger(pTlv->mYTravelDistance);
    mTlvId = tlvId;

    if (pTlv->mStartAtBottom)
    {
        mState = MeatSawStates::eGoingUp_2;
        mRenderYOffset = pTlv->mYTravelDistance + pTlv->mSwitchSpeed - pTlv->mYTravelDistance % pTlv->mSwitchSpeed;
    }

    if (mMotorAnim.Init(GetAnimRes(AnimId::MeatSawMotor), this))
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
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    GrindUpObjects();

    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    if (!(mFrameCountForSfx % 87))
    {
        SFX_Play_Camera(relive::SoundEffects::MeatsawOffscreen, 45, direction);
    }

    if (!(mFrameCountForSfx % 25))
    {
        SFX_Play_Camera(relive::SoundEffects::MeatsawIdle, 45, direction);
    }

    mFrameCountForSfx++;

    switch (mState)
    {
        case MeatSawStates::eIdle_0:
            // Switch used, activate the meat saw
            if ((mIdleTimer <= static_cast<s32>(sGnFrame) || mUsesSwitchId) &&
                (!mResetOffscreen || SwitchStates_Get(mSwitchId) == field_F0_switch_value))
            {
                mState = MeatSawStates::eGoingDown_1;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSaw_Moving));
                mAutomaticMeatSawIsDown = false;
                mCurrentSpeed = mSwitchSpeed;
                mSfxTimer = MakeTimer(2);
                return;
            }

            // Automatic meat saw, activate every x frames
            if (mResetOffscreen && !mUsesSwitchId && mAutomaticSpeed)
            {
                if (mIdleTimer <= static_cast<s32>(sGnFrame))
                {
                    mState = MeatSawStates::eGoingDown_1;
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSaw_Moving));
                    mAutomaticMeatSawIsDown = true;
                    mCurrentSpeed = mAutomaticSpeed;
                    mSfxTimer = MakeTimer(2);
                }
            }
            return;

        case MeatSawStates::eGoingDown_1:
            mRenderYOffset += mCurrentSpeed;

            if (!((sGnFrame - mSfxTimer) % 8))
            {
                SFX_Play_Camera(relive::SoundEffects::MeatsawDown, 50, direction);
            }

            if (mRenderYOffset >= mYTravelDistance)
            {
                mState = MeatSawStates::eGoingUp_2;
                mSfxTimer = MakeTimer(2);
            }
            return;

        case MeatSawStates::eGoingUp_2:
            if (!((sGnFrame - mSfxTimer) % 10))
            {
                mSfxTimer = sGnFrame;
                SFX_Play_Camera(relive::SoundEffects::MeatsawUp, 50, direction);
            }

            mRenderYOffset -= mCurrentSpeed;

            if (mRenderYOffset <= 0)
            {
                mState = MeatSawStates::eIdle_0;
                s16 minRnd = 0;
                s16 maxRnd = 0;
                if (mAutomaticMeatSawIsDown)
                {
                    maxRnd = mAutomaticMaxTimeOff;
                    minRnd = mAutomaticMinTimeOff;
                }
                else
                {
                    maxRnd = mSwitchMaxTimeOff;
                    minRnd = mSwitchMinTimeOff;
                }

                mIdleTimer = MakeTimer(Math_RandomRange(minRnd, maxRnd));
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MeatSaw_Idle));
                if (mUsesSwitchId)
                {
                    SwitchStates_Set(mSwitchId, field_F0_switch_value == 0 ? 1 : 0);
                }
            }
            return;
    }
}

void MeatSaw::GrindUpObjects()
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

        if (pObjIter->GetIsBaseAliveGameObject() && pObjIter->GetDrawable())
        {
            // Can't grind meat with a meat saw, that would be grindception
            if (pObjIter->Type() == ReliveTypes::eMeat)
            {
                continue;
            }

            const PSX_RECT objRect = pObjIter->VGetBoundingRect();

            if (RectsOverlap(ourRect, objRect) &&
                pObjIter->GetSpriteScale() == GetSpriteScale() &&
                pObjIter->mHealth > FP_FromInteger(0))
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

void MeatSaw::VRender(OrderingTable& ot)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mYOffset = mRenderYOffset;
        BaseAnimatedWithPhysicsGameObject::VRender(ot);

        mMotorAnim.VRender(
            FP_GetExponent(mXPos
                           + FP_FromInteger(gScreenManager->mCamXOff)
                           - gScreenManager->mCamPos->x),
            FP_GetExponent(mYPos
                           + (FP_FromInteger(gScreenManager->mCamYOff + mYTravelDistance))
                           - gScreenManager->mCamPos->y),
            ot,
            0,
            0);
    }
}

} // namespace AO
