#include "TlvObjectBaseRelive.hpp"

#include "TlvObjectBaseMacros.hpp"

#include "../../relive_lib/Types.hpp"

#include <nlohmann/json.hpp>

#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace ReliveAPI {
TlvObjectBaseRelive::TlvObjectBaseRelive(std::size_t sizeOfT, ReliveTypes tlvType, const std::string& typeName, relive::Path_TLV* pSelfTlv)
    : TlvObjectBase(typeName)
    , mSizeOfT(sizeOfT)
    , mType(tlvType)
    , mPSelfTlv{pSelfTlv}
{
}

TlvObjectBaseRelive::TlvObjectBaseRelive(std::size_t sizeOfT, TypesCollectionBase& globalTypes, ReliveTypes tlvType, const std::string& typeName, relive::Path_TLV* pSelfTlv)
    : TlvObjectBase(typeName)
    , mSizeOfT(sizeOfT)
    , mType(tlvType)
    , mPSelfTlv{pSelfTlv}
{
    ADD("xpos", mPSelfTlv->mTopLeftX);
    ADD("ypos", mPSelfTlv->mTopLeftY);

    ADD("width", mPSelfTlv->mBottomRightX);
    ADD("height", mPSelfTlv->mBottomRightY);
}

void TlvObjectBaseRelive::ConvertXYPos()
{
    if (mPSelfTlv->mBottomRightX - mPSelfTlv->mTopLeftX < 0 || mPSelfTlv->mBottomRightY - mPSelfTlv->mTopLeftY < 0)
    {
        // Sanity check on the data - passed on all OG data, left for any bad/corrupted lvls
        std::abort();
    }

    mPSelfTlv->mBottomRightX -= mPSelfTlv->mTopLeftX;
    mPSelfTlv->mBottomRightY -= mPSelfTlv->mTopLeftY;
}

void TlvObjectBaseRelive::InstanceFromJsonBase(const nlohmann::json& obj)
{
    obj.at("name").get_to(mStructTypeName);

    mPSelfTlv->mBottomRightX += mPSelfTlv->mTopLeftX;
    mPSelfTlv->mBottomRightY += mPSelfTlv->mTopLeftY;
}

void TlvObjectBaseRelive::InstanceToJsonBase(nlohmann::json& ret)
{
    ret["name"] = Name() + "_" + std::to_string(mInstanceNumber);
    ret["object_structures_type"] = Name();
}

[[nodiscard]] s16 TlvObjectBaseRelive::TlvLen() const
{
    return static_cast<s16>(mSizeOfT);
}

[[nodiscard]] std::vector<u8> TlvObjectBaseRelive::GetTlvData(bool setTerminationFlag)
{
    std::vector<u8> ret(mSizeOfT);
    mPSelfTlv->mTlvFlags.Set(relive::TlvFlags::eBit3_End_TLV_List, setTerminationFlag);
    std::memcpy(ret.data(), mPSelfTlv, mSizeOfT);
    return ret;
}

[[nodiscard]] ReliveTypes TlvObjectBaseRelive::TlvType() const
{
    return mType;
}
} // namespace ReliveAPI
