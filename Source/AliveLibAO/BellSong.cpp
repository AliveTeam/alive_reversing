#include "stdafx_ao.h"
#include "Function.hpp"
#include "BellSong.hpp"
#include "Abe.hpp"
#include "GameSpeak.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

namespace AO {

void BellSong::VUpdate_476130()
{
    if (field_10_code_idx >= field_1C_code_len)
    {
        field_14_bDone = TRUE;
    }

    if (!field_14_bDone)
    {
        const GameSpeakEvents code_looked_up = Code_LookUp_476050(field_18_code, static_cast<unsigned short>(field_10_code_idx), field_1C_code_len);
        switch(field_16_type)
        {
            case BellsongTypes::eChimes:
            {
                switch (code_looked_up)
                {
                case GameSpeakEvents::eWhistle1_1:
                    SFX_Play_43AD70(SoundEffect::BellChime_LowPitch_52, 0, 0);
                    break;
                case GameSpeakEvents::eWhistle2_2:
                    SFX_Play_43AD70(SoundEffect::BellChime_MediumPitch_51, 0, 0);
                    break;
                default:
                    SFX_Play_43AD70(SoundEffect::BellChime_HighPitch_50, 0, 0);
                }
                field_8_update_delay = 15;
                break;
            }
            case BellsongTypes::eWhistle:
            {
                switch (code_looked_up)
                {
                    case GameSpeakEvents::eWhistle1_1:
                        Mudokon_SFX_42A4D0(MudSounds::eWhistle1_1, 0, 0, 0);
                        break;
                    case GameSpeakEvents::eWhistle2_2:
                        Mudokon_SFX_42A4D0(MudSounds::eWhistle2_2, 0, 0, 0);
                        break;
                    default:
                        Mudokon_SFX_42A4D0(MudSounds::eFart_7, 0, 0, 0);
                }
                field_8_update_delay = 30;
                break;
            }
        }

        field_10_code_idx++;

        if (field_10_code_idx >= field_1C_code_len)
        {
            field_8_update_delay = 30;
        }
    }
}

void BellSong::VUpdate()
{
    VUpdate_476130();
}

BellSong* BellSong::ctor_4760B0(BellsongTypes type, unsigned int code)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BCD18);

    field_16_type = type;
    field_18_code = code;
    field_1C_code_len = Code_Length_475FD0(code);
    field_10_code_idx = 0;
    field_14_bDone = FALSE;
    field_8_update_delay = 15;
    return this;
}

BaseGameObject* BellSong::VDestructor(signed int flags)
{
    dtor_487DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

}

