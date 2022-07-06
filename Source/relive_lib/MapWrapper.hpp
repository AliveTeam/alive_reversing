#pragma once

#include "../AliveLibCommon/Types.hpp"

enum class EReliveLevelIds : s16
{
    eNone = 0, // TODO: Shouldn't depend on this value in the future
    eMenu = 2000,
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

enum class LevelIds : s16;
namespace AO {
    enum class LevelIds : s16;
}

class MapWrapper
{
public:
    // TODO: Need to remap EReliveLevelIds enums before we can use this
    static EReliveLevelIds FromAO(AO::LevelIds levelId);
    static AO::LevelIds ToAO(EReliveLevelIds levelId);

    static EReliveLevelIds FromAE(LevelIds levelId);
    static LevelIds MapWrapper::ToAE(EReliveLevelIds levelId);
};

class IMap
{
public:
    virtual ~IMap()
    {

    }

    EReliveLevelIds mCurrentLevel;
    s16 mCurrentPath;
    s16 mCurrentCamera;

    EReliveLevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;

    s16 mOverlayId;

    virtual s16 GetOverlayId() = 0;
};
