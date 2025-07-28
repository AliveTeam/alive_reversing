#include "stdafx.h"
#include "ObjectIds.hpp"
#include "GameObjects/BaseGameObject.hpp"
#include "FatalError.hpp"

ObjectIds sObjectIds;

ObjectIds::~ObjectIds()
{

}

ObjectIds::ObjectIds()
{

}

void ObjectIds::Insert(TObjectId_KeyType nextId, BaseGameObject* pGameObj)
{
    if (!nextId.IsValid())
    {
        ALIVE_FATAL("Inserting invalid guid");
    }

    mMap[nextId] = pGameObj;
}

s16 ObjectIds::Remove(TObjectId_KeyType idToRemove)
{
    auto it = mMap.find(idToRemove);
    if (it != std::end(mMap))
    {
        mMap.erase(it);
        return 1;
    }
    return 0;
}

BaseGameObject* ObjectIds::Find_Impl(TObjectId_KeyType idToFind)
{
    if (!idToFind.IsValid())
    {
        return nullptr;
    }

    auto it = mMap.find(idToFind);
    if (it != std::end(mMap))
    {
        return it->second;
    }
    return nullptr;
}
