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

PullRingRope* PullRingRope::ctor_4546B0(Path_PullRingRope* pTlv, s32 tlvInfo)
{
    
    SetVTable(this, 0x4BC058);
    field_4_typeId = Types::ePullRingRope_68;

    s32 lvl_x_off = 0;
    switch (gMap.mCurrentLevel)
    {
        case LevelIds::eRuptureFarms_1:
        case LevelIds::eBoardRoom_12:
        case LevelIds::eRuptureFarmsReturn_13:
        {
            const AnimRecord& rec1 = AO::AnimRec(AnimId::Pullring_Farms_Idle);
            u8** ppRes1 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec1.mResourceId, 1, 0);
            Animation_Init_417FD0(rec1.mFrameTableOffset, rec1.mMaxW, rec1.mMaxH, ppRes1, 1);
            lvl_x_off = -2;
            break;
        }

        case LevelIds::eDesert_8:
        {
            const AnimRecord& rec2 = AO::AnimRec(AnimId::Pullring_Desert_Idle);
            u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init_417FD0(rec2.mFrameTableOffset, rec2.mMaxW, rec2.mMaxH, ppRes2, 1);
            lvl_x_off = 2;
            break;
        }

        default:
        {
            const AnimRecord& rec2 = AO::AnimRec(AnimId::Pullring_Desert_Idle);
            u8** ppRes2 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec2.mResourceId, 1, 0);
            Animation_Init_417FD0(rec2.mFrameTableOffset, rec2.mMaxW, rec2.mMaxH, ppRes2, 1);
            lvl_x_off = 0;
            break;
        }
    }

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_EE_switch_id = pTlv->field_18_switch_id;
    field_F0_action = pTlv->field_1A_action;
    field_E8_tlv_info = tlvInfo;
    field_EC_state = States::eIdle_0;
    field_E4_stay_in_state_ticks = 0;

    field_AC_ypos += FP_FromInteger(pTlv->field_1C_rope_length + pTlv->field_10_top_left.field_2_y + 24);
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);

    if (pTlv->field_1E_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = Layer::eLayer_8;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = Layer::eLayer_27;
        field_C6_scale = 1;
    }

    field_100_sound_direction = pTlv->field_24_sound_direction;

    field_FC_on_sound = pTlv->field_20_on_sound;
    field_FE_off_sound = pTlv->field_22_off_sound;

    field_F4_pPuller = nullptr;

    field_F8_pRope = ao_new<Rope>();
    if (field_F8_pRope)
    {
        field_F8_pRope->ctor_458520(
            FP_GetExponent(field_A8_xpos + FP_FromInteger((lvl_x_off + 1))),
            FP_GetExponent(field_AC_ypos) - pTlv->field_1C_rope_length,
            FP_GetExponent(field_AC_ypos + (FP_FromInteger(field_C8_yOffset))),
            field_BC_sprite_scale);

        field_F8_pRope->field_C_refCount++;
    }
    return this;
}

Bool32 PullRingRope::vIsNotBeingPulled_454D60()
{
    return field_EC_state != States::eBeingPulled_1;
}

void PullRingRope::VScreenChanged_454D70()
{
    if (!field_F4_pPuller)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void PullRingRope::VScreenChanged()
{
    VScreenChanged_454D70();
}

PullRingRope* PullRingRope::Vdtor_454D80(s32 flags)
{
    dtor_454910();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* PullRingRope::VDestructor(s32 flags)
{
    return Vdtor_454D80(flags);
}

BaseGameObject* PullRingRope::dtor_454910()
{
    SetVTable(this, 0x4BC058);
    gMap.TLV_Reset_446870(field_E8_tlv_info, -1, 0, 0);

    if (field_F4_pPuller)
    {
        field_F4_pPuller->field_C_refCount--;
    }

    if (field_F8_pRope)
    {
        field_F8_pRope->mFlags.Set(Options::eDead);
        field_F8_pRope->field_C_refCount--;
    }

    return dtor_417D10();
}

s16 PullRingRope::Pull_454CB0(BaseAliveGameObject* pFrom)
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
    field_F4_pPuller->field_C_refCount++;

    field_EC_state = States::eBeingPulled_1;
    field_B8_vely = FP_FromInteger(2);
    field_E4_stay_in_state_ticks = 6;

    SwitchStates_Do_Operation_436A10(field_EE_switch_id, field_F0_action);

    if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRemoved_11 || gMap.mCurrentLevel == LevelIds::eBoardRoom_12 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
    {
        const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Farms_UseBegin);
        field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
    }
    else
    {
        const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Desert_UseBegin);
        field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
    }
    SFX_Play_43AD70(SoundEffect::RingRopePull_65, 0, 0);
    return 1;
}

void PullRingRope::VUpdate()
{
    VUpdate_4549A0();
}

void PullRingRope::VUpdate_4549A0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (field_F4_pPuller)
    {
        if (field_F4_pPuller->mFlags.Get(BaseGameObject::eDead))
        {
            field_F4_pPuller->field_C_refCount--;
            field_F4_pPuller = nullptr;
        }
    }

    switch (field_EC_state)
    {
        case States::eBeingPulled_1:
            if (field_10_anim.field_92_current_frame == 2)
            {
                SFX_Play_43AD70(SoundEffect::RingRopePull_65, 0);
            }

            field_AC_ypos += field_B8_vely;
            field_F4_pPuller->field_AC_ypos += field_B8_vely;
            field_E4_stay_in_state_ticks--;

            if (field_E4_stay_in_state_ticks == 0)
            {
                field_B8_vely = FP_FromInteger(0);
                field_EC_state = States::eTriggerEvent_2;

                if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eBoardRoom_12 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
                {
                    SFX_Play_43AD70(SoundEffect::IndustrialTrigger_97, 0);
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
                                SFX_Play_43ADE0(SoundEffect::WellExit_24, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case PullRingSwitchSound::RingBellHammer_2:
                                SFX_Play_43ADE0(SoundEffect::RingBellHammer_9, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case PullRingSwitchSound::eDoorEffect_3:
                                SFX_Play_43ADE0(SoundEffect::DoorEffect_66, 75 * volLeft + 15, 75 * volRight + 15, nullptr);
                                break;
                        }
                    }
                    else
                    {
                        switch (field_FE_off_sound)
                        {
                            case PullRingSwitchSound::eWellExit_1:
                                SFX_Play_43ADE0(SoundEffect::WellExit_24, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case PullRingSwitchSound::RingBellHammer_2:
                                SFX_Play_43ADE0(SoundEffect::RingBellHammer_9, 60 * volLeft + 10, 60 * volRight + 10, nullptr);
                                break;
                            case PullRingSwitchSound::eDoorEffect_3:
                                SFX_Play_43ADE0(SoundEffect::DoorEffect_66, 75 * volLeft + 15, 75 * volRight + 15, nullptr);
                                break;
                        }
                    }
                }
            }
            break;

        case States::eTriggerEvent_2:
            field_B8_vely = FP_FromInteger(4);
            field_EC_state = States::eReturnToIdle_3;
            field_F4_pPuller->field_C_refCount--;
            field_F4_pPuller = nullptr;

            field_E4_stay_in_state_ticks = 3;

            if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eBoardRoom_12 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
            {
                const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Farms_UseEnd);
                field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
            }
            else
            {
                const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Desert_UseEnd);
                field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
            }
            break;

        case States::eReturnToIdle_3:
            field_AC_ypos -= field_B8_vely;
            field_E4_stay_in_state_ticks--;
            if (field_E4_stay_in_state_ticks == 0)
            {
                field_B8_vely = FP_FromInteger(0);
                field_EC_state = States::eIdle_0;

                if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eBoardRoom_12 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
                {
                    const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Farms_Idle);
                    field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
                }
                else
                {
                    const AnimRecord& rec = AO::AnimRec(AnimId::Pullring_Desert_Idle);
                    field_10_anim.Set_Animation_Data_402A40(rec.mFrameTableOffset, nullptr);
                }
            }
            break;

        default:
            break;
    }

    field_F8_pRope->field_AC_ypos = FP_NoFractional(FP_FromInteger(field_C8_yOffset - 16) + field_AC_ypos);
}

} // namespace AO
