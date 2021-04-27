#include "stdafx.h"
#include "RockSack.hpp"
#include "Rock.hpp"
#include "Function.hpp"
#include "ThrowableArray.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "Abe.hpp"

RockSack* RockSack::ctor_49F100(Path_RockSack* pTlv, s32 tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x546B88);

    field_4_typeId = AETypes::eRockSack_106;

    const AnimRecord& rec = AnimRec(AnimId::Rock_Sack_A);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);

    // Set RockSack idle animation speed.
    auto pAnimationHeader = reinterpret_cast<AnimationHeader*>(*ppRes + rec.mFrameTableOffset);
    pAnimationHeader->field_0_fps = 0;

    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_DC_bApplyShadows &= ~1u;

    field_118_tlvInfo = tlvInfo;
    field_11C_has_been_hit = 0;
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_124_x_vel = FP_FromRaw(pTlv->field_12_x_vel << 8);
    field_128_y_vel = FP_FromRaw(-256 * pTlv->field_14_y_vel);

    if (pTlv->field_10_fall_direction == XDirection_short::eLeft_0)
    {
        field_124_x_vel = -field_124_x_vel;
    }

    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_11E_rock_amount = pTlv->field_18_rock_amount;
    field_120_can_play_wobble_sound = 1;
    field_122_force_wobble_sound = 1;

    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    field_DC_bApplyShadows |= 2u;
    return this;
}

BaseGameObject* RockSack::VDestructor(s32 flags)
{
    return vdtor_49F2E0(flags);
}

void RockSack::VScreenChanged()
{
    vScreenChanged_49F700();
}

void RockSack::VUpdate()
{
    vUpdate_49F3A0();
}

RockSack* RockSack::vdtor_49F2E0(s32 flags)
{
    dtor_49F310();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void RockSack::dtor_49F310()
{
    SetVTable(this, 0x546B88);
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

void RockSack::vScreenChanged_49F700()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void RockSack::vUpdate_49F3A0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_120_can_play_wobble_sound)
        {
            if (Math_NextRandom() < 40u || field_122_force_wobble_sound)
            {
                field_120_can_play_wobble_sound = 0;
                field_122_force_wobble_sound = 0;
                SFX_Play_46FBA0(SoundEffect::SackWobble_29, 24, Math_RandomRange_496AB0(-2400, -2200));
            }
        }
    }
    else
    {
        field_120_can_play_wobble_sound = 1;
    }

    if (field_11C_has_been_hit)
    {
        if (field_11C_has_been_hit == 1 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            const AnimRecord& animRec = AnimRec(AnimId::Rock_Sack_A);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            field_11C_has_been_hit = 0;
        }
    }
    else
    {
        if (field_20_animation.field_E_frame_change_counter == 0)
        {
            field_20_animation.field_E_frame_change_counter = Math_RandomRange_496AB0(2, 10);
        }

        PSX_RECT bPlayerRect = {};
        sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bPlayerRect, 1);

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);

        if (bRect.x <= bPlayerRect.w && bRect.w >= bPlayerRect.x && bRect.h >= bPlayerRect.y && bRect.y <= bPlayerRect.h && field_CC_sprite_scale == sActiveHero_5C1B68->field_CC_sprite_scale)
        {
            if (gpThrowableArray_5D1E2C)
            {
                if (gpThrowableArray_5D1E2C->field_20_count)
                {
                    if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_31_RunJumpMid_452C10)
                    {
                        const AnimRecord& animRec = AnimRec(AnimId::Rock_Sack_Hit);
                        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                    }
                    else
                    {
                        const AnimRecord& animRec = AnimRec(AnimId::Rock_Sack_B);
                        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                    }
                    field_11C_has_been_hit = 1;
                    return;
                }
            }
            else
            {
                gpThrowableArray_5D1E2C = ae_new<ThrowableArray>();
                gpThrowableArray_5D1E2C->ctor_49A630();
            }

            gpThrowableArray_5D1E2C->Add_49A7A0(field_11E_rock_amount);

            auto pRock = ae_new<Rock>();
            if (pRock)
            {
                pRock->ctor_49E150(field_B8_xpos, field_BC_ypos - FP_FromInteger(30), field_11E_rock_amount);
            }

            pRock->VThrow_49E460(field_124_x_vel, field_128_y_vel);

            SFX_Play_46FA90(SoundEffect::SackHit_25, 0);
            Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

            if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_31_RunJumpMid_452C10)
            {
                const AnimRecord& animRec = AnimRec(AnimId::Rock_Sack_Hit);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }
            else
            {
                const AnimRecord& animRec = AnimRec(AnimId::Rock_Sack_B);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }
            field_11C_has_been_hit = 1;
        }
    }
}
