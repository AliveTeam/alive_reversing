#pragma once

#include "../../AliveLibCommon/Types.hpp"
#include "../MapWrapper.hpp"

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
