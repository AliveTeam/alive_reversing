#include "stdafx.h"
#include "SecurityOrb.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "ScreenShake.hpp"
#include "Events.hpp"
#include "Flash.hpp"
#include "ParticleBurst.hpp"
#include "Particle.hpp"
#include "Abe.hpp"
#include "PossessionFlicker.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Explosion.hpp"
#include "ZapLine.hpp"
#include "Function.hpp"
#include "Sparks.hpp"

const TintEntry sSecurityOrbTints_55C1EC[18] = {
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u}, // TODO: eMudomoVault_3 exists twice. eMudomoVault_Ender_11 should probably be here? OG bug?
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};


SecurityOrb::SecurityOrb(Path_SecurityOrb* pTlv, s32 tlvInfo)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eSecurityOrb);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

    const AnimRecord& rec = AnimRec(AnimId::Security_Orb);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    SetTint(sSecurityOrbTints_55C1EC, gMap.mCurrentLevel);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_118_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
    }

    mApplyShadows |= 2u;
    field_11C_state = 0;
    field_124_sound_channels_mask = 0;
}

SecurityOrb::~SecurityOrb()
{
    if (field_124_sound_channels_mask)
    {
        SND_Stop_Channels_Mask(field_124_sound_channels_mask);
    }

    if (mHealth > FP_FromInteger(0))
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_118_tlvInfo, -1, 0, 1);
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

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    mHealth = FP_FromInteger(0);

    if (pFrom->Type() == ReliveTypes::eMineCar || pFrom->Type() == ReliveTypes::eAbilityRing || pFrom->Type() == ReliveTypes::eShrykull)
    {
        relive_new Explosion(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(5)),
            mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            0);

        relive_new Gibs(
            GibType::Metal_5,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            FP_FromInteger(0),
            FP_FromInteger(0),
            mBaseAnimatedWithPhysicsGameObject_SpriteScale,
            0);
    }

    return 1;
}

void SecurityOrb::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    // TODO: untangle
    if (field_11C_state)
    {
        const s32 stateM1 = field_11C_state - 1;
        if (stateM1)
        {
            if (stateM1 == 1)
            {
                if (static_cast<s32>(sGnFrame) == field_120_timer - 5 || static_cast<s32>(sGnFrame) == field_120_timer - 1)
                {
                    relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 0, 0, 1, TPageAbr::eBlend_3, 1);
                }
                if (static_cast<s32>(sGnFrame) == field_120_timer - 4)
                {
                    relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 0, 0, 1, TPageAbr::eBlend_1, 1);
                }

                const s32 timerFrame = field_120_timer - sGnFrame;
                if (timerFrame == 4)
                {
                    SFX_Play_Mono(SoundEffect::Zap1_49, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }
                else if (timerFrame == 1)
                {
                    SFX_Play_Mono(SoundEffect::Zap2_50, 0, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                }

                if (static_cast<s32>(sGnFrame) > field_120_timer)
                {
                    field_11C_state = 0;
                }
            }
        }
        else if (static_cast<s32>(sGnFrame) > field_120_timer)
        {
            const PSX_RECT bRect = sActiveHero->VGetBoundingRect();

            const FP xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP ypos = FP_FromInteger((bRect.y + bRect.h) / 2);

            relive_new ZapLine(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale),
                xpos,
                ypos,
                8,
                ZapLineType::eThick_0,
                Layer::eLayer_ZapLinesMuds_28);

            relive_new PossessionFlicker(sActiveHero, 8, 255, 100, 100);

            if (sActiveHero->mHealth > FP_FromInteger(0))
            {
                sActiveHero->VTakeDamage(this);
            }

            field_120_timer = sGnFrame + 8;
            field_11C_state = 2;

            relive_new ScreenShake(1, 0);

            auto pSpark = relive_new Sparks(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale), mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            if (pSpark)
            {
                pSpark->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(65, 65, 255);
            }

            auto pSpark2 = relive_new Sparks(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos - (FP_FromInteger(8) * mBaseAnimatedWithPhysicsGameObject_SpriteScale), mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            if (pSpark2)
            {
                pSpark2->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(65, 65, 255);
            }

            for (s32 i = 0; i < 9; i++)
            {
                auto pSpark3 = relive_new Sparks(xpos, ypos, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                if (pSpark3)
                {
                    pSpark3->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(65, 65, 255);
                }
            }
        }
    }
    else
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 2 || mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 6 || mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 10)
        {
            if (field_124_sound_channels_mask)
            {
                SND_Stop_Channels_Mask(field_124_sound_channels_mask);
            }

            if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
            {
                field_124_sound_channels_mask = SFX_Play_Pitch(SoundEffect::SecurityOrb_48, 35, 720, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
            else
            {
                field_124_sound_channels_mask = SFX_Play_Pitch(SoundEffect::SecurityOrb_48, 55, 700, mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }
        }

        if (Event_Get(kEventAbeOhm))
        {
            if (!sActiveHero->field_168_ring_pulse_timer || !sActiveHero->field_16C_bHaveShrykull || sActiveHero->mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1))
            {
                field_11C_state = 1;
                field_120_timer = sGnFrame + 20;
            }
        }
    }
}
