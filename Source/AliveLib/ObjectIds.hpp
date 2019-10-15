#pragma once

#include "FunctionFwd.hpp"

class BaseGameObject;

EXPORT void CC static_ObjectIds_init_43EC30();

using TObjectId_KeyType = int;

struct ObjectId_Record
{
    TObjectId_KeyType field_0_id;
    BaseGameObject* field_4_obj_ptr;
    struct ObjectId_Record* field_8_pNext;
};
ALIVE_ASSERT_SIZEOF(ObjectId_Record, 0xC);

class BaseGameObject;
enum class Types : __int16;

class ObjectIds
{
public:
    void Destructor();
    EXPORT static void dtor_43EC90();
    EXPORT void ctor_449AE0(unsigned int size);
    EXPORT unsigned int Id_To_Buffer_Size_Range_449BA0(TObjectId_KeyType id);
    EXPORT ObjectId_Record* Find_By_Id_449BC0(TObjectId_KeyType idToFind, ObjectId_Record** ppLastMatch);
    EXPORT void Insert_449C10(TObjectId_KeyType objCount, BaseGameObject* pGameObj);
    EXPORT signed __int16 Remove_449C60(TObjectId_KeyType idToRemove);

    // Use the "checked" Find() so the types are verified where possible (i.e its to a specific single derived type
    EXPORT BaseGameObject* Find_449CF0(TObjectId_KeyType idToFind);
public:
    BaseGameObject* Find(TObjectId_KeyType idToFind, Types type);

private:
    unsigned int field_0_buffer_size;
    ObjectId_Record** field_4_pBuffer;
};
ALIVE_ASSERT_SIZEOF(ObjectIds, 0x8);

ALIVE_VAR_EXTERN(ObjectIds, sObjectIds_5C1B70);

namespace Test
{
    void ObjectIdsTests();
}