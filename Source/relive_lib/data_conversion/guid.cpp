#include "stdafx.h"
#include "guid.hpp"
#include "../../AliveLibCommon/logger.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

static u32 nextPesudoRandomGuidValue = 1;

Guid Guid::NewGuidFromTlvInfo(u32 tlvInfo)
{
    Guid r;
    r.mGuid.mTlvInfoOrData1.mData = tlvInfo;
    return r;
}

Guid Guid::NewGuid()
{
    Guid r;

    r.mGuid.mTlvInfoOrData1.mData = 0xFFFFFFFF; // A TLVInfo can never be this, and it will never set data2 fields
    for (u32 i = 0; i < 3; i++)
    {
        r.mGuid.mData2[i] = nextPesudoRandomGuidValue++; // 4 billion * 4 values
    }

    return r;
}

Guid Guid::FromString(const std::string& str)
{
    Guid r;
    if (str.length() > 2 && str[0] == '{' && str[str.length() - 1] == '}')
    {
        // Iterate the data between the braces
        const char* pStart = &str[1];
        char* pEnd = nullptr;
        r.mGuid.mTlvInfoOrData1.mData = ::strtoul(pStart, &pEnd, 16);
        if (*pEnd != '-')
        {
            return {};
        }
        pStart = pEnd + 1;

        for (u32 i = 0; i < 3; i++)
        {
            r.mGuid.mData2[i] = ::strtoul(pStart, &pEnd, 16);
            if (*pEnd == '}')
            {
                return r;
            }

            if (*pEnd != '-')
            {
                return {};
            }
            pStart = pEnd + 1;
        }
    }

    return r;
}

std::string Guid::ToString() const
{
    char buffer[128] = {};
    ::sprintf(buffer, "{%08X-%08X-%08X-%08X}", mGuid.mTlvInfoOrData1.mData, mGuid.mData2[0], mGuid.mData2[1], mGuid.mData2[2]);
    return buffer;
}

bool Guid::IsValid() const
{
    // A "not defaut constructed" check (any components non zero)
    return mGuid.mTlvInfoOrData1.mData != 0 || mGuid.mData2[0] != 0 || mGuid.mData2[1] != 0 || mGuid.mData2[2];
}

bool Guid::operator < (const Guid& rhs) const
{
    return std::tie(mGuid.mTlvInfoOrData1.mData, mGuid.mData2[0], mGuid.mData2[1], mGuid.mData2[2]) < std::tie(rhs.mGuid.mTlvInfoOrData1.mData, rhs.mGuid.mData2[0], rhs.mGuid.mData2[1], rhs.mGuid.mData2[2]);
}

bool Guid::operator==(const Guid& rhs) const
{
    if (this != &rhs)
    {
        return mGuid.mTlvInfoOrData1.mData == rhs.mGuid.mTlvInfoOrData1.mData && 
            mGuid.mData2[0] == rhs.mGuid.mData2[0] &&
            mGuid.mData2[1] == rhs.mGuid.mData2[1] && 
            mGuid.mData2[2] == rhs.mGuid.mData2[2];
    }
    return true;
}

bool Guid::operator!=(const Guid& rhs) const
{
    return !(*this == rhs);
}
