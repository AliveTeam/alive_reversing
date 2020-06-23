#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class BaseGameObject;

enum Event : __int16
{
    kEventDeathReset_4 = 4,
    kEvent_7 = 7,
};

EXPORT BaseGameObject* CC Event_Get_417250(__int16 eventType);

EXPORT void CC Event_Broadcast_417220(int eventType, BaseGameObject* pObject);


END_NS_AO
