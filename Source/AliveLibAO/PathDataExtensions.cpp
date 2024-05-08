#include "stdafx_ao.h"
#include "PathDataExtensions.hpp"
#include "PathData.hpp"
#include "LCDScreen.hpp"
#include "HintFly.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/PathDataExtensionsTypes.hpp"

namespace AO {

struct MudCounts final
{
    s32 mTotal = 99;
    s32 mBadEnding = 75;
    s32 mGoodEnding = 50;
};
static MudCounts sMudExtData[static_cast<u32>(LevelIds::eDesertEscape_15) + 1][99];

s32 Path_GetTotalMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAO(lvlId))][pathNum].mTotal;
}

s32 Path_BadEndingMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAO(lvlId))][pathNum].mBadEnding;
}

s32 Path_GoodEndingMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAO(lvlId))][pathNum].mGoodEnding;
}

//static u8* sPathExtData[static_cast<u32>(LevelIds::eDesertEscape_15) + 1] = {};

s32 sub_402560();

template<typename T>
static void SetAndLog(const char_type* propertyName, T& dst, T newVal)
{
    if (dst != newVal)
    {
        LOG_INFO("Update %s from %d to %d", propertyName, static_cast<s32>(dst), static_cast<s32>(newVal));
        dst = newVal;
    }
}

} // namespace AO
