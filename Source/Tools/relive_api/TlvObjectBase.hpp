#pragma once

#include "TypesCollectionBase.hpp"
#include "TypedProperty.hpp"
#include "PropertyCollection.hpp"

#include <string>
#include <type_traits>
#include <vector>

// #define ADD_PROP(name, type) AddProperty(name, globalTypes.TypeName(typeid(type)), &type)

// #define WRITE_BASIC(field) WriteBasicType(field, properties)
// #define WRITE_ENUMS(field) WriteEnumValue(types, properties, field)

// #define READ_BASIC(field) ReadBasicType(field, properties)
// #define READ_ENUMS(field) ReadEnumValue(types, field, properties)

#define TLVOBJECTBASE_ADD(name, prop)  AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), &prop, true)
#define TLVOBJECTBASE_ADD_HIDDEN(name, prop)  AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), &prop, false)

#define TLVOBJECTBASE_COPY_TLV() if (pTlv) { mTlv = *reinterpret_cast<decltype(&mTlv)>(pTlv); }

// Defined here as it depends on a full definition of PropertyCollection
template <class T>
void TypedProperty<T>::Read(PropertyCollection& propertyCollection, TypesCollectionBase& types, jsonxx::Object& properties)
{
    if constexpr (std::is_enum_v<T>)
    {
        propertyCollection.ReadEnumValue(types, *m_data, properties);
    }
    else
    {
        propertyCollection.ReadBasicType(*m_data, properties);
        (void)types; // statically compiled out in this branch
    }
}

template <class T>
void TypedProperty<T>::Write(PropertyCollection& propertyCollection, TypesCollectionBase& types, jsonxx::Object& properties)
{
    if constexpr (std::is_enum_v<T>)
    {
        propertyCollection.WriteEnumValue(types, properties, *m_data);
    }
    else
    {
        propertyCollection.WriteBasicType(*m_data, properties);
        (void)types; // statically compiled out in this branch
    }
}

class TlvObjectBase : public PropertyCollection
{
public:
    TlvObjectBase(const std::string& typeName);

    virtual void AddTypes(TypesCollectionBase& /*types*/);

    virtual s16 TlvLen() const = 0;
    virtual std::vector<u8> GetTlvData(bool setTerminationFlag) = 0;

    void SetInstanceNumber(s32 instanceNumber);

    [[nodiscard]] const std::string& Name() const;
    [[nodiscard]] jsonxx::Object StructureToJson();

    void InstanceFromJson(TypesCollectionBase& types, jsonxx::Object& obj);
    [[nodiscard]]jsonxx::Object InstanceToJson(TypesCollectionBase& types);

    virtual void InstanceFromJsonBase(jsonxx::Object& obj) = 0;
    virtual void InstanceToJsonBase(jsonxx::Object& ret) = 0;

    [[nodiscard]] s32 InstanceNumber() const;

protected:
    std::string mStructTypeName;
    s32 mInstanceNumber = 0;
};
