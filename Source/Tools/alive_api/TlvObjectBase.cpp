#include "../AliveLibCommon/stdafx_common.h"
#include "TlvObjectBase.hpp"
#include "TypedProperty.hpp"

void TlvObjectBase::PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties)
{
    for (auto& prop : mProperties)
    {
        prop->Read(*this, types, properties);
    }
}

void TlvObjectBase::PropertiesToJson(TypesCollection& types, jsonxx::Object& properties)
{
    for (auto& prop : mProperties)
    {
        prop->Write(*this, types, properties);
    }
}
