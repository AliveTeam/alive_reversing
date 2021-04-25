#include "PropertyCollection.hpp"

#include "relive_api.hpp"

#include "../AliveLibCommon/stdafx_common.h"

#include <jsonxx/jsonxx.h>

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
