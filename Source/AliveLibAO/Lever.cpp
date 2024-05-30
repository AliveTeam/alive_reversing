#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Lever.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

struct Lever_Data final
{
    AnimId mIdleAnimId;
    AnimId mPullingLeftAnimId;
    AnimId mReleasingLeftAnimId;
    AnimId mPullingRightAnimId;
    AnimId mReleasingRightAnimId;
};

static const Lever_Data sLeverData[16] = {
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // menu
    {AnimId::RuptureFarms_Lever_Idle, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // rupture farms
    {AnimId::Lines_Lever_Idle, AnimId::Lines_Lever_Pull_Left, AnimId::Lines_Lever_Pull_Release_Left, AnimId::Lines_Lever_Pull_Right, AnimId::Lines_Lever_Pull_Release_Right},                                    // lines
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest temple
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // stock yards
    {AnimId::RuptureFarms_Lever_Idle, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // stock yards return
    {AnimId::Lines_Lever_Idle, AnimId::Lines_Lever_Pull_Left, AnimId::Lines_Lever_Pull_Release_Left, AnimId::Lines_Lever_Pull_Right, AnimId::Lines_Lever_Pull_Release_Right},                                    // removed
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // desert
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // desert temple
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // credits
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // removed
    {AnimId::RuptureFarms_Lever_Idle, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // board room
    {AnimId::RuptureFarms_Lever_Idle, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // rupture farms return
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest chase
    {AnimId::Lever_Idle, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right}};                                                                 // desert escape

void Lever::LoadAnimations()
{
    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eRuptureFarms:
        case EReliveLevelIds::eStockYards:
        case EReliveLevelIds::eStockYardsReturn:
        case EReliveLevelIds::eBoardRoom:
        case EReliveLevelIds::eRuptureFarmsReturn:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RuptureFarms_Lever_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RuptureFarms_Lever_Pull_Left));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RuptureFarms_Lever_Pull_Release_Left));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RuptureFarms_Lever_Pull_Right));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RuptureFarms_Lever_Pull_Release_Right));
            break;

        case EReliveLevelIds::eLines:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lines_Lever_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lines_Lever_Pull_Left));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lines_Lever_Pull_Release_Left));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lines_Lever_Pull_Right));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lines_Lever_Pull_Release_Right));
            break;

        case EReliveLevelIds::eDesert:
        case EReliveLevelIds::eDesertTemple:
        case EReliveLevelIds::eDesertEscape:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Left));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Release_Left));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Right));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Release_Right));
            break;

        default:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Left));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Release_Left));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Right));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Lever_Pull_Release_Right));
            break;
    }
}

Lever::Lever(relive::Path_Lever* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eLever);

    LoadAnimations();

    const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
    Animation_Init(GetAnimRes(sLeverData[lvl_idx].mIdleAnimId));

    GetAnimation().SetSemiTrans(true);

    mXPos = FP_FromInteger((pTlv->mBottomRightX
                                    + pTlv->mTopLeftX)
                                   / 2);

    mSwitchId = pTlv->mSwitchId;
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mAction = pTlv->mAction;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
        SetScale(Scale::Fg);
    }

    mOnSound = pTlv->mOnSound;
    mOffSound = pTlv->mOffSound;
    mTlvId = tlvId;
    mSoundDirection = pTlv->mSoundDirection;

    mState = LeverState::eWaiting_0;
}

Lever::~Lever()
{
    Path::TLV_Reset(mTlvId);
}

void Lever::VScreenChanged()
{
    SetDead(true);
}

void Lever::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
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
            EventBroadcast(Event::kEventNoise, this);
            EventBroadcast(Event::kEventSuspiciousNoise, this);
            const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
                || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom
                || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                SfxPlayMono(relive::SoundEffects::IndustrialTrigger, 60);
            }
            mState = LeverState::eFinished_2;

            AnimId animId = AnimId::None;
            if (mPulledFromLeft)
            {
                animId = sLeverData[lvl_idx].mReleasingLeftAnimId;
            }
            else
            {
                animId = sLeverData[lvl_idx].mReleasingRightAnimId;
            }

            GetAnimation().Set_Animation_Data(GetAnimRes(animId));

            const auto oldSwitchState = SwitchStates_Get(mSwitchId);
            SwitchStates_Do_Operation(mSwitchId, mAction);
            const auto newSwitchState = SwitchStates_Get(mSwitchId);

            if (oldSwitchState != newSwitchState)
            {
                s32 leftVol = 0;
                s32 rightVol = 0;

                if (mSoundDirection == relive::Path_Lever::LeverSoundDirection::eLeft)
                {
                    leftVol = 1;
                    rightVol = 0;
                }
                else if (mSoundDirection == relive::Path_Lever::LeverSoundDirection::eRight)
                {
                    leftVol = 0;
                    rightVol = 1;
                }
                else
                {
                    leftVol = 1;
                    rightVol = 1;
                }

                if (SwitchStates_Get(mSwitchId))
                {
                    switch (mOnSound)
                    {
                        case relive::Path_Lever::LeverSoundType::eWell:
                            SFX_Play_Stereo(relive::SoundEffects::WellExit, 50 * leftVol + 10, 50 * rightVol + 10, 0);
                            break;
                        case relive::Path_Lever::LeverSoundType::eSwitchBellHammer:
                            SFX_Play_Stereo(relive::SoundEffects::SwitchBellHammer, 100 * leftVol + 25, 100 * rightVol + 25, 0);
                            return;
                        case relive::Path_Lever::LeverSoundType::eDoor:
                            SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * leftVol + 15, 75 * rightVol + 15, 0);
                            break;
                        case relive::Path_Lever::LeverSoundType::eElectricWall:
                            SFX_Play_Stereo(relive::SoundEffects::Zap1, 35 * leftVol + 25, 35 * rightVol + 25, 0);
                            break;
                        case relive::Path_Lever::LeverSoundType::eSecurityOrb:
                            SFX_Play_Stereo(relive::SoundEffects::SecurityOrb, 35 * leftVol + 25, 35 * rightVol + 25, 0);
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
                            SFX_Play_Stereo(relive::SoundEffects::WellExit, 50 * leftVol + 10, 50 * rightVol + 10, 0);
                            break;
                        case relive::Path_Lever::LeverSoundType::eSwitchBellHammer:
                            SFX_Play_Stereo(relive::SoundEffects::SwitchBellHammer, 110 * leftVol + 25, 110 * rightVol + 25, 0);
                            break;
                        case relive::Path_Lever::LeverSoundType::eDoor:
                            SFX_Play_Stereo(relive::SoundEffects::DoorEffect, 75 * leftVol + 15, 75 * rightVol + 15, 0);
                            break;
                        case relive::Path_Lever::LeverSoundType::eElectricWall:
                            SFX_Play_Stereo(relive::SoundEffects::Zap1, 80 * leftVol + 25, 80 * rightVol + 25, 0);
                            break;
                        case relive::Path_Lever::LeverSoundType::eSecurityOrb:
                            SFX_Play_Stereo(relive::SoundEffects::SecurityOrb, 35 * leftVol + 75, 35 * rightVol + 75, 0);
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
            GetAnimation().Set_Animation_Data(
                GetAnimRes(sLeverData[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].mIdleAnimId));
        }
    }
}




s32 Lever::VPull(s16 bLeftDirection)
{
    if (mState == LeverState::eWaiting_0)
    {
        const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
        mState = LeverState::ePulled_1;
        if (bLeftDirection)
        {
            GetAnimation().Set_Animation_Data(
                GetAnimRes(sLeverData[lvl_idx].mPullingLeftAnimId));
            mPulledFromLeft = true;
        }
        else
        {
            GetAnimation().Set_Animation_Data(
                GetAnimRes(sLeverData[lvl_idx].mPullingRightAnimId));
            mPulledFromLeft = false;
        }
    }
    return SwitchStates_Get(mSwitchId);
}

} // namespace AO
