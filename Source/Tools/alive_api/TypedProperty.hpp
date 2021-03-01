#pragma once


class BaseProperty
{
public:
    BaseProperty(const std::string& name) : m_name(name) {}
    virtual ~BaseProperty() {}
    virtual void Read(TlvObjectBase& tlvObjBase, TypesCollection& types, jsonxx::Object& properties) = 0;
    virtual void Write(TlvObjectBase& tlvObjBase, TypesCollection& types, jsonxx::Object& properties) = 0;
private:
    std::string m_name;
};
