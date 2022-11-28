#pragma once

#include "TypesCollectionBase.hpp"
#include "BaseProperty.hpp"
#include "TypedProperty.hpp"
#include "ApiContext.hpp"

#include <jsonxx/jsonxx.h>

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

    [[nodiscard]] jsonxx::Array PropertiesToJson() const;

    template <class T>
    void ReadEnumValue(const TypesCollectionBase& types, T& field, const jsonxx::Object& properties, Context& context) const
    {
        const std::string& propName = PropName(&field);
        const std::string& propType = PropType(&field);

        if (!properties.has<std::string>(propName))
        {
            LOG_ERROR("Missing json property %s", propName.c_str());
            context.MissingEnumType(propType, propName);
        }

        field = types.EnumValueFromString<T>(propType, properties.get<std::string>(propName), context);
    }

    template <class T>
    void WriteEnumValue(const TypesCollectionBase& types, jsonxx::Object& properties, const T& field, Context& context) const
    {
        properties << PropName(&field) << types.EnumValueToString<T>(field, context);
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

    void PropertiesFromJson(const TypesCollectionBase& types, const jsonxx::Object& properties, Context& context);
    void PropertiesToJson(const TypesCollectionBase& types, jsonxx::Object& properties, Context& context);

protected:
    std::unordered_map<const void*, std::unique_ptr<BaseProperty>> mProperties;
    std::vector<const void*> mPropertiesInsertionOrdering;
    std::unordered_set<std::string> mRegisteredPropertyNames;
};
} // namespace ReliveAPI
