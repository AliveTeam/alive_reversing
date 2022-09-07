#pragma once

#include "TypesCollectionBase.hpp"
#include "TlvFactory.hpp"

#include <memory>
#include <string>

#include "../../relive_lib/data_conversion/PathTlvsAE.hpp"

namespace ReliveAPI {
class TlvObjectBase;

class TypesCollectionAE final : public TypesCollectionBase
{
public:
    TypesCollectionAE();

    void AddTlvsToJsonArray(jsonxx::Array& array) override;

    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv, s32 instanceCount);
    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvAE(const std::string& tlvTypeName, Path_TLV* pTlv);

    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvFromString(const std::string& tlvTypeName) override;

private:
    void AddAETypes();

    TlvFactory<TlvTypes, Path_TLV> mTlvFactoryAE;
};
} // namespace ReliveAPI
