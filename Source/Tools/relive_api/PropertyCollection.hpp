#pragma once

#include "TypesCollectionBase.hpp"
#include "BaseProperty.hpp"
#include "TypedProperty.hpp"
#include "ApiContext.hpp"

#include <nlohmann/json.hpp>

#include <string>
#include <unordered_set>

namespace ReliveAPI {
class Context;
class PropertyCollection
{
private:
    void ThrowOnAddPropertyError(const std::string& name, const std::string& typeName, void* key);

public:
    virtual ~PropertyCollection();

    template <typename PropertyType>
    void AddProperty(const std::string& name, const std::string& typeName, void* key, bool visibleInEditor, const char* idStr = nullptr)
    {
        ThrowOnAddPropertyError(name, typeName, key);

        // Using `std::make_unique` here unfortunately significantly increases compilation time on MinGW + GCC.
        mProperties[key].reset(new TypedProperty<PropertyType>(name, typeName, idStr, visibleInEditor, static_cast<PropertyType*>(key)));
        mPropertiesInsertionOrdering.emplace_back(key);
        mRegisteredPropertyNames.emplace(name);
    }

    [[nodiscard]] const std::string& PropType(const void* key) const;
    [[nodiscard]] const std::string& PropName(const void* key) const;

    [[nodiscard]] nlohmann::json PropertiesToJson() const;

    template <class T>
    void ReadEnumValue(const TypesCollectionBase& types, T& field, const nlohmann::json& properties, Context& context) const
    {
        const std::string& propName = PropName(&field);
        const std::string& propType = PropType(&field);

        if (!properties.contains(propName))
        {
            LOG_ERROR("Missing json property %s", propName.c_str());
            context.MissingEnumType(propType, propName);
        }

        field = types.EnumValueFromString<T>(propType, properties.at(propName).get<std::string>(), context);
    }

    template <class T>
    void WriteEnumValue(const TypesCollectionBase& types, nlohmann::json& properties, const T& field, Context& context) const
    {
        properties[PropName(&field)] = types.EnumValueToString<T>(field, context);
    }

    template <class T>
    void ReadBasicType(T& field, const nlohmann::json& properties) const
    {
        field = properties.at(PropName(&field)).get<T>();
    }

    template <class T>
    void WriteBasicType(const T& field, nlohmann::json& properties) const
    {
        properties[PropName(&field)] = static_cast<s32>(field);
    }

    void PropertiesFromJson(const TypesCollectionBase& types, const nlohmann::json& properties, Context& context);
    void PropertiesToJson(const TypesCollectionBase& types, nlohmann::json& properties, Context& context);

protected:
    std::unordered_map<const void*, std::unique_ptr<BaseProperty>> mProperties;
    std::vector<const void*> mPropertiesInsertionOrdering;
    std::unordered_set<std::string> mRegisteredPropertyNames;
};
} // namespace ReliveAPI
