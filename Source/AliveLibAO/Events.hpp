#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"

namespace AO {

class BaseGameObject;
class BaseAnimatedWithPhysicsGameObject;

enum Event : s16
{
    kEventNoise,
    kEventSpeaking,
    kEventShooting,
    kEventHeroDying,
    kEventDeathReset,
    kEventDeathResetEnd,
    kEventResetting,
    kEventAbeOhm,
    kEventSuspiciousNoise,
    kEventLoudNoise,
    kEventMudokonDead,
    kEventMudokonComfort,
    kEventAlarm,
    kEventPortalOpen,
    kEventScreenShake,
    kEventMax, // Must be last, do not use
};

BaseGameObject* Event_Get(s16 eventType);

void Event_Broadcast(s32 eventType, BaseGameObject* pObject);

BaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(s16 eventType, FP xpos, FP ypos, FP scale);

void Events_Reset_Active();

void Event_Cancel_For_Obj(BaseGameObject* pObj);

} // namespace AO
