#pragma once

#include "TlvObjectBaseAOBase.hpp"

#include "../AliveLibAO/Map.hpp"
#include "../AliveLibAO/PathData.hpp"

#include <string>

template<class T>
class TlvObjectBaseAO : public TlvObjectBaseAOBase
{
private:
    [[nodiscard]] AO::Path_TLV* initTlv()
    {
        mTlv = {};
        return &mTlv;
    }

    static void copyFn(AO::Path_TLV* dst, AO::Path_TLV* src)
    {
        if(src != nullptr)
        {
            *(reinterpret_cast<T*>(dst)) = *(reinterpret_cast<T*>(src));
        }
    }

public:
    // Used only to get "typeName"
    TlvObjectBaseAO(AO::TlvTypes tlvType, const std::string& typeName)
        : TlvObjectBaseAOBase(sizeof(T), tlvType, typeName, initTlv())
    {
    }

    TlvObjectBaseAO(TypesCollectionBase& globalTypes, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pTlv)
        : TlvObjectBaseAOBase(sizeof(T), globalTypes, tlvType, typeName, initTlv(), pTlv, &copyFn)
    {
    }

protected:
    T mTlv;
};
