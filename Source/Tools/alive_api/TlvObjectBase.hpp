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

#define ADD(name, prop)  AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), &prop, true)
#define ADD_HIDDEN(name, prop)  AddProperty<decltype(prop)>(name, globalTypes.TypeName(typeid(prop)), &prop, false)

#define COPY_TLV() if (pTlv) { mTlv = *reinterpret_cast<decltype(&mTlv)>(pTlv); }

class BaseProperty;


template<typename T>
class TypedProperty : public BaseProperty
{
public:
    TypedProperty(const std::string& name, const std::string& typeName, bool isVisibleToEditor, T* data)
        : BaseProperty(name, typeName, isVisibleToEditor), m_data(data) { }

    void Read(PropertyCollection& propertyCollection, TypesCollection& types, jsonxx::Object& properties) override;

    void Write(PropertyCollection& propertyCollection, TypesCollection& types, jsonxx::Object& properties) override;

private:
    T* m_data = nullptr;
};

class PropertyCollection
{
public:
    virtual ~PropertyCollection() { }

    template<typename PropertyType>
    void AddProperty(const std::string& name, const std::string& typeName, PropertyType* key, bool visibleInEditor)
    {
        if (name.empty())
        {
            abort();
        }

        if (typeName.empty())
        {
            abort();
        }

        for (const auto& [keyIt, valueIt] : mProperties)
        {
            if (keyIt == key)
            {
                abort(); // dup key
            }

            if (name == valueIt->Name())
            {
                abort(); // dup prop name
            }
        }

        mProperties[key] = std::make_unique<TypedProperty<PropertyType>>(name, typeName, visibleInEditor, key);
    }

    std::string PropType(void* key) const
    {
        auto it = mProperties.find(key);
        if (it == std::end(mProperties))
        {
            abort();
        }
        return it->second->TypeName();
    }

    jsonxx::Array PropertiesToJson() const
    {
        jsonxx::Array ret;
        for (const auto& [key, value] : mProperties)
        {
            jsonxx::Object property;
            property << "Type" << value->TypeName();
            property << "Visible" << value->IsVisibleToEditor();
            property << "name" << value->Name();
            ret << property;
        }
        return ret;
    }

    std::string PropName(void* key) const
    {
        auto it = mProperties.find(key);
        if (it == std::end(mProperties))
        {
            abort();
        }
        return it->second->Name();
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
        field = static_cast<T>(properties.get<jsonxx::Number>(PropName(&field)));
    }

    template<class T>
    void WriteBasicType(T& field, jsonxx::Object& properties)
    {
        properties << PropName(&field) << static_cast<int>(field);
    }

    void PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties);
    void PropertiesToJson(TypesCollection& types, jsonxx::Object& properties);

protected:
    std::map<void*, std::unique_ptr<BaseProperty>> mProperties;
};

class TlvObjectBase : public PropertyCollection
{
public:
    TlvObjectBase(const std::string& typeName)
        : mStructTypeName(typeName)
    {

    }

    virtual void AddTypes(TypesCollection& /*types*/)
    {
        // Default empty to prevent having to explicitly implement in every TLV wrapper
    }

    virtual __int16 TlvLen() const = 0;
    virtual std::vector<BYTE> GetTlvData(bool setTerminationFlag) = 0;

    void SetInstanceNumber(int instanceNumber)
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

    void InstanceFromJson(TypesCollection& types, jsonxx::Object& obj)
    {
        jsonxx::Object properties = obj.get<jsonxx::Object>("properties");
        PropertiesFromJson(types, properties);
        InstanceFromJsonBase(obj);
    }

    jsonxx::Object InstanceToJson(TypesCollection& types)
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

    int InstanceNumber() const
    {
        return mInstanceNumber;
    }

protected:
    std::string mStructTypeName;
    int mInstanceNumber = 0;
};

template <class T>
void TypedProperty<T>::Read(PropertyCollection& propertyCollection, TypesCollection& types, jsonxx::Object& properties)
{
    if constexpr (std::is_enum<T>::value)
    {
        propertyCollection.ReadEnumValue(types, *m_data, properties);
    }
    else
    {
        propertyCollection.ReadBasicType(*m_data, properties);
    }
}

template <class T>
void TypedProperty<T>::Write(PropertyCollection& propertyCollection, TypesCollection& types, jsonxx::Object& properties)
{
    if constexpr (std::is_enum<T>::value)
    {
        propertyCollection.WriteEnumValue(types, properties, *m_data);
    }
    else
    {
        propertyCollection.WriteBasicType(*m_data, properties);
    }
}


template<class T>
class TlvObjectBaseAE : public TlvObjectBase
{
public:
    // Used only to get "typeName"
    TlvObjectBaseAE(TlvTypes tlvType, const std::string& typeName)
        : TlvObjectBase(typeName), mType(tlvType)
    {

    }

    TlvObjectBaseAE(TypesCollection& globalTypes, TlvTypes tlvType, const std::string& typeName, Path_TLV* pTlv)
        : TlvObjectBase(typeName), mType(tlvType)
    {
        mTlv.field_2_length = sizeof(T);
        mTlv.field_4_type.mType = mType;
        COPY_TLV();

        if (mTlv.field_C_bottom_right.field_0_x - mTlv.field_8_top_left.field_0_x < 0 ||
            mTlv.field_C_bottom_right.field_2_y - mTlv.field_8_top_left.field_2_y < 0)
        {
            abort();
        }

        ADD("xpos", mTlv.field_8_top_left.field_0_x);
        ADD("ypos", mTlv.field_8_top_left.field_2_y);

        mTlv.field_C_bottom_right.field_0_x -= mTlv.field_8_top_left.field_0_x;
        mTlv.field_C_bottom_right.field_2_y -= mTlv.field_8_top_left.field_2_y;

        ADD("width", mTlv.field_C_bottom_right.field_0_x);
        ADD("height", mTlv.field_C_bottom_right.field_2_y);
    }

    void InstanceFromJsonBase(jsonxx::Object& obj) override
    {
        mStructTypeName = obj.get<std::string>("name");

        mTlv.field_C_bottom_right.field_0_x += mTlv.field_8_top_left.field_0_x;
        mTlv.field_C_bottom_right.field_2_y += mTlv.field_8_top_left.field_2_y;
    }

    void InstanceToJsonBase(jsonxx::Object& ret) override
    {
        ret << "name" << Name() + "_" + std::to_string(mInstanceNumber);
        ret << "object_structures_type" << Name();
    }

    __int16 TlvLen() const override
    {
        return static_cast<__int16>(sizeof(T));
    }

    std::vector<BYTE> GetTlvData(bool setTerminationFlag) override
    {
        std::vector<BYTE> ret(sizeof(T));
        mTlv.field_0_flags.Set(TLV_Flags::eBit3_End_TLV_List, setTerminationFlag);
        memcpy(ret.data(), &mTlv, sizeof(T));
        return ret;
    }

    TlvTypes TlvType() const
    {
        return mType;
    }

protected:
    const TlvTypes mType = {};
    T mTlv = {};
};


template<class T>
class TlvObjectBaseAO : public TlvObjectBase
{
public:
    // Ctor used only to get "typeName"
    TlvObjectBaseAO(AO::TlvTypes tlvType, const std::string& typeName)
         : TlvObjectBase(typeName), mType(tlvType), mBase(&mTlv)
    {

    }

    TlvObjectBaseAO(TypesCollection& globalTypes, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pTlv)
        : TlvObjectBase(typeName), mType(tlvType), mBase(&mTlv)
    {
        mTlv.field_4_type.mType = mType;
        mTlv.field_2_length = sizeof(T);
        COPY_TLV();

        if (mBase->field_14_bottom_right.field_0_x - mBase->field_10_top_left.field_0_x < 0 ||
            mBase->field_14_bottom_right.field_2_y - mBase->field_10_top_left.field_2_y < 0)
        {
            abort();
        }

        ADD("xpos", mBase->field_10_top_left.field_0_x);
        ADD("ypos", mBase->field_10_top_left.field_2_y);

        mBase->field_14_bottom_right.field_0_x -= mBase->field_10_top_left.field_0_x;
        mBase->field_14_bottom_right.field_2_y -= mBase->field_10_top_left.field_2_y;

        ADD("width", mBase->field_14_bottom_right.field_0_x);
        ADD("height", mBase->field_14_bottom_right.field_2_y);
    }

    void InstanceFromJsonBase(jsonxx::Object& obj) override
    {
        mStructTypeName = obj.get<std::string>("name");

        mBase->field_14_bottom_right.field_0_x += mBase->field_10_top_left.field_0_x;
        mBase->field_14_bottom_right.field_2_y += mBase->field_10_top_left.field_2_y;

        mBase->field_C_sound_pos.field_0_x = mBase->field_10_top_left.field_0_x;
        mBase->field_C_sound_pos.field_2_y = mBase->field_10_top_left.field_2_y;
    }

    void InstanceToJsonBase(jsonxx::Object& ret) override
    {
        ret << "name" << Name() + "_" + std::to_string(mInstanceNumber);
        ret << "object_structures_type" << Name();
    }

    __int16 TlvLen() const override
    {
        return static_cast<__int16>(sizeof(T));
    }

    std::vector<BYTE> GetTlvData(bool setTerminationFlag) override
    {
        std::vector<BYTE> ret(sizeof(T));
        mTlv.field_0_flags.Set(AO::TLV_Flags::eBit3_End_TLV_List, setTerminationFlag);
        memcpy(ret.data(), &mTlv, sizeof(T));
        return ret;
    }

    AO::TlvTypes TlvType() const
    {
        return mType;
    }

protected:
    const AO::TlvTypes mType = {};
    T mTlv = {};
    AO::Path_TLV* mBase = nullptr;
};
