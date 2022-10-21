#include "stdafx.h"
#include "BaseGameObject.hpp"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Map.hpp"
#include "QuikSave.hpp"
#include "ObjectIds.hpp"

ALIVE_VAR(1, 0xBB47C4, DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_BB47C4, nullptr);

void BaseGameObject::VUpdate()
{
    // Empty 0x4DC080
}

void BaseGameObject::VRender(PrimHeader** /*ppOt*/)
{
    // Empty 0x4DBF80
}

void BaseGameObject::VScreenChanged()
{
    ScreenChanged_4DC0A0();
}

void BaseGameObject::ScreenChanged_4DC0A0()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void BaseGameObject::VStopAudio()
{
    // Empty 0x4DC0F0
}

s32 BaseGameObject::VGetSaveState(u8* /*pSaveBuffer*/)
{
    return 0;
}

u8** BaseGameObject::Add_Resource_4DC130(u32 type, s32 resourceID)
{
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(type, resourceID, 1, 0);
    if (ppRes)
    {
        field_10_resources_array.Push_Back_40CAF0(ppRes);
    }
    return ppRes;
}

void BaseGameObject::BaseGameObject_ctor_4DBFA0(s16 bAddToObjectList, s16 resourceArraySize)
{
    SetVTable(this, 0x547AC4);

    field_10_resources_array.ctor_40C9E0(resourceArraySize);
    field_1C_update_delay = 0;
    SetType(AETypes::eNone_0);
    field_6_flags.Clear(BaseGameObject::Options::eListAddFailed_Bit1);
    field_6_flags.Clear(BaseGameObject::Options::eDead_Bit3);
    field_6_flags.Clear(BaseGameObject::Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    field_6_flags.Clear(BaseGameObject::Options::eIsBaseAliveGameObject_Bit6);
    field_6_flags.Clear(BaseGameObject::Options::eCanExplode_Bit7);
    field_6_flags.Clear(BaseGameObject::Options::eInteractive_Bit8);
    field_6_flags.Clear(BaseGameObject::Options::eSurviveDeathReset_Bit9);
    field_6_flags.Clear(BaseGameObject::Options::eUpdateDuringCamSwap_Bit10);
    field_6_flags.Clear(BaseGameObject::Options::eCantKill_Bit11);
    field_6_flags.Set(BaseGameObject::eUpdatable_Bit2);

    if (bAddToObjectList)
    {
        if (!gBaseGameObject_list_BB47C4->Push_Back(this))
        {
            field_6_flags.Set(BaseGameObject::eListAddFailed_Bit1);
        }
    }

    s32 nextId = sObjectIds_5C1B70.EnsureIdIsUnique(sAccumulatedObjectCount_5C1BF4);
    field_C_objectId = nextId;
    field_8_object_id = nextId;
    sObjectIds_5C1B70.Insert_449C10(nextId, this);

    sAccumulatedObjectCount_5C1BF4 = ++nextId;
}

EXPORT void BaseGameObject::BaseGameObject_dtor_4DBEC0()
{
    SetVTable(this, 0x547AC4); // gVtbl_BaseGameObject_547AC4
    Event_Cancel_For_Obj_422DF0(this);

    for (s32 i = 0; i < field_10_resources_array.Size(); i++)
    {
        if (field_10_resources_array.ItemAt(i))
        {
            ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(i));
        }
    }

    sObjectIds_5C1B70.Remove_449C60(field_8_object_id);

    field_10_resources_array.dtor_40CAD0();
}

EXPORT s32 CCSTD BaseGameObject::Find_Flags_4DC170(s32 objectId)
{
    if (objectId != -1)
    {
        for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
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
