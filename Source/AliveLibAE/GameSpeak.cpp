#include "stdafx.h"
#include "GameSpeak.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Function.hpp"

const s32 code_base_560F0C[12] = {
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
        return code2 + code1 * code_base_560F0C[Code_Length(code2) + 1];
    }
    else
    {
        return code1;
    }
}


GameSpeakEvents Code_LookUp(u32 code, u16 idx, u16 code_len)
{
    if (!code)
    {
        return GameSpeakEvents::eUnknown_0;
    }

    u16 code_len_to_use = code_len;
    if (code_len == 0)
    {
        code_len_to_use = Code_Length(code);
    }

    return static_cast<GameSpeakEvents>(code / code_base_560F0C[code_len_to_use - idx] % 10);
}

GameSpeak* gEventSystem = nullptr;

GameSpeak::GameSpeak()
    : BaseGameObject(true, 0)
{
    SetSurviveDeathReset(true); // Dont destroy on loading save
    mEventBuffer[0] = -1;
    mLastEvent = GameSpeakEvents::eNone_m1;
    SetType(ReliveTypes::eGameSpeak);
    mLastEventIndex = 0;
}

GameSpeakMatch GameSpeak::MatchBuffer(u8* pBuffer, s16 max_idx, s16 src_idx)
{
    if (src_idx == -1)
    {
        src_idx = static_cast<s16>(mLastEventIndex - max_idx);
        if (src_idx < 0)
        {
            src_idx += ALIVE_COUNTOF(mEventBuffer);
        }
    }

    s16 dst_idx = 0;
    while (1)
    {
        if (mEventBuffer[src_idx] == static_cast<s8>(GameSpeakEvents::eNone_m1))
        {
            bool bContinue = true;
            while (src_idx != mLastEventIndex)
            {
                src_idx++;
                if (src_idx == ALIVE_COUNTOF(mEventBuffer))
                {
                    src_idx = 0;
                }

                if (mEventBuffer[src_idx] != -1)
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

        if (pBuffer[dst_idx] != mEventBuffer[src_idx])
        {
            return GameSpeakMatch::eNoMatch_0;
        }

        if (dst_idx == max_idx - 1)
        {
            return GameSpeakMatch::eFullMatch_1;
        }

        if (src_idx == mLastEventIndex)
        {
            return GameSpeakMatch::ePartMatch_2;
        }

        src_idx++;
        if (src_idx == ALIVE_COUNTOF(mEventBuffer))
        {
            src_idx = 0;
        }
        dst_idx++;
    }
}

s32 GameSpeak::FillBuffer(s32 code, u8* pBufffer)
{
    const s16 len = Code_Length(code);
    for (s16 idx = 0; idx < len; idx++)
    {
        pBufffer[idx] = static_cast<u8>(Code_LookUp(code, idx, len));
    }
    return len;
}

GameSpeak::~GameSpeak()
{
    gEventSystem = nullptr;
}

void GameSpeak::VRender(OrderingTable& /*ot*/)
{
    // Null @ 0x4DBF80
}


void GameSpeak::VScreenChanged()
{
    // Null @ 0x421AB0
}

void GameSpeak::VUpdate()
{
    if (mLastEvent != GameSpeakEvents::eNone_m1 && sGnFrame > mLastEventFrame)
    {
        PushEvent_Impl(GameSpeakEvents::eNone_m1);
    }
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
