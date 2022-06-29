#include "TlvObjectBaseAO.hpp"

#include "TlvObjectBaseMacros.hpp"

#include "../../AliveLibAO/Map.hpp"
#include "../../AliveLibAE/Path.hpp"

#include "../../AliveLibCommon/Types.hpp"

#include <jsonxx/jsonxx.h>

#include <string>
#include <utility>
#include <vector>

namespace ReliveAPI {
TlvObjectBaseAO::TlvObjectBaseAO(std::size_t sizeOfT, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pSelfTlv)
    : TlvObjectBase(typeName)
    , mSizeOfT(sizeOfT)
    , mType(tlvType)
    , mPSelfTlv{pSelfTlv}
{
}

TlvObjectBaseAO::TlvObjectBaseAO(std::size_t sizeOfT, TypesCollectionBase& globalTypes, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pSelfTlv)
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

void TlvObjectBaseAO::ConvertXYPos()
{
    if (mPSelfTlv->mBottomRight.x - mPSelfTlv->mTopLeft.x < 0 || mPSelfTlv->mBottomRight.y - mPSelfTlv->mTopLeft.y < 0)
    {
        // Sanity check on the data - passed on all OG data, left for any bad/corrupted lvls
        std::abort();
    }
    mPSelfTlv->mBottomRight.x -= mPSelfTlv->mTopLeft.x;
    mPSelfTlv->mBottomRight.y -= mPSelfTlv->mTopLeft.y;
}

void TlvObjectBaseAO::InstanceFromJsonBase(const jsonxx::Object& obj)
{
    mStructTypeName = obj.get<std::string>("name");

    mPSelfTlv->mBottomRight.x += mPSelfTlv->mTopLeft.x;
    mPSelfTlv->mBottomRight.y += mPSelfTlv->mTopLeft.y;

    mPSelfTlv->field_C_sound_pos.x = mPSelfTlv->mTopLeft.x;
    mPSelfTlv->field_C_sound_pos.y = mPSelfTlv->mTopLeft.y;
}

void TlvObjectBaseAO::InstanceToJsonBase(jsonxx::Object& ret)
{
    ret << "name" << Name() + "_" + std::to_string(mInstanceNumber);
    ret << "object_structures_type" << Name();
}

[[nodiscard]] s16 TlvObjectBaseAO::TlvLen() const
{
    return static_cast<s16>(mSizeOfT);
}

[[nodiscard]] std::vector<u8> TlvObjectBaseAO::GetTlvData(bool setTerminationFlag)
{
    std::vector<u8> ret(mSizeOfT);
    mPSelfTlv->mTlvFlags.Set(AO::TlvFlags::eBit3_End_TLV_List, setTerminationFlag);
    std::memcpy(ret.data(), mPSelfTlv, mSizeOfT);
    return ret;
}

[[nodiscard]] AO::TlvTypes TlvObjectBaseAO::TlvType() const
{
    return mType;
}
} // namespace ReliveAPI
