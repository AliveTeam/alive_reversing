#pragma once

namespace relive
{
    class Path_TLV;
}

class Guid;

namespace AO {

class Map;

enum class LoadMode : s16
{
    ConstructObject_0 = 0,
    LoadResourceFromList_1 = 1,
    LoadResource_2 = 2
};


void ConstructTLVObject(relive::Path_TLV* pTlv, Map*, const Guid& tlvId, LoadMode);

} // namespace AO
