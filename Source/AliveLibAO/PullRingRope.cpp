#include "stdafx_ao.h"
#include "Function.hpp"
#include "PullRingRope.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "Events.hpp"

namespace AO {

PullRingRope::PullRingRope(Path_PullRingRope* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::ePullRingRope;

    s32 lvl_x_off = 0;
    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eRuptureFarms:
        case EReliveLevelIds::eBoardRoom:
        case EReliveLevelIds::eRuptureFarmsReturn:
        {
            const AnimRecord rec1 = AO::AnimRec(AnimId::Pullring_Farms_Idle);
            u8** ppRes1 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec1.mResourceId, 1, 0);
            Animation_Init_417FD0(rec1.mFrameTableOffset, rec1.mMaxW, rec1.mMaxH, ppRes1, 1);
            lvl_x_off = -2;
            break;
        }

        case EReliveLevelIds::eDesert:
        {
            const AnimRecord rec2 = AO::AnimRec(AnimId::Pullring_Desert_Idle);
            u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init_417FD0(rec2.mFrameTableOffset, rec2.mMaxW, rec2.mMaxH, ppRes2, 1);
            lvl_x_off = 2;
            break;
        }

        default:
        {
            const AnimRecord rec2 = AO::AnimRec(AnimId::Pullring_Desert_Idle);
            u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init_417FD0(rec2.mFrameTableOffset, rec2.mMaxW, rec2.mMaxH, ppRes2, 1);
            lvl_x_off = 0;
            break;
        }
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_EE_switch_id = pTlv->field_18_switch_id;
    field_F0_action = pTlv->field_1A_action;
    field_E8_tlv_info = tlvInfo;
    field_EC_state = States::eIdle_0;
    field_E4_stay_in_state_ticks = 0;

    mBaseAnimatedWithPhysicsGameObject_YPos += FP_FromInteger(pTlv->field_1C_rope_length + pTlv->field_10_top_left.field_2_y + 24);
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);

    if (pTlv->field_1E_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_100_sound_direction = pTlv->field_24_sound_direction;

    field_FC_on_sound = pTlv->field_20_on_sound;
    field_FE_off_sound = pTlv->field_22_off_sound;

    field_F4_pPuller = nullptr;

    field_F8_pRope = relive_new Rope(
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger((lvl_x_off + 1))),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) - pTlv->field_1C_rope_length,
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + (FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_YOffset))),
        mBaseAnimatedWithPhysicsGameObject_SpriteScale);
    if (field_F8_pRope)
    {
        field_F8_pRope->mBaseGameObjectRefCount++;
    }
}

Bool32 PullRingRope::vIsNotBeingPulled()
{
    return field_EC_state != States::eBeingPulled_1;
}

void PullRingRope::VScreenChanged()
{
    if (!field_F4_pPuller)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

PullRingRope::~PullRingRope()
{
    gMap.TLV_Reset(field_E8_tlv_info, -1, 0, 0);

    if (field_F4_pPuller)
    {
        field_F4_pPuller->mBaseGameObjectRefCount--;
    }

    if (field_F8_pRope)
    {
        field_F8_pRope->mBaseGameObjectFlags.Set(Options::eDead);
        field_F8_pRope->mBaseGameObjectRefCount--;
    }
}

s16 PullRingRope::Pull(BaseAliveGameObject* pFrom)
{
    if (!pFrom)
    {
        return 0;
    }

    if (field_EC_state != States::eIdle_0)
    {
        return 0;
    }

    field_F4_pPuller = pFrom;
    field_F4_pPuller->mBaseGameObjectRefCount++;

    field_EC_state = States::eBeingPulled_1;
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(2);
    field_E4_stay_in_state_ticks = 6;

    SwitchStates_Do_Operation(field_EE_switch_id, field_F0_action);

    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
    {
        const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Farms_UseBegin);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
    }
    else
    {
        const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Desert_UseBegin);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
    }
    SFX_Play_Mono(SoundEffect::RingRopePull_65, 0, 0);
    return 1;
}

void PullRingRope::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_F4_pPuller)
    {
        if (field_F4_pPuller->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            field_F4_pPuller->mBaseGameObjectRefCount--;
            field_F4_pPuller = nullptr;
        }
    }

    switch (field_EC_state)
    {
        case States::eBeingPulled_1:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 2)
            {
                SFX_Play_Mono(SoundEffect::RingRopePull_65, 0);
            }

            mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
            field_F4_pPuller->mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
            field_E4_stay_in_state_ticks--;

            if (field_E4_stay_in_state_ticks == 0)
            {
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                field_EC_state = States::eTriggerEvent_2;

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SFX_Play_Mono(SoundEffect::IndustrialTrigger_97, 0);
                }

                const auto oldSwitchValue = SwitchStates_Get(field_EE_switch_id);
                // TODO: OG bug - operation isn't applied to the switch ??
                const auto switchValAfterOperation = SwitchStates_Get(field_EE_switch_id);

                // Due to seemingly OG bug this can never execute
                if (oldSwitchValue != switchValAfterOperation)
                {
                    s32 volLeft = 0;
                    s32 volRight = 0;
                    if (field_100_sound_direction == PullRingSoundDirection::eLeft_1)
                    {
                        volLeft = 1;
                        volRight = 0;
                    }
                    else if (field_100_sound_direction == PullRingSoundDirection::eRight_2)
                    {
                        volLeft =  0;
                        volRight = 1;
                    }
                    else
                    {
                        volLeft = 1;
                        volRight = 1;
                    }

                    if (SwitchStates_Get(field_EE_switch_id))
                    {
                        switch (field_FC_on_sound)
                        {
                            case PullRingSwitchSound::eWellExit_1:
                                SFX_Play_Stereo(SoundEffect::WellExit_24, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case PullRingSwitchSound::RingBellHammer_2:
                                SFX_Play_Stereo(SoundEffect::RingBellHammer_9, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case PullRingSwitchSound::eDoorEffect_3:
                                SFX_Play_Stereo(SoundEffect::DoorEffect_66, 75 * volLeft + 15, 75 * volRight + 15, nullptr);
                                break;
                        }
                    }
                    else
                    {
                        switch (field_FE_off_sound)
                        {
                            case PullRingSwitchSound::eWellExit_1:
                                SFX_Play_Stereo(SoundEffect::WellExit_24, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case PullRingSwitchSound::RingBellHammer_2:
                                SFX_Play_Stereo(SoundEffect::RingBellHammer_9, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case PullRingSwitchSound::eDoorEffect_3:
                                SFX_Play_Stereo(SoundEffect::DoorEffect_66, 75 * volLeft + 15, 75 * volRight + 15, nullptr);
                                break;
                        }
                    }
                }
            }
            break;

        case States::eTriggerEvent_2:
            mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(4);
            field_EC_state = States::eReturnToIdle_3;
            field_F4_pPuller->mBaseGameObjectRefCount--;
            field_F4_pPuller = nullptr;

            field_E4_stay_in_state_ticks = 3;

            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Farms_UseEnd);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
            }
            else
            {
                const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Desert_UseEnd);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
            }
            break;

        case States::eReturnToIdle_3:
            mBaseAnimatedWithPhysicsGameObject_YPos -= mBaseAnimatedWithPhysicsGameObject_VelY;
            field_E4_stay_in_state_ticks--;
            if (field_E4_stay_in_state_ticks == 0)
            {
                mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);
                field_EC_state = States::eIdle_0;

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Farms_Idle);
                    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                }
                else
                {
                    const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Desert_Idle);
                    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
                }
            }
            break;

        default:
            break;
    }

    field_F8_pRope->mBaseAnimatedWithPhysicsGameObject_YPos = FP_NoFractional(FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_YOffset - 16) + mBaseAnimatedWithPhysicsGameObject_YPos);
}

} // namespace AO
