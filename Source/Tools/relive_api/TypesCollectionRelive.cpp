#include "TypesCollectionRelive.hpp"

#include "TlvsRelive.hpp"
#include "TlvObjectBase.hpp"

#include "../../AliveLibAE/PathData.hpp"
#include "../../relive_lib/SwitchStates.hpp"
#include "../../AliveLibAE/Path.hpp"
#include "../../AliveLibAE/Map.hpp"

#include "../../relive_lib/stdafx.h"

namespace ReliveAPI {
TypesCollectionRelive::TypesCollectionRelive()
{
    AddReliveTypes();
}

void TypesCollectionRelive::AddTlvsToJsonArray(jsonxx::Array& array)
{
    mTlvFactoryRelive.AddTlvsToJsonArray(*this, array);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvRelive(ReliveTypes tlvType, relive::Path_TLV* pTlv, s32 instanceCount)
{
    return mTlvFactoryRelive.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvRelive(const std::string& tlvTypeName, relive::Path_TLV* pTlv)
{
    return mTlvFactoryRelive.MakeTlvByName(*this, tlvTypeName, pTlv);
}

[[nodiscard]] std::unique_ptr<TlvObjectBase> TypesCollectionRelive::MakeTlvFromString(const std::string& tlvTypeName)
{
    return mTlvFactoryRelive.MakeTlvByName(*this, tlvTypeName, nullptr);
}

void TypesCollectionRelive::AddReliveTypes()
{
#define REGISTER_TYPE_RELIVE(TlvWrapperType) mTlvFactoryRelive.DoRegisterType<TlvWrapperType>(*this)

    REGISTER_TYPE_RELIVE(ReliveTlvs::Path_TimedMine);
    // TODO: Others

#undef REGISTER_TYPE_RELIVE

    // TODO: relive type global enums
    /*
    AddEnum<::Path_FlyingSlig_Data::SpawnDelayStates>("Enum_FlyingSligSpawnDelayStates",
    {
        {::Path_FlyingSlig_Data::SpawnDelayStates::eMoveImmediately_0, "Move Immediately"},
        {::Path_FlyingSlig_Data::SpawnDelayStates::eUseCustomSpawnMoveDelay_1, "Use Custom Spawn Move Delay"},
    });*/

    AddEnum<relive::Path_Slig_Data::DeathMode>("Enum_SligDeathMode",
    {
        {relive::Path_Slig_Data::DeathMode::StandIdle, "Stand Idle"},
        {relive::Path_Slig_Data::DeathMode::FallForward, "Fall Forward"},
    });

    AddEnum<relive::Path_Slig_Data::StartState>("Enum_SligStartState",
    {
        {relive::Path_Slig_Data::StartState::Listening, "Listening"},
        {relive::Path_Slig_Data::StartState::Patrol, "Patrol"},
        {relive::Path_Slig_Data::StartState::Sleeping, "Sleeping"},
        {relive::Path_Slig_Data::StartState::Chase, "Chase"},
        {relive::Path_Slig_Data::StartState::ChaseAndDisappear, "Chase And Disappear"},
        {relive::Path_Slig_Data::StartState::eFallingToChase, "Falling To Chase"},
        {relive::Path_Slig_Data::StartState::ListeningToGlukkon, "Listening To Glukkon"},
    });

    AddEnum<relive::reliveScale>("Enum_reliveScale",
    {
        {relive::reliveScale::eFull, "Full"},
        {relive::reliveScale::eHalf, "Half"},
    });

    AddEnum<relive::reliveXDirection>("Enum_reliveXDirection",
    {
        {relive::reliveXDirection::eLeft, "Left"},
        {relive::reliveXDirection::eRight, "Right"},
    });

    AddEnum<relive::reliveChoice>("Enum_reliveChoice",
    {
        {relive::reliveChoice::eNo, "No"},
        {relive::reliveChoice::eYes, "Yes"},
    });

    AddEnum<relive::reliveSwitchOp>("Enum_reliveSwitchOp",
    {
        {relive::reliveSwitchOp::eSetTrue, "Set True"},
        {relive::reliveSwitchOp::eSetFalse, "Set False"},
        {relive::reliveSwitchOp::eToggle, "Toggle"},
        {relive::reliveSwitchOp::eIncrement, "Increment"},
        {relive::reliveSwitchOp::eDecrement, "Decrement"},
    });

    AddEnum<eLineTypes>("Enum_LineTypes",
    {
        {eLineTypes::eFloor_0, "Floor"},
        {eLineTypes::eWallLeft_1, "Wall Left"},
        {eLineTypes::eWallRight_2, "Wall Right"},
        {eLineTypes::eCeiling_3, "Ceiling"},
        {eLineTypes::eBackgroundFloor_4, "Background Floor"},
        {eLineTypes::eBackgroundWallLeft_5, "Background Wall Left"},
        {eLineTypes::eBackgroundWallRight_6, "Background Wall Right"},
        {eLineTypes::eBackgroundCeiling_7, "Background Ceiling"},
        {eLineTypes::eTrackLine_8, "Track Line"},
        {eLineTypes::eArt_9, "Art"},
        {eLineTypes::eBulletWall_10, "Bullet Wall"},
        {eLineTypes::eMineCarFloor_11, "Mine Car Floor"},
        {eLineTypes::eMineCarWall_12, "Mine Car Wall"},
        {eLineTypes::eMineCarCeiling_13, "Mine Car Ceiling"},
        {eLineTypes::eBackgroundMineCarFloor_14, "Background Mine Car Floor"},
        {eLineTypes::eBackgroundMineCarWall_15, "Background Mine Car Wall"},
        {eLineTypes::eBackgroundMineCarCeiling_16, "Background Mine Car Ceiling"},
        {eLineTypes::eFlyingObjectWall_17, "Flying Slig Ceiling"},
        {eLineTypes::eBackgroundFlyingObjectWall_18, "Background Flying Slig Ceiling"},
        {eLineTypes::eDynamicCollision_32, "Dynamic Collision"},
        {eLineTypes::eBackgroundDynamicCollision_36, "Background Dynamic Collision"},
    });

    AddEnum<relive::reliveScreenChangeEffects>("Enum_ScreenChangeEffects",
        {{relive::reliveScreenChangeEffects::ePlay1FMV, "Play 1 FMV"},
        {relive::reliveScreenChangeEffects::eRightToLeft, "Right To Left"},
        {relive::reliveScreenChangeEffects::eLeftToRight, "Left To Right"},
        {relive::reliveScreenChangeEffects::eBottomToTop, "Bottom To Top"},
        {relive::reliveScreenChangeEffects::eTopToBottom, "Top To Bottom"},
        {relive::reliveScreenChangeEffects::eBoxOut, "Box Out"},
        {relive::reliveScreenChangeEffects::eVerticalSplit, "Vertical Split"},
        {relive::reliveScreenChangeEffects::eHorizontalSplit, "Horizontal Split"},
        {relive::reliveScreenChangeEffects::eUnknown_8, "Unknown8"},
        {relive::reliveScreenChangeEffects::eInstantChange, "Instant Change"}});

    AddEnum<EReliveLevelIds>("Enum_EReliveLevelIds",
    {
        {EReliveLevelIds::eNone, "None"},
        {EReliveLevelIds::eMenu, "Menu"},
        {EReliveLevelIds::eMines, "Mines"},
        {EReliveLevelIds::eNecrum, "Necrum"},
        {EReliveLevelIds::eMudomoVault, "Mudomo Vault"},
        {EReliveLevelIds::eMudancheeVault, "Mudanchee Vault"},
        {EReliveLevelIds::eFeeCoDepot, "FeeCo Depot"},
        {EReliveLevelIds::eBarracks, "Barracks"},
        {EReliveLevelIds::eMudancheeVault_Ender, "Mudanchee Vault Ender"},
        {EReliveLevelIds::eBonewerkz, "Bonewerkz"},
        {EReliveLevelIds::eBrewery, "Brewery"},
        {EReliveLevelIds::eBrewery_Ender, "Brewery Ender"},
        {EReliveLevelIds::eMudomoVault_Ender, "Mudomo Vault Ender"},
        {EReliveLevelIds::eFeeCoDepot_Ender, "FeeCo Depot Ender"},
        {EReliveLevelIds::eBarracks_Ender, "Barracks Ender"},
        {EReliveLevelIds::eBonewerkz_Ender, "Bonewerkz Ender"},
        {EReliveLevelIds::eCredits, "Credits"},
        {EReliveLevelIds::eRuptureFarms, "Rupture Farms"},
        {EReliveLevelIds::eLines, "Lines"},
        {EReliveLevelIds::eForest, "Forest"},
        {EReliveLevelIds::eForestTemple, "Forest Temple"},
        {EReliveLevelIds::eStockYards, "Stock Yards"},
        {EReliveLevelIds::eStockYardsReturn, "Stock Yards Return"},
        {EReliveLevelIds::eDesert, "Desert"},
        {EReliveLevelIds::eDesertTemple, "Desert Temple"},
        {EReliveLevelIds::eBoardRoom, "Board Room"},
        {EReliveLevelIds::eRuptureFarmsReturn, "Rupture Farms Return"},
        {EReliveLevelIds::eForestChase, "Forest Chase"},
        {EReliveLevelIds::eDesertEscape, "Desert Escape"},
    });

}
} // namespace ReliveAPI
