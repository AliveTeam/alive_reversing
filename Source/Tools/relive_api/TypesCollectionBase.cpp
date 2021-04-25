#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollectionBase.hpp"
#include "TlvObjectBase.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAE/Path.hpp"

TypesCollectionBase::TypesCollectionBase()
{
    #define ADD_BASIC_TYPE(name, type) AddBasicType<type>(name, std::numeric_limits<type>::min(), std::numeric_limits<type>::max());

    ADD_BASIC_TYPE("Byte", u8);
    ADD_BASIC_TYPE("UInt16", u16);
    ADD_BASIC_TYPE("Uint32", u32);
    ADD_BASIC_TYPE("SInt16", s16);
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

[[nodiscard]] std::string TypesCollectionBase::TypeName(std::type_index typeIndex) const
{
    for (const auto& e : mTypes)
    {
        if (e->TypeIndex() == typeIndex)
        {
            return e->Name();
        }
    }

    return "";
}
