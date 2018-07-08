#include "stdafx.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5C1BB0, ResourceManager*, pResourceManager_5C1BB0, nullptr);
ALIVE_VAR(1, 0xab4a04, int, sManagedMemoryUsedSize_AB4A04, 0);

void ResourceManager::VDestructor(signed int flags)
{
    dtor_4649B0(flags);
}

void ResourceManager::dtor_4649B0(signed int flags)
{
    dtor_4649E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void ResourceManager::dtor_4649E0()
{
    NOT_IMPLEMENTED();
}

ResourceManager::ResourceManager()
{
    Ctor_464910();
    SetVTable(this, 0x545EBC);
}

void ResourceManager::Ctor_464910()
{
    NOT_IMPLEMENTED();
}

void ResourceManager::Shutdown_465610()
{
    NOT_IMPLEMENTED();
}

void ResourceManager::sub_465590(int /*a1*/)
{
    NOT_IMPLEMENTED();
}

signed __int16 __cdecl ResourceManager::LoadResourceFile_49C170(const char* pFileName, Camera* a2)
{
    NOT_IMPLEMENTED();
    return 0;
}

void* CC ResourceManager::GetLoadedResource_49C2A0(DWORD type, int resourceID, unsigned __int16 addUseCount, __int16 a4)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

signed __int16 CC ResourceManager::FreeResource_49C330(BaseHandle handle)
{
    if (!handle.Valid())
    {
        return 1;
    }
    return FreeResource_Impl_49C360(handle.ToRawHandle());
}

signed __int16 CC ResourceManager::FreeResource_Impl_49C360(RawHandle handle)
{
    if (handle.Valid())
    {
        Header* pHeader = handle.GetHeader();
        if (pHeader->field_4_ref_count)
        {
            pHeader->field_4_ref_count--;
            if (pHeader->field_4_ref_count > 0)
            {
                return 0;
            }
            pHeader->field_8_type = Resource_Free;
            pHeader->field_6_flags = 0;
            sManagedMemoryUsedSize_AB4A04 -= pHeader->field_0_size;
        }
    }
    return 1;
}
