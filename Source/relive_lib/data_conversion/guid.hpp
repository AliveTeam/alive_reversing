#pragma once

#include "../../AliveLibCommon/Types.hpp"
#include <string>

class Guid final
{
public:
    static Guid NewGuid();
    static Guid FromString(const std::string& str);
    std::string ToString() const;
    bool IsValid() const;
    bool operator<(const Guid& rhs) const;
    bool operator==(const Guid& rhs) const;
    bool operator!=(const Guid& rhs) const;

private:
    struct GuidInternal final
    {
        u32 Data1;
        u16 Data2;
        u16 Data3;
        u8 Data4[8];
    };
    GuidInternal mGuid = {};

};
