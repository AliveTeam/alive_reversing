#include "stdafx_ao.h"
#include "GameSpeak.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

const s32 dword_4CFFCC[11] = {
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
    1000000000};


s16 Code_Length(u32 code)
{
    s16 i = 0;
    for (i = 0; code; ++i)
    {
        code /= 10u;
    }
    return i;
}

s32 Code_Convert(u16 code1, u16 code2)
{
    if (code2)
    {
        return code2 + code1 * dword_4CFFCC[(Code_Length(code2)) + 1];
    }
    else
    {
        return code1;
    }
}

GameSpeakEvents Code_LookUp(u32 code, u16 idx, u16 code_len)
{
    u16 code_len_to_use = code_len;
    if (code_len == 0)
    {
        code_len_to_use = Code_Length(code);
    }
    return static_cast<GameSpeakEvents>(code / dword_4CFFCC[code_len_to_use - idx] % 10);
}

GameSpeak* gEventSystem = nullptr;

GameSpeak::GameSpeak()
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectFlags.Set(BaseGameObject::eSurviveDeathReset_Bit9);
    field_1C_event_buffer[0] = -1;
    field_10_last_event = GameSpeakEvents::eNone_m1;
    SetType(ReliveTypes::eGameSpeak);
    field_18_last_event_index = 0;
}

void GameSpeak::VScreenChanged()
{
    // Empty
}

GameSpeak::~GameSpeak()
{
    gEventSystem = nullptr;
}

void GameSpeak::VPushEvent(GameSpeakEvents event)
{
    PushEvent_Impl(event);
    field_14_last_event_frame = sGnFrame + 60;
}


void GameSpeak::PushEvent_Impl(GameSpeakEvents event)
{
    field_18_last_event_index++;

    // Wrap around
    if (field_18_last_event_index >= ALIVE_COUNTOF(field_1C_event_buffer))
    {
        field_18_last_event_index = 0;
    }

    field_1C_event_buffer[field_18_last_event_index] = static_cast<s8>(event);
    field_10_last_event = event;
}

void GameSpeak::VUpdate()
{
    // If the last thing pushed wasn't nothing and its been around for a while then set the last event to nothing
    if (field_10_last_event != GameSpeakEvents::eNone_m1)
    {
        if (sGnFrame > field_14_last_event_frame)
        {
            PushEvent_Impl(GameSpeakEvents::eNone_m1);
        }
    }
}

s16 GameSpeak::sub_40FA60(s32 code, u8* pBuffer)
{
    const s16 len = Code_Length(code);
    for (s16 idx = 0; idx < len; ++idx)
    {
        pBuffer[idx] = static_cast<u8>(Code_LookUp(code, idx, len));
    }
    return len;
}

GameSpeakMatch GameSpeak::MatchBuffer(u8* pBuffer, s16 bufferLen, s16 bufferStartIdx)
{
    if (bufferStartIdx == -1)
    {
        bufferStartIdx = static_cast<s16>(field_18_last_event_index - bufferLen);
        if (bufferStartIdx < 0)
        {
            bufferStartIdx += ALIVE_COUNTOF(field_1C_event_buffer);
        }
    }

    s16 pBufferIdx = 0;
    while (1)
    {
        while (field_1C_event_buffer[bufferStartIdx] == static_cast<s8>(GameSpeakEvents::eNone_m1))
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

} // namespace AO
