#include "stdafx_ao.h"
#include "ResourceManager.hpp"
#include "Function.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5076A0, short, bHideLoadingIcon_5076A0, 0);
ALIVE_VAR(1, 0x5076A4, int, loading_ticks_5076A4, 0);


void CC ResourceManager::CancelPendingResourcesFor_41EA60(BaseAliveGameObject* /*pObj*/)
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
}

void CC ResourceManager::LoadResource_446C90(const char* /*pFileName*/, int /*type*/, int /*resourceId*/, __int16 /*loadMode*/, __int16 /*bDontLoad*/)
{
    NOT_IMPLEMENTED();
}


EXPORT void CC AO::ResourceManager::Free_Resources_For_Camera_447170(Camera* /*pCamera*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC AO::ResourceManager::Free_Resource_Of_Type_455810(int /*type*/)
{
    NOT_IMPLEMENTED();
}

EXPORT __int16 CC AO::ResourceManager::LoadResourceFile_455270(const char* /*filename*/, Camera* /*pCam*/, int /*allocMethod*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

EXPORT void CC AO::ResourceManager::LoadingLoop_41EAD0(__int16 /*bShowLoadingIcon*/)
{
    NOT_IMPLEMENTED();
}

BYTE** CC AO::ResourceManager::GetLoadedResource_4554F0(int /*type*/, int /*resourceId*/, __int16 /*addUseCount*/, __int16 /*bLock*/)
{
    NOT_IMPLEMENTED();
}

void CC Game_ShowLoadingIcon_445EB0()
{
    NOT_IMPLEMENTED();
}

END_NS_AO



