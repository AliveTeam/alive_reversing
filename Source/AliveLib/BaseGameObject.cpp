#include "stdafx.h"
#include "BaseGameObject.hpp"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0xBB47C4, DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_BB47C4, nullptr);

void BaseGameObject::VUpdate()
{
    // Empty 0x4DC080
}

void BaseGameObject::VRender(int** /*pOrderingTable*/)
{
    // Empty 0x4DBF80
}

void BaseGameObject::vsub_4DC0A0()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId
    || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId
    || (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710()))
    {
        field_6_flags |= BaseGameObject::eDead;
    }
}

void BaseGameObject::vnullsub_4DC0F0()
{
    // Empty 0x4DC0F0
}

int BaseGameObject::GetSaveState_4DC110(BYTE* /*pSaveBuffer*/)
{
    return 0;
}

ALIVE_VAR(1, 0x5C1BF4, int, sAccumulatedObjectCount_5C1BF4, 0);

ALIVE_VAR(1, 0x5C1B70, ObjectIds, sObjectIds_5C1B70, {});


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
    field_4_typeId = Types::eNone;
    field_6_flags = field_6_flags & 0xF80A | BaseGameObject::eUpdatable; // TODO: To enum

    if (bAddToObjectList)
    {
        if (!gBaseGameObject_list_BB47C4->Push_Back(this))
        {
            field_6_flags |= BaseGameObject::eListAddFailed;
        }
    }

    field_C_objectId = sAccumulatedObjectCount_5C1BF4;
    field_8_flags_ex = sAccumulatedObjectCount_5C1BF4;
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

    sObjectIds_5C1B70.Remove_449C60(field_8_flags_ex);

    field_10_resources_array.dtor_40CAD0();
}

EXPORT void ObjectIds::Insert_449C10(int /*objCount*/, void* /*pGameObj*/)
{
    NOT_IMPLEMENTED();
}

signed __int16 ObjectIds::Remove_449C60(int /*id*/)
{
    NOT_IMPLEMENTED();
    return 0;
}