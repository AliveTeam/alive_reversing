#include "BasicTypeBase.hpp"

#include "../../relive_lib/Types.hpp"

#include <nlohmann/json.hpp>

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

void BasicTypeBase::ToJson(nlohmann::json& obj) const
{
    nlohmann::json ret = nlohmann::json::object();
    ret["min_value"] = mMinVal;
    ret["max_value"] = mMaxVal;
    ret["name"] = Name();

    obj.push_back(ret);
}
} // namespace ReliveAPI
