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
                        SfxPlayMono(relive::SoundEffects::BellChime_LowPitch, 0);
                        break;
                    case GameSpeakEvents::eWhistleLow_2:
                        SfxPlayMono(relive::SoundEffects::BellChime_MediumPitch, 0);
                        break;
                    default:
                        SfxPlayMono(relive::SoundEffects::BellChime_HighPitch, 0);
                }
                SetUpdateDelay(15);
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
                SetUpdateDelay(30);
                break;
            }
        }

        mCodeIdx++;

        if (mCodeIdx >= mCodeLength)
        {
            SetUpdateDelay(30);
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
    SetUpdateDelay(15);
}

} // namespace AO
