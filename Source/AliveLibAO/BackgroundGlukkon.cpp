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
        Path::TLV_Reset(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
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

    field_10C_tlvInfo = tlvId;

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

    field_110_state = BackgroundGlukkon::State::eToSetSpeakPauseTimer_0;
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
        field_110_state = BackgroundGlukkon::State::eKilledByShrykull_7;
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

    switch (field_110_state)
    {
        case BackgroundGlukkon::State::eToSetSpeakPauseTimer_0:
            field_110_state = BackgroundGlukkon::State::eSetSpeakPauseTimer_1;
            field_118_never_read = MakeTimer(Math_RandomRange(20, 40));
            break;

        case BackgroundGlukkon::State::eSetSpeakPauseTimer_1:
            field_110_state = BackgroundGlukkon::State::eRandomizedLaugh_2;
            field_114_speak_pause_timer = MakeTimer(Math_RandomRange(12, 20));
            break;

        case BackgroundGlukkon::State::eRandomizedLaugh_2:
            if (static_cast<s32>(sGnFrame) > field_114_speak_pause_timer)
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
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
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
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
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
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 3:
                        if (gAbe->mHealth > FP_FromInteger(0))
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_KillHim2));
                            SFX_Play_Pitch(relive::SoundEffects::Empty, rndVol, rndPitch);
                        }
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;

                    case 4:
                        return;

                    default:
                        field_110_state = BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3;
                        break;
                }
            }
            break;

        case BackgroundGlukkon::State::eAfterLaugh_SetSpeakPauseTimer_3:
            if (GetAnimation().GetIsLastFrame())
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Background_Glukkon_Idle));
                field_110_state = BackgroundGlukkon::State::eSetSpeakPauseTimer_1;
            }
            break;

        default:
            return;
    }
}

} // namespace AO
