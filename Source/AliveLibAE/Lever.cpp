#include "stdafx.h"
#include "Lever.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"

static const TintEntry kLeverTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

void Lever::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Release_Left));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Release_Right));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Left));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Right));
}

Lever::Lever(relive::Path_Lever* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eLever);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Lever_Idle));

    GetAnimation().SetSemiTrans(true);
    mSwitchId = pTlv->mSwitchId;
    mAction = pTlv->mAction;
    mPulledFromLeft = false;

    if (pTlv->mPersistOffscreen == relive::reliveChoice::eYes)
    {
        mPersistOffscreen = true;
    }
    else
    {
        mPersistOffscreen = false;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        SetScale(Scale::Bg);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
        SetScale(Scale::Fg);
    }

    SetTint(&kLeverTints[0], gMap.mCurrentLevel);
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mXPos = FP_FromInteger(SnapToXGrid_AE(GetSpriteScale(), FP_GetExponent(mXPos)));
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    PathLine* pPathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &pPathLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor))
    {
        mYPos = hitY;
    }

    mOnSound = pTlv->mOnSound;
    mOffSound = pTlv->mOffSound;
    mTlvId = tlvId;
    mSoundDirection = pTlv->mSoundDirection;

    mState = LeverState::eWaiting_0;
    SetDoPurpleLightEffect(true);
}

Lever::~Lever()
{
    Path::TLV_Reset(mTlvId);
}

void Lever::VScreenChanged()
{
    if (!mPersistOffscreen || gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

void Lever::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (mState == LeverState::ePulled_1)
    {
        if (GetAnimation().GetCurrentFrame() == 3)
        {
            SfxPlayMono(relive::SoundEffects::LeverPull, 0);
        }

        if (GetAnimation().GetIsLastFrame())
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eMines
                || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz
                || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender
                || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot
                || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender
                || gMap.mCurrentLevel == EReliveLevelIds::eBarracks
                || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender
                || gMap.mCurrentLevel == EReliveLevelIds::eBrewery
                || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                SFX_Play_Pitch(relive::SoundEffects::IndustrialTrigger, 30, 400);
            }

            mState = LeverState::eFinished_2;

            if (mPulledFromLeft)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Lever_Pull_Release_Left));
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Lever_Pull_Release_Right));
            }

            const s32 switch_state = SwitchStates_Get(mSwitchId);
            SwitchStates_Do_Operation(mSwitchId, mAction);
            const s32 switch_state_after_op = SwitchStates_Get(mSwitchId);
            if (switch_state != switch_state_after_op)
            {
                s32 volLeft = 0;
                s32 volRight = 0;

                if (mSoundDirection == relive::Path_Lever::LeverSoundDirection::eLeft)
                {
                    volLeft = 1;
                    volRight = 0;
                }
                else if (mSoundDirection == relive::Path_Lever::LeverSoundDirection::eRight)
                {
                    volLeft = 0;
                    volRight = 1;
                }
                else
                {
                    volLeft = 1;
                    volRight = 1;
                }

                if (switch_state_after_op)
                {
                    switch (mOnSound)
                    {
                        case relive::Path_Lever::LeverSoundType::eWell:
                            SFX_Play_Stereo(relive::SoundEffects::WellExit, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eSwitchBellHammer:
                            SFX_Play_Stereo(relive::SoundEffects::SwitchUnknownTrigger, 100 * volLeft + 25, 100 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eDoor:
                            SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * volLeft + 15, 75 * volRight + 15);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eElectricWall:
                            SFX_Play_Stereo(relive::SoundEffects::Zap1, 35 * volLeft + 25, 35 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eSecurityOrb:
                            SFX_Play_Stereo(relive::SoundEffects::SecurityOrb, 35 * volLeft + 25, 35 * volRight + 25);
                            return;

                        case relive::Path_Lever::LeverSoundType::eLift:
                            SFX_Play_Stereo(relive::SoundEffects::LiftStop, 35 * volLeft + 25, 35 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        default:
                            return;
                    }
                }
                else
                {
                    switch (mOffSound)
                    {
                        case relive::Path_Lever::LeverSoundType::eWell:
                            SFX_Play_Stereo(relive::SoundEffects::WellExit, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eSwitchBellHammer:
                            SFX_Play_Stereo(relive::SoundEffects::SwitchUnknownTrigger, 110 * volLeft + 25, 110 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eDoor:
                            SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * volLeft + 15, 75 * volRight + 15);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eElectricWall:
                            SFX_Play_Stereo(relive::SoundEffects::Zap1, 80 * volLeft + 25, 80 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        case relive::Path_Lever::LeverSoundType::eSecurityOrb:
                            SFX_Play_Stereo(relive::SoundEffects::SecurityOrb, 35 * volLeft + 75, 35 * volRight + 75);
                            break;

                        case relive::Path_Lever::LeverSoundType::eLift:
                            SFX_Play_Stereo(relive::SoundEffects::LiftStop, 35 * volLeft + 25, 35 * volRight + 25);
                            EventBroadcast(kEventNoise, this);
                            EventBroadcast(kEventSuspiciousNoise, this);
                            break;

                        default:
                            return;
                    }
                }
            }
        }
    }
    else if (mState == LeverState::eFinished_2)
    {
        if (GetAnimation().GetForwardLoopCompleted())
        {
            mState = LeverState::eWaiting_0;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Lever_Idle));
        }
    }
}

s16 Lever::VPull(s16 bLeftDirection)
{
    if (mState != LeverState::eWaiting_0)
    {
        return 0;
    }

    mState = LeverState::ePulled_1;

    if (bLeftDirection)
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Lever_Pull_Left));
        mPulledFromLeft = true;
    }
    else
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Lever_Pull_Right));
        mPulledFromLeft = false;
    }

    return 1;
}
