#include "stdafx_ao.h"
#include "Function.hpp"
#include "Lever.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"

namespace AO {

struct Lever_Data final
{
    AnimId field_0_idle_animId;
    s32 field_4_maxW;
    s32 field_8_maxH;
    AnimId field_C_pulling_left_animId;
    AnimId field_10_releasing_left_animId;
    AnimId field_14_pulling_right_animId;
    AnimId field_18_releasing_right_animId;
};
ALIVE_ASSERT_SIZEOF(Lever_Data, 0x1C);

const Lever_Data gLeverData_4BCF40[16] = {
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // menu
    {AnimId::RuptureFarms_Lever_Idle, 67, 40, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // rupture farms
    {AnimId::Lines_Lever_Idle, 66, 41, AnimId::Lines_Lever_Pull_Left, AnimId::Lines_Lever_Pull_Release_Left, AnimId::Lines_Lever_Pull_Right, AnimId::Lines_Lever_Pull_Release_Right},                                    // lines
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest temple
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // stock yards
    {AnimId::RuptureFarms_Lever_Idle, 67, 40, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // stock yards return
    {AnimId::Lines_Lever_Idle, 66, 41, AnimId::Lines_Lever_Pull_Left, AnimId::Lines_Lever_Pull_Release_Left, AnimId::Lines_Lever_Pull_Right, AnimId::Lines_Lever_Pull_Release_Right},                                    // removed
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // desert
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // desert temple
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // credits
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // removed
    {AnimId::RuptureFarms_Lever_Idle, 67, 40, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // board room
    {AnimId::RuptureFarms_Lever_Idle, 67, 40, AnimId::RuptureFarms_Lever_Pull_Left, AnimId::RuptureFarms_Lever_Pull_Release_Left, AnimId::RuptureFarms_Lever_Pull_Right, AnimId::RuptureFarms_Lever_Pull_Release_Right}, // rupture farms return
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right},                                                                  // forest chase
    {AnimId::Lever_Idle, 66, 41, AnimId::Lever_Pull_Left, AnimId::Lever_Pull_Release_Left, AnimId::Lever_Pull_Right, AnimId::Lever_Pull_Release_Right}};                                                                 // desert escape

void Lever::LoadAnimations()
{
    for (u32 i = 0; ALIVE_COUNTOF(gLeverData_4BCF40); i++)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(gLeverData_4BCF40[i].field_0_idle_animId));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(gLeverData_4BCF40[i].field_10_releasing_left_animId));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(gLeverData_4BCF40[i].field_14_pulling_right_animId));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(gLeverData_4BCF40[i].field_18_releasing_right_animId));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(gLeverData_4BCF40[i].field_C_pulling_left_animId));
    }
}

void Lever::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mState == LeverState::ePulled_1)
    {
        if (mAnim.mCurrentFrame == 3)
        {
            SfxPlayMono(relive::SoundEffects::LeverPull, 0);
        }

        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
            const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
                || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom
                || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                SfxPlayMono(relive::SoundEffects::IndustrialTrigger, 60);
            }
            mState = LeverState::eFinished_2;

            AnimId animId = AnimId::None;
            if (field_F0_bPulledFromLeft == 0)
            {
                animId = gLeverData_4BCF40[lvl_idx].field_18_releasing_right_animId;
            }
            else
            {
                animId = gLeverData_4BCF40[lvl_idx].field_10_releasing_left_animId;
            }

            mAnim.Set_Animation_Data(GetAnimRes(animId));

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
        if (mAnim.mFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            mState = LeverState::eWaiting_0;
            mAnim.Set_Animation_Data(
                GetAnimRes(gLeverData_4BCF40[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].field_0_idle_animId));
        }
    }
}

void Lever::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

Lever::~Lever()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

Lever::Lever(relive::Path_Lever* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mBaseGameObjectTypeId = ReliveTypes::eLever;

    LoadAnimations();

    const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
    const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[lvl_idx].field_0_idle_animId);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(gLeverData_4BCF40[lvl_idx].field_0_idle_animId, ppRes);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mXPos = FP_FromInteger((pTlv->mBottomRightX
                                    + pTlv->mTopLeftX)
                                   / 2);

    mSwitchId = pTlv->mSwitchId;
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mAction = pTlv->mAction;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mScale = Scale::Fg;
    }

    mOnSound = pTlv->mOnSound;
    mOffSound = pTlv->mOffSound;
    mTlvInfo = tlvId;
    mSoundDirection = pTlv->mSoundDirection;

    mState = LeverState::eWaiting_0;
}

s32 Lever::VPull(s16 bLeftDirection)
{
    if (mState == LeverState::eWaiting_0)
    {
        const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
        mState = LeverState::ePulled_1;
        if (bLeftDirection)
        {
            mAnim.Set_Animation_Data(
                GetAnimRes(gLeverData_4BCF40[lvl_idx].field_C_pulling_left_animId));
            field_F0_bPulledFromLeft = 1;
        }
        else
        {
            mAnim.Set_Animation_Data(
                GetAnimRes(gLeverData_4BCF40[lvl_idx].field_14_pulling_right_animId));
            field_F0_bPulledFromLeft = 0;
        }
    }
    return SwitchStates_Get(mSwitchId);
}

} // namespace AO
