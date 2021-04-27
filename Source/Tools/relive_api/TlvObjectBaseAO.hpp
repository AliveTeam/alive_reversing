#pragma once

#include "TlvObjectBaseAOBase.hpp"

#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAO/PathData.hpp"

#include <cassert>
#include <string>

template <class T>
class TlvObjectBaseAO : public T
    , public TlvObjectBaseAOBase
{
private:
    static void copyFn(AO::Path_TLV* dst, AO::Path_TLV* src)
    {
        if (src != nullptr)
        {
            *(static_cast<T*>(dst)) = *(static_cast<T*>(src));
        }
    }

public:
    // Used only to get "typeName"
    TlvObjectBaseAO(AO::TlvTypes tlvType, const std::string& typeName)
        : T{}
        , TlvObjectBaseAOBase(sizeof(T), tlvType, typeName, &tlv())
    {
    }

    TlvObjectBaseAO(TypesCollectionBase& globalTypes, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pTlv)
        : T{}
        , TlvObjectBaseAOBase(sizeof(T), globalTypes, tlvType, typeName, &tlv(), pTlv, &copyFn)
    {
    }

    [[nodiscard]] T& tlv()
    {
        return static_cast<T&>(*this);
    }
    [[nodiscard]] const T& tlv() const
    {
        return static_cast<T&>(*this);
    }
};
