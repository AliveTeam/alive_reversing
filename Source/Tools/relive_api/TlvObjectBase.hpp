#pragma once

#include "TypesCollectionAO.hpp"
#include "TypedProperty.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAO/Map.hpp"
#include "relive_api.hpp"
#include "PropertyCollection.hpp"
#include <string>

#define ADD_PROP(name, type) AddProperty(name, globalTypes.TypeName(typeid(type)), &type)

#define WRITE_BASIC(field) WriteBasicType(field, properties)
#define WRITE_ENUMS(field) WriteEnumValue(types, properties, field)

#define READ_BASIC(field) ReadBasicType(field, properties)
#define READ_ENUMS(field) ReadEnumValue(types, field, properties)

#define ADD(name, prop)  AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), &prop, true)
#define ADD_HIDDEN(name, prop)  AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), &prop, false)

#define COPY_TLV() if (pTlv) { mTlv = *reinterpret_cast<decltype(&mTlv)>(pTlv); }

class TlvObjectBase : public PropertyCollection
{
public:
    TlvObjectBase(const std::string& typeName)
        : mStructTypeName(typeName)
    {

    }

    virtual void AddTypes(TypesCollectionBase& /*types*/)
    {
        // Default empty to prevent having to explicitly implement in every TLV wrapper
    }

    virtual s16 TlvLen() const = 0;
    virtual std::vector<u8> GetTlvData(bool setTerminationFlag) = 0;

    void SetInstanceNumber(s32 instanceNumber)
    {
        mInstanceNumber = instanceNumber;
    }

    std::string Name() const
    {
        return mStructTypeName;
    }

    jsonxx::Object StructureToJson()
    {
        jsonxx::Object ret;
        ret << "name" << Name();
        ret << "enum_and_basic_type_properties" << PropertiesToJson();
        return ret;
    }

    void InstanceFromJson(TypesCollectionBase& types, jsonxx::Object& obj)
    {
        jsonxx::Object properties = obj.get<jsonxx::Object>("properties");
        PropertiesFromJson(types, properties);
        InstanceFromJsonBase(obj);
    }

    jsonxx::Object InstanceToJson(TypesCollectionBase& types)
    {
        jsonxx::Object ret;
        InstanceToJsonBase(ret);

        jsonxx::Object properties;
        PropertiesToJson(types, properties);
        ret << "properties" << properties;

        return ret;
    }

    virtual void InstanceFromJsonBase(jsonxx::Object& obj) = 0;
    virtual void InstanceToJsonBase(jsonxx::Object& ret) = 0;

    s32 InstanceNumber() const
    {
        return mInstanceNumber;
    }

protected:
    std::string mStructTypeName;
    s32 mInstanceNumber = 0;
};
