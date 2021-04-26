#pragma once

#include "ITypeBase.hpp"
#include "relive_api.hpp"

#include <jsonxx/jsonxx.h>

#include <map>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <type_traits>

template<class T>
class EnumTypeBase : public ITypeBase
{
    static_assert(std::is_integral_v<T> && !std::is_enum_v<T>, "Must be underlying type of enum");

protected:
    explicit EnumTypeBase(const std::string& typeName, const std::type_index& typeIndex)
        : ITypeBase(typeName), mTypeIndex(typeIndex)
    {
    }

    void Add(T enumValue, const std::string& name)
    {
        mMapping[enumValue] = name;
        mReverseMapping[name] = enumValue;
    }

    [[nodiscard]] const std::type_index& TypeIndex() const override
    {
        return mTypeIndex;
    }

    [[nodiscard]] T ValueFromString(const std::string& valueString) const
    {
        const auto it = mReverseMapping.find(valueString);

        return it != mReverseMapping.end()
            ? it->second
            : throw ReliveAPI::UnknownEnumValueException(valueString);
    }

    [[nodiscard]] const std::string& ValueToString(T valueToFind) const
    {
        const auto it = mMapping.find(valueToFind);

        return it != mMapping.end()
            ? it->second
            : throw ReliveAPI::UnknownEnumValueException();
    }

    [[nodiscard]] bool IsBasicType() const override
    {
        return false;
    }

    void ToJson(jsonxx::Array& obj) const override
    {
        jsonxx::Array enumVals;
        for (const auto& [key, value] : mMapping)
        {
            enumVals << value;
        }

        jsonxx::Object enumObj;
        enumObj << "name" << Name();
        enumObj << "values" << enumVals;

        obj << enumObj;
    }

private:
    std::map<T, std::string> mMapping;
    std::unordered_map<std::string, T> mReverseMapping;
    std::type_index mTypeIndex;
};
