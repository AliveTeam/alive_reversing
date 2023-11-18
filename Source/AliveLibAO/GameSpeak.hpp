#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

namespace AO {

// NOTE: the order actually matters
enum class GameSpeakEvents : s16
{
    eSameAsLast = -2,
    eNone = -1,
    eUnknown_0 = 0,
    eAbe_WhistleHigh = 1,
    eAbe_WhistleLow = 2,
    eAbe_Fart = 3,
    eAbe_Laugh = 4,
    eSlig_BS = 5,
    eSlig_LookOut = 6,
    eSlig_BS2 = 7,
    eSlig_Laugh = 8,
    eAbe_Hello = 9,
    eAbe_FollowMe = 10,
    eAbe_Anger = 11,
    eAbe_Wait = 12,
    eSlig_Hi = 23,
    eSlig_HereBoy = 24,
    eSlig_GetEm = 25,
    eSlig_Freeze = 27,
};

enum class GameSpeakMatch : s16
{
    eNoMatch = 0,
    eFullMatch = 1,
    ePartMatch = 2,
};

class GameSpeak final : public ::BaseGameObject
{
public:
    GameSpeak();
    ~GameSpeak();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;

    void PushEvent(GameSpeakEvents event);

    GameSpeakMatch MatchBuffer(u8* pBuffer, s16 bufferLen, s16 bufferStartIdx);
    static s16 FillBuffer(s32 code, u8* pBuffer);

private:
    void PushEvent_Impl(GameSpeakEvents event);

public:
    GameSpeakEvents mLastEvent = GameSpeakEvents::eNone;
    u32 mLastEventFrame = 0;
    s32 mLastEventIndex = 0;
    s8 mEventBuffer[32] = {};
};

extern GameSpeak* gEventSystem;

s16 Code_Length(u32 code);
s32 Code_Convert(u16 code1, u16 code2);
GameSpeakEvents Code_LookUp(u32 code, u16 idx, u16 code_len);

} // namespace AO
