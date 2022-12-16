#include "stdafx.h"
#include "SecurityOrb.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "ScreenShake.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/Flash.hpp"
#include "Abe.hpp"
#include "PossessionFlicker.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "AirExplosion.hpp"
#include "ZapLine.hpp"
#include "../relive_lib/Function.hpp"
#include "ZapSpark.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "FixedPoint.hpp"

static const TintEntry sSecurityOrbTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
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
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};


SecurityOrb::SecurityOrb(relive::Path_SecurityOrb* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    SetType(ReliveTypes::eSecurityOrb);

    SetCanExplode(true);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Orb));
    Animation_Init(GetAnimRes(AnimId::Security_Orb));

    SetTint(sSecurityOrbTints, gMap.mCurrentLevel);

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mTlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
    }

    SetDoPurpleLightEffect(true);
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
    SetDead(true);
}

bool SecurityOrb::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return false;
    }

    mHealth = FP_FromInteger(0);

    if (pFrom->Type() == ReliveTypes::eMineCar || pFrom->Type() == ReliveTypes::eAbilityRing || pFrom->Type() == ReliveTypes::eShrykull)
    {
        relive_new AirExplosion(
            mXPos,
            mYPos - (GetSpriteScale() * FP_FromInteger(5)),
            GetSpriteScale(),
            0);

        relive_new Gibs(
            GibType::Metal_5,
            mXPos,
            mYPos,
            FP_FromInteger(0),
            FP_FromInteger(0),
            GetSpriteScale(),
            0);
    }

    SetDead(true);

    return true;
}

void SecurityOrb::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case States::eIdle_0:
            if (GetAnimation().GetCurrentFrame() == 2 || GetAnimation().GetCurrentFrame() == 6 || GetAnimation().GetCurrentFrame() == 10)
            {
                if (mSoundChannelsMask)
                {
                    SND_Stop_Channels_Mask(mSoundChannelsMask);
                }

                if (GetSpriteScale() == FP_FromDouble(0.5))
                {
                    mSoundChannelsMask = SFX_Play_Pitch(relive::SoundEffects::SecurityOrb, 35, 720, GetSpriteScale());
                }
                else
                {
                    mSoundChannelsMask = SFX_Play_Pitch(relive::SoundEffects::SecurityOrb, 55, 700, GetSpriteScale());
                }
            }

            if (EventGet(kEventAbeOhm))
            {
                if (!sActiveHero->mRingPulseTimer || !sActiveHero->mHaveShrykull || sActiveHero->GetSpriteScale() != FP_FromInteger(1))
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
                    mYPos - (FP_FromInteger(8) * GetSpriteScale()),
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

                auto pSpark = relive_new ZapSpark(mXPos, mYPos - (FP_FromInteger(8) * GetSpriteScale()), GetSpriteScale());
                if (pSpark)
                {
                    pSpark->mRGB.SetRGB(255, 65, 65);
                }

                auto pSpark2 = relive_new ZapSpark(mXPos, mYPos - (FP_FromInteger(8) * GetSpriteScale()), GetSpriteScale());
                if (pSpark2)
                {
                    pSpark2->mRGB.SetRGB(255, 65, 65);
                }

                for (s32 i = 0; i < 9; i++)
                {
                    auto pSpark3 = relive_new ZapSpark(xpos, ypos, GetSpriteScale());
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
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 0, 0, TPageAbr::eBlend_3, 1);
            }

            if (static_cast<s32>(sGnFrame) == mTimer - 4)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 0, 0, TPageAbr::eBlend_1, 1);
            }

            const s32 timerFrame = mTimer - sGnFrame;
            if (timerFrame == 4)
            {
                SfxPlayMono(relive::SoundEffects::Zap1, 0, GetSpriteScale());
            }
            else if (timerFrame == 1)
            {
                SfxPlayMono(relive::SoundEffects::Zap2, 0, GetSpriteScale());
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = States::eIdle_0;
            }
            break;
    }
}
