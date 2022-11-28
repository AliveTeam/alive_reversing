#pragma once

#include "TlvObjectBase.hpp"

#include "../../AliveLibAO/Path.hpp"

#include "../../AliveLibCommon/Types.hpp"

#include <string>
#include <utility>
#include <vector>

namespace jsonxx {
class Object;
}

namespace ReliveAPI {
class TlvObjectBaseAO : public TlvObjectBase
{
public:
    using CopyFn = void (*)(AO::Path_TLV* dst, const AO::Path_TLV* src);
    using InitFn = void (*)(AO::Path_TLV* dst);

    // Ctor used only to get "typeName"
    TlvObjectBaseAO(std::size_t sizeOfT, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pSelfTlv);

    TlvObjectBaseAO(std::size_t sizeOfT, TypesCollectionBase& globalTypes, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pSelfTlv);

    void InstanceFromJsonBase(const jsonxx::Object& obj) override;
    void InstanceToJsonBase(jsonxx::Object& ret) override;

    TlvObjectBaseAO(const TlvObjectBaseAO&) = delete;
    TlvObjectBaseAO(TlvObjectBaseAO&&) = delete;

    [[nodiscard]] s16 TlvLen() const override;
    [[nodiscard]] std::vector<u8> GetTlvData(bool setTerminationFlag) override;
    [[nodiscard]] AO::TlvTypes TlvType() const;

protected:
    void ConvertXYPos();

    const std::size_t mSizeOfT;
    const AO::TlvTypes mType;
    AO::Path_TLV* const mPSelfTlv;
};
} // namespace ReliveAPI
