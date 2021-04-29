#pragma once

#include "TlvObjectBase.hpp"

#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAE/Path.hpp"

#include "../AliveLibCommon/Types.hpp"

#include <string>
#include <utility>
#include <vector>

namespace jsonxx {
class Object;
}

class TlvObjectBaseAOBase : public TlvObjectBase
{
public:
    using CopyFn = void (*)(AO::Path_TLV* dst, AO::Path_TLV* src);
    using InitFn = void(*)(AO::Path_TLV* dst);

    // Ctor used only to get "typeName"
    TlvObjectBaseAOBase(std::size_t sizeOfT, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pSelfTlv, InitFn initFn);

    TlvObjectBaseAOBase(std::size_t sizeOfT, TypesCollectionBase& globalTypes, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pSelfTlv, AO::Path_TLV* pTlv, CopyFn copyFn, InitFn initFn);

    void InstanceFromJsonBase(const jsonxx::Object& obj) override;
    void InstanceToJsonBase(jsonxx::Object& ret) override;

    TlvObjectBaseAOBase(const TlvObjectBaseAOBase&) = delete;
    TlvObjectBaseAOBase(TlvObjectBaseAOBase&&) = delete;

    [[nodiscard]] s16 TlvLen() const override;
    [[nodiscard]] std::vector<u8> GetTlvData(bool setTerminationFlag) override;
    [[nodiscard]] AO::TlvTypes TlvType() const;

protected:
    const std::size_t mSizeOfT;
    const AO::TlvTypes mType;
    AO::Path_TLV* const mPSelfTlv;
};
