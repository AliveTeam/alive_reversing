#pragma once

#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

enum class GameSpeakEvents : s16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,

    eAbe_WhistleHigh_1 = 1,
    eAbe_WhistleLow_2 = 2,
    eAbe_Fart_3 = 3,
    eAbe_Laugh_4 = 4,
    eSlig_Bullshit1_5 = 5,
    Slig_LookOut_6 = 6,
    eSlig_Bullshit2_7 = 7,
    eSlig_Laugh_8 = 8,
    eAbe_Hello_9 = 9,
    eAbe_FollowMe_10 = 10,
    eAbe_Anger_11 = 11,
    eAbe_Wait_12 = 12,
    Slig_Hi_23 = 23,
    Slig_HereBoy_24 = 24,
    eSlig_GetHim_25 = 25,
    eSlig_Freeze_27 = 27,
};

enum class GameSpeakMatch : s16
{
    eNoMatch_0 = 0,
    eFullMatch_1 = 1,
    ePartMatch_2 = 2,
};

class GameSpeak final : public ::BaseGameObject
{
public:
    GameSpeak();
    ~GameSpeak();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    // A new virtual that is never overridden as there are no other known super classes
    virtual void VPushEvent(GameSpeakEvents event);

    void PushEvent_Impl(GameSpeakEvents event);
    static s16 sub_40FA60(s32 code, u8* pBuffer);
    GameSpeakMatch MatchBuffer(u8* pBuffer, s16 bufferLen, s16 bufferStartIdx);

    GameSpeakEvents mLastEvent = GameSpeakEvents::eNone_m1;
    u32 mLastEventFrame = 0;
    s32 mLastEventIndex = 0;
    s8 mEventBuffer[32] = {};
};

extern GameSpeak* gEventSystem;

s16 Code_Length(u32 code);
s32 Code_Convert(u16 code1, u16 code2);
GameSpeakEvents Code_LookUp(u32 code, u16 idx, u16 code_len);

} // namespace AO
