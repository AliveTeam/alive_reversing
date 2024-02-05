#pragma once
#include "../MapWrapper.hpp"

// serialization support for each relive tlv type and any supporting nested types
inline void to_json(nlohmann::json& j, const Guid& p)
{
    j = nlohmann::json{
        {"guid", p.ToString()},
    };
}
inline void from_json(const nlohmann::json& j, Guid& p)
{
    // Pull the json value into the string
    std::string tmp;
    j.at("guid").get_to(tmp);
    // Make the guid from the string
    p = Guid::FromString(tmp);
}

inline void to_json(nlohmann::json& j, const PSX_RECT& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
        {"w", p.w},
        {"h", p.h},
    };
}

inline void from_json(const nlohmann::json& j, PSX_RECT& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("w").get_to(p.w);
    j.at("h").get_to(p.h);
}

inline void to_json(nlohmann::json& j, const PSX_Point& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
    };
}

inline void from_json(const nlohmann::json& j, PSX_Point& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

inline void to_json(nlohmann::json& j, const FP& p)
{
    j = nlohmann::json{
        {"fp_value", p.fpValue},
    };
}

inline void from_json(const nlohmann::json& j, FP& p)
{
    j.at("fp_value").get_to(p.fpValue);
}

NLOHMANN_JSON_SERIALIZE_ENUM(EReliveLevelIds, {
    {EReliveLevelIds::eNone, "none"},
    {EReliveLevelIds::eMenu, "menu"},
    {EReliveLevelIds::eMines, "mines"},
    {EReliveLevelIds::eNecrum, "necrum"},
    {EReliveLevelIds::eMudomoVault, "mudomo_vault"},
    {EReliveLevelIds::eMudancheeVault, "mudanchee_vault"},
    {EReliveLevelIds::eFeeCoDepot, "feeco_depot"},
    {EReliveLevelIds::eBarracks, "barracks"},
    {EReliveLevelIds::eMudancheeVault_Ender, "mudanchee_vault_ender"},
    {EReliveLevelIds::eBonewerkz, "bonewerkz"},
    {EReliveLevelIds::eBrewery, "brewery"},
    {EReliveLevelIds::eBrewery_Ender, "brewery_ender"},
    {EReliveLevelIds::eMudomoVault_Ender, "mudomo_vault_ender"},
    {EReliveLevelIds::eFeeCoDepot_Ender, "feeco_depot_ender"},
    {EReliveLevelIds::eBarracks_Ender, "barracks_ender"},
    {EReliveLevelIds::eBonewerkz_Ender, "bonewerkz_ender"},
    {EReliveLevelIds::eCredits, "credits"},
    {EReliveLevelIds::eRuptureFarms, "rupture_farms"},
    {EReliveLevelIds::eLines, "lines"},
    {EReliveLevelIds::eForest, "forest"},
    {EReliveLevelIds::eForestTemple, "forest_temple"},
    {EReliveLevelIds::eStockYards, "stock_yards"},
    {EReliveLevelIds::eStockYardsReturn, "stock_yards_return"},
    {EReliveLevelIds::eDesert, "desert"},
    {EReliveLevelIds::eDesertTemple, "desert_temple"},
    {EReliveLevelIds::eBoardRoom, "board_room"},
    {EReliveLevelIds::eRuptureFarmsReturn, "rupture_farms_return"},
    {EReliveLevelIds::eForestChase, "forest_chase"},
    {EReliveLevelIds::eDesertEscape, "desert_escape"},
})