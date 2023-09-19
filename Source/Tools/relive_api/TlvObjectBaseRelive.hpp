#pragma once

#include "TlvObjectBase.hpp"

#include "../../relive_lib/Types.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "../../relive_lib/data_conversion/relive_tlvs.hpp"

namespace jsonxx {
class Object;
}

namespace ReliveAPI {
class TlvObjectBaseRelive : public TlvObjectBase
{
public:
    // Used only to get "typeName"
    TlvObjectBaseRelive(std::size_t sizeOfT, ReliveTypes tlvType, const std::string& typeName, relive::Path_TLV* pSelfTlv);

    TlvObjectBaseRelive(std::size_t sizeOfT, TypesCollectionBase& globalTypes, ReliveTypes tlvType, const std::string& typeName, relive::Path_TLV* pSelfTlv);

    TlvObjectBaseRelive(const TlvObjectBaseRelive&) = delete;
    TlvObjectBaseRelive(TlvObjectBaseRelive&&) = delete;

    void InstanceFromJsonBase(const jsonxx::Object& obj) override;
    void InstanceToJsonBase(jsonxx::Object& ret) override;

    [[nodiscard]] s16 TlvLen() const override;
    [[nodiscard]] std::vector<u8> GetTlvData(bool setTerminationFlag) override;
    [[nodiscard]] ReliveTypes TlvType() const;

protected:
    void ConvertXYPos();

    const std::size_t mSizeOfT;
    const ReliveTypes mType;
    relive::Path_TLV* const mPSelfTlv;
};
} // namespace ReliveAPI
