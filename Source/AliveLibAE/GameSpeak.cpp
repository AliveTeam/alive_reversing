#include "stdafx.h"
#include "GameSpeak.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

void GameSpeak_ForceLink() { }

const int code_base_560F0C[12] =
{
    0,
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
    0,
};

__int16 CC Code_Length_4C9DB0(unsigned int code)
{
    __int16 i = 0;
    for (i = 0; code; ++i)
    {
        code /= 10u;
    }
    return i;
}

int CC Code_Convert_4C9DF0(unsigned __int16 code1, unsigned __int16 code2)
{
    if (code2)
    {
        return code2 + code1 * code_base_560F0C[Code_Length_4C9DB0(code2) + 1];
    }
    else
    {
        return code1;
    }
}


GameSpeakEvents CC Code_LookUp_4C9E40(unsigned int code, unsigned __int16 idx, unsigned __int16 code_len)
{
    if (!code)
    {
        return GameSpeakEvents::eUnknown_0;
    }

    unsigned __int16 code_len_to_use = code_len;
    if (code_len == 0)
    {
        code_len_to_use = Code_Length_4C9DB0(code);
    }

    return static_cast<GameSpeakEvents>(code / code_base_560F0C[code_len_to_use - idx] % 10);
}

ALIVE_VAR(1, 0x5BC11C, GameSpeak*, pEventSystem_5BC11C, nullptr);

GameSpeak* GameSpeak::ctor_421820()
{
    BaseGameObject::BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x544B20);

    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9); // Dont destroy on loading save
    field_2C_event_buffer[0] = -1;
    field_20_last_event = GameSpeakEvents::eNone_m1;
    field_4_typeId = Types::eGameSpeak_38;
    field_28_last_event_index = 0;
    return this;
}

GameSpeakMatch GameSpeak::MatchBuffer_4219E0(BYTE* pBuffer, __int16 max_idx, signed __int16 src_idx)
{
    if (src_idx == -1)
    {
        src_idx = static_cast<short>(field_28_last_event_index - max_idx);
        if (src_idx < 0)
        {
            src_idx += ALIVE_COUNTOF(field_2C_event_buffer);
        }
    }

    __int16 dst_idx = 0;
    while (1)
    {
        if (field_2C_event_buffer[src_idx] == static_cast<char>(GameSpeakEvents::eNone_m1))
        {
            bool bContinue = true;
            while (src_idx != field_28_last_event_index)
            {
                src_idx++;
                if (src_idx == ALIVE_COUNTOF(field_2C_event_buffer))
                {
                    src_idx = 0;
                }

                if (field_2C_event_buffer[src_idx] != -1)
                {
                    bContinue = false;
                   break;
                }
            }

            if (bContinue)
            {
                return GameSpeakMatch::ePartMatch_2;
            }
        }

        if (pBuffer[dst_idx] != field_2C_event_buffer[src_idx])
        {
            return GameSpeakMatch::eNoMatch_0;
        }

        if (dst_idx == max_idx - 1)
        {
            return GameSpeakMatch::eFullMatch_1;
        }

        if (src_idx == field_28_last_event_index)
        {
            return GameSpeakMatch::ePartMatch_2;
        }

        src_idx++;
        if (src_idx == ALIVE_COUNTOF(field_2C_event_buffer))
        {
            src_idx = 0;
        }
        dst_idx++;
    }
}

int CC GameSpeak::FillBuffer_421970(int code, BYTE* pBufffer)
{
    const __int16 len = Code_Length_4C9DB0(code);
    for (short idx = 0; idx < len; idx++)
    {
        pBufffer[idx] = static_cast<BYTE>(Code_LookUp_4C9E40(code, idx, len));
    }
    return len;
}

void GameSpeak::dtor_4218A0()
{
    pEventSystem_5BC11C = nullptr;
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* GameSpeak::vdtor_421870(signed int flags)
{
    dtor_4218A0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

BaseGameObject* GameSpeak::VDestructor(signed int flags)
{
    return vdtor_421870(flags);
}

void GameSpeak::VUpdate()
{
    Update_421920();
}

void GameSpeak::VRender(PrimHeader** /*ppOt*/)
{
    // Null @ 0x4DBF80
}


void GameSpeak::VScreenChanged()
{
    // Null @ 0x421AB0
}

void GameSpeak::Update_421920()
{
    if (field_20_last_event != GameSpeakEvents::eNone_m1 && sGnFrame_5C1B84 > field_24_last_event_frame)
    {
        PushEvent_Impl(GameSpeakEvents::eNone_m1);
    }
}

void GameSpeak::PushEvent_4218D0(GameSpeakEvents event)
{
    PushEvent_Impl(event);
    field_24_last_event_frame = sGnFrame_5C1B84 + 60;
}

void GameSpeak::PushEvent_Impl(GameSpeakEvents event)
{
    field_28_last_event_index++;

    // Wrap around
    if (field_28_last_event_index >= ALIVE_COUNTOF(field_2C_event_buffer))
    {
        field_28_last_event_index = 0;
    }

    // TODO: This isn't ever used ??
    field_2C_event_buffer[field_28_last_event_index] = static_cast<char>(event);
    field_20_last_event = event;
}
