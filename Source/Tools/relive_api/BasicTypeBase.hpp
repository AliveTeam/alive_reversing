#pragma once

#include "ITypeBase.hpp"

#include "../../AliveLibCommon/Types.hpp"

#include <string>
#include <typeindex>

namespace jsonxx {
class Array;
}

namespace ReliveAPI {
class BasicTypeBase : public ITypeBase
{
public:
    explicit BasicTypeBase(const std::type_index& typeIndex, const std::string& typeName, s32 minVal, s32 maxVal);

    [[nodiscard]] const std::type_index& TypeIndex() const override;
    [[nodiscard]] bool IsBasicType() const override;

    void ToJson(jsonxx::Array& obj) const override;

private:
    std::type_index mTypeIndex;
    s32 mMinVal = 0;
    s32 mMaxVal = 0;
};
} // namespace ReliveAPI