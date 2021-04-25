#pragma once

#include "TlvObjectBaseAEBase.hpp"

#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/PathData.hpp"

#include <string>

template<class T>
class TlvObjectBaseAE : public TlvObjectBaseAEBase
{
private:
    [[nodiscard]] Path_TLV* initTlv()
    {
        mTlv = {};
        return &mTlv;
    }

    static void copyFn(Path_TLV* dst, Path_TLV* src)
    {
        if(src != nullptr)
        {
            *(reinterpret_cast<T*>(dst)) = *(reinterpret_cast<T*>(src));
        }
    }

public:
    // Used only to get "typeName"
    TlvObjectBaseAE(TlvTypes tlvType, const std::string& typeName)
        : TlvObjectBaseAEBase(sizeof(T), tlvType, typeName, initTlv())
    {
    }

    TlvObjectBaseAE(TypesCollectionBase& globalTypes, TlvTypes tlvType, const std::string& typeName, Path_TLV* pTlv)
        : TlvObjectBaseAEBase(sizeof(T), globalTypes, tlvType, typeName, initTlv(), pTlv, &copyFn)
    {
    }

protected:
    T mTlv;
};
