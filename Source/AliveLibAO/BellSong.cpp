#include "stdafx_ao.h"
#include "Function.hpp"
#include "BellSong.hpp"
#include "Abe.hpp"
#include "GameSpeak.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

namespace AO {

void BellSong::VUpdate()
{
    if (field_10_code_idx >= field_1C_code_len)
    {
        field_14_bDone = TRUE;
    }

    if (!field_14_bDone)
    {
        const GameSpeakEvents code_looked_up = Code_LookUp(field_18_code, static_cast<u16>(field_10_code_idx), field_1C_code_len);
        switch (field_16_type)
        {
            case BellsongTypes::eChimes:
            {
                switch (code_looked_up)
                {
                    case GameSpeakEvents::eWhistleHigh_1:
                        SFX_Play_Mono(SoundEffect::BellChime_LowPitch_52, 0);
                        break;
                    case GameSpeakEvents::eWhistleLow_2:
                        SFX_Play_Mono(SoundEffect::BellChime_MediumPitch_51, 0);
                        break;
                    default:
                        SFX_Play_Mono(SoundEffect::BellChime_HighPitch_50, 0);
                }
                mBaseGameObjectUpdateDelay = 15;
                break;
            }
            case BellsongTypes::eWhistle:
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

        field_10_code_idx++;

        if (field_10_code_idx >= field_1C_code_len)
        {
            mBaseGameObjectUpdateDelay = 30;
        }
    }
}

BellSong::BellSong(BellsongTypes type, u32 code)
    : BaseGameObject(TRUE, 0)
{
    field_16_type = type;
    field_18_code = code;
    field_1C_code_len = Code_Length(code);
    field_10_code_idx = 0;
    field_14_bDone = FALSE;
    mBaseGameObjectUpdateDelay = 15;
}

} // namespace AO
