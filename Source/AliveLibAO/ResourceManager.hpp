#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class BaseAliveGameObject;

class ResourceManager
{
public:
    static EXPORT void CC CancelPendingResourcesFor_41EA60(BaseAliveGameObject* pObj);

    static EXPORT void CC Reclaim_Memory_455660(DWORD sizeToReclaim);

    static EXPORT __int16 CC FreeResource_455550(BYTE** ppRes);
};

END_NS_AO
