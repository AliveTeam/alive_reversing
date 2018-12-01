#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"

void GameSpeak_ForceLink();

class GameSpeak : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags);
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    // A new virtual that is never override as there are no other known super classes
    virtual EXPORT void PushEvent_4218D0(char event);

    EXPORT GameSpeak* ctor_421820();
    EXPORT void dtor_4218A0();
    EXPORT BaseGameObject* vdtor_421870(signed int flags);

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
