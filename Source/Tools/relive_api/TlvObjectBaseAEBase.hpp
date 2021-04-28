#pragma once

#include "TlvObjectBase.hpp"

#include "../AliveLibAE/Path.hpp"

#include "../AliveLibCommon/Types.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace jsonxx {
class Object;
}

class TlvObjectBaseAEBase : public TlvObjectBase
{
public:
    using CopyFn = void (*)(Path_TLV* dst, Path_TLV* src);
    using InitFn = void(*)(Path_TLV* dst);

    // Used only to get "typeName"
    TlvObjectBaseAEBase(std::size_t sizeOfT, TlvTypes tlvType, const std::string& typeName, Path_TLV* pSelfTlv, InitFn initFn);

    TlvObjectBaseAEBase(std::size_t sizeOfT, TypesCollectionBase& globalTypes, TlvTypes tlvType, const std::string& typeName, Path_TLV* pSelfTlv, Path_TLV* pTlv, CopyFn copyFn, InitFn initFn);

    TlvObjectBaseAEBase(const TlvObjectBaseAEBase&) = delete;
    TlvObjectBaseAEBase(TlvObjectBaseAEBase&&) = delete;

    void InstanceFromJsonBase(const jsonxx::Object& obj) override;
    void InstanceToJsonBase(jsonxx::Object& ret) override;

    [[nodiscard]] s16 TlvLen() const override;
    [[nodiscard]] std::vector<u8> GetTlvData(bool setTerminationFlag) override;
    [[nodiscard]] TlvTypes TlvType() const;

protected:
    const std::size_t mSizeOfT;
    const TlvTypes mType;
    Path_TLV* const mPSelfTlv;
};
