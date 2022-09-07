#pragma once

#include "TlvObjectBase.hpp"

#include "../../AliveLibAE/Path.hpp"

#include "../../AliveLibCommon/Types.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "../../relive_lib/data_conversion/PathTlvsAE.hpp"

namespace jsonxx {
class Object;
}

namespace ReliveAPI {
class TlvObjectBaseAE : public TlvObjectBase
{
public:
    // Used only to get "typeName"
    TlvObjectBaseAE(std::size_t sizeOfT, TlvTypes tlvType, const std::string& typeName, Path_TLV* pSelfTlv);

    TlvObjectBaseAE(std::size_t sizeOfT, TypesCollectionBase& globalTypes, TlvTypes tlvType, const std::string& typeName, Path_TLV* pSelfTlv);

    TlvObjectBaseAE(const TlvObjectBaseAE&) = delete;
    TlvObjectBaseAE(TlvObjectBaseAE&&) = delete;

    void InstanceFromJsonBase(const jsonxx::Object& obj) override;
    void InstanceToJsonBase(jsonxx::Object& ret) override;

    [[nodiscard]] s16 TlvLen() const override;
    [[nodiscard]] std::vector<u8> GetTlvData(bool setTerminationFlag) override;
    [[nodiscard]] TlvTypes TlvType() const;

protected:
    void ConvertXYPos();

    const std::size_t mSizeOfT;
    const TlvTypes mType;
    Path_TLV* const mPSelfTlv;
};
} // namespace ReliveAPI
