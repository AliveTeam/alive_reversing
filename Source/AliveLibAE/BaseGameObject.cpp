#include "stdafx.h"
#include "BaseGameObject.hpp"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Map.hpp"
#include "QuikSave.hpp"
#include "ObjectIds.hpp"

ALIVE_VAR(1, 0xBB47C4, DynamicArrayT<BaseGameObject>*, gBaseGameObjects, nullptr);

BaseGameObject::BaseGameObject(s16 bAddToObjectList, s16 resourceArraySize)
    : field_10_resources_array(resourceArraySize)
{
    // Pre-allocate the array with nullptr entries so that it can be
    // used like a fixed size array in the derived type.
    // TODO: If its never used dynamically in the derived type then it actually can become a fixed
    // compile time sized array at some point.
    for (s32 i = 0; i < resourceArraySize; i++)
    {
        field_10_resources_array.Push_Back(nullptr);
    }

    mUpdateDelay = 0;

    SetType(AETypes::eNone_0);
    
    mGameObjectFlags.Clear(BaseGameObject::Options::eListAddFailed_Bit1);
    mGameObjectFlags.Clear(BaseGameObject::Options::eDead);
    mGameObjectFlags.Clear(BaseGameObject::Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    mGameObjectFlags.Clear(BaseGameObject::Options::eIsBaseAliveGameObject_Bit6);
    mGameObjectFlags.Clear(BaseGameObject::Options::eCanExplode_Bit7);
    mGameObjectFlags.Clear(BaseGameObject::Options::eInteractive_Bit8);
    mGameObjectFlags.Clear(BaseGameObject::Options::eSurviveDeathReset_Bit9);
    mGameObjectFlags.Clear(BaseGameObject::Options::eUpdateDuringCamSwap_Bit10);
    mGameObjectFlags.Clear(BaseGameObject::Options::eCantKill_Bit11);
    mGameObjectFlags.Set(BaseGameObject::eUpdatable_Bit2);

    if (bAddToObjectList)
    {
        if (!gBaseGameObjects->Push_Back(this))
        {
            mGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
        }
    }

    s32 nextId = sObjectIds.EnsureIdIsUnique(sAccumulatedObjectCount_5C1BF4);
    field_C_objectId = nextId;
    field_8_object_id = nextId;
    sObjectIds.Insert(nextId, this);

    sAccumulatedObjectCount_5C1BF4 = ++nextId;
}

BaseGameObject::~BaseGameObject()
{
    Event_Cancel_For_Obj(this);

    for (s32 i = 0; i < field_10_resources_array.Size(); i++)
    {
        if (field_10_resources_array.ItemAt(i))
        {
            ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(i));
        }
    }

    sObjectIds.Remove(field_8_object_id);
}

void BaseGameObject::VUpdate()
{
    // Empty 0x4DC080
}

void BaseGameObject::VRender(PrimHeader** /*ppOt*/)
{
    // Empty 0x4DBF80
}


void BaseGameObject::VStopAudio()
{
    // Empty 0x4DC0F0
}

s32 BaseGameObject::VGetSaveState(u8* /*pSaveBuffer*/)
{
    return 0;
}

u8** BaseGameObject::Add_Resource(u32 type, s32 resourceID)
{
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(type, resourceID, 1, 0);
    if (ppRes)
    {
        field_10_resources_array.Push_Back(ppRes);
    }
    return ppRes;
}

s32 BaseGameObject::RefreshId(s32 objectId)
{
    if (objectId != -1)
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_C_objectId == objectId)
            {
                return pObj->field_8_object_id;
            }
        }
    }
    return -1;
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
