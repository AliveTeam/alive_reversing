#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class Path;

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

void ConstructTLVObject(relive::Path_TLV* pTlv, Path* pPath, const TLVUniqueId& tlvId, LoadMode loadMode);
