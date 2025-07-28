#pragma once

#include "data_conversion/guid.hpp"
#include "FatalError.hpp"

class BaseGameObject;

using TObjectId_KeyType = Guid;


class BaseGameObject;
enum class ReliveTypes : s16;

class ObjectIds final
{
public:
    ~ObjectIds();
    ObjectIds();
    void Insert(TObjectId_KeyType objCount, BaseGameObject* pGameObj);
    s16 Remove(TObjectId_KeyType idToRemove);

    // Use the "checked" Find() so the types are verified where possible (i.e its to a specific single derived type
    BaseGameObject* Find_Impl(TObjectId_KeyType idToFind);

public:
    template <typename T>
    T* Find(TObjectId_KeyType idToFind, ReliveTypes type)
    {
        BaseGameObject* pItem = Find_Impl(idToFind);
        if (pItem && pItem->Type() != type)
        {
            ALIVE_FATAL("Expected type %d for object with id %s but got %d", static_cast<s32>(type), idToFind.ToString().c_str(), static_cast<s32>(pItem->Type()));
        }
        return static_cast<T*>(pItem);
    }

    BaseGameObject* Find(TObjectId_KeyType idToFind, ReliveTypes type)
    {
        return Find<BaseGameObject>(idToFind, type);
    }

private:
    std::map<TObjectId_KeyType, BaseGameObject*> mMap;
};

extern ObjectIds sObjectIds;
