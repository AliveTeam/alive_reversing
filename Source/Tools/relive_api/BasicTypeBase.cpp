#include "BasicTypeBase.hpp"

#include "../../relive_lib/Types.hpp"

#include <jsonxx/jsonxx.h>

#include <string>
#include <typeindex>

namespace ReliveAPI {
BasicTypeBase::BasicTypeBase(const std::type_index& typeIndex, const std::string& typeName, s32 minVal, s32 maxVal)
    : ITypeBase(typeName)
    , mTypeIndex(typeIndex)
    , mMinVal(minVal)
    , mMaxVal(maxVal)
{
}

[[nodiscard]] const std::type_index& BasicTypeBase::TypeIndex() const
{
    return mTypeIndex;
}

[[nodiscard]] bool BasicTypeBase::IsBasicType() const
{
    return true;
}

void BasicTypeBase::ToJson(jsonxx::Array& obj) const
{
    jsonxx::Object ret;
    ret << "min_value" << mMinVal;
    ret << "max_value" << mMaxVal;
    ret << "name" << Name();

    obj << ret;
}
} // namespace ReliveAPI
