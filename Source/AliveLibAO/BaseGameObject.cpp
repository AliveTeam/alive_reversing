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
    mUpdateDelay = 0;
    field_C_refCount = 0;

    mTypeId = Types::eNone_0;

    mGameObjectFlags.Clear(BaseGameObject::Options::eListAddFailed_Bit1);
    mGameObjectFlags.Clear(BaseGameObject::Options::eDead);
    mGameObjectFlags.Clear(BaseGameObject::Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    mGameObjectFlags.Clear(BaseGameObject::Options::eIsBaseAliveGameObject_Bit6);
    mGameObjectFlags.Clear(BaseGameObject::Options::eCanExplode_Bit7);
    mGameObjectFlags.Clear(BaseGameObject::Options::eInteractive_Bit8);
    mGameObjectFlags.Clear(BaseGameObject::Options::eSurviveDeathReset_Bit9);
    mGameObjectFlags.Clear(BaseGameObject::Options::eUpdateDuringCamSwap_Bit10);
    //field_6_flags.Clear(BaseGameObject::Options::eCantKill_Bit11); // NOTE: AE clears this too
    mGameObjectFlags.Set(BaseGameObject::Options::eUpdatable_Bit2);

    if (addToObjectList)
    {
        if (!gBaseGameObjects->Push_Back(this))
        {
            mGameObjectFlags.Set(Options::eListAddFailed_Bit1);
        }
    }
}

BaseGameObject::~BaseGameObject()
{
    Event_Cancel_For_Obj(this);
}

void BaseGameObject::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel
        || gMap.mCurrentPath != gMap.mPath
        || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
