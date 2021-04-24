#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "TlvObjectBase.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAE/Path.hpp"

TypesCollection::TypesCollection(Game gameType)
    : mGameType(gameType)
{
    #define ADD_BASIC_TYPE(name, type) AddBasicType<type>(name, std::numeric_limits<type>::min(), std::numeric_limits<type>::max());

    ADD_BASIC_TYPE("Byte", BYTE);
    ADD_BASIC_TYPE("UInt16", unsigned short);
    ADD_BASIC_TYPE("Uint32", u32);
    ADD_BASIC_TYPE("SInt16", signed short);
    ADD_BASIC_TYPE("SInt32", signed int);

    #undef ADD_BASIC_TYPE

    if (mGameType == Game::AO)
    {
        AddAOTypes();
    }
    else
    {
        AddAETypes();
    }
}

void TypesCollection::AddTlvsToJsonArray(jsonxx::Array& array)
{
    if (mGameType == Game::AO)
    {
        mTlvFactoryAO.AddTlvsToJsonArray(*this, array);
    }
    else
    {
        mTlvFactoryAE.AddTlvsToJsonArray(*this, array);
    }
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv, int instanceCount)
{
    return mTlvFactoryAE.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv, int instanceCount)
{
    return mTlvFactoryAO.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAE(const std::string& tlvTypeName, Path_TLV* pTlv)
{
    return mTlvFactoryAE.MakeTlvByName(*this, tlvTypeName, pTlv);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv)
{
    return mTlvFactoryAO.MakeTlvByName(*this, tlvTypeName, pTlv);
}
