#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class BaseGameObject;

enum Event : __int16
{
    kEventDeathReset_4 = 4,
};

EXPORT BaseGameObject* CC Event_Get_417250(__int16 eventType);

END_NS_AO
