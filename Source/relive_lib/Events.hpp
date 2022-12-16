#pragma once

#include "../relive_lib/FixedPoint_common.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

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

void EventBroadcast(Event event, BaseGameObject* pObject);
BaseGameObject* EventGet(Event eventType);
void EventsReset();
void EventsResetActive();

enum class EventScale
{
    Full,
    Half,
    Both
};

inline constexpr EventScale AsEventScale(Scale scale)
{
    if (scale == Scale::Fg)
    {
        return EventScale::Full;
    }
    return EventScale::Half;
}

BaseAnimatedWithPhysicsGameObject* IsEventInRange(Event eventType, FP xpos, FP ypos, EventScale scale);

// TODO: Get AO to stop using the FP scale and merge both copies
BaseAnimatedWithPhysicsGameObject* IsEventInRange(Event eventType, FP xpos, FP ypos, FP scale);

void EventCancelForObj(BaseGameObject* pObj);
