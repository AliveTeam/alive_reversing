#include "stdafx_ao.h"
#include "Function.hpp"
#include "BackgroundAnimation.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/Animation.hpp"
#include "Midi.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
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

BackgroundAnimation::BackgroundAnimation(relive::Path_BackgroundAnimation* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mBaseGameObjectTypeId = ReliveTypes::eBackgroundAnimation;
    mTlvInfo = tlvId;

    // TODO: Convert to animid/combine them
    /*
    const BgAnimRecord& anim = AO::BgAnimRec(pTlv->mAnimId);
    field_E4_res = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, anim.mBgAnimId, 1, 0);
    if (!field_E4_res)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    auto pHeader = reinterpret_cast<AnimationFileHeader*>(*field_E4_res);
    */
    AnimResource res = ResourceManagerWrapper::LoadAnimation(AnimId::Abe_Head_Gib);

    

    // TODO: Refactor to use min/max
    auto xMax = pTlv->mTopLeftX + static_cast<s32>(res.mJsonPtr->mAttributes.mMaxWidth);
    if (pTlv->mTopLeftX <= pTlv->mTopLeftX + static_cast<s32>(res.mJsonPtr->mAttributes.mMaxWidth))
    {
        xMax = pTlv->mTopLeftX;
    }
    field_E8_xpos = static_cast<s16>(xMax);

    auto wMax = pTlv->mTopLeftX + static_cast<s32>(res.mJsonPtr->mAttributes.mMaxWidth);
    if (wMax <= static_cast<s32>(pTlv->mTopLeftX))
    {
        wMax = pTlv->mTopLeftX;
    }
    field_EC_w = static_cast<s16>(wMax);

    auto yMax = pTlv->mTopLeftY + res.mJsonPtr->mAttributes.mMaxHeight;
    if (pTlv->mTopLeftY <= static_cast<s32>(yMax))
    {
        yMax = pTlv->mTopLeftY;
    }
    field_EA_ypos = static_cast<s16>(yMax);

    auto hMax = pTlv->mTopLeftY + res.mJsonPtr->mAttributes.mMaxHeight;
    if (static_cast<s32>(pTlv->mTopLeftY + res.mJsonPtr->mAttributes.mMaxHeight) <= pTlv->mTopLeftY)
    {
        hMax = pTlv->mTopLeftY;
    }
    field_EE_h = static_cast<s16>(hMax);

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    field_F8_animXPos = FP_FromInteger(pTlv->mTopLeftX);
    field_FC_animYPos = FP_FromInteger(pTlv->mTopLeftY);

    /*
    if (pHeader->field_4_frame_table_offset != anim.mFrameTableOffset ||
        pHeader->field_0_max_w != anim.mMaxW ||
        pHeader->field_2_max_h != anim.mMaxH)
    {
        LOG_WARNING("anim id entry data doesn't match OG data!");
        LOG_WARNING("OG data: anim id " << pTlv->mAnimId << " frametableoffset " << pHeader->field_4_frame_table_offset << " maxW " << pHeader->field_0_max_w << " maxH " << pHeader->field_2_max_h);
        LOG_WARNING("anim id data: anim id " << anim.mBgAnimId << " frametableoffset " << anim.mFrameTableOffset << " maxW " << anim.mMaxW << " maxH " << anim.mMaxH);
    }*/

    Animation_Init(res);

    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans, pTlv->mIsSemiTrans == relive::reliveChoice::eYes);
    mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);

    mAnim.mRenderMode = pTlv->mSemiTransMode;

    mAnim.mRenderLayer = Layer::eLayer_1;
    mYOffset = 0;

    mSoundEffect = pTlv->mSoundEffect;
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
        if (!field_104_sound_channels_mask)
        {
            switch (mSoundEffect)
            {
                case relive::Path_BackgroundAnimation::BgAnimSounds::eFire:
                    field_104_sound_channels_mask = SfxPlayMono(relive::SoundEffects::LoudFire, 0);
                    break;
            }
        }
        mXPos = field_F8_animXPos + FP_FromInteger(gTweak_X_5076D8);
        mYPos = field_FC_animYPos + FP_FromInteger(gTweak_Y_5076DC);
    }
}

void BackgroundAnimation::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

BackgroundAnimation::~BackgroundAnimation()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    if (field_104_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_104_sound_channels_mask);
    }
}
} // namespace AO
