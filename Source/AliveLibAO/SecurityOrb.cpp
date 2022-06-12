#include "stdafx_ao.h"
#include "Function.hpp"
#include "SecurityOrb.hpp"
#include "ResourceManager.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "Sparks.hpp"
#include "Abe.hpp"
#include "PossessionFlicker.hpp"
#include "ScreenShake.hpp"
#include "Flash.hpp"
#include "Game.hpp"
#include "Events.hpp"

namespace AO {

SecurityOrb::SecurityOrb(Path_SecurityOrb* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);

    mBaseGameObjectTypeId = ReliveTypes::SecurityOrb;

    const AnimRecord& rec = AO::AnimRec(AnimId::Security_Orb);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_10C_tlvInfo = tlvInfo;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    field_110_state = SecurityOrbStates::eIdle_0;
    field_118_sound_channels = 0;
}

SecurityOrb::~SecurityOrb()
{
    if (field_118_sound_channels)
    {
        SND_Stop_Channels_Mask_4774A0(field_118_sound_channels);
    }

    if (mHealth > FP_FromInteger(0))
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_10C_tlvInfo, -1, 0, 1);
    }
}

void SecurityOrb::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

s16 SecurityOrb::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->mBaseGameObjectTypeId)
    {
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eShrykull:
        {
            relive_new Explosion(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5)),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            relive_new Gibs(
                GibType::Metal_5,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                mBaseAnimatedWithPhysicsGameObject_SpriteScale);

            mHealth = FP_FromInteger(0);
        }
        break;

        default:
            break;
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

    return 1;
}

void SecurityOrb::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (field_110_state)
    {
        case SecurityOrbStates::eIdle_0:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 2 || mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 6 || mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 10)
            {
                if (field_118_sound_channels)
                {
                    SND_Stop_Channels_Mask_4774A0(field_118_sound_channels);
                }

                if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
                {
                    field_118_sound_channels = SFX_Play_Pitch(SoundEffect::SecurityOrb_56, 35, 720);
                }
                else
                {
                    field_118_sound_channels = SFX_Play_Pitch(SoundEffect::SecurityOrb_56, 55, 700);
                }
            }

            if (Event_Get(kEventAbeOhm))
            {
                field_110_state = SecurityOrbStates::eDoZapEffects_1;
                field_114_timer = sGnFrame + 20;
            }
            break;

        case SecurityOrbStates::eDoZapEffects_1:
            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                const PSX_RECT abeRect = sActiveHero_507678->VGetBoundingRect();

                const s32 width = abeRect.w + abeRect.x;
                const s32 height = abeRect.h + abeRect.y;

                relive_new ZapLine(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                    FP_FromInteger(width / 2),
                    FP_FromInteger(height / 2),
                    8,
                    ZapLineType::eThick_0,
                    Layer::eLayer_ZapLinesElum_28);

                relive_new PossessionFlicker(sActiveHero_507678, 8, 255, 100, 100);

                sActiveHero_507678->VTakeDamage(this);
                field_114_timer = sGnFrame + 8;
                field_110_state = SecurityOrbStates::eDoFlashAndSound_2;

                relive_new ScreenShake(1);

                auto pSpark1 = relive_new Sparks(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                if (pSpark1)
                {
                    pSpark1->mBaseAnimatedWithPhysicsGameObject_RGB.g = 65;
                    pSpark1->mBaseAnimatedWithPhysicsGameObject_RGB.b = 65;
                    pSpark1->mBaseAnimatedWithPhysicsGameObject_RGB.r = 255;
                }

                auto pSpark2 = relive_new Sparks(
                    mBaseAnimatedWithPhysicsGameObject_XPos,
                    mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                    mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                if (pSpark2)
                {
                    pSpark2->mBaseAnimatedWithPhysicsGameObject_RGB.g = 65;
                    pSpark2->mBaseAnimatedWithPhysicsGameObject_RGB.b = 65;
                    pSpark2->mBaseAnimatedWithPhysicsGameObject_RGB.r = 255;
                }


                for (s32 i = 0; i < 9; i++)
                {
                    auto pSparks = relive_new Sparks(
                        FP_FromInteger(width / 2),
                        FP_FromInteger(height / 2),
                        mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                    if (pSparks)
                    {
                        pSparks->mBaseAnimatedWithPhysicsGameObject_RGB.g = 65;
                        pSparks->mBaseAnimatedWithPhysicsGameObject_RGB.b = 65;
                        pSparks->mBaseAnimatedWithPhysicsGameObject_RGB.r = 255;
                    }
                }
            }
            break;

        case SecurityOrbStates::eDoFlashAndSound_2:
            if (static_cast<s32>(sGnFrame) == field_114_timer - 5 || static_cast<s32>(sGnFrame) == field_114_timer - 1)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 0);
            }

            if (static_cast<s32>(sGnFrame) == field_114_timer - 4)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 0, 1, TPageAbr::eBlend_1, 1);
            }

            if (field_114_timer - sGnFrame == 4)
            {
                SFX_Play_Mono(SoundEffect::Zap1_57, 0, 0);
            }
            else if (field_114_timer - sGnFrame == 1)
            {
                SFX_Play_Mono(SoundEffect::Zap2_58, 0, 0);
            }

            if (static_cast<s32>(sGnFrame) > field_114_timer)
            {
                field_110_state = SecurityOrbStates::eIdle_0;
            }
            break;
    }
}

} // namespace AO
