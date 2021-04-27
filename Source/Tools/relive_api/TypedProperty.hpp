#pragma once

#include "BaseProperty.hpp"

#include <string>

namespace jsonxx {
class Object;
}

class TypesCollectionBase;
class PropertyCollection;

template <typename T>
class TypedProperty : public BaseProperty
{
public:
    TypedProperty(const std::string& name, const std::string& typeName, bool isVisibleToEditor, T* data)
        : BaseProperty(name, typeName, isVisibleToEditor)
        , m_data(data)
    { }

    void Read(const PropertyCollection& propertyCollection, const TypesCollectionBase& types, const jsonxx::Object& properties) override;

    void Write(const PropertyCollection& propertyCollection, const TypesCollectionBase& types, jsonxx::Object& properties) override;

private:
    T* m_data = nullptr;
};
