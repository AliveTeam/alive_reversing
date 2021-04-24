#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

namespace AO {

enum class GameSpeakEvents : s16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,

    eWhistle1_1 = 1,
    eWhistle2_2 = 2,
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

class GameSpeak : public BaseGameObject
{
public:
    EXPORT GameSpeak* ctor_40F990();

    EXPORT GameSpeak* Vdtor_40FB50(s32 flags);

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* dtor_40F9C0();

    // A new virtual that is never overridden as there are no other known super classes
    virtual EXPORT void VPushEvent_40F9E0(GameSpeakEvents event);

    void PushEvent_Impl(GameSpeakEvents event);

    virtual void VUpdate() override;

    EXPORT void VUpdate_40FA20();

    static EXPORT short CC sub_40FA60(s32 code, u8* pBuffer);

    EXPORT GameSpeakMatch MatchBuffer_40FAA0(u8* pBuffer, s16 bufferLen, s16 bufferStartIdx);

    GameSpeakEvents field_10_last_event;
    s16 field_12;
    u32 field_14_last_event_frame;
    s32 field_18_last_event_index;
    char field_1C_event_buffer[32];
};
ALIVE_ASSERT_SIZEOF(GameSpeak, 0x3C);

ALIVE_VAR_EXTERN(GameSpeak*, pEventSystem_4FF954);

EXPORT s16 CC Code_Length_475FD0(u32 code);
EXPORT s32 CC Code_Convert_476000(u16 code1, u16 code2);
EXPORT GameSpeakEvents CC Code_LookUp_476050(u32 code, u16 idx, u16 code_len);

}
