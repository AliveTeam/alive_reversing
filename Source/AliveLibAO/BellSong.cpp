#include "stdafx_ao.h"
#include "Function.hpp"
#include "BellSong.hpp"
#include "Abe.hpp"
#include "GameSpeak.hpp"
#include "Sfx.hpp"
#include "stdlib.hpp"

START_NS_AO

void BellSong::VUpdate_476130()
{
    if (field_10_code_idx >= field_1C_code_len)
    {
        field_14_bDone = TRUE;
    }

    if (!field_14_bDone)
    {
        const __int16 code_looked_up = Code_LookUp_476050(field_18_code, static_cast<unsigned short>(field_10_code_idx), field_1C_code_len);
        if (field_16_type)
        {
            switch (code_looked_up)
            {
            case 1:
                SFX_Play_43AD70(SoundEffect::BellChime2_51, 0, 0);
                break;
            case 2:
                SFX_Play_43AD70(SoundEffect::BellChime1_50, 0, 0);
                break;
            default:
                SFX_Play_43AD70(SoundEffect::BellChime3_52, 0, 0);
            }
            field_8_update_delay = 15;
        }
        else
        {
            switch (code_looked_up)
            {
            case 1:
                Mudokon_GameSpeak_SFX_42A4D0(MudokonGameSpeakSfx::eFart_7, 0, 0, 0);
            case 2:
                Mudokon_GameSpeak_SFX_42A4D0(MudokonGameSpeakSfx::eWhistle2_2, 0, 0, 0);
            default:
                Mudokon_GameSpeak_SFX_42A4D0(MudokonGameSpeakSfx::eWhistle1_1, 0, 0, 0);
            }
            field_8_update_delay = 30;
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

BellSong* BellSong::ctor_4760B0(__int16 type, unsigned int code)
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

END_NS_AO

