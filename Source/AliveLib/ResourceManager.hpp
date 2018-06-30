#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class ResourceManager : public BaseGameObject
{
public:
    // TODO
    virtual void VDestructor(signed int);
    EXPORT void dtor_4649B0(signed int flags);
    EXPORT void dtor_4649E0();

    ResourceManager();
    EXPORT void Ctor_464910();

    EXPORT void Shutdown_465610();
    EXPORT void sub_465590(int a1);
    EXPORT static signed __int16 __cdecl LoadResourceFile_49C170(const char *pFileName, int a2);
};

// TODO: Object size

ALIVE_VAR_EXTERN(ResourceManager*, pResourceManager_5C1BB0);
