#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "TlvObjectBase.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAE/Path.hpp"

TypesCollectionBase::TypesCollectionBase()
{
    #define ADD_BASIC_TYPE(name, type) AddBasicType<type>(name, std::numeric_limits<type>::min(), std::numeric_limits<type>::max());

    ADD_BASIC_TYPE("Byte", u8);
    ADD_BASIC_TYPE("UInt16", u16);
    ADD_BASIC_TYPE("Uint32", u32);
    ADD_BASIC_TYPE("SInt16", s16);
    ADD_BASIC_TYPE("SInt32", s32);

    #undef ADD_BASIC_TYPE
}


TypesCollectionBase::~TypesCollectionBase()
{

}

void TypesCollectionAO::AddTlvsToJsonArray(jsonxx::Array& array)
{
    mTlvFactoryAO.AddTlvsToJsonArray(*this, array);
}

void TypesCollectionAE::AddTlvsToJsonArray(jsonxx::Array& array)
{
    mTlvFactoryAE.AddTlvsToJsonArray(*this, array);
}

std::unique_ptr<TlvObjectBase> TypesCollectionAE::MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv, s32 instanceCount)
{
    return mTlvFactoryAE.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

std::unique_ptr<TlvObjectBase> TypesCollectionAO::MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv, s32 instanceCount)
{
    return mTlvFactoryAO.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

std::unique_ptr<TlvObjectBase> TypesCollectionAE::MakeTlvAE(const std::string& tlvTypeName, Path_TLV* pTlv)
{
    return mTlvFactoryAE.MakeTlvByName(*this, tlvTypeName, pTlv);
}

TypesCollectionAE::TypesCollectionAE()
{
    AddAETypes();
}

std::unique_ptr<TlvObjectBase> TypesCollectionAE::MakeTlvFromString(const std::string& tlvTypeName)
{
    return mTlvFactoryAE.MakeTlvByName(*this, tlvTypeName, nullptr);
}

std::unique_ptr<TlvObjectBase> TypesCollectionAO::MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv)
{
    return mTlvFactoryAO.MakeTlvByName(*this, tlvTypeName, pTlv);
}

TypesCollectionAO::TypesCollectionAO()
{
    AddAOTypes();
}

std::unique_ptr<TlvObjectBase> TypesCollectionAO::MakeTlvFromString(const std::string& tlvTypeName)
{
    return mTlvFactoryAO.MakeTlvByName(*this, tlvTypeName, nullptr);
}
