#include "stdafx.h"
#include "ResourceManager.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x5C1BB0, ResourceManager*, pResourceManager_5C1BB0, nullptr);

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

signed __int16 __cdecl ResourceManager::LoadResourceFile_49C170(const char *pFileName, int a2)
{
    NOT_IMPLEMENTED();
    return 0;
}