#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class ResourceManager : public BaseGameObject
{
public:
    // TODO

    EXPORT void Shutdown_465610();
    EXPORT void sub_465590(int a1);
    EXPORT static signed __int16 __cdecl LoadResourceFile_49C170(const char *pFileName, int a2);
};

// TODO: Object size

ALIVE_VAR_EXTERN(ResourceManager*, pResourceManager_5C1BB0);
