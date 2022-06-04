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

BackgroundAnimation::BackgroundAnimation(Path_BackgroundAnimation* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eBackgroundAnimation;
    field_F0_tlvInfo = tlvInfo;

    const BgAnimRecord& anim = AO::BgAnimRec(pTlv->field_18_animation_id);
    field_E4_res = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, anim.mBgAnimId, 1, 0);
    if (!field_E4_res)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
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

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

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

    field_10_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->field_1A_is_semi_trans == Choice_short::eYes_1);
    field_10_anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

    field_10_anim.mRenderMode = pTlv->field_1C_semi_trans_mode;

    field_10_anim.mRenderLayer = Layer::eLayer_1;
    field_C8_yOffset = 0;

    field_100_sound_effect = pTlv->field_1E_sound_effect;
    if (field_100_sound_effect == BgAnimSounds::eFire_1) // Apparently there is only 1 possible sound effect
    {
        field_100_sound_effect = BgAnimSounds::eFireIdx_40;
    }
    else
    {
        field_100_sound_effect = BgAnimSounds::eNone_m1;
    }
    field_104_sound_channels_mask = 0;
}

BackgroundAnimation::~BackgroundAnimation()
{
    gMap.TLV_Reset(field_F0_tlvInfo, -1, 0, 0);
    if (field_104_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4774A0(field_104_sound_channels_mask);
    }
}

void BackgroundAnimation::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void BackgroundAnimation::VStopAudio()
{
    if (field_104_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4774A0(field_104_sound_channels_mask);
        field_104_sound_channels_mask = 0;
    }
}

void BackgroundAnimation::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (!field_104_sound_channels_mask && field_100_sound_effect >= BgAnimSounds::eNone_0)
        {
            // play fire sounds
            field_104_sound_channels_mask = SFX_Play_Mono(static_cast<SoundEffect>(field_100_sound_effect), 0, 0);
        }
        mBaseAnimatedWithPhysicsGameObject_XPos = field_F8_animXPos + FP_FromInteger(gTweak_X_5076D8);
        mBaseAnimatedWithPhysicsGameObject_YPos = field_FC_animYPos + FP_FromInteger(gTweak_Y_5076DC);
    }
}

} // namespace AO
