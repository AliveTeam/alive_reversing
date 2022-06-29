#include "stdafx_ao.h"
#include "Function.hpp"
#include "Lever.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
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

void Lever::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_E8_state == LeverState::ePulled_1)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3)
        {
            SfxPlayMono(SoundEffect::LeverPull_75, 0, 0);
        }

        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            EventBroadcast(kEventNoise, this);
            EventBroadcast(kEventSuspiciousNoise, this);
            const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms
                || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom
                || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                SfxPlayMono(SoundEffect::IndustrialTrigger_97, 60, 0);
            }
            field_E8_state = LeverState::eFinished_2;

            AnimId animId = AnimId::None;
            if (field_F0_bPulledFromLeft == 0)
            {
                animId = gLeverData_4BCF40[lvl_idx].field_18_releasing_right_animId;
            }
            else
            {
                animId = gLeverData_4BCF40[lvl_idx].field_10_releasing_left_animId;
            }

            const AnimRecord& rec = AO::AnimRec(animId);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);

            const auto oldSwitchState = SwitchStates_Get(field_E4_switch_id);
            SwitchStates_Do_Operation(field_E4_switch_id, field_F2_action);
            const auto newSwitchState = SwitchStates_Get(field_E4_switch_id);

            if (oldSwitchState != newSwitchState)
            {
                s32 leftVol = 0;
                s32 rightVol = 0;

                if (field_F8_sound_direction == LeverSoundDirection::eLeft_1)
                {
                    leftVol = 1;
                    rightVol = 0;
                }
                else if (field_F8_sound_direction == LeverSoundDirection::eRight_2)
                {
                    leftVol = 0;
                    rightVol = 1;
                }
                else
                {
                    leftVol = 1;
                    rightVol = 1;
                }

                if (SwitchStates_Get(field_E4_switch_id))
                {
                    switch (field_F4_on_sound)
                    {
                        case LeverSoundType::eWell_1:
                            SFX_Play_Stereo(SoundEffect::WellExit_24, 50 * leftVol + 10, 50 * rightVol + 10, 0);
                            break;
                        case LeverSoundType::eSwitchBellHammer_2:
                            SFX_Play_Stereo(SoundEffect::SwitchBellHammer_12, 100 * leftVol + 25, 100 * rightVol + 25, 0);
                            return;
                        case LeverSoundType::eDoor_3:
                            SFX_Play_Stereo(SoundEffect::DoorEffect_66, 75 * leftVol + 15, 75 * rightVol + 15, 0);
                            break;
                        case LeverSoundType::eElectricWall_4:
                            SFX_Play_Stereo(SoundEffect::Zap1_57, 35 * leftVol + 25, 35 * rightVol + 25, 0);
                            break;
                        case LeverSoundType::eSecurityOrb_5:
                            SFX_Play_Stereo(SoundEffect::SecurityOrb_56, 35 * leftVol + 25, 35 * rightVol + 25, 0);
                            break;
                        default:
                            return;
                    }
                }
                else
                {
                    switch (field_F6_off_sound)
                    {
                        case LeverSoundType::eWell_1:
                            SFX_Play_Stereo(SoundEffect::WellExit_24, 50 * leftVol + 10, 50 * rightVol + 10, 0);
                            break;
                        case LeverSoundType::eSwitchBellHammer_2:
                            SFX_Play_Stereo(SoundEffect::SwitchBellHammer_12, 110 * leftVol + 25, 110 * rightVol + 25, 0);
                            break;
                        case LeverSoundType::eDoor_3:
                            SFX_Play_Stereo(SoundEffect::DoorEffect_66, 75 * leftVol + 15, 75 * rightVol + 15, 0);
                            break;
                        case LeverSoundType::eElectricWall_4:
                            SFX_Play_Stereo(SoundEffect::Zap1_57, 80 * leftVol + 25, 80 * rightVol + 25, 0);
                            break;
                        case LeverSoundType::eSecurityOrb_5:
                            SFX_Play_Stereo(SoundEffect::SecurityOrb_56, 35 * leftVol + 75, 35 * rightVol + 75, 0);
                            break;
                        default:
                            return;
                    }
                }
            }
        }
    }
    else if (field_E8_state == LeverState::eFinished_2)
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_E8_state = LeverState::eWaiting_0;
            const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].field_0_idle_animId);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(
                rec.mFrameTableOffset,
                nullptr);
        }
    }
}

void Lever::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

Lever::~Lever()
{
    gMap.TLV_Reset(field_EC_tlvInfo, -1, 0, 0);
}

Lever::Lever(Path_Lever* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eLever;
    const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
    const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[lvl_idx].field_0_idle_animId);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(
        rec.mFrameTableOffset,
        rec.mMaxW,
        rec.mMaxH,
        ppRes,
        1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->mBottomRight.x
                                    + pTlv->mTopLeft.x)
                                   / 2);

    field_E4_switch_id = pTlv->field_18_switch_id;
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);
    field_F2_action = pTlv->field_1A_action;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_F4_on_sound = pTlv->field_1E_on_sound;
    field_F6_off_sound = pTlv->field_20_off_sound;
    field_EC_tlvInfo = tlvInfo;
    field_F8_sound_direction = pTlv->field_22_sound_direction;

    field_E8_state = LeverState::eWaiting_0;
}

s32 Lever::VPull(s16 bLeftDirection)
{
    if (field_E8_state == LeverState::eWaiting_0)
    {
        const s32 lvl_idx = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
        field_E8_state = LeverState::ePulled_1;
        if (bLeftDirection)
        {
            const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[lvl_idx].field_C_pulling_left_animId);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(
                rec.mFrameTableOffset,
                nullptr);
            field_F0_bPulledFromLeft = 1;
        }
        else
        {
            const AnimRecord& rec = AO::AnimRec(gLeverData_4BCF40[lvl_idx].field_14_pulling_right_animId);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(
                rec.mFrameTableOffset,
                nullptr);
            field_F0_bPulledFromLeft = 0;
        }
    }
    return SwitchStates_Get(field_E4_switch_id);
}

} // namespace AO
