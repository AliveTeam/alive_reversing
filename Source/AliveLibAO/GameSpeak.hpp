#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

namespace AO {

enum class GameSpeakEvents : s16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,

    eWhistleHigh_1 = 1,
    eWhistleLow_2 = 2,
    eFart_3 = 3,
    eLaugh_4 = 4,
    eUnknown_5 = 5,
    Slig_LookOut_6 = 6,
    eUnknown_7 = 7,
    eUnknown_8 = 8,
    eHello_9 = 9,
    eFollowMe_10 = 10,
    eAnger_11 = 11,
    eWait_12 = 12,
    eUnknown_13 = 13,
    eUnknown_14 = 14,
    eUnknown_15 = 15,
    eUnknown_16 = 16,
    eUnknown_17 = 17,
    eUnknown_18 = 18,
    eUnknown_19 = 19,
    eUnknown_20 = 20,
    eUnknown_21 = 21,
    eUnknown_22 = 22,
    Slig_Hi_23 = 23,
    Slig_HereBoy_24 = 24,
    eUnknown_25 = 25,
    eUnknown_26 = 26,
    eUnknown_27 = 27,
    eUnknown_28 = 28,
    eUnknown_29 = 29,
    eUnknown_30 = 30,
    eUnknown_31 = 31,
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

    GameSpeakEvents field_10_last_event = GameSpeakEvents::eNone_m1;
    s16 field_12 = 0;
    u32 field_14_last_event_frame = 0;
    s32 field_18_last_event_index = 0;
    s8 field_1C_event_buffer[32] = {};
};
ALIVE_ASSERT_SIZEOF(GameSpeak, 0x3C);

extern GameSpeak* pEventSystem_4FF954;

s16 Code_Length(u32 code);
s32 Code_Convert(u16 code1, u16 code2);
GameSpeakEvents Code_LookUp(u32 code, u16 idx, u16 code_len);

} // namespace AO
