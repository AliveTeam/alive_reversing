#pragma once

#include "../AliveLibCommon/Types.hpp"

// Temp adapter interface
class ResourceManagerWrapper
{
public:
    static s16 FreeResource(u8** ppRes);
    static u8** GetLoadedResource(u32 type, u32 resourceID, u16 addUseCount, u16 bLock);
};
