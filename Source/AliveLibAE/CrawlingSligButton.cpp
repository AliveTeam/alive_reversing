#include "stdafx.h"
#include "CrawlingSligButton.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Path.hpp"

struct ButtonSfxEntry final
{
    relive::Path_CrawlingSligButton::ButtonSounds mTlvButton;
    // TODO: Give sane names
    relive::SoundEffects mBlockIdx;
    s8 mNote;
    s16 mPitchMin;
};

static const ButtonSfxEntry sButtonSfxInfo[8] = {
    {relive::Path_CrawlingSligButton::ButtonSounds::None, relive::SoundEffects::WellExit, 25u, 80}, // Can never be used ??
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit1, relive::SoundEffects::SackHit, 95u, 57},
    {relive::Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2, relive::SoundEffects::FallingItemPresence2, 49u, 25},
    {relive::Path_CrawlingSligButton::ButtonSounds::SecurityOrb, relive::SoundEffects::SecurityOrb, 25u, 35},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit2, relive::SoundEffects::SackHit, 35u, 0},
    {relive::Path_CrawlingSligButton::ButtonSounds::Bullet1, relive::SoundEffects::Bullet1, 0u, 19040},
    {relive::Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement, relive::SoundEffects::AbeGenericMovement, 65u, 19344},
};

void CrawlingSligButton::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::CrawlingSligButton));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::CrawlingSligButtonUse));
}

CrawlingSligButton::CrawlingSligButton(relive::Path_CrawlingSligButton* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0),
    mTlvId(tlvId),
    mSwitchId(pTlv->mSwitchId),
    mAction(pTlv->mAction),
    mOnSound(pTlv->mOnSound),
    mOffSound(pTlv->mOffSound),
    mSoundDirection(pTlv->mSoundDirection + 1)
{
    SetType(ReliveTypes::eSligButton);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::CrawlingSligButton));

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
    }

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRightY);
}

void CrawlingSligButton::UseButton()
{
    if (!mInUse)
    {
        mInUse = true;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::CrawlingSligButtonUse));
    }
}

CrawlingSligButton::~CrawlingSligButton()
{
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void CrawlingSligButton::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (mInUse)
    {
        SfxPlayMono(relive::SoundEffects::LeverPull, 0);
        EventBroadcast(kEventNoise, this);
        EventBroadcast(kEventSuspiciousNoise, this);

        const s32 old_switch_state = SwitchStates_Get(mSwitchId);
        SwitchStates_Do_Operation(mSwitchId, mAction);
        const s32 new_switch_state = SwitchStates_Get(mSwitchId);

        if (old_switch_state != new_switch_state)
        {
            const auto sound_id = new_switch_state ? mOnSound : mOffSound;
            if (sound_id != relive::Path_CrawlingSligButton::ButtonSounds::None)
            {
                for (const auto& entry : sButtonSfxInfo)
                {
                    if (entry.mTlvButton == sound_id)
                    {
                        SFX_Play_Stereo(
                            entry.mBlockIdx,
                            entry.mNote + entry.mPitchMin * (mSoundDirection & 2),
                            entry.mNote + entry.mPitchMin * (mSoundDirection & 1),
                            GetSpriteScale());
                        break;
                    }
                }
            }
        }

        mInUse = false;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::CrawlingSligButton));
    }
}
