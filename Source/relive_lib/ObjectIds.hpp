#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../../relive_lib/data_conversion/guid.hpp"
#include <map> // TODO: consider our own lib

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
    BaseGameObject* Find(TObjectId_KeyType idToFind, ReliveTypes type);

private:
    std::map<TObjectId_KeyType, BaseGameObject*> mMap;
};

extern ObjectIds sObjectIds;
