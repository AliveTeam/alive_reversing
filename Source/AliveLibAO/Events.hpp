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
    kEvent_7 = 7, // Slig death event, never read by anything?
    kEventAbeOhm_8 = 8,
    kEvent_9 = 9, // Read but never posted by anything
    kEventSuspiciousNoise_10 = 10,
    kEvent_11 = 11, // Posted by elum but never read, some kind of elum noise event

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
