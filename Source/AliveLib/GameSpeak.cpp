#include "stdafx.h"
#include "GameSpeak.hpp"
#include "Function.hpp"

void GameSpeak_ForceLink() { }

void GameSpeak::ctor_421820()
{
    BaseGameObject::ctor_4DBFA0(1, 0);
    field_6_flags |= 1u;
    field_2C_event_buffer[0] = -1;
    field_20_last_event = -1;
    field_4_typeId = 38;
    field_28_last_event_index = 0;
}

void GameSpeak::VUpdate()
{
    if (field_20_last_event != -1 && sGnFrame_5C1B84 > field_24_last_event_frame)
    {
        const int nextIndex = field_28_last_event_index + 1;
        field_28_last_event_index = nextIndex;
        if (nextIndex >= sizeof(field_2C_event_buffer))
            field_28_last_event_index = 0;
        field_2C_event_buffer[field_28_last_event_index] = -1;
        field_20_last_event = -1;
    }
}

void GameSpeak::PushEvent_4218D0(char event)
{
    const int nextIndex = field_28_last_event_index + 1;
    field_28_last_event_index = nextIndex;
    if (nextIndex >= sizeof(field_2C_event_buffer))
        field_28_last_event_index = 0;
    field_2C_event_buffer[field_28_last_event_index] = event;
    field_20_last_event = event;
    field_24_last_event_frame = sGnFrame_5C1B84 + 60;
}