#include "stdafx_ao.h"
#include "Function.hpp"
#include "Bells.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ScreenWave.hpp"
#include "ZapSpark.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Sfx.hpp"

namespace AO {

void Bells::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BigChime));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MediumChime));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SmallChime));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BigChime_Moving));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::MediumChime_Moving));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::SmallChime_Moving));
}

Bells::Bells(BellSize bellType, FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetCanExplode(false);

    SetType(ReliveTypes::eBells);

    LoadAnimations();

    switch (bellType)
    {
        case BellSize::eBig:
            mBellPitch = BellPitch::eLowPitch;
            Animation_Init(GetAnimRes(AnimId::BigChime));
            break;

        case BellSize::eMedium:
            mBellPitch = BellPitch::eMediumPitch;
            Animation_Init(GetAnimRes(AnimId::MediumChime));
            break;
        case BellSize::eSmall:
            mBellPitch = BellPitch::eHighPitch;
            Animation_Init(GetAnimRes(AnimId::SmallChime));
            break;
    }

    SetApplyShadowZoneColour(false);
    SetSpriteScale(scale);

    mXPos = xpos;
    mYPos = ypos;

    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);

    mSmashing = false;

    mDoScreenWave = false;
    mSoundCooldownTimer = 0;
    mSoundPitchFactor = 0;
}

void Bells::VUpdate()
{
    if (mSoundPitchFactor > 0 && static_cast<s32>(sGnFrame) >= mSoundCooldownTimer)
    {
        mSoundCooldownTimer = sGnFrame + 4;
        mSoundPitchFactor--;

        if (mBellPitch == BellPitch::eLowPitch)
        {
            SfxPlayMono(relive::SoundEffects::BellChime_LowPitch, 0);
        }
        else if (mBellPitch == BellPitch::eMediumPitch)
        {
            SFX_Play_Pitch(relive::SoundEffects::BellChime_MediumPitch, 45 * (mSoundPitchFactor + 1), 128 - (mSoundPitchFactor << 7));
        }
        else if (mBellPitch == BellPitch::eHighPitch)
        {
            SFX_Play_Pitch(relive::SoundEffects::BellChime_HighPitch, 30 * (mSoundPitchFactor + 1), (2 - mSoundPitchFactor) << 7);
        }
    }

    if (mSmashing)
    {
        if (mDoScreenWave)
        {
            mDoScreenWave = false;

            FP xOff = {};
            FP yOff = {};
            if (mBellPitch == BellPitch::eLowPitch)
            {
                xOff = FP_FromInteger(-35);
                yOff = FP_FromInteger(36);
                const FP wave_ypos = mYPos + FP_FromInteger(36);
                const FP wave_xpos = mXPos - FP_FromInteger(35);
                relive_new ScreenWave(wave_xpos, wave_ypos, Layer::eLayer_FG1_37, FP_FromInteger(18), FP_FromInteger(12), 0);
            }
            else if (mBellPitch == BellPitch::eHighPitch)
            {
                xOff = FP_FromInteger(37);
                yOff = FP_FromInteger(32);
                const FP wave_ypos = mYPos + FP_FromInteger(32);
                const FP wave_xpos = mXPos + FP_FromInteger(37);
                relive_new ScreenWave(wave_xpos, wave_ypos, Layer::eLayer_FG1_37, FP_FromInteger(12), FP_FromInteger(12), 0);
            }
            else if (mBellPitch == BellPitch::eMediumPitch)
            {
                xOff = FP_FromInteger(-4);
                yOff = FP_FromInteger(24);
                const FP wave_ypos = mYPos + FP_FromInteger(24);
                const FP wave_xpos = mXPos - FP_FromInteger(4);
                relive_new ScreenWave(wave_xpos, wave_ypos, Layer::eLayer_FG1_37, FP_FromInteger(14), FP_FromInteger(12), 0);
            }

            for (s32 i = 0; i < 4; i++)
            {
                const FP sparkx = mXPos + FP_FromInteger(Math_RandomRange(-2, 2)) + xOff;
                const FP sparky = mYPos + FP_FromInteger(Math_RandomRange(-2, 2)) + yOff;
                relive_new ZapSpark(sparkx, sparky, GetSpriteScale());
            }
        }

        PlaySounds();
    }
}

void Bells::PlaySounds()
{
    if (GetAnimation().GetIsLastFrame())
    {
        if (mBellPitch == BellPitch::eLowPitch)
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BigChime));
        }
        else if (mBellPitch == BellPitch::eMediumPitch)
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MediumChime));
        }
        else if (mBellPitch == BellPitch::eHighPitch)
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::SmallChime));
        }

        mSmashing = false;
    }
}

bool Bells::CanSmash()
{
    return !mSmashing;
}

void Bells::Ring()
{
    if (CanSmash())
    {
        mSmashing = true;
        mDoScreenWave = true;
        mSoundCooldownTimer = 0;

        if (mBellPitch == BellPitch::eLowPitch)
        {
            mSoundPitchFactor = 1;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BigChime_Moving));
        }
        else if (mBellPitch == BellPitch::eMediumPitch)
        {
            mSoundPitchFactor = 2;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::MediumChime_Moving));
        }
        else if (mBellPitch == BellPitch::eHighPitch)
        {
            mSoundPitchFactor = 3;
            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::SmallChime_Moving));
        }
    }
}

} // namespace AO
