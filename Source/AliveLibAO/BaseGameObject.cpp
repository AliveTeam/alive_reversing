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
    field_8_update_delay = 0;
    field_C_refCount = 0;

    field_4_typeId = Types::eNone_0;

    mFlags.Clear(BaseGameObject::Options::eListAddFailed_Bit1);
    mFlags.Clear(BaseGameObject::Options::eDead);
    mFlags.Clear(BaseGameObject::Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    mFlags.Clear(BaseGameObject::Options::eIsBaseAliveGameObject_Bit6);
    mFlags.Clear(BaseGameObject::Options::eCanExplode_Bit7);
    mFlags.Clear(BaseGameObject::Options::eInteractive_Bit8);
    mFlags.Clear(BaseGameObject::Options::eSurviveDeathReset_Bit9);
    mFlags.Clear(BaseGameObject::Options::eUpdateDuringCamSwap_Bit10);
    //field_6_flags.Clear(BaseGameObject::Options::eCantKill_Bit11); // NOTE: AE clears this too
    mFlags.Set(BaseGameObject::Options::eUpdatable_Bit2);

    if (addToObjectList)
    {
        if (!gBaseGameObjects->Push_Back(this))
        {
            mFlags.Set(Options::eListAddFailed_Bit1);
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
        mFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
