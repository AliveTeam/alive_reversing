#pragma once

#include <nlohmann/json_fwd.hpp>

class Guid;
struct PSX_RECT;
struct PSX_Point;
struct FixedPoint;
using FP = FixedPoint;

// serialization support for each relive tlv type and any supporting nested types
void to_json(nlohmann::json& j, const Guid& p);
void from_json(const nlohmann::json& j, Guid& p);

void to_json(nlohmann::json& j, const PSX_RECT& p);
void from_json(const nlohmann::json& j, PSX_RECT& p);

void to_json(nlohmann::json& j, const PSX_Point& p);
void from_json(const nlohmann::json& j, PSX_Point& p);

void to_json(nlohmann::json& j, const FP& p);
void from_json(const nlohmann::json& j, FP& p);