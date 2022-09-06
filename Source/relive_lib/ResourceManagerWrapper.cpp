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

u8** ResourceManagerWrapper::Alloc_New_Resource(u32 type, u32 id, u32 size)
{
    if (GetGameType() == GameType::eAe)
    {
        return ResourceManager::Alloc_New_Resource_49BED0(type, id, size);
    }
    else
    {
        return AO::ResourceManager::Alloc_New_Resource_454F20(type, id, size);
    }
}

void ResourceManagerWrapper::Inc_Ref_Count(u8** ppRes)
{
    Get_Header(ppRes)->field_4_ref_count++;
}

ResourceManagerWrapper::Header* ResourceManagerWrapper::Get_Header(u8** ppRes)
{
    return reinterpret_cast<Header*>((*ppRes - sizeof(Header)));
}
