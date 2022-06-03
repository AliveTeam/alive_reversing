#include "stdafx_ao.h"
#include "BaseGameObject.hpp"
#include "DynamicArray.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Map.hpp"

void AliveLibAO_ForceLink()
{ }

namespace AO {

ALIVE_VAR(1, 0x9F2DF0, DynamicArrayT<BaseGameObject>*, gBaseGameObjects, nullptr);

BaseGameObject::BaseGameObject(s16 addToObjectList)
{
    mBaseGameObjectUpdateDelay = 0;
    mBaseGameObjectRefCount = 0;

    mBaseGameObjectTypeId = Types::eNone_0;

    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eListAddFailed_Bit1);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eDead);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eIsBaseAliveGameObject_Bit6);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eCanExplode_Bit7);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eInteractive_Bit8);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eUpdateDuringCamSwap_Bit10);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eCantKill_Bit11);
    mBaseGameObjectFlags.Set(BaseGameObject::Options::eUpdatable_Bit2);

    if (addToObjectList)
    {
        if (!gBaseGameObjects->Push_Back(this))
        {
            mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
        }
    }
}

BaseGameObject::~BaseGameObject()
{
    Event_Cancel_For_Obj(this);
}

void BaseGameObject::VUpdate()
{
    // Empty
}

void BaseGameObject::VRender(PrimHeader**)
{
    // Empty
}

void BaseGameObject::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel
        || gMap.mCurrentPath != gMap.mPath
        || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BaseGameObject::VStopAudio()
{
    // Empty
}

} // namespace AO
