#include "stdafx.h"
#include "NakedSligButton.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"

// TODO: Check this is correct
const SfxDefinition stru_544488[8] =
{
    { 20u, 0u, 25u, 0u, 80, 11 }, // Can never be used ??
    { 25u, 0u, 95u, 0u, 57, 15 },
    { 75u, 0u, 49u, 0u, 25, 35 },
    { 48u, 0u, 25u, 0u, 35, 30 },
    { 25u, 0u, 35u, 0u, 0, 0 },
    { 0u, 0u, 0u, 0u, 19040, 65 },
    { 32u, 75u, 65u, 0u, 19344, 66 },
    // { (char)160u, (char)192u, 77u, 0u, (short)49392, 77 }
};

NakedSligButton* NakedSligButton::ctor_4148F0(Path_NakedSligButton* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5444B4);

    field_4_typeId = Types::eSligButton_16;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kUnknwonResID_1057);
    Animation_Init_424E10(408, 25, 12, ppRes, 1, 1);
    field_F4_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 6;
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_20_animation.field_C_render_layer = 25;
    }

    field_F8_id = pTlv->field_12_id;
    field_FA_id_action = pTlv->field_14_id_action;
    field_FC_on_sound = pTlv->field_16_on_sound;
    field_FE_off_sound = pTlv->field_18_off_sound;

    field_100_sound_direction = pTlv->field_1A_sound_direction + 1;

    field_102_in_use = 0;

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    return this;
}

BaseGameObject* NakedSligButton::VDestructor(signed int flags)
{
    return vdtor_414A60(flags);
}

void NakedSligButton::VUpdate()
{
    vUpdate_414B20();
}

void NakedSligButton::UseButton_414C60()
{
    if (!field_102_in_use)
    {
        field_102_in_use = 1;
        field_20_animation.Set_Animation_Data_409C80(420, nullptr);
    }
}

NakedSligButton* NakedSligButton::vdtor_414A60(signed int flags)
{
    dtor_414A90();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void NakedSligButton::dtor_414A90()
{
    SetVTable(this, 0x5444B4);
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void NakedSligButton::vUpdate_414B20()
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

        const int old_switch_state = SwitchStates_Get_466020(field_F8_id);
        SwitchStates_Do_Operation_465F00(field_F8_id, field_FA_id_action);
        const int new_switch_state = SwitchStates_Get_466020(field_F8_id);

        if (old_switch_state != new_switch_state)
        {
            const __int16 sound_id = new_switch_state ? field_FC_on_sound : field_FE_off_sound;
            if (sound_id)
            {
                SFX_Play_46FB10(
                    stru_544488[sound_id].field_0_block_idx,
                    stru_544488[sound_id].field_2_note + stru_544488[sound_id].field_4_pitch_min * (field_100_sound_direction & 2),
                    stru_544488[sound_id].field_2_note + stru_544488[sound_id].field_4_pitch_min * (field_100_sound_direction & 1),
                    field_CC_sprite_scale);
            }
        }

        field_102_in_use = 0;
        field_20_animation.Set_Animation_Data_409C80(408, nullptr);
    }
}
