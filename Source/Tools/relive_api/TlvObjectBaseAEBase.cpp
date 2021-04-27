#include "TlvObjectBaseAEBase.hpp"

#include "TlvObjectBaseMacros.hpp"

#include "../AliveLibAE/Path.hpp"

#include "../AliveLibCommon/Types.hpp"

#include <jsonxx/jsonxx.h>

#include <cstring>
#include <string>
#include <utility>
#include <vector>

TlvObjectBaseAEBase::TlvObjectBaseAEBase(std::size_t sizeOfT, TlvTypes tlvType, const std::string& typeName, Path_TLV* pSelfTlv)
    : TlvObjectBase(typeName)
    , mSizeOfT(sizeOfT)
    , mType(tlvType)
    , mPSelfTlv{pSelfTlv}
{
}

TlvObjectBaseAEBase::TlvObjectBaseAEBase(std::size_t sizeOfT, TypesCollectionBase& globalTypes, TlvTypes tlvType, const std::string& typeName, Path_TLV* pSelfTlv, Path_TLV* pTlv, CopyFn copyFn)
    : TlvObjectBase(typeName)
    , mSizeOfT(sizeOfT)
    , mType(tlvType)
    , mPSelfTlv{pSelfTlv}
{
    mPSelfTlv->field_2_length = static_cast<s16>(mSizeOfT);
    mPSelfTlv->field_4_type.mType = mType;

    copyFn(pSelfTlv /* dst */, pTlv /* src */);

    if (mPSelfTlv->field_C_bottom_right.field_0_x - mPSelfTlv->field_8_top_left.field_0_x < 0 || mPSelfTlv->field_C_bottom_right.field_2_y - mPSelfTlv->field_8_top_left.field_2_y < 0)
    {
        // Sanity check on the data - passed on all OG data, left for any bad/corrupted lvls
        std::abort();
    }

    ADD("xpos", mPSelfTlv->field_8_top_left.field_0_x);
    ADD("ypos", mPSelfTlv->field_8_top_left.field_2_y);

    mPSelfTlv->field_C_bottom_right.field_0_x -= mPSelfTlv->field_8_top_left.field_0_x;
    mPSelfTlv->field_C_bottom_right.field_2_y -= mPSelfTlv->field_8_top_left.field_2_y;

    ADD("width", mPSelfTlv->field_C_bottom_right.field_0_x);
    ADD("height", mPSelfTlv->field_C_bottom_right.field_2_y);
}

void TlvObjectBaseAEBase::InstanceFromJsonBase(const jsonxx::Object& obj)
{
    mStructTypeName = obj.get<std::string>("name");

    mPSelfTlv->field_C_bottom_right.field_0_x += mPSelfTlv->field_8_top_left.field_0_x;
    mPSelfTlv->field_C_bottom_right.field_2_y += mPSelfTlv->field_8_top_left.field_2_y;
}

void TlvObjectBaseAEBase::InstanceToJsonBase(jsonxx::Object& ret)
{
    ret << "name" << Name() + "_" + std::to_string(mInstanceNumber);
    ret << "object_structures_type" << Name();
}

[[nodiscard]] s16 TlvObjectBaseAEBase::TlvLen() const
{
    return static_cast<s16>(mSizeOfT);
}

[[nodiscard]] std::vector<u8> TlvObjectBaseAEBase::GetTlvData(bool setTerminationFlag)
{
    std::vector<u8> ret(mSizeOfT);
    mPSelfTlv->field_0_flags.Set(TLV_Flags::eBit3_End_TLV_List, setTerminationFlag);
    std::memcpy(ret.data(), mPSelfTlv, mSizeOfT);
    return ret;
}

[[nodiscard]] TlvTypes TlvObjectBaseAEBase::TlvType() const
{
    return mType;
}
