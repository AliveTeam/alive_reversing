#pragma once

#include "TypesCollection.hpp"
#include "TypedProperty.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAO/Map.hpp"
#include <string>

#define ADD_PROP(name, type) AddProperty(name, globalTypes.TypeName(typeid(type)), &type)

#define WRITE_BASIC(field) WriteBasicType(field, properties)
#define WRITE_ENUMS(field) WriteEnumValue(types, properties, field)

#define READ_BASIC(field) ReadBasicType(field, properties)
#define READ_ENUMS(field) ReadEnumValue(types, field, properties)

#define ADD(name, prop)  AddProperty(name, globalTypes.TypeName(typeid(prop)), &prop); mProperties.push_back(std::make_unique<TypedProperty<decltype(prop)>>(name, &prop));

#define COPY_TLV() if (pTlv) { mTlv = *reinterpret_cast<decltype(&mTlv)>(pTlv); }

class BaseProperty;


template< typename T >
class TypedProperty : public BaseProperty
{
public:
    TypedProperty(const std::string& name, T* data) : BaseProperty(name), m_data(data) { }

    void Read(TlvObjectBase& tlvObjBase, TypesCollection& types, jsonxx::Object& properties) override;

    void Write(TlvObjectBase& tlvObjBase, TypesCollection& types, jsonxx::Object& properties) override;

private:
    T* m_data = nullptr;
};

class TlvObjectBase
{
public:
    TlvObjectBase(const std::string& typeName)
        : mName(typeName)
    {

    }

    virtual ~TlvObjectBase() {}


    virtual void AddTypes(TypesCollection& /*types*/)
    {

    }

    std::string Name() const
    {
        return mName;
    }

    void AddProperty(const std::string& name, const std::string& typeName, void* key)
    {
        mInfo[key] = { name, typeName };
    }

    jsonxx::Object PropertiesToJson() const
    {
        jsonxx::Object ret;
        for (const auto& [key, value] : mInfo)
        {
            ret << value.mName << value.mTypeName;
        }
        return ret;
    }

    std::string PropName(void* key) const
    {
        auto it = mInfo.find(key);
        if (it == std::end(mInfo))
        {
            abort();
        }
        return it->second.mName;
    }

    std::string PropType(void* key) const
    {
        auto it = mInfo.find(key);
        if (it == std::end(mInfo))
        {
            abort();
        }
        return it->second.mTypeName;
    }

    template<class T>
    void ReadEnumValue(TypesCollection& types, T& field, jsonxx::Object& properties)
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
    void WriteEnumValue(TypesCollection& types, jsonxx::Object& properties, T& field)
    {
        properties << PropName(&field) << types.EnumValueToString<T>(field);
    }

    template<class T>
    void ReadBasicType(T& field, jsonxx::Object& properties)
    {
        field = properties.get<jsonxx::Number>(PropName(&field));
    }

    template<class T>
    void WriteBasicType(T& field, jsonxx::Object& properties)
    {
        properties << PropName(&field) << static_cast<int>(field);
    }

    jsonxx::Object StructureToJson()
    {
        jsonxx::Object ret;
        ret << "name" << Name();
        ret << "enum_and_basic_type_properties" << PropertiesToJson();
        return ret;
    }

    void InstanceFromJson(TypesCollection& types, jsonxx::Object& obj)
    {
        InstanceFromJsonBase(obj);
        jsonxx::Object properties = obj.get<jsonxx::Object>("properties");
        PropertiesFromJson(types, properties);
    }

    void PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties);

    jsonxx::Object InstanceToJson(TypesCollection& types)
    {
        jsonxx::Object ret;
        InstanceToJsonBase(ret);

        jsonxx::Object properties;
        PropertiesToJson(types, properties);
        ret << "properties" << properties;
        return ret;
    }

    void PropertiesToJson(TypesCollection& types, jsonxx::Object& properties);

    virtual void InstanceFromJsonBase(jsonxx::Object& obj) = 0;
    virtual void InstanceToJsonBase(jsonxx::Object& ret) = 0;

protected:
    struct PropertyInfo
    {
        std::string mName;
        std::string mTypeName;
    };
    std::map<void*, PropertyInfo> mInfo; // TODO: Combine with mProperties
    std::vector<std::unique_ptr<BaseProperty>> mProperties;
    std::string mName;
    std::string mDescription;
};

template <class T>
void TypedProperty<T>::Read(TlvObjectBase& tlvObjBase, TypesCollection& types, jsonxx::Object& properties)
{
    if constexpr (std::is_enum<T>::value)
    {
        tlvObjBase.ReadEnumValue(types, *m_data, properties);
    }
    else
    {
        tlvObjBase.ReadBasicType(*m_data, properties);
    }
}

template <class T>
void TypedProperty<T>::Write(TlvObjectBase& tlvObjBase, TypesCollection& types, jsonxx::Object& properties)
{
    if constexpr (std::is_enum<T>::value)
    {
        tlvObjBase.WriteEnumValue(types, properties, *m_data);
    }
    else
    {
        tlvObjBase.WriteBasicType(*m_data, properties);
    }
}


template<class T>
class TlvObjectBaseAE : public TlvObjectBase
{
public:
    TlvObjectBaseAE(const std::string& typeName, Path_TLV* pTlv)
        : TlvObjectBase(typeName)
    {
        COPY_TLV();
    }

    void InstanceFromJsonBase(jsonxx::Object& obj) override
    {
        mDescription = obj.get<std::string>("name");

        mTlv.field_8_top_left.field_0_x = obj.get<jsonxx::Number>("xpos");
        mTlv.field_8_top_left.field_2_y = obj.get<jsonxx::Number>("ypos");
        mTlv.field_C_bottom_right.field_0_x = obj.get<jsonxx::Number>("width");
        mTlv.field_C_bottom_right.field_2_y = obj.get<jsonxx::Number>("height");
    }

    void InstanceToJsonBase(jsonxx::Object& ret) override
    {
        ret << "name" << mDescription;

        ret << "xpos" << static_cast<int>(mTlv.field_8_top_left.field_0_x);
        ret << "ypos" << static_cast<int>(mTlv.field_8_top_left.field_2_y);
        ret << "width" << static_cast<int>(mTlv.field_C_bottom_right.field_0_x);
        ret << "height" << static_cast<int>(mTlv.field_C_bottom_right.field_2_y);

        ret << "object_structures_type" << Name();
    }

protected:
    T mTlv;
};


template<class T>
class TlvObjectBaseAO : public TlvObjectBase
{
public:
    TlvObjectBaseAO(const std::string& typeName, AO::Path_TLV* pTlv)
        : TlvObjectBase(typeName), mBase(&mTlv)
    {
        COPY_TLV();
    }

    void InstanceFromJsonBase(jsonxx::Object& obj) override
    {
        mDescription = obj.get<std::string>("name");

        mBase->field_10_top_left.field_0_x = obj.get<jsonxx::Number>("xpos");
        mBase->field_10_top_left.field_2_y = obj.get<jsonxx::Number>("ypos");
        mBase->field_14_bottom_right.field_0_x = obj.get<jsonxx::Number>("width");
        mBase->field_14_bottom_right.field_2_y = obj.get<jsonxx::Number>("height");
    }

    void InstanceToJsonBase(jsonxx::Object& ret) override
    {
        ret << "name" << mDescription;

        ret << "xpos" << static_cast<int>(mBase->field_10_top_left.field_0_x);
        ret << "ypos" << static_cast<int>(mBase->field_10_top_left.field_2_y);
        ret << "width" << static_cast<int>(mBase->field_14_bottom_right.field_0_x);
        ret << "height" << static_cast<int>(mBase->field_14_bottom_right.field_2_y);

        ret << "object_structures_type" << Name();
    }

protected:
    T mTlv = {};
    AO::Path_TLV* mBase = nullptr;
};
