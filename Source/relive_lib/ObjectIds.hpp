#pragma once

#include "../AliveLibCommon/Function.hpp"

class BaseGameObject;

using TObjectId_KeyType = s32;

struct ObjectId_Record final
{
    TObjectId_KeyType field_0_id = 0;
    BaseGameObject* field_4_obj_ptr = nullptr;
    struct ObjectId_Record* field_8_pNext = nullptr;
};

class BaseGameObject;
enum class AETypes : s16;
enum class Types : s16; // AO types
enum class ReliveTypes : s16;

class ObjectIds final
{
public:
    ~ObjectIds();
    explicit ObjectIds(u32 size);
    u32 Id_To_Buffer_Size_Range(TObjectId_KeyType id);
    ObjectId_Record* Find_By_Id(TObjectId_KeyType idToFind, ObjectId_Record** ppLastMatch);
    void Insert(TObjectId_KeyType objCount, BaseGameObject* pGameObj);
    s16 Remove(TObjectId_KeyType idToRemove);

    // Use the "checked" Find() so the types are verified where possible (i.e its to a specific single derived type
    BaseGameObject* Find_Impl(TObjectId_KeyType idToFind);

public:
    BaseGameObject* Find(TObjectId_KeyType idToFind, AETypes type);
    s32 EnsureIdIsUnique(s32 nextId);
    s32 GetHighestUsedId();

private:
    u32 field_0_buffer_size = 0;
    ObjectId_Record** field_4_pBuffer = nullptr;
};

extern ObjectIds sObjectIds;
