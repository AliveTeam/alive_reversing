#pragma once

#include "TypesCollectionBase.hpp"
#include "TlvFactory.hpp"

#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/Map.hpp"

#include <memory>
#include <string>

class TlvObjectBase;

class TypesCollectionAO final : public TypesCollectionBase
{
public:
    TypesCollectionAO();

    void AddTlvsToJsonArray(jsonxx::Array& array) override;

    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv, s32 instanceCount);
    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv);

    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvFromString(const std::string& tlvTypeName) override;

private:
    void AddAOTypes();

    TlvFactory<AO::TlvTypes, AO::Path_TLV> mTlvFactoryAO;
};
