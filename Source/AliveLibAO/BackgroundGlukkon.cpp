#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "BackgroundGlukkon.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/GameObjects/AirExplosion.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Game.hpp"
#include "../relive_lib/FatalError.hpp"
#include "Path.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

BackgroundGlukkon::~BackgroundGlukkon()
{
    if (mHealth <= FP_FromInteger(0))
    {
        Path::TLV_Delete(mTlvId);
    }
    else
    {
        Path::TLV_Reset(mTlvId);
    }
}

void BackgroundGlukkon::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Background_Glukkon_Dying));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Background_Glukkon_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Background_Glukkon_KillHim1));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Background_Glukkon_KillHim2));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Background_Glukkon_Laugh));
}

BackgroundGlukkon::BackgroundGlukkon(relive::Path_BackgroundGlukkon* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eBackgroundGlukkon);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::Background_Glukkon_Idle));

    mTlvId = tlvId;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    SetSpriteScale(FP_FromInteger(pTlv->mScalePercent) / FP_FromInteger(100));

    PalId pal = PalId::Default;
    if (pTlv->mPalId == AOResourceID::kGlukredAOResID)
    {
        pal = PalId::RedBackgroundGlukkon;
    }
    else if (pTlv->mPalId == AOResourceID::kGlukgrenAOResID)
    {
        pal = PalId::GreenBackgroundGlukkon;
    }
    else if (pTlv->mPalId == AOResourceID::kGlukblueAOResID)
    {
        pal = PalId::BlueBackgroundGlukkon;
    }
    else if (pTlv->mPalId == AOResourceID::kGlukaquaAOResID)
    {
        pal = PalId::AquaBackgroundGlukkon;
    }
    else
    {
        ALIVE_FATAL("Invalid background glukkon pal");
    }

    mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(pal));
    GetAnimation().LoadPal(GetPalRes(pal));

    mState = BackgroundGlukkon::State::eToSetSpeakPauseTimer;
}

void BackgroundGlukkon::VScreenChanged()
{
    SetDead(true);
}

bool BackgroundGlukkon::VTakeDamage(BaseGameObject* pFrom)
{
    if (GetDead())
    {
        return false;
    }

    if (pFrom->Type() == ReliveTypes::eShrykull)
    {
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_Dying));
        const auto rndVol = Math_RandomRange(110, 127);
        const auto rndPitch = (75 * (Math_NextRandom() % 4)) + 200;

        if (Math_NextRandom() >= 128u)
        {
            SFX_Play_Pitch(relive::SoundEffects::Empty, rndVol, rndPitch);
        }
        else
        {
            SFX_Play_Pitch(relive::SoundEffects::Empty, rndVol, rndPitch);
        }

        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_Dying));
        mState = BackgroundGlukkon::State::eKilledByShrykull;
    }
    else if (pFrom->Type() == ReliveTypes::eElectrocute && mHealth > FP_FromInteger(0))
    {
        mHealth = FP_FromInteger(0);

        relive_new AirExplosion(
            mXPos,
            mYPos - (GetSpriteScale() * FP_FromInteger(40)),
            GetSpriteScale(),
            false);

        SetDead(true);
    }
    return true;
}

void BackgroundGlukkon::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case BackgroundGlukkon::State::eToSetSpeakPauseTimer:
            mState = BackgroundGlukkon::State::eSetSpeakPauseTimer;
            MakeTimer(Math_RandomRange(20, 40)); // TODO: remove and make a new recording
            break;

        case BackgroundGlukkon::State::eSetSpeakPauseTimer:
            mState = BackgroundGlukkon::State::eRandomizedLaugh;
            mSpeakPauseTimer = MakeTimer(Math_RandomRange(12, 20));
            break;

        case BackgroundGlukkon::State::eRandomizedLaugh:
            if (static_cast<s32>(sGnFrame) > mSpeakPauseTimer)
            {
                const auto rndVol = Math_RandomRange(110, 127);
                const auto rndPitch = ((Math_NextRandom() % 4) * 128) + 200;

                switch (Math_NextRandom() % 5)
                {
                    case 0:
                        if (gAbe->mHealth <= FP_FromInteger(0))
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_Laugh));
                            SFX_Play_Pitch(relive::SoundEffects::GlukkonLaugh1, rndVol, rndPitch);
                        }
                        else
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_KillHim1));
                            SFX_Play_Pitch(relive::SoundEffects::GlukkonKillHim1, rndVol, rndPitch);
                        }
                        mState = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer;
                        break;

                    case 1:
                        if (gAbe->mHealth <= FP_FromInteger(0))
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_Laugh));
                            SFX_Play_Pitch(relive::SoundEffects::GlukkonLaugh1, rndVol, rndPitch);
                        }
                        else
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_KillHim2));
                            SFX_Play_Pitch(relive::SoundEffects::GlukkonKillHim2, rndVol, rndPitch);
                        }
                        mState = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer;
                        break;

                    case 2:
                        if (gAbe->mHealth <= FP_FromInteger(0))
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_Laugh));
                            SFX_Play_Pitch(relive::SoundEffects::GlukkonLaugh1, rndVol, rndPitch);
                        }
                        else
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_KillHim1));
                            SFX_Play_Pitch(relive::SoundEffects::Empty, rndVol, rndPitch);
                        }
                        mState = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer;
                        break;

                    case 3:
                        if (gAbe->mHealth > FP_FromInteger(0))
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_KillHim2));
                            SFX_Play_Pitch(relive::SoundEffects::Empty, rndVol, rndPitch);
                        }
                        mState = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer;
                        break;

                    case 4:
                        return;

                    default:
                        mState = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer;
                        break;
                }
            }
            break;

        case BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer:
            if (GetAnimation().GetIsLastFrame())
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_Idle));
                mState = BackgroundGlukkon::State::eSetSpeakPauseTimer;
            }
            break;

        default:
            return;
    }
}

} // namespace AO
