#include "../../relive_lib/stdafx.h"
#include "TlvObjectBase.hpp"
#include <nlohmann/json.hpp>

namespace ReliveAPI {
TlvObjectBase::TlvObjectBase(const std::string& typeName)
    : mStructTypeName(typeName)
{
}

void TlvObjectBase::AddTypes(TypesCollectionBase& /*types*/)
{
    // Default empty to prevent having to explicitly implement in every TLV wrapper
}

void TlvObjectBase::SetInstanceNumber(s32 instanceNumber)
{
    mInstanceNumber = instanceNumber;
}

[[nodiscard]] const std::string& TlvObjectBase::Name() const
{
    return mStructTypeName;
}

[[nodiscard]] nlohmann::json TlvObjectBase::StructureToJson()
{
    nlohmann::json ret = nlohmann::json::object();
    ret["name"] = Name();
    ret["enum_and_basic_type_properties"] = PropertiesToJson();
    return ret;
}

void TlvObjectBase::InstanceFromJson(TypesCollectionBase& types, const nlohmann::json& obj, Context& context)
{
    const nlohmann::json& properties = obj.at("properties");
    PropertiesFromJson(types, properties, context);
    InstanceFromJsonBase(obj);
}

[[nodiscard]] nlohmann::json TlvObjectBase::InstanceToJson(TypesCollectionBase& types, Context& context)
{
    nlohmann::json ret = nlohmann::json::object();
    InstanceToJsonBase(ret);

    nlohmann::json properties = nlohmann::json::object();
    PropertiesToJson(types, properties, context);
    ret["properties"] = properties;

    return ret;
}

[[nodiscard]] s32 TlvObjectBase::InstanceNumber() const
{
    return mInstanceNumber;
}

void TlvObjectBase::AddResource(AnimId res, AddResourceTo type)
{
    mRequiredResources.push_back({res, type});
}

} // namespace ReliveAPI
