#pragma once

#include "../AliveLibCommon/Types.hpp"

enum class EReliveLevelIds : s16
{
    eNone = -1, // TODO: Shouldn't depend on this value in the future
    eMenu,
    eMines,
    eNecrum,
    eMudomoVault,
    eMudancheeVault,
    eFeeCoDepot,
    eBarracks,
    eMudancheeVault_Ender,
    eBonewerkz,
    eBrewery,
    eBrewery_Ender,
    eMudomoVault_Ender,
    eFeeCoDepot_Ender,
    eBarracks_Ender,
    eBonewerkz_Ender,
    eCredits,
    eRuptureFarms,
    eLines,
    eForest,
    eForestTemple,
    eStockYards,
    eStockYardsReturn,
    eDesert,
    eDesertTemple,
    eBoardRoom,
    eRuptureFarmsReturn,
    eForestChase,
    eDesertEscape,
};

class MapWrapper
{
public:
    // TODO: Need to remap EReliveLevelIds enums before we can use this

    EReliveLevelIds mCurrentLevel;
    s16 mCurrentPath;
    s16 mCurrentCamera;

    //LevelIds mLevel;
    s16 mPath;
    s16 mCamera;

    s16 GetOverlayId();
};

extern MapWrapper gMapWrapper;
