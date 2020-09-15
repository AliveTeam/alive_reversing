#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

enum class GameSpeakEvents : __int16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,

    eUnknown_1 = 1,
    eUnknown_2 = 2,
    eUnknown_3 = 3,
    eUnknown_4 = 4,
    eUnknown_5 = 5,
    eUnknown_6 = 6,
    eUnknown_7 = 7,
    eUnknown_8 = 8,
    eUnknown_9 = 9,
    eUnknown_10 = 10,
    eUnknown_11 = 11,
    eUnknown_12 = 12,
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
    eUnknown_23 = 23,
    eUnknown_24 = 24,
    eUnknown_25 = 25,
    eUnknown_26 = 26,
    eUnknown_27 = 27,
    eUnknown_28 = 28,

    eUnknown_30 = 30,
    eUnknown_31 = 31,
};

enum class GameSpeakMatch : __int16
{
    eNoMatch_0 = 0,
    eFullMatch_1 = 1,
    ePartMatch_2 = 2,
};

class GameSpeak : public BaseGameObject
{
public:
    EXPORT GameSpeak* ctor_40F990();

    EXPORT GameSpeak* Vdtor_40FB50(signed int flags);

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* dtor_40F9C0();

    // A new virtual that is never overridden as there are no other known super classes
    virtual EXPORT void VPushEvent_40F9E0(GameSpeakEvents event);

    void PushEvent_Impl(GameSpeakEvents event);

    virtual void VUpdate() override;

    EXPORT void VUpdate_40FA20();

    static EXPORT int CC sub_40FA60(int code, BYTE* pBuffer);

    EXPORT GameSpeakMatch MatchBuffer_40FAA0(BYTE* pBuffer, __int16 bufferLen, signed __int16 bufferStartIdx);

    GameSpeakEvents field_10_last_event;
    __int16 field_12;
    unsigned int field_14_last_event_frame;
    int field_18_last_event_index;
    char field_1C_event_buffer[32];
};
ALIVE_ASSERT_SIZEOF(GameSpeak, 0x3C);

ALIVE_VAR_EXTERN(GameSpeak*, pEventSystem_4FF954);

EXPORT __int16 CC Code_Length_475FD0(unsigned int code);
EXPORT int CC Code_Convert_476000(unsigned __int16 code1, unsigned __int16 code2);
EXPORT __int16 CC Code_LookUp_476050(unsigned int code, unsigned __int16 idx, unsigned __int16 code_len);

END_NS_AO
