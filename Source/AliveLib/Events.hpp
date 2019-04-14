#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

namespace Test
{
    void EventTests();
}

// Note, names taken from Abe's Exoddus PC Demo
enum Event : __int16
{
    kEventNoise = 0,
    kEventSpeaking = 1,
    kEventShooting = 2,
    kEventHeroDying = 4,
    kEventDeathReset = 5,
    kEventDeathResetEnd = 6,
    kEventResetting = 7,
    kEventAbeOhm = 8,
    kEventSuspiciousNoise = 9,
    kEventLoudNoise = 10,
    kEventMudokonDied = 11,
    kEventMudokonLaugh = 12,
    kEventMudokonAbuse = 13,
    kEventMudokonComfort = 14,
    kEventAlarm = 18,
    kEventPortalOpen = 19,
    kEventScreenShake = 21,
    kEventMax = 22, // Must be last, do not use
};

class BaseGameObject;
class BaseAnimatedWithPhysicsGameObject;

EXPORT void CC Event_Broadcast_422BC0(int event, BaseGameObject* pObject);
EXPORT BaseGameObject* CC Event_Get_422C00(__int16 eventType);
EXPORT void CC Events_Reset_422D70();
EXPORT void CC Events_Reset_Active_422DA0();
EXPORT BaseAnimatedWithPhysicsGameObject* CC Event_Is_Event_In_Range_422C30(__int16 eventType, FP xpos, FP ypos, __int16 scale);
EXPORT void CC Event_Cancel_For_Obj_422DF0(BaseGameObject* pObj);
