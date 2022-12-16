#include "PathDataExtensions.hpp"
#include "LCDScreen.hpp"
#include "../relive_lib/PathDataExtensionsTypes.hpp"
#include "../relive_lib/MapWrapper.hpp"

struct MudCounts final
{
    s32 mTotal = 300;
    s32 mBadEnding = 255;
    s32 mGoodEnding = 150;
};
static MudCounts sMudExtData[static_cast<u32>(LevelIds::eCredits_16) + 1][99];


s32 Path_GetTotalMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAE(lvlId))][pathNum].mTotal;
}

s32 Path_BadEndingMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAE(lvlId))][pathNum].mBadEnding;
}

s32 Path_GoodEndingMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAE(lvlId))][pathNum].mGoodEnding;
}

//static u8* sPathExtData[static_cast<u32>(LevelIds::eCredits_16) + 1] = {};

template <typename T>
static void SetAndLog(const char_type* propertyName, T& dst, T newVal)
{
    if (dst != newVal)
    {
        LOG_INFO("Update %s from %d to %d", propertyName, static_cast<s32>(dst), static_cast<s32>(newVal));
        dst = newVal;
    }
}

