#include "PropertyCollection.hpp"

#include "relive_api.hpp"
#include "BaseProperty.hpp"
#include "TypedProperty.hpp"

#include "../AliveLibCommon/stdafx_common.h"

#include <jsonxx/jsonxx.h>

void PropertyCollection::ThrowOnAddPropertyError(const std::string& name, const std::string& typeName, void* key)
{
    if (name.empty())
    {
        throw ReliveAPI::EmptyPropertyNameException();
    }

    if (typeName.empty())
    {
        throw ReliveAPI::EmptyTypeNameException();
    }

    if (mProperties.find(key) != mProperties.end())
    {
        throw ReliveAPI::DuplicatePropertyKeyException();
    }

    if (mRegisteredPropertyNames.find(name) != mRegisteredPropertyNames.end())
    {
        throw ReliveAPI::DuplicatePropertyNameException(name.c_str());
    }
}

PropertyCollection::~PropertyCollection() = default;

[[nodiscard]] const std::string& PropertyCollection::PropType(void* key) const
{
    auto it = mProperties.find(key);

    if (it == mProperties.end())
    {
        throw ReliveAPI::PropertyNotFoundException();
    }

    return it->second->TypeName();
}

[[nodiscard]] const std::string& PropertyCollection::PropName(void* key) const
{
    auto it = mProperties.find(key);

    if (it == mProperties.end())
    {
        throw ReliveAPI::PropertyNotFoundException();
    }

    return it->second->Name();
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

void PropertyCollection::PropertiesFromJson(TypesCollectionBase& types, jsonxx::Object& properties)
{
    for (auto& [Key, value] : mProperties)
    {
        value->Read(*this, types, properties);
    }
}

void PropertyCollection::PropertiesToJson(TypesCollectionBase& types, jsonxx::Object& properties)
{
    for (auto& [Key, value] : mProperties)
    {
        value->Write(*this, types, properties);
    }
}
