#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class BaseAliveGameObject;

class ResourceManager
{
public:
    enum ResourceType : DWORD
    {
        Resource_Animation = 0x6D696E41,
        Resource_Font = 0x746E6F46,
    };

    static EXPORT void CC CancelPendingResourcesFor_41EA60(BaseAliveGameObject* pObj);

    static EXPORT void CC Reclaim_Memory_455660(DWORD sizeToReclaim);

    static EXPORT __int16 CC FreeResource_455550(BYTE** ppRes);

    static EXPORT void CC LoadResource_446C90(const char* pFileName, int type, int resourceId, __int16 loadMode, __int16 bDontLoad = FALSE);
};

END_NS_AO
