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

CrawlingSligButton::CrawlingSligButton(Path_CrawlingSligButton* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eSligButton_16);

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSligButton);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
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

    field_F8_switch_id = pTlv->field_12_switch_id;
    field_FA_action = pTlv->field_14_action;
    field_FC_on_sound = pTlv->field_16_on_sound;
    field_FE_off_sound = pTlv->field_18_off_sound;

    field_100_sound_direction = pTlv->field_1A_sound_direction + 1;

    field_102_in_use = 0;

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
}

void CrawlingSligButton::UseButton()
{
    if (!field_102_in_use)
    {
        field_102_in_use = 1;
        const AnimRecord& rec = AnimRec(AnimId::CrawlingSligButtonUse);
        field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
    }
}

CrawlingSligButton::~CrawlingSligButton()
{
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}

void CrawlingSligButton::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (field_102_in_use == 1)
    {
        SFX_Play_Mono(SoundEffect::LeverPull_63, 0);
        Event_Broadcast(kEventNoise, this);
        Event_Broadcast(kEventSuspiciousNoise, this);

        const s32 old_switch_state = SwitchStates_Get(field_F8_switch_id);
        SwitchStates_Do_Operation(field_F8_switch_id, field_FA_action);
        const s32 new_switch_state = SwitchStates_Get(field_F8_switch_id);

        if (old_switch_state != new_switch_state)
        {
            const auto sound_id = new_switch_state ? field_FC_on_sound : field_FE_off_sound;
            if (sound_id != CrawlingSligButtonSounds::None_0)
            {
                SFX_Play_Stereo(
                    static_cast<SoundEffect>(buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_0_block_idx),
                    buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_2_note + buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_4_pitch_min * (field_100_sound_direction & 2),
                    buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_2_note + buttonSfxInfo_544488[static_cast<u16>(sound_id)].field_4_pitch_min * (field_100_sound_direction & 1),
                    field_CC_sprite_scale);
            }
        }

        field_102_in_use = 0;
        const AnimRecord& rec = AnimRec(AnimId::CrawlingSligButton);
        field_20_animation.Set_Animation_Data(rec.mFrameTableOffset, nullptr);
    }
}
