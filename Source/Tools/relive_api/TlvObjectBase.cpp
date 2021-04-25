#include "../AliveLibCommon/stdafx_common.h"
#include "TlvObjectBase.hpp"
#include "TypedProperty.hpp"

[[nodiscard]] std::string PropertyCollection::PropType(void* key) const
{
    auto it = mProperties.find(key);

    if (it == std::end(mProperties))
    {
        throw ReliveAPI::PropertyNotFoundException();
    }

    return it->second->TypeName();
}

[[nodiscard]] jsonxx::Array PropertyCollection::PropertiesToJson() const
{
    jsonxx::Array ret;

    for (const auto& [key, value] : mProperties)
    {
        jsonxx::Object property;
        property << "Type" << value->TypeName();
        property << "Visible" << value->IsVisibleToEditor();
        property << "name" << value->Name();
        ret << property;
    }

    return ret;
}

[[nodiscard]] std::string PropertyCollection::PropName(void* key) const
{
    auto it = mProperties.find(key);

    if (it == std::end(mProperties))
    {
        throw ReliveAPI::PropertyNotFoundException();
    }

    return it->second->Name();
}

void PropertyCollection::PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties)
{
    for (auto& [Key, value] : mProperties)
    {
        value->Read(*this, types, properties);
    }
}

void PropertyCollection::PropertiesToJson(TypesCollection& types, jsonxx::Object& properties)
{
    for (auto& [Key, value] : mProperties)
    {
        value->Write(*this, types, properties);
    }
}

TlvObjectBase::TlvObjectBase(const std::string& typeName)
    : mStructTypeName(typeName)
{

}

void TlvObjectBase::AddTypes(TypesCollection& /*types*/)
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

[[nodiscard]] jsonxx::Object TlvObjectBase::StructureToJson()
{
    jsonxx::Object ret;
    ret << "name" << Name();
    ret << "enum_and_basic_type_properties" << PropertiesToJson();
    return ret;
}

void TlvObjectBase::InstanceFromJson(TypesCollection& types, jsonxx::Object& obj)
{
    jsonxx::Object properties = obj.get<jsonxx::Object>("properties");
    PropertiesFromJson(types, properties);
    InstanceFromJsonBase(obj);
}

[[nodiscard]] jsonxx::Object TlvObjectBase::InstanceToJson(TypesCollection& types)
{
    jsonxx::Object ret;
    InstanceToJsonBase(ret);

    jsonxx::Object properties;
    PropertiesToJson(types, properties);
    ret << "properties" << properties;

    return ret;
}

s32 TlvObjectBase::InstanceNumber() const
{
    return mInstanceNumber;
}
