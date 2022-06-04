#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "FixedPoint.hpp"

namespace AO {

class BaseGameObject;
class BaseAnimatedWithPhysicsGameObject;

enum Event : s16
{
    kEventNoise_0 = 0,
    kEventSpeaking_1 = 1,
    kEventShooting_2 = 2,
    kEventHeroDying_3 = 3,
    kEventDeathReset_4 = 4,
    kEventDeathResetEnd_5 = 5,
    kEventResetting_6 = 6,
    kEventAbeOhm_8 = 8,
    kEventSuspiciousNoise_10 = 10,
    kEventLoudNoise_14 = 14,
    kEventMudokonDead_15 = 15,
    kEventMudokonComfort_16 = 16,
    kEvent_Alarm_17 = 17,
    kEventPortalOpen_18 = 18,
    kEventScreenShake_19 = 19,
};

BaseGameObject* Event_Get(s16 eventType);

void Event_Broadcast(s32 eventType, BaseGameObject* pObject);

BaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(s16 eventType, FP xpos, FP ypos, FP scale);

void Events_Reset_Active();

void Event_Cancel_For_Obj(BaseGameObject* pObj);

} // namespace AO
