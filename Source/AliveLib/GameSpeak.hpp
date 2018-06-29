#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Game.hpp"

void GameSpeak_ForceLink();

class GameSpeak : public BaseGameObject
{
public:
    EXPORT void Update_421920();
    EXPORT void PushEvent_4218D0(char event);

    /*EXPORT*/ void ctor_421820();

public:
    __int16 field_20_last_event;
    __int16 field_22;
    int field_24_last_event_frame;
    int field_28_last_event_index;
    char field_2C_event_buffer[32];
};