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
    field_1C_update_delay = 0;

    SetType(AETypes::eNone_0);
    
    mFlags.Clear(BaseGameObject::Options::eListAddFailed_Bit1);
    mFlags.Clear(BaseGameObject::Options::eDead);
    mFlags.Clear(BaseGameObject::Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    mFlags.Clear(BaseGameObject::Options::eIsBaseAliveGameObject_Bit6);
    mFlags.Clear(BaseGameObject::Options::eCanExplode_Bit7);
    mFlags.Clear(BaseGameObject::Options::eInteractive_Bit8);
    mFlags.Clear(BaseGameObject::Options::eSurviveDeathReset_Bit9);
    mFlags.Clear(BaseGameObject::Options::eUpdateDuringCamSwap_Bit10);
    mFlags.Clear(BaseGameObject::Options::eCantKill_Bit11);
    mFlags.Set(BaseGameObject::eUpdatable_Bit2);

    if (bAddToObjectList)
    {
        if (!gBaseGameObjects->Push_Back(this))
        {
            mFlags.Set(BaseGameObject::eListAddFailed_Bit1);
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

    sObjectIds.Remove_449C60(field_8_object_id);
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
        field_10_resources_array.Push_Back_40CAF0(ppRes);
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
        mFlags.Set(BaseGameObject::eDead);
    }
}
