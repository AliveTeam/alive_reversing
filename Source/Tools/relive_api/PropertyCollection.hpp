#pragma once

#include "relive_api.hpp"
#include "TypesCollectionBase.hpp"
#include "BaseProperty.hpp"
#include "TypedProperty.hpp"

#include <jsonxx/jsonxx.h>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace ReliveAPI {
class PropertyCollection
{
private:
    void ThrowOnAddPropertyError(const std::string& name, const std::string& typeName, void* key);

public:
    virtual ~PropertyCollection();

    template <typename PropertyType>
    void AddProperty(const std::string& name, const std::string& typeName, void* key, bool visibleInEditor)
    {
        ThrowOnAddPropertyError(name, typeName, key);

        // Using `std::make_unique` here unfortunately significantly increases compilation time on MinGW + GCC.
        mProperties[key].reset(new TypedProperty<PropertyType>(name, typeName, visibleInEditor, static_cast<PropertyType*>(key)));
        mRegisteredPropertyNames.emplace(name);
    }

    [[nodiscard]] const std::string& PropType(const void* key) const;
    [[nodiscard]] const std::string& PropName(const void* key) const;

    [[nodiscard]] jsonxx::Array PropertiesToJson() const;

    template <class T>
    void ReadEnumValue(const TypesCollectionBase& types, T& field, const jsonxx::Object& properties) const
    {
        const std::string& propName = PropName(&field);
        const std::string& propType = PropType(&field);

        if (!properties.has<std::string>(propName))
        {
            LOG_ERROR("Missing json property " << propName);
        }

        field = types.EnumValueFromString<T>(propType, properties.get<std::string>(propName));
    }

    template <class T>
    void WriteEnumValue(const TypesCollectionBase& types, jsonxx::Object& properties, const T& field) const
    {
        properties << PropName(&field) << types.EnumValueToString<T>(field);
    }

    template <class T>
    void ReadBasicType(T& field, const jsonxx::Object& properties) const
    {
        field = static_cast<T>(properties.get<jsonxx::Number>(PropName(&field)));
    }

    template <class T>
    void WriteBasicType(const T& field, jsonxx::Object& properties) const
    {
        properties << PropName(&field) << static_cast<s32>(field);
    }

    void PropertiesFromJson(const TypesCollectionBase& types, const jsonxx::Object& properties);
    void PropertiesToJson(const TypesCollectionBase& types, jsonxx::Object& properties);

protected:
    std::unordered_map<const void*, std::unique_ptr<BaseProperty>> mProperties;
    std::unordered_set<std::string> mRegisteredPropertyNames;
};
} // namespace ReliveAPI
