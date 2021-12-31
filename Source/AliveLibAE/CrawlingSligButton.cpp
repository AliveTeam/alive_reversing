#include "stdafx.h"
#include "CrawlingSligButton.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"

// TODO: Check this is correct
const SfxDefinition buttonSfxInfo_544488[8] = {
    {20u, 0u, 25u, 0u, 80, 11}, // Can never be used ??
    {25u, 0u, 95u, 0u, 57, 15},
    {75u, 0u, 49u, 0u, 25, 35},
    {48u, 0u, 25u, 0u, 35, 30},
    {25u, 0u, 35u, 0u, 0, 0},
    {0u, 0u, 0u, 0u, 19040, 65},
    {32u, 75u, 65u, 0u, 19344, 66},
    // { (s8)160u, (s8)192u, 77u, 0u, (s16)49392, 77 }
};

CrawlingSligButton* CrawlingSligButton::ctor_4148F0(Path_CrawlingSligButton* pTlv, s32 tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5444B4);

    SetType(AETypes::eSligButton_16);

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSligButton);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    field_F4_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else if (pTlv->field_10_scale == Scale_short::eFull_0)
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_25;
    }

    field_F8_id = pTlv->field_12_id;
    field_FA_action = pTlv->field_14_action;
    field_FC_on_sound = pTlv->field_16_on_sound;
    field_FE_off_sound = pTlv->field_18_off_sound;

    field_100_sound_direction = pTlv->field_1A_sound_direction + 1;

    field_102_in_use = 0;

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    return this;
}

BaseGameObject* CrawlingSligButton::VDestructor(s32 flags)
{
    return vdtor_414A60(flags);
}

void CrawlingSligButton::VUpdate()
{
    vUpdate_414B20();
}

void CrawlingSligButton::UseButton_414C60()
{
    if (!field_102_in_use)
    {
        field_102_in_use = 1;
        const AnimRecord& rec = AnimRec(AnimId::CrawlingSligButtonUse);
        field_20_animation.Set_Animation_Data_409C80(rec.mFrameTableOffset, nullptr);
    }
}

CrawlingSligButton* CrawlingSligButton::vdtor_414A60(s32 flags)
{
    dtor_414A90();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void CrawlingSligButton::dtor_414A90()
{
    SetVTable(this, 0x5444B4);
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void CrawlingSligButton::vUpdate_414B20()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_102_in_use == 1)
    {
        SFX_Play_46FA90(SoundEffect::LeverPull_63, 0);
        Event_Broadcast_422BC0(kEventNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

        const s32 old_switch_state = SwitchStates_Get_466020(field_F8_id);
        SwitchStates_Do_Operation_465F00(field_F8_id, field_FA_action);
        const s32 new_switch_state = SwitchStates_Get_466020(field_F8_id);

        if (old_switch_state != new_switch_state)
        {
            const auto sound_id = new_switch_state ? field_FC_on_sound : field_FE_off_sound;
            if (sound_id != CrawlingSligButtonSounds::None_0)
            {
                SFX_Play_46FB10(
                    static_cast<SoundEffect>(buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_0_block_idx),
                    buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_2_note + buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_4_pitch_min * (field_100_sound_direction & 2),
                    buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_2_note + buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_4_pitch_min * (field_100_sound_direction & 1),
                    field_CC_sprite_scale);
            }
        }

        field_102_in_use = 0;
        const AnimRecord& rec = AnimRec(AnimId::CrawlingSligButton);
        field_20_animation.Set_Animation_Data_409C80(rec.mFrameTableOffset, nullptr);
    }
}
