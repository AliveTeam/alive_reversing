#pragma once

#include "relive_tlvs.hpp"

// serialization support for each relive tlv type and any supporting nested types

// RGB16
void to_json(nlohmann::json& j, const RGB16& p)
{
    j = nlohmann::json{
        {"r", p.r},
        {"g", p.g},
        {"b", p.b}};
}

void from_json(const nlohmann::json& j, RGB16& p)
{
    j.at("r").get_to(p.r);
    j.at("g").get_to(p.g);
    j.at("b").get_to(p.b);
}

namespace relive {
template<class T>
static const relive::Path_TLV& ToBase(const T& derivedType)
{
    return derivedType;
}

template <class T>
static relive::Path_TLV& ToBase(T& derivedType)
{
    return derivedType;
}

// Path_TLV
void to_json(nlohmann::json& j, const Path_TLV& p)
{
    j = nlohmann::json{
        {"x", p.mX},
        {"y", p.mY},
        {"width", p.mWidth},
        {"height", p.mHeight},
    };
}

void from_json(const nlohmann::json& j, Path_TLV& p)
{
    j.at("x").get_to(p.mX);
    j.at("y").get_to(p.mY);
    j.at("width").get_to(p.mWidth);
    j.at("height").get_to(p.mHeight);
}

// Path_ShadowZone
NLOHMANN_JSON_SERIALIZE_ENUM(Path_ShadowZone::Scale, {
    {Path_ShadowZone::Scale::eBoth, "both"},
    {Path_ShadowZone::Scale::eFull, "full"},
    {Path_ShadowZone::Scale::eHalf, "half"},
})

void to_json(nlohmann::json& j, const Path_ShadowZone& p)
{
    j = nlohmann::json{
        {"base", ToBase(p)},
        {"rgb", p.mRGB},
        {"scale", p.mScale},
    };
}


void from_json(const nlohmann::json& j, Path_ShadowZone& p)
{
    j.at("base").get_to(ToBase(p));
    j.at("rgb").get_to(p.mRGB);
    j.at("scale").get_to(p.mScale);
}



}

