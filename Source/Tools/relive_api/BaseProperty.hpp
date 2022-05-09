#pragma once

#include <string>

namespace jsonxx {
class Object;
}


namespace ReliveAPI {
class TypesCollectionBase;
class PropertyCollection;
class Context;

class BaseProperty
{
public:
    BaseProperty(const std::string& name, const std::string& typeName, const char* idStr, bool isVisibleToEditor);
    virtual ~BaseProperty();

    virtual void Read(const PropertyCollection& propertyCollection, const TypesCollectionBase& types, const jsonxx::Object& properties, Context& context) = 0;
    virtual void Write(const PropertyCollection& propertyCollection, const TypesCollectionBase& types, jsonxx::Object& properties, Context& context) = 0;

    [[nodiscard]] const std::string& Name() const;
    [[nodiscard]] const std::string& TypeName() const;
    [[nodiscard]] bool IsVisibleToEditor() const;
    [[nodiscard]] const char* IdStr() const;

private:
    std::string m_name;
    std::string m_typeName;
    const char* m_idStr = nullptr;
    bool m_isVisibleToEditor = true;
};
} // namespace ReliveAPI
