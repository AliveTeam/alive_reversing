#include "stdafx.h"
#include "Events.hpp"
#include "Function.hpp"
#include "Animation.hpp" // TODO: Move BaseAnimatedWithPhysicsGameObject to its own file
#include "BaseAliveGameObject.hpp"
#include "DebugHelpers.hpp"
#include <gmock/gmock.h>

struct EventsArray
{
    BaseGameObject* field_0_event_ptrs[Event::kEventMax];
};
ALIVE_ASSERT_SIZEOF(EventsArray, 0x58);

struct Events
{
    // 2 copies for double buffering
    EventsArray field_0_events[2];
};
ALIVE_ASSERT_SIZEOF(Events, 0xB0);

ALIVE_VAR(1, 0x5BC1D4, short, sEventsToUse_5BC1D4, 0);
ALIVE_VAR(1, 0x5BC124, Events, sEventPtrs_5BC124, {});

const char * sEventEnumString[]
{
    "Noise",
    "Speaking",
    "Shooting",
    "",
    "HeroDying",
    "DeathReset",
    "DeathResetEnd",
    "Resetting",
    "AbeOhm",
    "SuspiciousNoise",
    "LoudNoise",
    "MudokonDied",
    "MudokonLaugh",
    "MudokonAbuse",
    "MudokonComfort",
    "",
    "",
    "",
    "Alarm",
    "PortalOpen",
    "",
    "ScreenShake",
};

EXPORT void CC Event_Broadcast_422BC0(Event eventType, BaseGameObject* pObject)
{
    sEventPtrs_5BC124.field_0_events[!sEventsToUse_5BC1D4].field_0_event_ptrs[eventType] = pObject;

    if (sDebugEnabled_VerboseEvents)
    {
        switch (eventType)
        {
            // Ignore these as they get spammed.
        case kEventNoise:
        case kEventSuspiciousNoise:
            return;
        }
        DEV_CONSOLE_MESSAGE_C("Event: " + std::string(sEventEnumString[eventType]), 5, 0, 0, 127);
    }
}

EXPORT BaseGameObject* CC Event_Get_422C00(__int16 eventType)
{
    return sEventPtrs_5BC124.field_0_events[sEventsToUse_5BC1D4].field_0_event_ptrs[eventType];
}

EXPORT void CC Events_Reset_422D70()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < Event::kEventMax; j++)
        {
            sEventPtrs_5BC124.field_0_events[i].field_0_event_ptrs[j] = nullptr;
        }
    }
    sEventsToUse_5BC1D4 = 0;
}

EXPORT void CC Events_Reset_Active_422DA0()
{
    for (int i = 0; i < Event::kEventMax; i++)
    {
        sEventPtrs_5BC124.field_0_events[sEventsToUse_5BC1D4].field_0_event_ptrs[i] = nullptr;
    }

    sEventsToUse_5BC1D4 = !sEventsToUse_5BC1D4;
}

const int kGridMapWidth = 375;
const int kGridMapHeight = 260;

EXPORT BaseAnimatedWithPhysicsGameObject* CC Event_Is_Event_In_Range_422C30(Event eventType, FP xpos, FP ypos, __int16 scale)
{
    BaseGameObject* pObj = sEventPtrs_5BC124.field_0_events[sEventsToUse_5BC1D4].field_0_event_ptrs[eventType];
    if (!pObj)
    {
        return nullptr;
    }

    if (!(pObj->field_6_flags.Get(BaseAliveGameObject::eIsBaseAnimatedWithPhysicsObj)) || !(pObj->field_6_flags.Get(BaseAliveGameObject::eDrawable)))
    {
        return nullptr;
    }
    
    // At this point we known the type must be BaseAnimatedWithPhysicsGameObject
    auto pDerived = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);
    if ((scale == -1 || pDerived->field_D6_scale == scale)
        && FP_GetExponent(xpos) / kGridMapWidth  == FP_GetExponent(pDerived->field_B8_xpos) / kGridMapWidth
        && FP_GetExponent(ypos) / kGridMapHeight == FP_GetExponent(pDerived->field_BC_ypos) / kGridMapHeight)
    {
        return pDerived;
    }

    return nullptr;
}

EXPORT void CC Event_Cancel_For_Obj_422DF0(BaseGameObject* pObj)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < Event::kEventMax; j++)
        {
            if (sEventPtrs_5BC124.field_0_events[i].field_0_event_ptrs[j] == pObj)
            {
                sEventPtrs_5BC124.field_0_events[i].field_0_event_ptrs[j] = nullptr;
            }
        }
    }
}

namespace Test
{
    void EventTests()
    {
        Events_Reset_422D70();

        BaseAnimatedWithPhysicsGameObject bang;
        bang.field_B8_xpos = FP_FromInteger(10);
        bang.field_BC_ypos = FP_FromInteger(20);
        bang.field_D6_scale = 0;
        bang.field_6_flags.Set(BaseAliveGameObject::eIsBaseAnimatedWithPhysicsObj);
        bang.field_6_flags.Set(BaseAliveGameObject::eDrawable);
        Event_Broadcast_422BC0(Event::kEventLoudNoise, &bang);

        Events_Reset_Active_422DA0();
        
        ASSERT_EQ(&bang, Event_Get_422C00(Event::kEventLoudNoise));
        ASSERT_EQ(nullptr, Event_Get_422C00(Event::kEventAlarm));

        ASSERT_EQ(&bang, Event_Is_Event_In_Range_422C30(Event::kEventLoudNoise, FP_FromInteger(10), FP_FromInteger(20), 0));
        ASSERT_EQ(nullptr, Event_Is_Event_In_Range_422C30(Event::kEventLoudNoise, FP_FromInteger(kGridMapWidth * 2), FP_FromInteger(kGridMapHeight * 2), 0));

        Event_Cancel_For_Obj_422DF0(&bang);
        ASSERT_EQ(nullptr, Event_Get_422C00(Event::kEventLoudNoise));

        sEventPtrs_5BC124.field_0_events[0].field_0_event_ptrs[Event::kEventLoudNoise] = &bang;
        sEventPtrs_5BC124.field_0_events[1].field_0_event_ptrs[Event::kEventLoudNoise] = &bang;

        Event_Cancel_For_Obj_422DF0(&bang);
        ASSERT_EQ(nullptr, sEventPtrs_5BC124.field_0_events[0].field_0_event_ptrs[Event::kEventLoudNoise]);
        ASSERT_EQ(nullptr, sEventPtrs_5BC124.field_0_events[1].field_0_event_ptrs[Event::kEventLoudNoise]);
    }
}
