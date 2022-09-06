#include "stdafx.h"
#include "CrawlingSligButton.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

struct ButtonSfxEntry final
{
    relive::Path_CrawlingSligButton::ButtonSounds mTlvButton;
    // TODO: Give sane names
    relive::SoundEffects field_0_block_idx;
    s8 field_2_note;
    s16 field_4_pitch_min;
};

const ButtonSfxEntry buttonSfxInfo_544488[8] = {
    {relive::Path_CrawlingSligButton::ButtonSounds::None, relive::SoundEffects::WellExit, 25u, 80}, // Can never be used ??
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit1, relive::SoundEffects::SackHit, 95u, 57},
    {relive::Path_CrawlingSligButton::ButtonSounds::FallingItemPresence2, relive::SoundEffects::FallingItemPresence2, 49u, 25},
    {relive::Path_CrawlingSligButton::ButtonSounds::SecurityOrb, relive::SoundEffects::SecurityOrb, 25u, 35},
    {relive::Path_CrawlingSligButton::ButtonSounds::SackHit2, relive::SoundEffects::SackHit, 35u, 0},
    {relive::Path_CrawlingSligButton::ButtonSounds::Bullet1, relive::SoundEffects::Bullet1, 0u, 19040},
    {relive::Path_CrawlingSligButton::ButtonSounds::AbeGenericMovement, relive::SoundEffects::AbeGenericMovement, 65u, 19344},
};

CrawlingSligButton::CrawlingSligButton(relive::Path_CrawlingSligButton* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eSligButton);

    const AnimRecord& rec = AnimRec(AnimId::CrawlingSligButton);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::CrawlingSligButton, ppRes);
    field_F4_tlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }

    field_F8_switch_id = pTlv->mSwitchId;
    field_FA_action = pTlv->mAction;
    field_FC_on_sound = pTlv->mOnSound;
    field_FE_off_sound = pTlv->mOffSound;

    field_100_sound_direction = pTlv->mSoundDirection + 1;

    field_102_in_use = 0;

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mBottomRightY);
}

void CrawlingSligButton::UseButton()
{
    if (!field_102_in_use)
    {
        field_102_in_use = 1;
        mAnim.Set_Animation_Data(AnimId::CrawlingSligButtonUse, nullptr);
    }
}

CrawlingSligButton::~CrawlingSligButton()
{
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}

void CrawlingSligButton::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_102_in_use == 1)
    {
        SfxPlayMono(relive::SoundEffects::LeverPull, 0);
        EventBroadcast(kEventNoise, this);
        EventBroadcast(kEventSuspiciousNoise, this);

        const s32 old_switch_state = SwitchStates_Get(field_F8_switch_id);
        SwitchStates_Do_Operation(field_F8_switch_id, field_FA_action);
        const s32 new_switch_state = SwitchStates_Get(field_F8_switch_id);

        if (old_switch_state != new_switch_state)
        {
            const auto sound_id = new_switch_state ? field_FC_on_sound : field_FE_off_sound;
            if (sound_id != relive::Path_CrawlingSligButton::ButtonSounds::None)
            {
                for (const auto& entry : buttonSfxInfo_544488)
                {
                    if (entry.mTlvButton == sound_id)
                    {
                        SFX_Play_Stereo(
                            entry.field_0_block_idx,
                            entry.field_2_note + entry.field_4_pitch_min * (field_100_sound_direction & 2),
                            entry.field_2_note + entry.field_4_pitch_min * (field_100_sound_direction & 1),
                            mSpriteScale);
                        break;
                    }
                }
            }
        }

        field_102_in_use = 0;
        mAnim.Set_Animation_Data(AnimId::CrawlingSligButton, nullptr);
    }
}
