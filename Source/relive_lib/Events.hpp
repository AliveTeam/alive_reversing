#pragma once

#include "../AliveLibCommon/FixedPoint_common.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

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
    kEventOtherPortalOpen,
    kEventScreenShake,
    kEventMudokonDead,
    kEventMax, // Must be last, do not use
};

class BaseGameObject;

void Event_Broadcast(Event event, BaseGameObject* pObject);
BaseGameObject* Event_Get(Event eventType);
void Events_Reset();
void Events_Reset_Active();
IBaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(Event eventType, FP xpos, FP ypos, Scale scale);

// TODO: Get AO to stop using the FP scale and merge both copies
IBaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(Event eventType, FP xpos, FP ypos, FP scale);

void Event_Cancel_For_Obj(BaseGameObject* pObj);
