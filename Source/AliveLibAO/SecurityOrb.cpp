#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "SecurityOrb.hpp"
#include "Midi.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "AirExplosion.hpp"
#include "Gibs.hpp"
#include "ZapLine.hpp"
#include "Sfx.hpp"
#include "ZapSpark.hpp"
#include "Abe.hpp"
#include "PossessionFlicker.hpp"
#include "ScreenShake.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

SecurityOrb::SecurityOrb(relive::Path_SecurityOrb* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetCanExplode(true);

    SetType(ReliveTypes::eSecurityOrb);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Security_Orb));
    Animation_Init(GetAnimRes(AnimId::Security_Orb));

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mTlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

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

    switch (pFrom->Type())
    {
        case ReliveTypes::eAbilityRing:
        case ReliveTypes::eShrykull:
        {
            relive_new AirExplosion(
                mXPos,
                mYPos - (GetSpriteScale() * FP_FromInteger(5)),
                GetSpriteScale());

            relive_new Gibs(
                GibType::Metal_5,
                mXPos,
                mYPos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                GetSpriteScale());

            mHealth = FP_FromInteger(0);
        }
        break;

        default:
            break;
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
                    mSoundChannelsMask = SFX_Play_Pitch(relive::SoundEffects::SecurityOrb, 35, 720);
                }
                else
                {
                    mSoundChannelsMask = SFX_Play_Pitch(relive::SoundEffects::SecurityOrb, 55, 700);
                }
            }

            if (EventGet(kEventAbeOhm))
            {
                mState = States::eDoZapEffects_1;
                mTimer = sGnFrame + 20;
            }
            break;

        case States::eDoZapEffects_1:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                const PSX_RECT abeRect = sActiveHero->VGetBoundingRect();

                const s32 width = abeRect.w + abeRect.x;
                const s32 height = abeRect.h + abeRect.y;

                relive_new ZapLine(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * GetSpriteScale()),
                    FP_FromInteger(width / 2),
                    FP_FromInteger(height / 2),
                    8,
                    ZapLineType::eThick_0,
                    Layer::eLayer_ZapLinesElumMuds_28);

                relive_new PossessionFlicker(sActiveHero, 8, 255, 100, 100);

                sActiveHero->VTakeDamage(this);
                mTimer = sGnFrame + 8;
                mState = States::eDoFlashAndSound_2;

                relive_new ScreenShake(1);

                auto pSpark1 = relive_new ZapSpark(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * GetSpriteScale()),
                    GetSpriteScale());
                if (pSpark1)
                {
                    pSpark1->mRGB.SetRGB(255, 65, 65);
                }

                auto pSpark2 = relive_new ZapSpark(
                    mXPos,
                    mYPos - (FP_FromInteger(8) * GetSpriteScale()),
                    GetSpriteScale());
                if (pSpark2)
                {
                    pSpark2->mRGB.SetRGB(255, 65, 65);
                }

                for (s32 i = 0; i < 9; i++)
                {
                    auto pSparks = relive_new ZapSpark(
                        FP_FromInteger(width / 2),
                        FP_FromInteger(height / 2),
                        GetSpriteScale());
                    if (pSparks)
                    {
                        pSparks->mRGB.SetRGB(255, 65, 65);
                    }
                }
            }
            break;

        case States::eDoFlashAndSound_2:
            if (static_cast<s32>(sGnFrame) == mTimer - 5 || static_cast<s32>(sGnFrame) == mTimer - 1)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 0);
            }

            if (static_cast<s32>(sGnFrame) == mTimer - 4)
            {
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 0, 0, TPageAbr::eBlend_1, 1);
            }

            if (mTimer - sGnFrame == 4)
            {
                SfxPlayMono(relive::SoundEffects::Zap1, 0);
            }
            else if (mTimer - sGnFrame == 1)
            {
                SfxPlayMono(relive::SoundEffects::Zap2, 0);
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = States::eIdle_0;
            }
            break;
    }
}

} // namespace AO
