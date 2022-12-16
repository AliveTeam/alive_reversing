#include "TlvObjectBaseAE.hpp"

#include "TlvObjectBaseMacros.hpp"

#include "../../AliveLibAE/Path.hpp"

#include "../../relive_lib/Types.hpp"

#include <jsonxx/jsonxx.h>

#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace ReliveAPI {
TlvObjectBaseAE::TlvObjectBaseAE(std::size_t sizeOfT, TlvTypes tlvType, const std::string& typeName, Path_TLV* pSelfTlv)
    : TlvObjectBase(typeName)
    , mSizeOfT(sizeOfT)
    , mType(tlvType)
    , mPSelfTlv{pSelfTlv}
{
}

TlvObjectBaseAE::TlvObjectBaseAE(std::size_t sizeOfT, TypesCollectionBase& globalTypes, TlvTypes tlvType, const std::string& typeName, Path_TLV* pSelfTlv)
    : TlvObjectBase(typeName)
    , mSizeOfT(sizeOfT)
    , mType(tlvType)
    , mPSelfTlv{pSelfTlv}
{
    ADD("xpos", mPSelfTlv->mTopLeft.x);
    ADD("ypos", mPSelfTlv->mTopLeft.y);

    ADD("width", mPSelfTlv->mBottomRight.x);
    ADD("height", mPSelfTlv->mBottomRight.y);
}

void TlvObjectBaseAE::ConvertXYPos()
{
    if (mPSelfTlv->mBottomRight.x - mPSelfTlv->mTopLeft.x < 0 || mPSelfTlv->mBottomRight.y - mPSelfTlv->mTopLeft.y < 0)
    {
        // Sanity check on the data - passed on all OG data, left for any bad/corrupted lvls
        std::abort();
    }

    mPSelfTlv->mBottomRight.x -= mPSelfTlv->mTopLeft.x;
    mPSelfTlv->mBottomRight.y -= mPSelfTlv->mTopLeft.y;
}

void TlvObjectBaseAE::InstanceFromJsonBase(const jsonxx::Object& obj)
{
    mStructTypeName = obj.get<std::string>("name");

    mPSelfTlv->mBottomRight.x += mPSelfTlv->mTopLeft.x;
    mPSelfTlv->mBottomRight.y += mPSelfTlv->mTopLeft.y;
}

void TlvObjectBaseAE::InstanceToJsonBase(jsonxx::Object& ret)
{
    ret << "name" << Name() + "_" + std::to_string(mInstanceNumber);
    ret << "object_structures_type" << Name();
}

[[nodiscard]] s16 TlvObjectBaseAE::TlvLen() const
{
    return static_cast<s16>(mSizeOfT);
}

[[nodiscard]] std::vector<u8> TlvObjectBaseAE::GetTlvData(bool setTerminationFlag)
{
    std::vector<u8> ret(mSizeOfT);
    mPSelfTlv->mTlvFlags.Set(TlvFlags::eBit3_End_TLV_List, setTerminationFlag);
    std::memcpy(ret.data(), mPSelfTlv, mSizeOfT);
    return ret;
}

[[nodiscard]] TlvTypes TlvObjectBaseAE::TlvType() const
{
    return mType;
}
} // namespace ReliveAPI
