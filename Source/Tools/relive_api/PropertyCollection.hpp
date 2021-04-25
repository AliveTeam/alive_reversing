#pragma once

#include "relive_api.hpp"
#include "TypesCollectionBase.hpp"
#include "BaseProperty.hpp"
#include "TypedProperty.hpp"

#include <jsonxx/jsonxx.h>

#include <string>
#include <map>
#include <memory>

class PropertyCollection
{
public:
    virtual ~PropertyCollection() { }

    template<typename PropertyType>
    void AddProperty(const std::string& name, const std::string& typeName, PropertyType* key, bool visibleInEditor)
    {
        if (name.empty())
        {
            throw ReliveAPI::EmptyPropertyNameException();
        }

        if (typeName.empty())
        {
            throw ReliveAPI::EmptyTypeNameException();
        }

        for (const auto&[keyIt, valueIt] : mProperties)
        {
            if (keyIt == key)
            {
                throw ReliveAPI::DuplicatePropertyKeyException();
            }

            if (name == valueIt->Name())
            {
                throw ReliveAPI::DuplicatePropertyNameException(name.c_str());
            }
        }

        mProperties[key] = std::make_unique<TypedProperty<PropertyType>>(name, typeName, visibleInEditor, key);
    }

    [[nodiscard]] std::string PropType(void* key) const;
    [[nodiscard]] jsonxx::Array PropertiesToJson() const;
    [[nodiscard]] std::string PropName(void* key) const;

    template<class T>
    void ReadEnumValue(TypesCollectionBase& types, T& field, jsonxx::Object& properties)
    {
        const std::string propName = PropName(&field);
        const std::string propType = PropType(&field);

        if (!properties.has<std::string>(propName))
        {
            LOG_ERROR("Missing json property " << propName);
        }

        const std::string jsonValue = properties.get<std::string>(propName);
        field = types.EnumValueFromString<T>(propType, jsonValue);
    }

    template<class T>
    void WriteEnumValue(TypesCollectionBase& types, jsonxx::Object& properties, T& field)
    {
        properties << PropName(&field) << types.EnumValueToString<T>(field);
    }

    template<class T>
    void ReadBasicType(T& field, jsonxx::Object& properties)
    {
        field = static_cast<T>(properties.get<jsonxx::Number>(PropName(&field)));
    }

    template<class T>
    void WriteBasicType(T& field, jsonxx::Object& properties)
    {
        properties << PropName(&field) << static_cast<s32>(field);
    }

    void PropertiesFromJson(TypesCollectionBase& types, jsonxx::Object& properties);
    void PropertiesToJson(TypesCollectionBase& types, jsonxx::Object& properties);

protected:
    std::map<void*, std::unique_ptr<BaseProperty>> mProperties;
};
