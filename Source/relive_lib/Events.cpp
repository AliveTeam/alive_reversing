#include "stdafx.h"
#include "Events.hpp"
#include "BaseGameObject.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "GameType.hpp"

struct EventsArray final
{
    IBaseGameObject* field_0_event_ptrs[Event::kEventMax];
};

struct Events final
{
    // 2 copies for double buffering
    EventsArray field_0_events[2];
};

static s16 sEventsToUse = 0;
static Events sEventPtrs = {};

void Event_Broadcast(Event eventType, IBaseGameObject* pObject)
{
    sEventPtrs.field_0_events[!sEventsToUse].field_0_event_ptrs[eventType] = pObject;
}

IBaseGameObject* Event_Get(Event eventType)
{
    return sEventPtrs.field_0_events[sEventsToUse].field_0_event_ptrs[eventType];
}

void Events_Reset()
{
    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < Event::kEventMax; j++)
        {
            sEventPtrs.field_0_events[i].field_0_event_ptrs[j] = nullptr;
        }
    }
    sEventsToUse = 0;
}

void Events_Reset_Active()
{
    for (s32 i = 0; i < Event::kEventMax; i++)
    {
        sEventPtrs.field_0_events[sEventsToUse].field_0_event_ptrs[i] = nullptr;
    }

    sEventsToUse = !sEventsToUse;
}

const s32 kGridMapWidth = 375;
const s32 kGridMapHeight = 260;

IBaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(Event eventType, FP xpos, FP ypos, s16 scale)
{
    IBaseGameObject* pObj = sEventPtrs.field_0_events[sEventsToUse].field_0_event_ptrs[eventType];
    if (!pObj)
    {
        return nullptr;
    }

    if (!(pObj->mBaseGameObjectFlags.Get(IBaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5)) || !(pObj->mBaseGameObjectFlags.Get(IBaseGameObject::eDrawable_Bit4)))
    {
        return nullptr;
    }

    // At this point we known the type must be BaseAnimatedWithPhysicsGameObject
    auto pDerived = dynamic_cast /*static_cast*/<IBaseAnimatedWithPhysicsGameObject*>(pObj); // HACK: use static_cast when glue/adapter interfaces are removed
    if (GetGameType() == GameType::eAe)
    {
        if ((scale == -1 || pDerived->Scale() == scale)
            && FP_GetExponent(xpos) / kGridMapWidth == FP_GetExponent(pDerived->XPos()) / kGridMapWidth
            && FP_GetExponent(ypos) / kGridMapHeight == FP_GetExponent(pDerived->YPos()) / kGridMapHeight)
        {
            return pDerived;
        }
    }
    else
    {
        if (pDerived->Scale() == scale &&
            FP_Abs(xpos - pDerived->XPos()) < FP_FromInteger(640) && // gPsxDisplay_504C78.field_0_width
            FP_Abs(ypos - pDerived->YPos()) < FP_FromInteger(240)) // gPsxDisplay_504C78.field_2_height
        {
            return pDerived;
        }
    }
    return nullptr;
}

IBaseAnimatedWithPhysicsGameObject* Event_Is_Event_In_Range(Event eventType, FP xpos, FP ypos, FP scale)
{
    IBaseGameObject* pObj = sEventPtrs.field_0_events[sEventsToUse].field_0_event_ptrs[eventType];
    if (!pObj)
    {
        return nullptr;
    }

    if (!(pObj->mBaseGameObjectFlags.Get(IBaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5)) || !(pObj->mBaseGameObjectFlags.Get(IBaseGameObject::eDrawable_Bit4)))
    {
        return nullptr;
    }

    // At this point we known the type must be BaseAnimatedWithPhysicsGameObject
    auto pDerived = dynamic_cast /*static_cast*/<IBaseAnimatedWithPhysicsGameObject*>(pObj);           // HACK: use static_cast when glue/adapter interfaces are removed
    if (pDerived->SpriteScale() == scale && FP_Abs(xpos - pDerived->XPos()) < FP_FromInteger(640) && // gPsxDisplay_504C78.field_0_width
        FP_Abs(ypos - pDerived->YPos()) < FP_FromInteger(240))                                       // gPsxDisplay_504C78.field_2_height
    {
        return pDerived;
    }
    return nullptr;
}

void Event_Cancel_For_Obj(IBaseGameObject* pObj)
{
    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < Event::kEventMax; j++)
        {
            if (sEventPtrs.field_0_events[i].field_0_event_ptrs[j] == pObj)
            {
                sEventPtrs.field_0_events[i].field_0_event_ptrs[j] = nullptr;
            }
        }
    }
}
