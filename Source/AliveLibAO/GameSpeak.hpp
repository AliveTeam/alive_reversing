#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

enum class GameSpeakEvents : __int16
{
    eSameAsLast_m2 = -2,
    eNone_m1 = -1,
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
