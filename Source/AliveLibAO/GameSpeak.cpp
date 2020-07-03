#include "stdafx_ao.h"
#include "GameSpeak.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

START_NS_AO

const int dword_4CFFCC[11] =
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
    1000000000
};


__int16 CC Code_Length_475FD0(unsigned int code)
{
    __int16 i = 0;
    for (i = 0; code; ++i)
    {
        code /= 10u;
    }
    return i;
}

int CC Code_Convert_476000(unsigned __int16 code1, unsigned __int16 code2)
{
    if (code2)
    {
        return code2 + code1 * dword_4CFFCC[(Code_Length_475FD0(code2)) + 1];
    }
    else
    {
        return code1;
    }
}

__int16 CC Code_LookUp_476050(unsigned int code, unsigned __int16 idx, unsigned __int16 code_len)
{
    unsigned __int16 code_len_to_use = code_len;
    if (code_len == 0)
    {
        code_len_to_use = Code_Length_475FD0(code);
    }
    return code / dword_4CFFCC[code_len_to_use - idx] % 10;
}

ALIVE_VAR(1, 0x4FF954, GameSpeak*, pEventSystem_4FF954, nullptr);

GameSpeak* GameSpeak::ctor_40F990()
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA890);

    field_6_flags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    field_1C_event_buffer[0] = -1;
    field_10_last_event = GameSpeakEvents::eNone_m1;
    field_4_typeId = Types::eGameSpeak_24;
    field_18_last_event_index = 0;
    return this;
}

GameSpeak* GameSpeak::Vdtor_40FB50(signed int flags)
{
    dtor_40F9C0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void GameSpeak::VScreenChanged()
{
    // Empty
}

BaseGameObject* GameSpeak::VDestructor(signed int flags)
{
    return Vdtor_40FB50(flags);
}

BaseGameObject* GameSpeak::dtor_40F9C0()
{
    SetVTable(this, 0x4BA890);
    pEventSystem_4FF954 = nullptr;
    return dtor_487DF0();
}

void GameSpeak::VPushEvent_40F9E0(GameSpeakEvents event)
{
    PushEvent_Impl(event);
    field_14_last_event_frame = gnFrameCount_507670 + 60;
}


void GameSpeak::PushEvent_Impl(GameSpeakEvents event)
{
    field_18_last_event_index++;

    // Wrap around
    if (field_18_last_event_index >= ALIVE_COUNTOF(field_1C_event_buffer))
    {
        field_18_last_event_index = 0;
    }

    field_1C_event_buffer[field_18_last_event_index] = static_cast<char>(event);
    field_10_last_event = event;
}

void GameSpeak::VUpdate()
{
    VUpdate_40FA20();
}

void GameSpeak::VUpdate_40FA20()
{
    // If the last thing pushed wasn't nothing and its been around for a while then set the last event to nothing
    if (field_10_last_event != GameSpeakEvents::eNone_m1)
    {
        if (gnFrameCount_507670 > field_14_last_event_frame)
        {
            PushEvent_Impl(GameSpeakEvents::eNone_m1);
        }
    }
}

int CC GameSpeak::sub_40FA60(int code, BYTE* pBuffer)
{
    const __int16 len = Code_Length_475FD0(code);
    for (short idx = 0; idx < len; ++idx)
    {
        pBuffer[idx] = static_cast<BYTE>(Code_LookUp_476050(code, idx, len));
    }
    return len;
}

GameSpeakMatch GameSpeak::MatchBuffer_40FAA0(BYTE* pBuffer, __int16 bufferLen, signed __int16 bufferStartIdx)
{
    if (bufferStartIdx == -1)
    {
        bufferStartIdx = static_cast<short>(field_18_last_event_index - bufferLen);
        if (bufferStartIdx < 0)
        {
            bufferStartIdx += ALIVE_COUNTOF(field_1C_event_buffer);
        }
    }

    __int16 pBufferIdx = 0;
    while (1)
    {
        while (field_1C_event_buffer[bufferStartIdx] == static_cast<char>(GameSpeakEvents::eNone_m1))
        {
            // Hit the end of events?
            if (bufferStartIdx == field_18_last_event_index)
            {
                // Partial match as we got to the end but ran out of events
                return GameSpeakMatch::ePartMatch_2;
            }

            // To next event
            bufferStartIdx++;
            if (bufferStartIdx == ALIVE_COUNTOF(field_1C_event_buffer))
            {
                bufferStartIdx = 0;
            }
        }

        if (pBuffer[pBufferIdx] != field_1C_event_buffer[bufferStartIdx])
        {
            // Buffers didn't match
            return GameSpeakMatch::eNoMatch_0;
        }

        if (pBufferIdx == bufferLen - 1)
        {
            // Buffers have fully matched
            return GameSpeakMatch::eFullMatch_1;
        }

        // Hit the end of events?
        if (bufferStartIdx == field_18_last_event_index)
        {
            // Partial match as we got to the end but ran out of events
            return GameSpeakMatch::ePartMatch_2;
        }

        // To next event
        bufferStartIdx++;
        if (bufferStartIdx == ALIVE_COUNTOF(field_1C_event_buffer))
        {
            bufferStartIdx = 0;
        }

        // To next source event
        pBufferIdx++;
    }
}

END_NS_AO
