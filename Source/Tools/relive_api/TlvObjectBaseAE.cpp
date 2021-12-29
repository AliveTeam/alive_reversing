#include "TlvObjectBaseAE.hpp"

#include "TlvObjectBaseMacros.hpp"

#include "../../AliveLibAE/Path.hpp"

#include "../../AliveLibCommon/Types.hpp"

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
    ADD("xpos", mPSelfTlv->field_8_top_left.field_0_x);
    ADD("ypos", mPSelfTlv->field_8_top_left.field_2_y);

    ADD("width", mPSelfTlv->field_C_bottom_right.field_0_x);
    ADD("height", mPSelfTlv->field_C_bottom_right.field_2_y);
}

void TlvObjectBaseAE::ConvertXYPos()
{
    if (mPSelfTlv->field_C_bottom_right.field_0_x - mPSelfTlv->field_8_top_left.field_0_x < 0 || mPSelfTlv->field_C_bottom_right.field_2_y - mPSelfTlv->field_8_top_left.field_2_y < 0)
    {
        // Sanity check on the data - passed on all OG data, left for any bad/corrupted lvls
        std::abort();
    }

    mPSelfTlv->field_C_bottom_right.field_0_x -= mPSelfTlv->field_8_top_left.field_0_x;
    mPSelfTlv->field_C_bottom_right.field_2_y -= mPSelfTlv->field_8_top_left.field_2_y;
}

void TlvObjectBaseAE::InstanceFromJsonBase(const jsonxx::Object& obj)
{
    mStructTypeName = obj.get<std::string>("name");

    mPSelfTlv->field_C_bottom_right.field_0_x += mPSelfTlv->field_8_top_left.field_0_x;
    mPSelfTlv->field_C_bottom_right.field_2_y += mPSelfTlv->field_8_top_left.field_2_y;
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
    mPSelfTlv->field_0_flags.Set(TLV_Flags::eBit3_End_TLV_List, setTerminationFlag);
    std::memcpy(ret.data(), mPSelfTlv, mSizeOfT);
    return ret;
}

[[nodiscard]] TlvTypes TlvObjectBaseAE::TlvType() const
{
    return mType;
}
} // namespace ReliveAPI
