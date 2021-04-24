#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

namespace AO {

class BaseGameObject;
class BaseAnimatedWithPhysicsGameObject;

enum Event : s16
{
    kEventNoise_0 = 0,
    kEventSpeaking_1 = 1,
    kEvent_2 = 2,
    kEventHeroDying_3 = 3,
    kEventDeathReset_4 = 4,
    kEvent_5 = 5,
    kEventResetting_6 = 6,
    kEvent_7 = 7,
    kEventAbeOhm_8 = 8,
    kEvent_9 = 9,
    kEventSuspiciousNoise_10 = 10,
    kEvent_11 = 11,

    kEvent_14 = 14,
    kEventMudokonDead_15 = 15,
    kEventMudokonComfort_16 = 16,
    kEvent_Alarm_17 = 17,
    kEvent_18 = 18,
    kEventScreenShake_19 = 19,
};

EXPORT BaseGameObject* CC Event_Get_417250(s16 eventType);

EXPORT void CC Event_Broadcast_417220(s32 eventType, BaseGameObject* pObject);

EXPORT BaseAnimatedWithPhysicsGameObject* CC Event_Is_Event_In_Range_417270(s16 eventType, FP xpos, FP ypos, FP scale);

EXPORT void CC Events_Reset_Active_417320();

EXPORT void CC Event_Cancel_For_Obj_417350(BaseGameObject* pObj);

}
