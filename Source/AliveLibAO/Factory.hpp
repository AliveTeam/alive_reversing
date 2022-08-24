#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

struct Path_TLV;
class Map;

struct TlvOffsetLevelIdPathId final
{
    u16 tlvOffset;
    u8 levelId;
    u8 pathId;
};

struct TlvOffsetCombinedLevelIdPathId final
{
    u16 tlvOffset;
    u16 levelIdPathId;
};

union TlvItemInfoUnion
{
    u32 all;
    TlvOffsetCombinedLevelIdPathId combined;
    TlvOffsetLevelIdPathId parts;
};
ALIVE_ASSERT_SIZEOF(TlvItemInfoUnion, 4);

enum class LoadMode : s16
{
    ConstructObject_0 = 0,
    LoadResourceFromList_1 = 1,
    LoadResource_2 = 2
};

using TPathFunctionFn = void(CC*)(relive::Path_TLV*, Map*, TlvItemInfoUnion, LoadMode);

struct PathFunctionTable final
{
    TPathFunctionFn object_funcs[256];
};

extern const PathFunctionTable kObjectFactory;


void ConstructTLVObject(relive::Path_TLV* pTlv, Map*, TlvItemInfoUnion, LoadMode);

} // namespace AO
