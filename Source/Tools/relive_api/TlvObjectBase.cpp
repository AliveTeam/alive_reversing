#include "../../AliveLibCommon/stdafx_common.h"
#include "TlvObjectBase.hpp"

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

[[nodiscard]] jsonxx::Object TlvObjectBase::StructureToJson()
{
    jsonxx::Object ret;
    ret << "name" << Name();
    ret << "enum_and_basic_type_properties" << PropertiesToJson();
    return ret;
}

void TlvObjectBase::InstanceFromJson(TypesCollectionBase& types, const jsonxx::Object& obj)
{
    const jsonxx::Object& properties = obj.get<jsonxx::Object>("properties");
    PropertiesFromJson(types, properties);
    InstanceFromJsonBase(obj);
}

[[nodiscard]] jsonxx::Object TlvObjectBase::InstanceToJson(TypesCollectionBase& types)
{
    jsonxx::Object ret;
    InstanceToJsonBase(ret);

    jsonxx::Object properties;
    PropertiesToJson(types, properties);
    ret << "properties" << properties;

    return ret;
}

[[nodiscard]] s32 TlvObjectBase::InstanceNumber() const
{
    return mInstanceNumber;
}
} // namespace ReliveAPI
