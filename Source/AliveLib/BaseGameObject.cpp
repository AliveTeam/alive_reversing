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

void BaseGameObject::VRender(int** /*pOrderingTable*/)
{
    // Empty 0x4DBF80
}

void BaseGameObject::VScreenChanged()
{
    ScreenChanged_4DC0A0();
}

void BaseGameObject::ScreenChanged_4DC0A0()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId
    || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId
    || (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710()))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void BaseGameObject::VStopAudio()
{
    // Empty 0x4DC0F0
}

int BaseGameObject::VGetSaveState(BYTE* /*pSaveBuffer*/)
{
    return 0;
}

BYTE** BaseGameObject::Add_Resource_4DC130(DWORD type, int resourceID)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(type, resourceID, 1, 0);
    if (ppRes)
    {
        field_10_resources_array.Push_Back_40CAF0(ppRes);
    }
    return ppRes;
}

void BaseGameObject::BaseGameObject_ctor_4DBFA0(__int16 bAddToObjectList, signed __int16 resourceArraySize)
{
    SetVTable(this, 0x547AC4);

    field_10_resources_array.ctor_40C9E0(resourceArraySize);
    field_1C_update_delay = 0;
    field_4_typeId = Types::eNone_0;
    field_6_flags.Raw().all = field_6_flags.Raw().all & 0xF80A | BaseGameObject::eUpdatable; // TODO: Refactor

    if (bAddToObjectList)
    {
        if (!gBaseGameObject_list_BB47C4->Push_Back(this))
        {
            field_6_flags.Set(BaseGameObject::eListAddFailed);
        }
    }

    field_C_objectId = sAccumulatedObjectCount_5C1BF4;
    field_8_object_id = sAccumulatedObjectCount_5C1BF4;
    const int nextId = sAccumulatedObjectCount_5C1BF4++;
    sObjectIds_5C1B70.Insert_449C10(nextId, this);
}

EXPORT void BaseGameObject::BaseGameObject_dtor_4DBEC0()
{
    SetVTable(this, 0x547AC4); // gVtbl_BaseGameObject_547AC4
    Event_Cancel_For_Obj_422DF0(this);

    for (int i = 0; i < field_10_resources_array.Size(); i++)
    {
        if (field_10_resources_array.ItemAt(i))
        {
            ResourceManager::FreeResource_49C330(field_10_resources_array.ItemAt(i));
        }
    }

    sObjectIds_5C1B70.Remove_449C60(field_8_object_id);

    field_10_resources_array.dtor_40CAD0();
}

EXPORT int CCSTD BaseGameObject::Find_Flags_4DC170(int objectId)
{
    if (objectId != -1)
    {
        for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
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
