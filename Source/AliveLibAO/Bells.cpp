#include "stdafx_ao.h"
#include "Function.hpp"
#include "Bells.hpp"
#include "ResourceManager.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ScreenWave.hpp"
#include "ZapSpark.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Sfx.hpp"

namespace AO {

Bells::Bells(BellSize bellType, FP xpos, FP ypos, FP scale)
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);

    mBaseGameObjectTypeId = ReliveTypes::eBells;

    const AnimRecord rec = AO::AnimRec(AnimId::BigChime);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);

    switch (bellType)
    {
        case BellSize::eBig:
            mBellPitch = BellPitch::eLowPitch;
            Animation_Init(AnimId::BigChime, ppRes);
            break;

        case BellSize::eMedium:
            mBellPitch = BellPitch::eMediumPitch;
            Animation_Init(AnimId::MediumChime, ppRes);
            break;
        case BellSize::eSmall:
            mBellPitch = BellPitch::eHighPitch;
            Animation_Init(AnimId::SmallChime, ppRes);
            break;
    }

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mSpriteScale = scale;

    mXPos = xpos;
    mYPos = ypos;

    mAnim.mRenderLayer = Layer::eLayer_Foreground_36;

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
            SfxPlayMono(relive::SoundEffects::BellChime_LowPitch, 0, 0);
        }
        else if (mBellPitch == BellPitch::eMediumPitch)
        {
            SFX_Play_Pitch(relive::SoundEffects::BellChime_MediumPitch, 45 * (mSoundPitchFactor + 1), 128 - (mSoundPitchFactor << 7), 0);
        }
        else if (mBellPitch == BellPitch::eHighPitch)
        {
            SFX_Play_Pitch(relive::SoundEffects::BellChime_HighPitch, 30 * (mSoundPitchFactor + 1), (2 - mSoundPitchFactor) << 7, 0);
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
                relive_new ZapSpark(sparkx, sparky, mSpriteScale);
            }
        }

        PlaySounds();
    }
}

void Bells::PlaySounds()
{
    if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mBellPitch == BellPitch::eLowPitch)
        {
            mAnim.Set_Animation_Data(AnimId::BigChime, nullptr);
        }
        else if (mBellPitch == BellPitch::eMediumPitch)
        {
            mAnim.Set_Animation_Data(AnimId::MediumChime, nullptr);
        }
        else if (mBellPitch == BellPitch::eHighPitch)
        {
            mAnim.Set_Animation_Data(AnimId::SmallChime, nullptr);
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
            mAnim.Set_Animation_Data(AnimId::BigChime_Moving, nullptr);
        }
        else if (mBellPitch == BellPitch::eMediumPitch)
        {
            mSoundPitchFactor = 2;
            mAnim.Set_Animation_Data(AnimId::MediumChime_Moving, nullptr);
        }
        else if (mBellPitch == BellPitch::eHighPitch)
        {
            mSoundPitchFactor = 3;
            mAnim.Set_Animation_Data(AnimId::SmallChime_Moving, nullptr);
        }
    }
}

} // namespace AO
