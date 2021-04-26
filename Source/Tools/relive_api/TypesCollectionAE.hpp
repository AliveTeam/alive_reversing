#pragma once

#include "TypesCollectionBase.hpp"
#include "TlvFactory.hpp"

class TypesCollectionAE final : public TypesCollectionBase
{
public:
    TypesCollectionAE();

    void AddTlvsToJsonArray(jsonxx::Array& array) override;
    std::unique_ptr<TlvObjectBase> MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv, s32 instanceCount);
    std::unique_ptr<TlvObjectBase> MakeTlvAE(const std::string& tlvTypeName, Path_TLV* pTlv);

    std::unique_ptr<TlvObjectBase> MakeTlvFromString(const std::string& tlvTypeName) override;
private:
    void AddAETypes();
    TlvFactory<TlvTypes, Path_TLV> mTlvFactoryAE;
};
