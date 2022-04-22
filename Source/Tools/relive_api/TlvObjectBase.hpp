#pragma once

#include "TypesCollectionBase.hpp"
#include "TypedProperty.hpp"
#include "PropertyCollection.hpp"
#include "ApiContext.hpp"

#include <string>
#include <type_traits>
#include <vector>

enum class AnimId;

namespace ReliveAPI {
class Context;

enum class AddResourceTo
{
    File,           // Resource is a file that needs adding to the lvl archive
    CameraBlock,    // Resource needs adding to the camera as a block
};

// Defined here as it depends on a full definition of PropertyCollection
template <class T>
void TypedProperty<T>::Read(const PropertyCollection& propertyCollection, const TypesCollectionBase& types, const jsonxx::Object& properties, Context& context)
{
    if constexpr (std::is_enum_v<T>)
    {
        propertyCollection.ReadEnumValue(types, *m_data, properties, context);
    }
    else
    {
        propertyCollection.ReadBasicType(*m_data, properties);
        (void) types; // statically compiled out in this branch
        (void) context; // ditto
    }
}

template <class T>
void TypedProperty<T>::Write(const PropertyCollection& propertyCollection, const TypesCollectionBase& types, jsonxx::Object& properties, Context& context)
{
    if constexpr (std::is_enum_v<T>)
    {
        propertyCollection.WriteEnumValue(types, properties, *m_data, context);
    }
    else
    {
        propertyCollection.WriteBasicType(*m_data, properties);
        (void) types; // statically compiled out in this branch
        (void) context; // ditto
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

    void InstanceFromJson(TypesCollectionBase& types, const jsonxx::Object& obj, Context& context);
    [[nodiscard]] jsonxx::Object InstanceToJson(TypesCollectionBase& types, Context& context);

    virtual void InstanceFromJsonBase(const jsonxx::Object& obj) = 0;
    virtual void InstanceToJsonBase(jsonxx::Object& ret) = 0;

    [[nodiscard]] s32 InstanceNumber() const;

    void AddResource(AnimId res, AddResourceTo type);

    struct RequiredResourceRecord final
    {
        AnimId mResource;
        AddResourceTo mAddMeTo;
    };
    
    [[nodiscard]] const std::vector<RequiredResourceRecord>& Resources() const
    {
        return mRequiredResources;
    }

protected:
    std::string mStructTypeName;
    s32 mInstanceNumber = 0;

private:
    std::vector<RequiredResourceRecord> mRequiredResources;
};
} // namespace ReliveAPI
