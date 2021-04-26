#pragma once

#include "TlvObjectBaseAEBase.hpp"

#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/PathData.hpp"

#include <cassert>
#include <string>

template<class T>
class TlvObjectBaseAE : public T, public TlvObjectBaseAEBase
{
private:
    static void copyFn(Path_TLV* dst, Path_TLV* src)
    {
        if(src != nullptr)
        {
            *(static_cast<T*>(dst)) = *(static_cast<T*>(src));
        }
    }

public:
    // Used only to get "typeName"
    TlvObjectBaseAE(TlvTypes tlvType, const std::string& typeName)
        : T{}, TlvObjectBaseAEBase(sizeof(T), tlvType, typeName, &tlv())
    {
    }

    TlvObjectBaseAE(TypesCollectionBase& globalTypes, TlvTypes tlvType, const std::string& typeName, Path_TLV* pTlv)
        : T{}, TlvObjectBaseAEBase(sizeof(T), globalTypes, tlvType, typeName, &tlv(), pTlv, &copyFn)
    {
    }

    [[nodiscard]]       T& tlv()       { return static_cast<T&>(*this); }
    [[nodiscard]] const T& tlv() const { return static_cast<T&>(*this); }
};
