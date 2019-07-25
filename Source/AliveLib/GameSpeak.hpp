#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"

void GameSpeak_ForceLink();

enum GameSpeakEvents : char
{
    eNone = -1,
    eFart_3 = 3,
    eUnknown_5 = 5,
    eLookOut_6 = 6,
    eUnknown_7 = 7,
    eUnknown_8 = 8,
    eHello_9 = 9,
    eFollowMe_10 = 10,
    eAnger_11 = 11,
    eWait_12 = 12,
    eWork_21 = 21,
    eStopIt_22 = 22,
    eAllYa_23 = 23,
    eSorry_24 = 24,

    eUnknown_27 = 27,
    eUnknown_28 = 28,
    eUnknown_29 = 29,

    eUnknown_31 = 31,

    eUnknown_53 = 53,
    eUnknown_54 = 54,
};

class GameSpeak : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    // A new virtual that is never overridden as there are no other known super classes
    virtual EXPORT void PushEvent_4218D0(GameSpeakEvents event);

    EXPORT GameSpeak* ctor_421820();

private:
    EXPORT void dtor_4218A0();
    EXPORT BaseGameObject* vdtor_421870(signed int flags);

private:
    EXPORT void Update_421920();
    void PushEvent_Impl(GameSpeakEvents event);

public:
    __int16 field_20_last_event;
    __int16 field_22;
    unsigned int field_24_last_event_frame;
    int field_28_last_event_index;
    char field_2C_event_buffer[32];
};
ALIVE_ASSERT_SIZEOF(GameSpeak, 0x4C);

ALIVE_VAR_EXTERN(GameSpeak*, pEventSystem_5BC11C);
