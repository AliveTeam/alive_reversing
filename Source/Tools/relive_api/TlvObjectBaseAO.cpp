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
    ADD("xpos", mPSelfTlv->field_10_top_left.field_0_x);
    ADD("ypos", mPSelfTlv->field_10_top_left.field_2_y);

    ADD("width", mPSelfTlv->field_14_bottom_right.field_0_x);
    ADD("height", mPSelfTlv->field_14_bottom_right.field_2_y);
}

void TlvObjectBaseAO::ConvertXYPos()
{
    if (mPSelfTlv->field_14_bottom_right.field_0_x - mPSelfTlv->field_10_top_left.field_0_x < 0 || mPSelfTlv->field_14_bottom_right.field_2_y - mPSelfTlv->field_10_top_left.field_2_y < 0)
    {
        // Sanity check on the data - passed on all OG data, left for any bad/corrupted lvls
        std::abort();
    }
    mPSelfTlv->field_14_bottom_right.field_0_x -= mPSelfTlv->field_10_top_left.field_0_x;
    mPSelfTlv->field_14_bottom_right.field_2_y -= mPSelfTlv->field_10_top_left.field_2_y;
}

void TlvObjectBaseAO::InstanceFromJsonBase(const jsonxx::Object& obj)
{
    mStructTypeName = obj.get<std::string>("name");

    mPSelfTlv->field_14_bottom_right.field_0_x += mPSelfTlv->field_10_top_left.field_0_x;
    mPSelfTlv->field_14_bottom_right.field_2_y += mPSelfTlv->field_10_top_left.field_2_y;

    mPSelfTlv->field_C_sound_pos.field_0_x = mPSelfTlv->field_10_top_left.field_0_x;
    mPSelfTlv->field_C_sound_pos.field_2_y = mPSelfTlv->field_10_top_left.field_2_y;
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
    mPSelfTlv->field_0_flags.Set(AO::TLV_Flags::eBit3_End_TLV_List, setTerminationFlag);
    std::memcpy(ret.data(), mPSelfTlv, mSizeOfT);
    return ret;
}

[[nodiscard]] AO::TlvTypes TlvObjectBaseAO::TlvType() const
{
    return mType;
}
} // namespace ReliveAPI
