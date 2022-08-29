#include "stdafx_ao.h"
#include "Function.hpp"
#include "BellSong.hpp"
#include "Abe.hpp"
#include "GameSpeak.hpp"
#include "Sfx.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

void BellSong::VUpdate()
{
    if (mCodeIdx >= mCodeLength)
    {
        mDone = true;
    }

    if (!mDone)
    {
        const GameSpeakEvents code_looked_up = Code_LookUp(mCode, static_cast<u16>(mCodeIdx), mCodeLength);
        switch (mType)
        {
            case relive::Path_BellsongStone::BellsongTypes::eChimes:
            {
                switch (code_looked_up)
                {
                    case GameSpeakEvents::eWhistleHigh_1:
                        SfxPlayMono(SoundEffect::BellChime_LowPitch_52, 0);
                        break;
                    case GameSpeakEvents::eWhistleLow_2:
                        SfxPlayMono(SoundEffect::BellChime_MediumPitch_51, 0);
                        break;
                    default:
                        SfxPlayMono(SoundEffect::BellChime_HighPitch_50, 0);
                }
                mBaseGameObjectUpdateDelay = 15;
                break;
            }
            case relive::Path_BellsongStone::BellsongTypes::eWhistle:
            {
                switch (code_looked_up)
                {
                    case GameSpeakEvents::eWhistleHigh_1:
                        Mudokon_SFX(MudSounds::eWhistleHigh_1, 0, 0, 0);
                        break;
                    case GameSpeakEvents::eWhistleLow_2:
                        Mudokon_SFX(MudSounds::eWhistleLow_2, 0, 0, 0);
                        break;
                    default:
                        Mudokon_SFX(MudSounds::eFart_7, 0, 0, 0);
                }
                mBaseGameObjectUpdateDelay = 30;
                break;
            }
        }

        mCodeIdx++;

        if (mCodeIdx >= mCodeLength)
        {
            mBaseGameObjectUpdateDelay = 30;
        }
    }
}

BellSong::BellSong(relive::Path_BellsongStone::BellsongTypes type, u32 code)
    : BaseGameObject(TRUE, 0)
{
    mType = type;
    mCode = code;
    mCodeLength = Code_Length(code);
    mCodeIdx = 0;
    mDone = false;
    mBaseGameObjectUpdateDelay = 15;
}

} // namespace AO
