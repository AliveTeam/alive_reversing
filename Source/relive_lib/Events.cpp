#include "stdafx.h"
#include "Events.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "GameType.hpp"

struct EventsArray final
{
    BaseGameObject* mEventPtrs[Event::kEventMax];
};

struct Events final
{
    // 2 copies for double buffering
    EventsArray mEvents[2];
};

static s16 sEventsToUse = 0;
static Events sEventPtrs = {};

void EventBroadcast(Event eventType, BaseGameObject* pObject)
{
    sEventPtrs.mEvents[!sEventsToUse].mEventPtrs[eventType] = pObject;
}

BaseGameObject* EventGet(Event eventType)
{
    return sEventPtrs.mEvents[sEventsToUse].mEventPtrs[eventType];
}

void EventsReset()
{
    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < Event::kEventMax; j++)
        {
            sEventPtrs.mEvents[i].mEventPtrs[j] = nullptr;
        }
    }
    sEventsToUse = 0;
}

void EventsResetActive()
{
    for (s32 i = 0; i < Event::kEventMax; i++)
    {
        sEventPtrs.mEvents[sEventsToUse].mEventPtrs[i] = nullptr;
    }

    sEventsToUse = !sEventsToUse;
}

const s32 kGridMapWidth = 375;
const s32 kGridMapHeight = 260;

BaseAnimatedWithPhysicsGameObject* IsEventInRange(Event eventType, FP xpos, FP ypos, EventScale scale)
{
    BaseGameObject* pObj = sEventPtrs.mEvents[sEventsToUse].mEventPtrs[eventType];
    if (!pObj)
    {
        return nullptr;
    }

    if (!pObj->GetIsBaseAnimatedWithPhysicsObj() || !(pObj->GetDrawable()))
    {
        return nullptr;
    }

    // At this point we known the type must be BaseAnimatedWithPhysicsGameObject
    auto pDerived = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);
    if (GetGameType() == GameType::eAe)
    {
        if ((scale == EventScale::Both || AsEventScale(pDerived->GetScale()) == scale)
            && FP_GetExponent(xpos) / kGridMapWidth == FP_GetExponent(pDerived->mXPos) / kGridMapWidth
            && FP_GetExponent(ypos) / kGridMapHeight == FP_GetExponent(pDerived->mYPos) / kGridMapHeight)
        {
            return pDerived;
        }
    }
    else
    {
        if (AsEventScale(pDerived->GetScale()) == scale && FP_Abs(xpos - pDerived->mXPos) < FP_FromInteger(640) && // gPsxDisplay.mWidth
            FP_Abs(ypos - pDerived->mYPos) < FP_FromInteger(240)) // gPsxDisplay.mHeight
        {
            return pDerived;
        }
    }
    return nullptr;
}

BaseAnimatedWithPhysicsGameObject* IsEventInRange(Event eventType, FP xpos, FP ypos, FP scale)
{
    BaseGameObject* pObj = sEventPtrs.mEvents[sEventsToUse].mEventPtrs[eventType];
    if (!pObj)
    {
        return nullptr;
    }

    if (!pObj->GetIsBaseAnimatedWithPhysicsObj() || !(pObj->GetDrawable()))
    {
        return nullptr;
    }

    // At this point we known the type must be BaseAnimatedWithPhysicsGameObject
    auto pDerived = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);
    if (pDerived->GetSpriteScale() == scale && FP_Abs(xpos - pDerived->mXPos) < FP_FromInteger(640) && // gPsxDisplay.mWidth
        FP_Abs(ypos - pDerived->mYPos) < FP_FromInteger(240))                                       // gPsxDisplay.mHeight
    {
        return pDerived;
    }
    return nullptr;
}

void EventCancelForObj(BaseGameObject* pObj)
{
    for (s32 i = 0; i < 2; i++)
    {
        for (s32 j = 0; j < Event::kEventMax; j++)
        {
            if (sEventPtrs.mEvents[i].mEventPtrs[j] == pObj)
            {
                sEventPtrs.mEvents[i].mEventPtrs[j] = nullptr;
            }
        }
    }
}
