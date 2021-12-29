#include "stdafx.h"
#include "ObjectIds.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Sys_common.hpp"

ALIVE_VAR(1, 0x5C1B70, ObjectIds, sObjectIds_5C1B70, {});

// static_'s are called before main
EXPORT void CC static_ObjectIds_ctor_43EC50()
{
    sObjectIds_5C1B70.ctor_449AE0(101);
}

EXPORT void CC static_ObjectIds_init_43EC30()
{
    static_ObjectIds_ctor_43EC50();
    atexit(ObjectIds::dtor_43EC90);
}

void ObjectIds::dtor_43EC90()
{
    sObjectIds_5C1B70.Destructor();
}

void ObjectIds::Destructor()
{
    for (u32 i = 0; i < field_0_buffer_size; i++)
    {
        while (field_4_pBuffer[i])
        {
            // Keep a copy of the current item
            ObjectId_Record* pCurrent = field_4_pBuffer[i];

            // Set the array to point to the next item
            field_4_pBuffer[i] = pCurrent->field_8_pNext;

            // Now free current, repeat until everything is gone
            ae_delete_free_495540(pCurrent);
        }
    }

    // Free the backing array
    ae_non_zero_free_495560(field_4_pBuffer);
}

void ObjectIds::ctor_449AE0(u32 size)
{
    // Allocate the array and set all items to nullptr
    field_0_buffer_size = size;
    field_4_pBuffer = reinterpret_cast<ObjectId_Record**>(ae_malloc_non_zero_4954F0(sizeof(ObjectId_Record*) * size));
    for (u32 i = 0; i < field_0_buffer_size; i++)
    {
        field_4_pBuffer[i] = nullptr;
    }
}

u32 ObjectIds::Id_To_Buffer_Size_Range_449BA0(TObjectId_KeyType id)
{
    return id % field_0_buffer_size;
}

ObjectId_Record* ObjectIds::Find_By_Id_449BC0(TObjectId_KeyType idToFind, ObjectId_Record** ppLastMatch)
{
    ObjectId_Record* pRecord = field_4_pBuffer[Id_To_Buffer_Size_Range_449BA0(idToFind)];
    *ppLastMatch = nullptr;

    while (pRecord)
    {
        if (pRecord->field_0_id == idToFind)
        {
            return pRecord;
        }

        // Keep track of what it was so we can fix links when removing found items
        *ppLastMatch = pRecord;

        // Go to the next record
        pRecord = pRecord->field_8_pNext;
    }
    return nullptr;
}

void ObjectIds::Insert_449C10(TObjectId_KeyType nextId, BaseGameObject* pGameObj)
{
    // Create new record
    ObjectId_Record* pRec = ae_new<ObjectId_Record>();
    pRec->field_0_id = nextId;
    pRec->field_4_obj_ptr = pGameObj;

    // Insert and fix links
    const u32 id = Id_To_Buffer_Size_Range_449BA0(nextId);

    pRec->field_8_pNext = field_4_pBuffer[id];
    field_4_pBuffer[id] = pRec;
}

s16 ObjectIds::Remove_449C60(TObjectId_KeyType idToRemove)
{
    // Find the record
    ObjectId_Record* pLastMatch = nullptr;
    ObjectId_Record* pFound = Find_By_Id_449BC0(idToRemove, &pLastMatch);
    if (!pFound)
    {
        return 0;
    }

    // Fix the links
    const u32 idx = Id_To_Buffer_Size_Range_449BA0(idToRemove);
    if (pLastMatch)
    {
        // There was an object before this, so point to the one after what we found
        // so we can remove it.
        pLastMatch->field_8_pNext = pFound->field_8_pNext;
    }
    else
    {
        // There was not an object before this, so set the root to point the one after
        // we found so we can remove it.
        field_4_pBuffer[idx] = pFound->field_8_pNext;
    }

    // Free the found record
    ae_delete_free_495540(pFound);

    return 1;
}

BaseGameObject* ObjectIds::Find_449CF0(TObjectId_KeyType idToFind)
{
    BaseGameObject* pFound = nullptr;
    if (idToFind != -1)
    {
        ObjectId_Record* tmp = nullptr;
        ObjectId_Record* pRecord = Find_By_Id_449BC0(idToFind, &tmp);
        if (pRecord)
        {
            pFound = pRecord->field_4_obj_ptr;
        }
    }
    return pFound;
}

#include "BaseGameObject.hpp"

BaseGameObject* ObjectIds::Find(TObjectId_KeyType idToFind, AETypes type)
{
    BaseGameObject* pItem = Find_449CF0(idToFind);
    if (pItem && pItem->Type() != type)
    {
        LOG_ERROR("Expected type " << static_cast<s32>(type) << " for object with id " << idToFind << " but got " << static_cast<s32>(pItem->Type()));
        ALIVE_FATAL("Wrong type!");
    }
    return pItem;
}

#include <gmock/gmock.h>

namespace AETest::TestsObjectIds {
class FakeGameObject final : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32) override
    {
        return this;
    }
};

void ObjectIdsTests()
{
    ObjectIds ids;
    ids.ctor_449AE0(101);

    FakeGameObject p;
    p.SetType(AETypes::eAlarm_1);
    ids.Insert_449C10(1, &p);

    ASSERT_EQ(&p, ids.Find(1, AETypes::eAlarm_1));

    FakeGameObject p2;
    p2.SetType(AETypes::eAlarm_1);
    ids.Insert_449C10(1, &p2);

    ASSERT_EQ(&p2, ids.Find(1, AETypes::eAlarm_1));

    ids.Remove_449C60(1);

    ASSERT_EQ(&p, ids.Find(1, AETypes::eAlarm_1));

    ids.Remove_449C60(1);

    ASSERT_EQ(nullptr, ids.Find(1, AETypes::eAlarm_1));

    ASSERT_EQ(nullptr, ids.Find(9999, AETypes::eAlarm_1));

    for (s32 i = 0; i < 3000; i++)
    {
        ids.Insert_449C10(i, &p);
    }

    for (s32 i = 0; i < 3000; i++)
    {
        ids.Remove_449C60(i);
    }

    ids.Destructor();
}
} // namespace AETest::TestsObjectIds
