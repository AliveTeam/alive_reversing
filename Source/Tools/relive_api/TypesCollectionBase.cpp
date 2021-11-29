#include "../../AliveLibCommon/stdafx_common.h"
#include "TypesCollectionBase.hpp"
#include "TlvObjectBase.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/Map.hpp"
#include "../../AliveLibAE/Path.hpp"

namespace ReliveAPI {
TypesCollectionBase::TypesCollectionBase()
{
#define ADD_BASIC_TYPE(name, type) AddBasicType<type>(name, std::numeric_limits<type>::min(), std::numeric_limits<type>::max());

    ADD_BASIC_TYPE("Byte", u8);
    ADD_BASIC_TYPE("UInt16", u16);
    ADD_BASIC_TYPE("SInt16", s16);
    ADD_BASIC_TYPE("Uint32", u32);
    ADD_BASIC_TYPE("SInt32", s32);

#undef ADD_BASIC_TYPE
}

TypesCollectionBase::~TypesCollectionBase() = default;

[[nodiscard]] jsonxx::Array TypesCollectionBase::EnumsToJson() const
{
    jsonxx::Array ret;

    for (const auto& basicType : mTypes)
    {
        if (!basicType->IsBasicType())
        {
            basicType->ToJson(ret);
        }
    }

    return ret;
}

[[nodiscard]] jsonxx::Array TypesCollectionBase::BasicTypesToJson() const
{
    jsonxx::Array ret;

    for (const auto& basicType : mTypes)
    {
        if (basicType->IsBasicType())
        {
            basicType->ToJson(ret);
        }
    }

    return ret;
}

[[nodiscard]] const std::string& TypesCollectionBase::TypeName(const std::type_index& typeIndex) const
{
    const ITypeBase* ptr = FindByTypeIndex(typeIndex);
    return ptr != nullptr ? ptr->Name() : mEmptyStr;
}

[[nodiscard]] ITypeBase* TypesCollectionBase::RegisterType(ITypeBase* typeBase)
{
    std::unique_ptr<ITypeBase>& emplaced = mTypes.emplace_back(typeBase);
    mTypesByName[typeBase->Name()] = mTypesByTypeIndex[typeBase->TypeIndex()] = emplaced.get();
    return emplaced.get();
}

[[nodiscard]] const ITypeBase* TypesCollectionBase::FindByTypeName(const std::string& typeName) const
{
    const auto it = mTypesByName.find(typeName);
    return it != mTypesByName.end() ? it->second : nullptr;
}

[[nodiscard]] const ITypeBase* TypesCollectionBase::FindByTypeIndex(const std::type_index& typeIndex) const
{
    const auto it = mTypesByTypeIndex.find(typeIndex);
    return it != mTypesByTypeIndex.end() ? it->second : nullptr;
}
} // namespace ReliveAPI
