#include "stdafx.h"
#include "GameSpeak.hpp"
#include "stdlib.hpp"
#include "Function.hpp"

void GameSpeak_ForceLink() { }

ALIVE_VAR(1, 0x5BC11C, GameSpeak*, pEventSystem_5BC11C, nullptr);

GameSpeak* GameSpeak::ctor_421820()
{
    BaseGameObject::BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x544B20);

    field_6_flags.Set(BaseGameObject::eBit08); // Dont destroy on loading save
    field_2C_event_buffer[0] = -1;
    field_20_last_event = -1;
    field_4_typeId = Types::eGameSpeak_38;
    field_28_last_event_index = 0;
    return this;
}

void GameSpeak::dtor_4218A0()
{
    pEventSystem_5BC11C = nullptr;
    BaseGameObject_dtor_4DBEC0();
}

BaseGameObject* GameSpeak::vdtor_421870(signed int flags)
{
    dtor_4218A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

BaseGameObject* GameSpeak::VDestructor(signed int flags)
{
    return vdtor_421870(flags);
}

void GameSpeak::VUpdate()
{
    Update_421920();
}

void GameSpeak::VRender(int** /*pOrderingTable*/)
{
    // Null @ 0x4DBF80
}


void GameSpeak::VScreenChanged()
{
    // Null @ 0x421AB0
}

void GameSpeak::Update_421920()
{
    if (field_20_last_event != -1 && sGnFrame_5C1B84 > field_24_last_event_frame)
    {
        PushEvent_Impl(-1);
    }
}

void GameSpeak::PushEvent_4218D0(char event)
{
    PushEvent_Impl(event);
    field_24_last_event_frame = sGnFrame_5C1B84 + 60;
}

void GameSpeak::PushEvent_Impl(char event)
{
    field_28_last_event_index++;

    // Wrap around
    if (field_28_last_event_index >= sizeof(field_2C_event_buffer))
    {
        field_28_last_event_index = 0;
    }

    field_2C_event_buffer[field_28_last_event_index] = event;
    field_20_last_event = event;
}
