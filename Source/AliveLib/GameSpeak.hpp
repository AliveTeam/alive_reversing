#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"

void GameSpeak_ForceLink();

class GameSpeak : public BaseGameObject
{
public:
    virtual void VDestructor(signed int flags);
    virtual void VUpdate() override;
    virtual EXPORT void PushEvent_4218D0(char event);

    GameSpeak();
    EXPORT GameSpeak* ctor_421820();
    EXPORT void dtor_4218A0();
    EXPORT void dtor_421870(signed int flags);

private:
    EXPORT void Update_421920();
    void PushEvent_Impl(char event);

private:
    __int16 field_20_last_event;
    __int16 field_22;
    unsigned int field_24_last_event_frame;
    int field_28_last_event_index;
    char field_2C_event_buffer[32];
};
ALIVE_ASSERT_SIZEOF(GameSpeak, 0x4C);

ALIVE_VAR_EXTERN(GameSpeak*, pEventSystem_5BC11C);
