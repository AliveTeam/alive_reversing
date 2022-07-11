#include "stdafx.h"
#include "SecurityOrb.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "ScreenShake.hpp"
#include "../relive_lib/Events.hpp"
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
#include "ZapSpark.hpp"

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
    Animation_Init(AnimId::Security_Orb, ppRes);

    SetTint(sSecurityOrbTints_55C1EC, gMap.mCurrentLevel);

    mXPos = FP_FromInteger(pTlv->mTopLeft.x);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    mTlvInfo = tlvInfo;

    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_8;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
        mAnim.mRenderLayer = Layer::eLayer_27;
    }

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    mState = States::eIdle_0;
    mSoundChannelsMask = 0;
}

SecurityOrb::~SecurityOrb()
{
    if (mSoundChannelsMask)
    {
        SND_Stop_Channels_Mask(mSoundChannelsMask);
    }

    if (mHealth > FP_FromInteger(0))
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 1);
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
            mXPos,
            mYPos - (mSpriteScale * FP_FromInteger(5)),
            mSpriteScale,
            0);

        relive_new Gibs(
            GibType::Metal_5,
            mXPos,
            mYPos,
            FP_FromInteger(0),
            FP_FromInteger(0),
            mSpriteScale,
            0);
    }

    return 1;
}

void SecurityOrb::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    switch (mState)
    {
        case States::eIdle_0:
            if (mAnim.mCurrentFrame == 2 || mAnim.mCurrentFrame == 6 || mAnim.mCurrentFrame == 10)
            {
                if (mSoundChannelsMask)
                {
                    SND_Stop_Channels_Mask(mSoundChannelsMask);
                }

                if (mSpriteScale == FP_FromDouble(0.5))
                {
                    mSoundChannelsMask = SFX_Play_Pitch(SoundEffect::SecurityOrb_48, 35, 720, mSpriteScale);
                }
                else
                {
                    mSoundChannelsMask = SFX_Play_Pitch(SoundEffect::SecurityOrb_48, 55, 700, mSpriteScale);
                }
            }

            if (EventGet(kEventAbeOhm))
            {
                if (!sActiveHero->mRingPulseTimer || !sActiveHero->mHaveShrykull || sActiveHero->mSpriteScale != FP_FromInteger(1))
                {
                    mState = States::eDoZapEffects_1;
                    mTimer = sGnFrame + 20;
                }
            }
            break;

        case States::eDoZapEffects_1:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                const PSX_RECT bRect = sActiveHero->VGetBoundingRect();

                const FP xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
                const FP ypos = FP_FromInteger((bRect.y + bRect.h) / 2);

                relive_new ZapLine(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * mSpriteScale),
                    xpos,
                    ypos,
                    8,
                    ZapLineType::eThick_0,
                    Layer::eLayer_ZapLinesElumMuds_28);

                relive_new PossessionFlicker(sActiveHero, 8, 255, 100, 100);

                if (sActiveHero->mHealth > FP_FromInteger(0))
                {
                    sActiveHero->VTakeDamage(this);
                }

                mTimer = sGnFrame + 8;
                mState = States::eDoFlashAndSound_2;

                relive_new ScreenShake(1, 0);

                auto pSpark = relive_new ZapSpark(mXPos, mYPos - (FP_FromInteger(8) * mSpriteScale), mSpriteScale);
                if (pSpark)
                {
                    pSpark->mRGB.SetRGB(255, 65, 65);
                }

                auto pSpark2 = relive_new ZapSpark(mXPos, mYPos - (FP_FromInteger(8) * mSpriteScale), mSpriteScale);
                if (pSpark2)
                {
                    pSpark2->mRGB.SetRGB(255, 65, 65);
                }

                for (s32 i = 0; i < 9; i++)
                {
                    auto pSpark3 = relive_new ZapSpark(xpos, ypos, mSpriteScale);
                    if (pSpark3)
                    {
                        pSpark3->mRGB.SetRGB(255, 65, 65);
                    }
                }
            }
            break;

        case States::eDoFlashAndSound_2:
            if (static_cast<s32>(sGnFrame) == mTimer - 5 || static_cast<s32>(sGnFrame) == mTimer - 1)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 0, 0, 1, TPageAbr::eBlend_3, 1);
            }
            if (static_cast<s32>(sGnFrame) == mTimer - 4)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 0, 0, 1, TPageAbr::eBlend_1, 1);
            }

            const s32 timerFrame = mTimer - sGnFrame;
            if (timerFrame == 4)
            {
                SfxPlayMono(SoundEffect::Zap1_49, 0, mSpriteScale);
            }
            else if (timerFrame == 1)
            {
                SfxPlayMono(SoundEffect::Zap2_50, 0, mSpriteScale);
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = States::eIdle_0;
            }
            break;
    }
}
