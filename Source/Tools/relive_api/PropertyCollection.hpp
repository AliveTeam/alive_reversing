#pragma once

#include "relive_api.hpp"
#include "TypesCollectionBase.hpp"
#include "BaseProperty.hpp"
#include "TypedProperty.hpp"

#include <jsonxx/jsonxx.h>

#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>

class PropertyCollection
{
private:
    void ThrowOnAddPropertyError(const std::string& name, const std::string& typeName, void* key);

public:
    virtual ~PropertyCollection();

    template<typename PropertyType>
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

    template<class T>
    void ReadEnumValue(const TypesCollectionBase& types, T& field, const jsonxx::Object& properties) const
    {
        const std::string& propName = PropName(&field);
        const std::string& propType = PropType(&field);

        if (!properties.has<std::string>(propName))
        {
            LOG_ERROR("Missing json property " << propName);
        }

        const std::string jsonValue = properties.get<std::string>(propName);
        field = types.EnumValueFromString<T>(propType, jsonValue);
    }

    template<class T>
    void WriteEnumValue(const TypesCollectionBase& types, jsonxx::Object& properties, const T& field) const
    {
        properties << PropName(&field) << types.EnumValueToString<T>(field);
    }

    template<class T>
    void ReadBasicType(T& field, const jsonxx::Object& properties) const
    {
        field = static_cast<T>(properties.get<jsonxx::Number>(PropName(&field)));
    }

    template<class T>
    void WriteBasicType(const T& field, jsonxx::Object& properties) const
    {
        properties << PropName(&field) << static_cast<s32>(field);
    }

    void PropertiesFromJson(TypesCollectionBase& types, jsonxx::Object& properties);
    void PropertiesToJson(TypesCollectionBase& types, jsonxx::Object& properties);

protected:
    std::map<void*, std::unique_ptr<BaseProperty>> mProperties;
    std::unordered_set<std::string> mRegisteredPropertyNames;
};
