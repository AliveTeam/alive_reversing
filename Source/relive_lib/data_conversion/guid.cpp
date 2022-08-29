#include "guid.hpp"
#include "../../AliveLibCommon/logger.hpp"
#include "../../AliveLibCommon/Sys_common.hpp"

#ifdef _WIN32

#include <windows.h>


Guid Guid::NewGuid()
{
    Guid r;
    UUID uuid = {};
    const RPC_STATUS err = ::UuidCreate(&uuid);
    if (err != RPC_S_OK)
    {
        LOG_ERROR("UuidCreate failed " << err);
        ALIVE_FATAL("UuidCreate failed");
    }

    r.mGuid.Data1 = uuid.Data1;
    r.mGuid.Data2 = uuid.Data2;
    r.mGuid.Data3 = uuid.Data3;
    for (u32 i = 0; i < 8; i++)
    {
        r.mGuid.Data4[i] = uuid.Data4[i];
    }
    return r;
}

Guid Guid::FromString(const std::string& /*str*/)
{
    // TODO
    Guid r;
    return r;
}

std::string Guid::ToString() const
{
    // TODO
    return "{dead-beef}";
}

bool Guid::IsValid() const
{
    // TODO
    return true;
}


bool Guid::operator < (const Guid& rhs) const
{
    return mGuid.Data1 < rhs.mGuid.Data1; // TODO: check all fields
}

bool Guid::operator==(const Guid& rhs) const
{
    if (this != &rhs)
    {
        return mGuid.Data1 == rhs.mGuid.Data1; // TODO: Check em all
    }
    return true;
}

bool Guid::operator!=(const Guid& rhs) const
{
    return !(*this == rhs);
}



#else
// TODO: Mac/Linux guid generation
#endif
