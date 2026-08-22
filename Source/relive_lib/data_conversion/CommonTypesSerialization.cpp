#include "CommonTypesSerialization.hpp"
#include <nlohmann/json.hpp>
#include "guid.hpp"
#include "../Psx.hpp"
#include "../FixedPoint.hpp"

// serialization support for each relive tlv type and any supporting nested types
void to_json(nlohmann::json& j, const Guid& p)
{
    j = nlohmann::json{
        {"guid", p.ToString()},
    };
}
void from_json(const nlohmann::json& j, Guid& p)
{
    // Pull the json value into the string
    std::string tmp;
    j.at("guid").get_to(tmp);
    // Make the guid from the string
    p = Guid::FromString(tmp);
}

void to_json(nlohmann::json& j, const PSX_RECT& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
        {"w", p.w},
        {"h", p.h},
    };
}

void from_json(const nlohmann::json& j, PSX_RECT& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("w").get_to(p.w);
    j.at("h").get_to(p.h);
}

void to_json(nlohmann::json& j, const PSX_Point& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
    };
}

void from_json(const nlohmann::json& j, PSX_Point& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

void to_json(nlohmann::json& j, const FP& p)
{
    j = nlohmann::json{
        {"fp_value", p.fpValue},
    };
}

void from_json(const nlohmann::json& j, FP& p)
{
    j.at("fp_value").get_to(p.fpValue);
}
