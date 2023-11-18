#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

// NOTE: the order actually matters
enum class GameSpeakEvents : s16
{
    eSameAsLast = -2,
    eNone = -1,
    eUnknown_0 = 0, // used
    eUnknown_1 = 1, // used
    eAbe_Fart = 3,
    eSlig_BS = 5,
    eSlig_LookOut = 6,
    eSlig_BS2 = 7,
    eSlig_Laugh = 8,
    eAbe_Hello = 9,
    eAbe_FollowMe = 10,
    eAbe_Anger = 11,
    eAbe_Wait = 12,
    eAbe_Work = 21,
    eAbe_StopIt = 22,
    eAbe_AllYa = 23,
    eAbe_Sorry = 24,
    eSlig_Hi = 27,
    eSlig_HereBoy = 28,
    eSlig_GetEm = 29,
    eSlig_Freeze = 31,
    eGlukkon_None = 35,
    eGlukkon_Hey = 36,
    eGlukkon_DoIt = 37,
    eGlukkon_StayHere = 38,
    eGlukkon_Commere = 39,
    eGlukkon_AllOYa = 40,
    eGlukkon_Heh = 41,
    eGlukkon_Help = 42,
    eGlukkon_Laugh = 43,
    eGlukkon_KillEm = 44,
    eGlukkon_Unknown = 45, // no sound
    eGlukkon_What = 47,

    eParamite_Howdy = 48,
    eParamite_Stay = 49,
    eParamite_CMon_or_Attack = 50,
    eParamite_DoIt = 51,
    eParamite_AllYa = 52,

    eScrab_Howl = 53,
    eScrab_Shriek = 54,
};

enum class GameSpeakMatch : s16
{
    eNoMatch = 0,
    eFullMatch = 1,
    ePartMatch = 2,
};

class GameSpeak final : public BaseGameObject
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
