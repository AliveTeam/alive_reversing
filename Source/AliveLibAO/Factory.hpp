#pragma once

#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

struct Path_TLV;
class Map;

enum class LoadMode : s16
{
    ConstructObject_0 = 0,
    LoadResourceFromList_1 = 1,
    LoadResource_2 = 2
};


void ConstructTLVObject(relive::Path_TLV* pTlv, Map*, const Guid& tlvId, LoadMode);

} // namespace AO
