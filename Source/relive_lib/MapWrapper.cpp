#include "MapWrapper.hpp"
#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibCommon/Sys_common.hpp"

MapWrapper gMapWrapper;

EReliveLevelIds MapWrapper::FromAO(AO::LevelIds levelId)
{
    switch (levelId)
    {
        case AO::LevelIds::eNone:
            return EReliveLevelIds::eNone;
        case AO::LevelIds::eMenu_0:
            return EReliveLevelIds::eMenu;
        case AO::LevelIds::eRuptureFarms_1:
            return EReliveLevelIds::eRuptureFarms;
        case AO::LevelIds::eLines_2:
            return EReliveLevelIds::eLines;
        case AO::LevelIds::eForest_3:
            return EReliveLevelIds::eForest;
        case AO::LevelIds::eForestTemple_4:
            return EReliveLevelIds::eForestTemple;
        case AO::LevelIds::eStockYards_5:
            return EReliveLevelIds::eStockYards;
        case AO::LevelIds::eStockYardsReturn_6:
            return EReliveLevelIds::eStockYardsReturn;
        case AO::LevelIds::eDesert_8:
            return EReliveLevelIds::eDesert;
        case AO::LevelIds::eDesertTemple_9:
            return EReliveLevelIds::eDesertTemple;
        case AO::LevelIds::eCredits_10:
            return EReliveLevelIds::eCredits;
        case AO::LevelIds::eBoardRoom_12:
            return EReliveLevelIds::eBoardRoom;
        case AO::LevelIds::eRuptureFarmsReturn_13:
            return EReliveLevelIds::eRuptureFarmsReturn;
        case AO::LevelIds::eForestChase_14:
            return EReliveLevelIds::eForestChase;
        case AO::LevelIds::eDesertEscape_15:
            return EReliveLevelIds::eDesertEscape;
        default:
            ALIVE_FATAL("Unknown AO level");
    }
}

AO::LevelIds MapWrapper::ToAO(EReliveLevelIds levelId)
{
    switch (levelId)
    {
        case EReliveLevelIds::eNone:
            return AO::LevelIds::eNone;
        case EReliveLevelIds::eMenu:
            return AO::LevelIds::eMenu_0;
        case EReliveLevelIds::eRuptureFarms:
            return AO::LevelIds::eRuptureFarms_1;
        case EReliveLevelIds::eLines:
            return AO::LevelIds::eLines_2;
        case EReliveLevelIds::eForest:
            return AO::LevelIds::eForest_3;
        case EReliveLevelIds::eForestTemple:
            return AO::LevelIds::eForestTemple_4;
        case EReliveLevelIds::eStockYards:
            return AO::LevelIds::eStockYards_5;
        case EReliveLevelIds::eStockYardsReturn:
            return AO::LevelIds::eStockYardsReturn_6;
        case EReliveLevelIds::eDesert:
            return AO::LevelIds::eDesert_8;
        case EReliveLevelIds::eDesertTemple:
            return AO::LevelIds::eDesertTemple_9;
        case EReliveLevelIds::eCredits:
            return AO::LevelIds::eCredits_10;
        case EReliveLevelIds::eBoardRoom:
            return AO::LevelIds::eBoardRoom_12;
        case EReliveLevelIds::eRuptureFarmsReturn:
            return AO::LevelIds::eRuptureFarmsReturn_13;
        case EReliveLevelIds::eForestChase:
            return AO::LevelIds::eForestChase_14;
        case EReliveLevelIds::eDesertEscape:
            return AO::LevelIds::eDesertEscape_15;
        default:
            ALIVE_FATAL("Can't map to an AO lvl");
    }
}
