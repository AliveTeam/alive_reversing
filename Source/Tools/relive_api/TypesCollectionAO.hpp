#pragma once

#include "TypesCollectionBase.hpp"
#include "TlvFactory.hpp"

class TypesCollectionAO final : public TypesCollectionBase
{
public:
    TypesCollectionAO();
    void AddTlvsToJsonArray(jsonxx::Array& array) override;
    std::unique_ptr<TlvObjectBase> MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv, s32 instanceCount);
    std::unique_ptr<TlvObjectBase> MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv);

    std::unique_ptr<TlvObjectBase> MakeTlvFromString(const std::string& tlvTypeName) override;
private:
    void AddAOTypes();
    TlvFactory<AO::TlvTypes, AO::Path_TLV> mTlvFactoryAO;
};
