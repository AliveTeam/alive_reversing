#include "stdafx_ao.h"
#include "ResourceManager.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5076A0, short, bHideLoadingIcon_5076A0, 0);
ALIVE_VAR(1, 0x5076A4, int, loading_ticks_5076A4, 0);

void CC ResourceManager::CancelPendingResourcesFor_41EA60(BaseGameObject* /*pObj*/)
{
    NOT_IMPLEMENTED();
}

void CC ResourceManager::Reclaim_Memory_455660(DWORD /*sizeToReclaim*/)
{
    NOT_IMPLEMENTED();
}

__int16 CC ResourceManager::FreeResource_455550(BYTE** /*ppRes*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

void CC ResourceManager::LoadResource_446C90(const char* /*pFileName*/, int /*type*/, int /*resourceId*/, __int16 /*loadMode*/, __int16 /*bDontLoad*/)
{
    NOT_IMPLEMENTED();
}

ResourceManager::Header* CC ResourceManager::Get_Header_455620(BYTE** ppRes)
{
    return reinterpret_cast<Header*>((*ppRes - sizeof(Header)));
}

EXPORT void CC ResourceManager::Free_Resources_For_Camera_447170(Camera* /*pCamera*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC ResourceManager::Free_Resource_Of_Type_455810(int /*type*/)
{
    NOT_IMPLEMENTED();
}

EXPORT __int16 CC ResourceManager::LoadResourceFile_455270(const char* /*filename*/, Camera* /*pCam*/, int /*allocMethod*/)
{
    NOT_IMPLEMENTED();
    return 0;
}


EXPORT ResourceManager::ResourceManager_FileRecord* CC ResourceManager::LoadResourceFile_4551E0(const char* /*pFileName*/, TLoaderFn /*fnOnLoad*/, Camera* /*pCamera1*/, Camera* /*pCamera2*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT void CC ResourceManager::LoadingLoop_41EAD0(__int16 /*bShowLoadingIcon*/)
{
    NOT_IMPLEMENTED();
}

BYTE** CC ResourceManager::GetLoadedResource_4554F0(int /*type*/, int /*resourceId*/, __int16 /*addUseCount*/, __int16 /*bLock*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void CC Game_ShowLoadingIcon_445EB0()
{
    NOT_IMPLEMENTED();
}

void CC ResourceManager::LoadResourcesFromList_446E80(const char* /*pFileName*/, ResourcesToLoadList* /*list*/, __int16 /*loadMode*/, __int16)
{
    NOT_IMPLEMENTED();
}

END_NS_AO



