#pragma once

#include "../../AliveLibCommon/Types.hpp"

class Guid final
{
public:
    static Guid NewGuidFromTlvInfo(u32 tlvInfo);
    static Guid NewGuid();
    static Guid FromString(const std::string& str);
    std::string ToString() const;
    bool IsValid() const;
    bool operator<(const Guid& rhs) const;
    bool operator==(const Guid& rhs) const;
    bool operator!=(const Guid& rhs) const;

    struct TlvOffsetLevelIdPathId final
    {
        u16 tlvOffset;
        u8 levelId;
        u8 pathId;
    };


private:

    struct GuidInternal final
    {
        // 128 bit globally unique number (provided it was generated via the correct API)
        union TlvInfoOrGuid
        {
            // When converted from an OG lvl we keep the original TLV info in the first 32 bits
            // as this allows us to convert saves as they containing TLV info records
            TlvOffsetLevelIdPathId mTlvInfo;
            u32 mData;
        };
        TlvInfoOrGuid mTlvInfoOrData1; // first 32 bits
        u32 mData2[3]; // remaining 96 bits
    };
    GuidInternal mGuid = {};

};
