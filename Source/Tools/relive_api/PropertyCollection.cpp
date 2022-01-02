#include "PropertyCollection.hpp"

#include "relive_api.hpp"
#include "BaseProperty.hpp"
#include "TypedProperty.hpp"

#include "../../AliveLibCommon/stdafx_common.h"

#include <jsonxx/jsonxx.h>

namespace ReliveAPI {
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

[[nodiscard]] const std::string& PropertyCollection::PropType(const void* key) const
{
    const auto it = mProperties.find(key);

    if (it == mProperties.end())
    {
        throw ReliveAPI::PropertyNotFoundException();
    }

    return it->second->TypeName();
}

[[nodiscard]] const std::string& PropertyCollection::PropName(const void* key) const
{
    const auto it = mProperties.find(key);

    if (it == mProperties.end())
    {
        throw ReliveAPI::PropertyNotFoundException();
    }

    return it->second->Name();
}

[[nodiscard]] jsonxx::Array PropertyCollection::PropertiesToJson() const
{
    jsonxx::Array ret;

     // Create the json in the order that properties got added (else in the Editor things will be in some seemingly random order).
    for (const auto& insertedOrder : mPropertiesInsertionOrdering)
    {
        auto it = mProperties.find(insertedOrder);
        if (it == std::end(mProperties))
        {
            // Shouldn't be possible
            abort();
        }

        jsonxx::Object property;
        property << "Type" << it->second->TypeName();
        property << "Visible" << it->second->IsVisibleToEditor();
        property << "name" << it->second->Name();

        ret << property;
    }

    return ret;
}

void PropertyCollection::PropertiesFromJson(const TypesCollectionBase& types, const jsonxx::Object& properties)
{
    for (auto& [Key, value] : mProperties)
    {
        value->Read(*this, types, properties);
    }
}

void PropertyCollection::PropertiesToJson(const TypesCollectionBase& types, jsonxx::Object& properties)
{
    for (auto& [Key, value] : mProperties)
    {
        value->Write(*this, types, properties);
    }
}
} // namespace ReliveAPI
