#pragma once

#include "ITypeBase.hpp"
#include "ApiContext.hpp"

#include "../../relive_lib/logger.hpp"

#include <nlohmann/json.hpp>

#include <cstdlib>
#include <map>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>

namespace ReliveAPI {
template <class T>
class EnumTypeBase : public ITypeBase
{
    static_assert(std::is_integral_v<T> && !std::is_enum_v<T>, "Must be underlying type of enum");

protected:
    explicit EnumTypeBase(const std::string& typeName, const std::type_index& typeIndex)
        : ITypeBase(typeName)
        , mTypeIndex(typeIndex)
    {
    }

    void Add(T enumValue, const std::string& name)
    {
        {
            const auto [it, inserted] = mValueToName.emplace(enumValue, name);

            if (!inserted)
            {
                LOG_ERROR("Enum value already present");

                // We never expect to have two enumerators with the same value.
                std::abort();
            }
        }

        {
            const auto [it, inserted] = mNameToValue.emplace(name, enumValue);

            if (!inserted)
            {
                LOG_INFO("Enum with name '%s' already exists", name.c_str());

                // We never expect to have two enumerators with the same name.
                std::abort();
            }
        }
    }

    [[nodiscard]] const std::type_index& TypeIndex() const override
    {
        return mTypeIndex;
    }

    [[nodiscard]] T ValueFromString(const std::string& valueString, Context& context) const
    {
        const auto it = mNameToValue.find(valueString);

        if (it == mNameToValue.end())
        {
            auto firstItem = mNameToValue.begin();
            context.UnknownEnumValue(Name(), valueString, firstItem->first);
            return firstItem->second;
        }

        return it->second;
    }

    [[nodiscard]] const std::string& ValueToString(T valueToFind, Context& context) const
    {
        auto it = mValueToName.find(valueToFind);

        if (it == mValueToName.end())
        {
            auto firstItem = mValueToName.begin();
            context.UnknownEnumValue(Name(), static_cast<s64>(valueToFind), static_cast<s64>(firstItem->first));
            return firstItem->second;
        }

        return it->second;
    }

    [[nodiscard]] bool IsBasicType() const override
    {
        return false;
    }

    void ToJson(nlohmann::json& obj) const override
    {
        nlohmann::json enumVals = nlohmann::json::array();
        for (const auto& [key, value] : mValueToName)
        {
            enumVals.push_back(value);
        }

        nlohmann::json enumObj = nlohmann::json::object();
        enumObj["name"] = Name();
        enumObj["values"] = enumVals;

        obj.push_back(enumObj);
    }

private:
    std::unordered_map<T, std::string> mValueToName;
    std::unordered_map<std::string, T> mNameToValue;

    std::type_index mTypeIndex;
};
} // namespace ReliveAPI
