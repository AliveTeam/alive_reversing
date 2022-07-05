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

void BackgroundAnimation::VStopAudio()
{
    if (field_104_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_104_sound_channels_mask);
        field_104_sound_channels_mask = 0;
    }
}

BackgroundAnimation::BackgroundAnimation(Path_BackgroundAnimation* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eBackgroundAnimation;
    field_F0_tlvInfo = tlvInfo;

    const BgAnimRecord& anim = AO::BgAnimRec(pTlv->field_18_animation_id);
    field_E4_res = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, anim.mBgAnimId, 1, 0);
    if (!field_E4_res)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    auto pHeader = reinterpret_cast<AnimHeader*>(*field_E4_res);

    // TODO: Refactor to use min/max
    auto xMax = pTlv->mTopLeft.x + pHeader->field_0_max_w;
    if (pTlv->mTopLeft.x <= pTlv->mTopLeft.x + pHeader->field_0_max_w)
    {
        xMax = pTlv->mTopLeft.x;
    }
    field_E8_xpos = static_cast<s16>(xMax);

    auto wMax = pTlv->mTopLeft.x + pHeader->field_0_max_w;
    if (wMax <= pTlv->mTopLeft.x)
    {
        wMax = pTlv->mTopLeft.x;
    }
    field_EC_w = static_cast<s16>(wMax);

    auto yMax = pTlv->mTopLeft.y + pHeader->field_2_max_h;
    if (pTlv->mTopLeft.y <= yMax)
    {
        yMax = pTlv->mTopLeft.y;
    }
    field_EA_ypos = static_cast<s16>(yMax);

    auto hMax = pTlv->mTopLeft.y + pHeader->field_2_max_h;
    if (pTlv->mTopLeft.y + pHeader->field_2_max_h <= pTlv->mTopLeft.y)
    {
        hMax = pTlv->mTopLeft.y;
    }
    field_EE_h = static_cast<s16>(hMax);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    field_F8_animXPos = FP_FromInteger(pTlv->mTopLeft.x);
    field_FC_animYPos = FP_FromInteger(pTlv->mTopLeft.y);

    if (pHeader->field_4_frame_table_offset != anim.mFrameTableOffset ||
        pHeader->field_0_max_w != anim.mMaxW ||
        pHeader->field_2_max_h != anim.mMaxH)
    {
        LOG_WARNING("anim id entry data doesn't match OG data!");
        LOG_WARNING("OG data: anim id " << pTlv->field_18_animation_id << " frametableoffset " << pHeader->field_4_frame_table_offset << " maxW " << pHeader->field_0_max_w << " maxH " << pHeader->field_2_max_h);
        LOG_WARNING("anim id data: anim id " << anim.mBgAnimId << " frametableoffset " << anim.mFrameTableOffset << " maxW " << anim.mMaxW << " maxH " << anim.mMaxH);
    }

    Animation_Init(anim.mFrameTableOffset, anim.mMaxW, anim.mMaxH, field_E4_res);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->field_1A_is_semi_trans == Choice_short::eYes_1);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = pTlv->field_1C_semi_trans_mode;

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_1;
    mBaseAnimatedWithPhysicsGameObject_YOffset = 0;

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

void BackgroundAnimation::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (!field_104_sound_channels_mask && field_100_sound_effect >= BgAnimSounds::eNone_0)
        {
            // play fire sounds
            field_104_sound_channels_mask = SfxPlayMono(static_cast<SoundEffect>(field_100_sound_effect), 0, 0);
        }
        mBaseAnimatedWithPhysicsGameObject_XPos = field_F8_animXPos + FP_FromInteger(gTweak_X_5076D8);
        mBaseAnimatedWithPhysicsGameObject_YPos = field_FC_animYPos + FP_FromInteger(gTweak_Y_5076DC);
    }
}

void BackgroundAnimation::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

BackgroundAnimation::~BackgroundAnimation()
{
    gMap.TLV_Reset(field_F0_tlvInfo, -1, 0, 0);
    if (field_104_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_104_sound_channels_mask);
    }
}
} // namespace AO
