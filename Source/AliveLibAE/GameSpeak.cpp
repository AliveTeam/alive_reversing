#include "stdafx.h"
#include "GameSpeak.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameType.hpp"

// NOTE: AO only had 11 entries
const static s32 sCodeBase[12] = {
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
    0
};

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
        return code2 + code1 * sCodeBase[Code_Length(code2) + 1];
    }
    else
    {
        return code1;
    }
}

GameSpeakEvents Code_LookUp(u32 code, u16 idx, u16 code_len)
{
    if (!code && GetGameType() == GameType::eAe)
    {
        return GameSpeakEvents::eUnknown_0;
    }

    u16 code_len_to_use = code_len;
    if (code_len == 0)
    {
        code_len_to_use = Code_Length(code);
    }
    return static_cast<GameSpeakEvents>(code / sCodeBase[code_len_to_use - idx] % 10);
}

GameSpeak* gEventSystem = nullptr;

GameSpeak::GameSpeak()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true); // Dont destroy on loading save
    mEventBuffer[0] = -1;
    SetType(ReliveTypes::eGameSpeak);
    mLastEventIndex = 0;
}

void GameSpeak::VScreenChanged()
{
    // Empty
}

GameSpeak::~GameSpeak()
{
    gEventSystem = nullptr;
}

void GameSpeak::PushEvent(GameSpeakEvents event)
{
    PushEvent_Impl(event);
    mLastEventFrame = MakeTimer(60);
}

void GameSpeak::PushEvent_Impl(GameSpeakEvents event)
{
    mLastEventIndex++;

    // Wrap around
    if (mLastEventIndex >= ALIVE_COUNTOF(mEventBuffer))
    {
        mLastEventIndex = 0;
    }

    // TODO: This isn't ever used ??
    mEventBuffer[mLastEventIndex] = static_cast<s8>(event);
    mLastEvent = event;
}

GameSpeakMatch GameSpeak::MatchBuffer(u8* pBuffer, s16 bufferLen, s16 bufferStartIdx)
{
    if (bufferStartIdx == -1)
    {
        bufferStartIdx = static_cast<s16>(mLastEventIndex - bufferLen);
        if (bufferStartIdx < 0)
        {
            bufferStartIdx += ALIVE_COUNTOF(mEventBuffer);
        }
    }

    s16 pBufferIdx = 0;
    while (1)
    {
        if (mEventBuffer[bufferStartIdx] == static_cast<s8>(GameSpeakEvents::eNone))
        {
            // Hit the end of events?
            if (bufferStartIdx == mLastEventIndex)
            {
                // Partial match as we got to the end but ran out of events
                return GameSpeakMatch::ePartMatch;
            }

            // To next event
            bufferStartIdx++;
            if (bufferStartIdx == ALIVE_COUNTOF(mEventBuffer))
            {
                bufferStartIdx = 0;
            }
        }

        if (pBuffer[pBufferIdx] != mEventBuffer[bufferStartIdx])
        {
            // Buffers didn't match
            return GameSpeakMatch::eNoMatch;
        }

        if (pBufferIdx == bufferLen - 1)
        {
            // Buffers have fully matched
            return GameSpeakMatch::eFullMatch;
        }

        // Hit the end of events?
        if (bufferStartIdx == mLastEventIndex)
        {
            // Partial match as we got to the end but ran out of events
            return GameSpeakMatch::ePartMatch;
        }

        // To next event
        bufferStartIdx++;
        if (bufferStartIdx == ALIVE_COUNTOF(mEventBuffer))
        {
            bufferStartIdx = 0;
        }

        // To next source event
        pBufferIdx++;
    }
}

void GameSpeak::VUpdate()
{
    // If the last thing pushed wasn't nothing and its been around for a while then set the last event to nothing
    if (mLastEvent != GameSpeakEvents::eNone && sGnFrame > mLastEventFrame)
    {
        PushEvent_Impl(GameSpeakEvents::eNone);
    }
}

s16 GameSpeak::FillBuffer(s32 code, u8* pBuffer)
{
    const s16 len = Code_Length(code);
    for (s16 idx = 0; idx < len; idx++)
    {
        pBuffer[idx] = static_cast<u8>(Code_LookUp(code, idx, len));
    }
    return len;
}

void GameSpeak::VRender(OrderingTable& /*ot*/)
{
    // Empty
}
