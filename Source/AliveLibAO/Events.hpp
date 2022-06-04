#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"

namespace AO {

class BaseGameObject;
class BaseAnimatedWithPhysicsGameObject;

enum Event : s16
{
    kEventNoise = 0,
    kEventSpeaking = 1,
    kEventShooting = 2,
    kEventHeroDying = 3,
    kEventDeathReset = 4,
    kEventDeathResetEnd = 5,
    kEventResetting = 6,
    kEventAbeOhm = 8,
    kEventSuspiciousNoise = 10,
    kEventLoudNoise = 14,
    kEventMudokonDead = 15,
    kEventMudokonComfort = 16,
    kEventAlarm = 17,
    kEventPortalOpen = 18,
    kEventScreenShake = 19,
};

BaseGameObject* Event_Get(s16 eventType);

void Event_Broadcast(s32 eventType, BaseGameObject* pObject);

BaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(s16 eventType, FP xpos, FP ypos, FP scale);

void Events_Reset_Active();

void Event_Cancel_For_Obj(BaseGameObject* pObj);

} // namespace AO
