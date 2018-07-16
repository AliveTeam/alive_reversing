#include "stdafx.h"
#include "Events.hpp"
#include "Function.hpp"
#include "Animation.hpp" // TODO: Move BaseAnimatedWithPhysicsGameObject to its own file
#include "BaseAliveGameObject.hpp"
#include <gmock/gmock.h>

struct EventsArray
{
    BaseAnimatedWithPhysicsGameObject* field_0_event_ptrs[Event::kEventMax];
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

EXPORT void CC Event_Broadcast_422BC0(Event eventType, BaseAnimatedWithPhysicsGameObject* pObject)
{
    sEventPtrs_5BC124.field_0_events[sEventsToUse_5BC1D4].field_0_event_ptrs[eventType] = pObject;
}

EXPORT BaseAnimatedWithPhysicsGameObject* CC Event_Get_422C00(Event eventType)
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
    BaseAnimatedWithPhysicsGameObject* pObj = sEventPtrs_5BC124.field_0_events[sEventsToUse_5BC1D4].field_0_event_ptrs[eventType];
    if (!pObj)
    {
        return nullptr;
    }

    if ((pObj->field_6_flags & BaseAliveGameObject::eBit04)
        && (pObj->field_6_flags & BaseAliveGameObject::eDrawable)
        && (scale == -1 || pObj->field_D6_scale == scale)
        && xpos.GetExponent() / kGridMapWidth  == pObj->field_B8_xpos.GetExponent() / kGridMapWidth
        && ypos.GetExponent() / kGridMapHeight == pObj->field_BC_ypos.GetExponent() / kGridMapHeight)
    {
        return pObj;
    }

    return nullptr;
}

namespace Test
{
    void EventTests()
    {
        Events_Reset_422D70();

        BaseAnimatedWithPhysicsGameObject bang;
        bang.field_B8_xpos = FP(10);
        bang.field_BC_ypos = FP(20);
        bang.field_D6_scale = 0;
        bang.field_6_flags = BaseAliveGameObject::eBit04 | BaseAliveGameObject::eDrawable;
        Event_Broadcast_422BC0(Event::kEventLoudNoise, &bang);

        ASSERT_EQ(&bang, Event_Get_422C00(Event::kEventLoudNoise));
        ASSERT_EQ(nullptr, Event_Get_422C00(Event::kEventAlarm));

        ASSERT_EQ(&bang, Event_Is_Event_In_Range_422C30(Event::kEventLoudNoise, FP(10), FP(20), 0));
        ASSERT_EQ(nullptr, Event_Is_Event_In_Range_422C30(Event::kEventLoudNoise, FP(kGridMapWidth * 2), FP(kGridMapHeight * 2), 0));

        Events_Reset_422D70();
    }
}
