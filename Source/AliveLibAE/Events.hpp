#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"

// Note, names taken from Abe's Exoddus PC Demo
enum Event : s16
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
    kEventUnknown17 = 17,
    kEventAlarm = 18,
    kEventPortalOpen = 19,
    kEventUnknown20 = 20,
    kEventScreenShake = 21,
    kEventMax = 22, // Must be last, do not use
};

class BaseGameObject;
class BaseAnimatedWithPhysicsGameObject;

void Event_Broadcast(s32 event, BaseGameObject* pObject);
BaseGameObject* Event_Get(s16 eventType);
void Events_Reset();
void Events_Reset_Active();
BaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(s16 eventType, FP xpos, FP ypos, s16 scale);
void Event_Cancel_For_Obj(BaseGameObject* pObj);
