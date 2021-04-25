#include "stdafx_ao.h"
#include "Events.hpp"
#include "Function.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "PsxDisplay.hpp"

namespace AO {

struct EventsArray
{
    BaseGameObject* field_0_event_ptrs[20];
};

struct Events
{
    // 2 copies for f64 buffering
    EventsArray field_0_events[2];
};

ALIVE_VAR(1, 0x4FFA48, s16, sEventsToUse_4FFA48, 0);
ALIVE_VAR(1, 0x4FF9A8, Events, sEventPtrs_4FF9A8, {});


BaseGameObject* CC Event_Get_417250(s16 eventType)
{
    return sEventPtrs_4FF9A8.field_0_events[sEventsToUse_4FFA48].field_0_event_ptrs[eventType];
}

void CC Event_Broadcast_417220(s32 eventType, BaseGameObject* pObject)
{
    sEventPtrs_4FF9A8.field_0_events[!sEventsToUse_4FFA48].field_0_event_ptrs[eventType] = pObject;
}

BaseAnimatedWithPhysicsGameObject* CC Event_Is_Event_In_Range_417270(s16 eventType, FP xpos, FP ypos, FP scale)
{
    BaseGameObject* pObj = sEventPtrs_4FF9A8.field_0_events[sEventsToUse_4FFA48].field_0_event_ptrs[eventType];
    if (!pObj)
    {
        return nullptr;
    }

    if (!pObj->field_6_flags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5) || !pObj->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
    {
        return nullptr;
    }

    // At this point we known the type must be BaseAnimatedWithPhysicsGameObject
    auto pDerived = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

    if (pDerived->field_BC_sprite_scale == scale &&
        FP_Abs(xpos - pDerived->field_A8_xpos) < FP_FromInteger(gPsxDisplay_504C78.field_0_width) &&
        FP_Abs(ypos - pDerived->field_AC_ypos) < FP_FromInteger(gPsxDisplay_504C78.field_2_height))
    {
        return pDerived;
    }

    return nullptr;
}

void CC Events_Reset_Active_417320()
{
    for (auto& ptr : sEventPtrs_4FF9A8.field_0_events[sEventsToUse_4FFA48].field_0_event_ptrs)
    {
        ptr = nullptr;
    }

    sEventsToUse_4FFA48 = !sEventsToUse_4FFA48;
}

void CC Event_Cancel_For_Obj_417350(BaseGameObject* pObj)
{
    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < ALIVE_COUNTOF(EventsArray::field_0_event_ptrs); j++)
        {
            if (sEventPtrs_4FF9A8.field_0_events[i].field_0_event_ptrs[j] == pObj)
            {
                sEventPtrs_4FF9A8.field_0_events[i].field_0_event_ptrs[j] = nullptr;
            }
        }
    }
}

}
