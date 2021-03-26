#pragma once


class BaseProperty
{
public:
    BaseProperty(const std::string& name, const std::string& typeName, bool isVisibleToEditor)
        : m_name(name), m_TypeName(typeName), m_isVisibleToEditor(isVisibleToEditor) {}
    virtual ~BaseProperty() {}
    virtual void Read(TlvObjectBase& tlvObjBase, TypesCollection& types, jsonxx::Object& properties) = 0;
    virtual void Write(TlvObjectBase& tlvObjBase, TypesCollection& types, jsonxx::Object& properties) = 0;

    std::string TypeName() const
    {
        return m_TypeName;
    }

    bool IsVisibleToEditor() const
    {
        return m_isVisibleToEditor;
    }

    std::string Name() const
    {
        return m_name;
    }
private:
    std::string m_name;
    std::string m_TypeName;
    bool m_isVisibleToEditor = true;
};
