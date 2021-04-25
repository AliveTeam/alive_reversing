#include "../AliveLibCommon/stdafx_common.h"
#include "TlvObjectBase.hpp"
#include "TypedProperty.hpp"

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
