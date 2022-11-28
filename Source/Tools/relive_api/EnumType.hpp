#pragma once

#include "EnumTypeBase.hpp"
#include "ApiContext.hpp"

#include <jsonxx/jsonxx.h>

#include <map>
#include <string>
#include <typeinfo>
#include <type_traits>

namespace ReliveAPI {
template <class T>
class EnumType final : public EnumTypeBase<std::underlying_type_t<T>>
{
public:
    using Underlying = std::underlying_type_t<T>;
    using Base = EnumTypeBase<Underlying>;

    explicit EnumType(const std::string& typeName)
        : Base(typeName, typeid(T))
    {
    }

    void Add(T enumValue, const std::string& name)
    {
        Base::Add(static_cast<Underlying>(enumValue), name);
    }

    [[nodiscard]] T ValueFromString(const std::string& valueString, Context& context) const
    {
        return static_cast<T>(Base::ValueFromString(valueString, context));
    }

    [[nodiscard]] const std::string& ValueToString(T valueToFind, Context& context) const
    {
        return Base::ValueToString(static_cast<Underlying>(valueToFind), context);
    }

    using Base::IsBasicType;
    using Base::ToJson;
    using Base::TypeIndex;
};
} // namespace ReliveAPI
