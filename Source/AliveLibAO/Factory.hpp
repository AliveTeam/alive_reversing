#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

struct Path_TLV;
class Map;

struct TLVUniqueId final
{
    EReliveLevelIds levelId = EReliveLevelIds::eNone;
    s16 pathId = -1;
    u16 camX = 0;
    u16 camY = 0;
    u32 tlvOffset = 0;

    bool IsValid() const
    {
        return levelId != EReliveLevelIds::eNone && pathId != -1;
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
