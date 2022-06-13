#include "ResourceManagerWrapper.hpp"
#include "GameType.hpp"
#include "../AliveLibAE/ResourceManager.hpp"
#include "../AliveLibAO/ResourceManager.hpp"


s16 ResourceManagerWrapper::FreeResource(u8** ppRes)
{
    if (GetGameType() == GameType::eAe)
    {
        return ResourceManager::FreeResource_49C330(ppRes);
    }
    else
    {
        return AO::ResourceManager::FreeResource_455550(ppRes);
    }
}


u8** ResourceManagerWrapper::GetLoadedResource(u32 type, u32 resourceID, u16 addUseCount, u16 bLock)
{
    if (GetGameType() == GameType::eAe)
    {
        return ResourceManager::GetLoadedResource(type, resourceID, addUseCount, bLock);
    }
    else
    {
        return AO::ResourceManager::GetLoadedResource(type, resourceID, addUseCount, bLock);
    }
}
