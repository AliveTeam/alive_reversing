#pragma once

#include "BaseProperty.hpp"

#include <string>

namespace jsonxx {
class Object;
}

namespace ReliveAPI {
class TypesCollectionBase;
class PropertyCollection;

template <typename T>
class TypedProperty final : public BaseProperty
{
public:
    TypedProperty(const std::string& name, const std::string& typeName, bool isVisibleToEditor, T* data)
        : BaseProperty(name, typeName, isVisibleToEditor)
        , m_data(data)
    { }

    void Read(const PropertyCollection& propertyCollection, const TypesCollectionBase& types, const jsonxx::Object& properties, Context& context) override;

    void Write(const PropertyCollection& propertyCollection, const TypesCollectionBase& types, jsonxx::Object& properties, Context& context) override;

private:
    T* m_data = nullptr;
};
} // namespace ReliveAPI
