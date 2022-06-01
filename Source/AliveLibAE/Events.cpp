#include "stdafx.h"
#include "Events.hpp"
#include "Function.hpp"
#include "Animation.hpp" // TODO: Move BaseAnimatedWithPhysicsGameObject to its own file
#include "BaseAliveGameObject.hpp"
#include "DebugHelpers.hpp"
#include <gmock/gmock.h>

struct EventsArray final
{
    BaseGameObject* field_0_event_ptrs[Event::kEventMax];
};
ALIVE_ASSERT_SIZEOF(EventsArray, 0x58);

struct Events final
{
    // 2 copies for f64 buffering
    EventsArray field_0_events[2];
};
ALIVE_ASSERT_SIZEOF(Events, 0xB0);

ALIVE_VAR(1, 0x5BC1D4, s16, sEventsToUse_5BC1D4, 0);
ALIVE_VAR(1, 0x5BC124, Events, sEventPtrs_5BC124, {});

const char_type* sEventEnumString[]{
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

void Event_Broadcast_422BC0(s32 eventType, BaseGameObject* pObject)
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

BaseGameObject* Event_Get_422C00(s16 eventType)
{
    return sEventPtrs_5BC124.field_0_events[sEventsToUse_5BC1D4].field_0_event_ptrs[eventType];
}

void Events_Reset_422D70()
{
    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < Event::kEventMax; j++)
        {
            sEventPtrs_5BC124.field_0_events[i].field_0_event_ptrs[j] = nullptr;
        }
    }
    sEventsToUse_5BC1D4 = 0;
}

void Events_Reset_Active_422DA0()
{
    for (s32 i = 0; i < Event::kEventMax; i++)
    {
        sEventPtrs_5BC124.field_0_events[sEventsToUse_5BC1D4].field_0_event_ptrs[i] = nullptr;
    }

    sEventsToUse_5BC1D4 = !sEventsToUse_5BC1D4;
}

const s32 kGridMapWidth = 375;
const s32 kGridMapHeight = 260;

BaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range_422C30(s16 eventType, FP xpos, FP ypos, s16 scale)
{
    BaseGameObject* pObj = sEventPtrs_5BC124.field_0_events[sEventsToUse_5BC1D4].field_0_event_ptrs[eventType];
    if (!pObj)
    {
        return nullptr;
    }

    if (!(pObj->mFlags.Get(BaseAliveGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5)) || !(pObj->mFlags.Get(BaseAliveGameObject::eDrawable_Bit4)))
    {
        return nullptr;
    }

    // At this point we known the type must be BaseAnimatedWithPhysicsGameObject
    auto pDerived = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);
    if ((scale == -1 || pDerived->field_D6_scale == scale)
        && FP_GetExponent(xpos) / kGridMapWidth == FP_GetExponent(pDerived->field_B8_xpos) / kGridMapWidth
        && FP_GetExponent(ypos) / kGridMapHeight == FP_GetExponent(pDerived->field_BC_ypos) / kGridMapHeight)
    {
        return pDerived;
    }

    return nullptr;
}

void Event_Cancel_For_Obj(BaseGameObject* pObj)
{
    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < Event::kEventMax; j++)
        {
            if (sEventPtrs_5BC124.field_0_events[i].field_0_event_ptrs[j] == pObj)
            {
                sEventPtrs_5BC124.field_0_events[i].field_0_event_ptrs[j] = nullptr;
            }
        }
    }
}
