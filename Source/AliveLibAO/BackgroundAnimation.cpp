#include "stdafx_ao.h"
#include "Function.hpp"
#include "BackgroundAnimation.hpp"
#include "ResourceManager.hpp"
#include "Animation.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "DDCheat.hpp"

namespace AO {

BackgroundAnimation* BackgroundAnimation::ctor_405A90(Path_BackgroundAnimation* pTlv, s32 tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BA170);

    field_4_typeId = Types::eBackgroundAnimation_5;
    field_F0_tlvInfo = tlvInfo;

    const BgAnimRecord& anim = AO::BgAnimRec(pTlv->field_18_animation_id);
    field_E4_res = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, anim.mBgAnimId, 1, 0);
    if (!field_E4_res)
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return this;
    }

    auto pHeader = reinterpret_cast<AnimHeader*>(*field_E4_res);

    // TODO: Refactor to use min/max
    auto xMax = pTlv->field_10_top_left.field_0_x + pHeader->field_0_max_w;
    if (pTlv->field_10_top_left.field_0_x <= pTlv->field_10_top_left.field_0_x + pHeader->field_0_max_w)
    {
        xMax = pTlv->field_10_top_left.field_0_x;
    }
    field_E8_xpos = static_cast<s16>(xMax);

    auto wMax = pTlv->field_10_top_left.field_0_x + pHeader->field_0_max_w;
    if (wMax <= pTlv->field_10_top_left.field_0_x)
    {
        wMax = pTlv->field_10_top_left.field_0_x;
    }
    field_EC_w = static_cast<s16>(wMax);

    auto yMax = pTlv->field_10_top_left.field_2_y + pHeader->field_2_max_h;
    if (pTlv->field_10_top_left.field_2_y <= yMax)
    {
        yMax = pTlv->field_10_top_left.field_2_y;
    }
    field_EA_ypos = static_cast<s16>(yMax);

    auto hMax = pTlv->field_10_top_left.field_2_y + pHeader->field_2_max_h;
    if (pTlv->field_10_top_left.field_2_y + pHeader->field_2_max_h <= pTlv->field_10_top_left.field_2_y)
    {
        hMax = pTlv->field_10_top_left.field_2_y;
    }
    field_EE_h = static_cast<s16>(hMax);

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_F8_animXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_FC_animYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    if (pHeader->field_4_frame_table_offset != anim.mFrameTableOffset ||
        pHeader->field_0_max_w != anim.mMaxW ||
        pHeader->field_2_max_h != anim.mMaxH)
    {
        LOG_WARNING("anim id entry data doesn't match OG data!");
        LOG_WARNING("OG data: anim id " << pTlv->field_18_animation_id << " frametableoffset " << pHeader->field_4_frame_table_offset << " maxW " << pHeader->field_0_max_w << " maxH " << pHeader->field_2_max_h);
        LOG_WARNING("anim id data: anim id " << anim.mBgAnimId << " frametableoffset " << anim.mFrameTableOffset << " maxW " << anim.mMaxW << " maxH " << anim.mMaxH);
    }

    Animation_Init_417FD0(
        anim.mFrameTableOffset,
        anim.mMaxW,
        anim.mMaxH,
        field_E4_res,
        1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->field_1A_is_semi_trans);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);

    field_10_anim.field_B_render_mode = pTlv->field_1C_semi_trans_mode;

    field_10_anim.field_C_layer = Layer::eLayer_1;
    field_C8_yOffset = 0;

    field_100_sound_effect = pTlv->field_1E_sound_effect;
    if (field_100_sound_effect == 1) // Apparently there is only 1 possible sound effect
    {
        field_100_sound_effect = 40;
    }
    else
    {
        field_100_sound_effect = -1;
    }
    field_104_sound_channels_mask = 0;

    return this;
}

BaseGameObject* BackgroundAnimation::dtor_405CB0()
{
    SetVTable(this, 0x4BA170);
    gMap_507BA8.TLV_Reset_446870(field_F0_tlvInfo, -1, 0, 0);
    if (field_104_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4774A0(field_104_sound_channels_mask);
    }
    return dtor_417D10();
}

BaseGameObject* BackgroundAnimation::VDestructor(s32 flags)
{
    return Vdtor_405D70(flags);
}

BackgroundAnimation* BackgroundAnimation::Vdtor_405D70(s32 flags)
{
    dtor_405CB0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BackgroundAnimation::VScreenChanged()
{
    VScreenChanged_405D30();
}

void BackgroundAnimation::VScreenChanged_405D30()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void BackgroundAnimation::VStopAudio()
{
    VStopAudio_405D40();
}

void BackgroundAnimation::VStopAudio_405D40()
{
    if (field_104_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4774A0(field_104_sound_channels_mask);
        field_104_sound_channels_mask = 0;
    }
}

void BackgroundAnimation::VUpdate()
{
    VUpdate_405C30();
}

void BackgroundAnimation::VUpdate_405C30()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        if (!field_104_sound_channels_mask && field_100_sound_effect >= 0)
        {
            field_104_sound_channels_mask = SFX_Play_43AD70(static_cast<SoundEffect>(field_100_sound_effect), 0, 0);
        }
        field_A8_xpos = field_F8_animXPos + FP_FromInteger(gTweak_X_5076D8);
        field_AC_ypos = field_FC_animYPos + FP_FromInteger(gTweak_Y_5076DC);
    }
}

} // namespace AO
