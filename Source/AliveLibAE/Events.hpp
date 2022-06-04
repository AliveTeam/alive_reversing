#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"

// Note, names taken from Abe's Exoddus PC Demo
enum Event : s16
{
    kEventNoise,
    kEventSpeaking,
    kEventShooting,
    kEventHeroDying,
    kEventAbeDead, // only at the point when the abe birds appear
    kScrabOrParamiteDied,
    kEventDeathReset,
    kEventDeathResetEnd,
    kEventResetting,
    kEventAbeOhm,
    kEventSuspiciousNoise,
    kEventLoudNoise,
    kEventMudokonDied,
    kEventMudokonLaugh,
    kEventMudokonAbuse,
    kEventMudokonComfort,
    kEventGlukkonUnknown,
    kEventAlarm,
    kEventPortalOpen,
    kEventOtherPortalOpen_20,
    kEventScreenShake,
    kEventMax, // Must be last, do not use
};

class BaseGameObject;
class BaseAnimatedWithPhysicsGameObject;

void Event_Broadcast(s32 event, BaseGameObject* pObject);
BaseGameObject* Event_Get(s16 eventType);
void Events_Reset();
void Events_Reset_Active();
BaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(s16 eventType, FP xpos, FP ypos, s16 scale);
void Event_Cancel_For_Obj(BaseGameObject* pObj);
