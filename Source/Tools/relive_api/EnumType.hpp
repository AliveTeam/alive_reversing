#pragma once

#include "EnumTypeBase.hpp"
#include "relive_api.hpp"

#include <jsonxx/jsonxx.h>

#include <map>
#include <string>
#include <typeinfo>
#include <type_traits>

template <class T>
class EnumType : public EnumTypeBase<std::underlying_type_t<T>>
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

    [[nodiscard]] T ValueFromString(const std::string& valueString) const
    {
        return static_cast<T>(Base::ValueFromString(valueString));
    }

    [[nodiscard]] const std::string& ValueToString(T valueToFind) const
    {
        return Base::ValueToString(static_cast<Underlying>(valueToFind));
    }

    using Base::IsBasicType;
    using Base::ToJson;
    using Base::TypeIndex;
};
