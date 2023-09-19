#pragma once

#include "TypesCollectionBase.hpp"
#include "TlvFactory.hpp"

#include <memory>
#include <string>

#include "../../relive_lib/data_conversion/PathTlvsAE.hpp"

namespace ReliveAPI {
class TlvObjectBase;

class TypesCollectionRelive final : public TypesCollectionBase
{
public:
    TypesCollectionRelive();

    void AddTlvsToJsonArray(jsonxx::Array& array) override;

    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvRelive(ReliveTypes tlvType, relive::Path_TLV* pTlv, s32 instanceCount);
    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvRelive(const std::string& tlvTypeName, relive::Path_TLV* pTlv);

    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvFromString(const std::string& tlvTypeName) override;

private:
    void AddReliveTypes();

    TlvFactory<ReliveTypes, relive::Path_TLV> mTlvFactoryRelive;
};
} // namespace ReliveAPI
