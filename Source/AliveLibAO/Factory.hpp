#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

struct Path_TLV;
class Map;

struct TLVUniqueId final
{
    EReliveLevelIds levelId;
    s16 pathId;
    u16 camX;
    u16 camY;
    u32 tlvOffset;

    bool IsValid() const
    {
        return tlvOffset != 0;
    }
};

enum class LoadMode : s16
{
    ConstructObject_0 = 0,
    LoadResourceFromList_1 = 1,
    LoadResource_2 = 2
};


void ConstructTLVObject(relive::Path_TLV* pTlv, Map*, const TLVUniqueId& tlvId, LoadMode);

} // namespace AO
