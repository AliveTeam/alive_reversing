#include "stdafx_ao.h"
#include "Function.hpp"
#include "PullRingRope.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"
#include "Rope.hpp"
#include "../relive_lib/Events.hpp"

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
            Animation_Init(AnimId::Pullring_Farms_Idle, ppRes1);
            lvl_x_off = -2;
            break;
        }

        case EReliveLevelIds::eDesert:
        {
            const AnimRecord rec2 = AO::AnimRec(AnimId::Pullring_Desert_Idle);
            u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init(AnimId::Pullring_Desert_Idle, ppRes2);
            lvl_x_off = 2;
            break;
        }

        default:
        {
            const AnimRecord rec2 = AO::AnimRec(AnimId::Pullring_Desert_Idle);
            u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init(AnimId::Pullring_Desert_Idle, ppRes2);
            lvl_x_off = 0;
            break;
        }
    }

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_EE_switch_id = pTlv->mSwitchId;
    field_F0_action = pTlv->mAction;
    field_E8_tlv_info = tlvInfo;
    field_EC_state = States::eIdle_0;
    field_E4_stay_in_state_ticks = 0;

    mYPos += FP_FromInteger(pTlv->mRopeLength + pTlv->mTopLeft.y + 24);
    mXPos = FP_FromInteger(pTlv->mTopLeft.x + 12);

    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_8;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_27;
        mScale = Scale::Fg;
    }

    field_100_sound_direction = pTlv->mSoundDirection;

    field_FC_on_sound = pTlv->mOnSound;
    field_FE_off_sound = pTlv->mOffSound;

    field_F4_pPuller = nullptr;

    field_F8_pRope = relive_new Rope(
        FP_GetExponent(mXPos + FP_FromInteger((lvl_x_off + 1))),
        FP_GetExponent(mYPos) - pTlv->mRopeLength,
        FP_GetExponent(mYPos + (FP_FromInteger(mYOffset))),
        mSpriteScale);
    if (field_F8_pRope)
    {
        field_F8_pRope->mBaseGameObjectRefCount++;
    }
}

bool PullRingRope::vIsNotBeingPulled()
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
    Path::TLV_Reset(field_E8_tlv_info, -1, 0, 0);

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
    mVelY = FP_FromInteger(2);
    field_E4_stay_in_state_ticks = 6;

    SwitchStates_Do_Operation(field_EE_switch_id, field_F0_action);

    if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
    {
        mAnim.Set_Animation_Data(AnimId::Pullring_Farms_UseBegin, nullptr);
    }
    else
    {
        mAnim.Set_Animation_Data(AnimId::Pullring_Desert_UseBegin, nullptr);
    }
    SfxPlayMono(SoundEffect::RingRopePull_65, 0, 0);
    return 1;
}

void PullRingRope::VUpdate()
{
    if (EventGet(kEventDeathReset))
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
            if (mAnim.mCurrentFrame == 2)
            {
                SfxPlayMono(SoundEffect::RingRopePull_65, 0);
            }

            mYPos += mVelY;
            field_F4_pPuller->mYPos += mVelY;
            field_E4_stay_in_state_ticks--;

            if (field_E4_stay_in_state_ticks == 0)
            {
                mVelY = FP_FromInteger(0);
                field_EC_state = States::eTriggerEvent_2;

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SfxPlayMono(SoundEffect::IndustrialTrigger_97, 0);
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
            mVelY = FP_FromInteger(4);
            field_EC_state = States::eReturnToIdle_3;
            field_F4_pPuller->mBaseGameObjectRefCount--;
            field_F4_pPuller = nullptr;

            field_E4_stay_in_state_ticks = 3;

            if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
            {
                mAnim.Set_Animation_Data(AnimId::Pullring_Farms_UseEnd, nullptr);
            }
            else
            {
                mAnim.Set_Animation_Data(AnimId::Pullring_Desert_UseEnd, nullptr);
            }
            break;

        case States::eReturnToIdle_3:
            mYPos -= mVelY;
            field_E4_stay_in_state_ticks--;
            if (field_E4_stay_in_state_ticks == 0)
            {
                mVelY = FP_FromInteger(0);
                field_EC_state = States::eIdle_0;

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    mAnim.Set_Animation_Data(AnimId::Pullring_Farms_Idle, nullptr);
                }
                else
                {
                    mAnim.Set_Animation_Data(AnimId::Pullring_Desert_Idle, nullptr);
                }
            }
            break;

        default:
            break;
    }

    field_F8_pRope->mYPos = FP_NoFractional(FP_FromInteger(mYOffset - 16) + mYPos);
}

} // namespace AO
