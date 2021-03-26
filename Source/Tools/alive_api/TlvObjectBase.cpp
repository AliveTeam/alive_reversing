#include "../AliveLibCommon/stdafx_common.h"
#include "TlvObjectBase.hpp"
#include "TypedProperty.hpp"

void TlvObjectBase::PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties)
{
    for (auto& [Key, value] : mProperties)
    {
        value->Read(*this, types, properties);
    }
}

void TlvObjectBase::PropertiesToJson(TypesCollection& types, jsonxx::Object& properties)
{
    for (auto& [Key, value] : mProperties)
    {
        value->Write(*this, types, properties);
    }
}
