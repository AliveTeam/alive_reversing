#pragma once

#include <string>

namespace jsonxx { class Object; }

class TypesCollectionBase;
class PropertyCollection;

class BaseProperty
{
public:
    BaseProperty(const std::string& name, const std::string& typeName, bool isVisibleToEditor);
    virtual ~BaseProperty();

    virtual void Read(PropertyCollection& propertyCollection, TypesCollectionBase& types, jsonxx::Object& properties) = 0;
    virtual void Write(PropertyCollection& propertyCollection, TypesCollectionBase& types, jsonxx::Object& properties) = 0;

    [[nodiscard]] const std::string& Name() const;
    [[nodiscard]] const std::string& TypeName() const;
    [[nodiscard]] bool IsVisibleToEditor() const;


private:
    std::string m_name;
    std::string m_typeName;
    bool m_isVisibleToEditor = true;
};
