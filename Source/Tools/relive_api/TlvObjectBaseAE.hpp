#pragma once

#include "TlvObjectBase.hpp"
#include "TlvObjectBaseMacros.hpp"

#include "../AliveLibAE/Path.hpp"

template<class T>
class TlvObjectBaseAE : public TlvObjectBase
{
public:
    // Used only to get "typeName"
    TlvObjectBaseAE(TlvTypes tlvType, const std::string& typeName)
        : TlvObjectBase(typeName), mType(tlvType)
    {

    }

    ~TlvObjectBaseAE() { }

    TlvObjectBaseAE(TypesCollectionBase& globalTypes, TlvTypes tlvType, const std::string& typeName, Path_TLV* pTlv)
        : TlvObjectBase(typeName), mType(tlvType)
    {
        mTlv.field_2_length = sizeof(T);
        mTlv.field_4_type.mType = mType;
        COPY_TLV();

        if (mTlv.field_C_bottom_right.field_0_x - mTlv.field_8_top_left.field_0_x < 0 ||
            mTlv.field_C_bottom_right.field_2_y - mTlv.field_8_top_left.field_2_y < 0)
        {
            // Sanity check on the data - passed on all OG data, left for any bad/corrupted lvls
            abort();
        }

        ADD("xpos", mTlv.field_8_top_left.field_0_x);
        ADD("ypos", mTlv.field_8_top_left.field_2_y);

        mTlv.field_C_bottom_right.field_0_x -= mTlv.field_8_top_left.field_0_x;
        mTlv.field_C_bottom_right.field_2_y -= mTlv.field_8_top_left.field_2_y;

        ADD("width", mTlv.field_C_bottom_right.field_0_x);
        ADD("height", mTlv.field_C_bottom_right.field_2_y);
    }

    void InstanceFromJsonBase(jsonxx::Object& obj) override
    {
        mStructTypeName = obj.get<std::string>("name");

        mTlv.field_C_bottom_right.field_0_x += mTlv.field_8_top_left.field_0_x;
        mTlv.field_C_bottom_right.field_2_y += mTlv.field_8_top_left.field_2_y;
    }

    void InstanceToJsonBase(jsonxx::Object& ret) override
    {
        ret << "name" << Name() + "_" + std::to_string(mInstanceNumber);
        ret << "object_structures_type" << Name();
    }

    s16 TlvLen() const override
    {
        return static_cast<s16>(sizeof(T));
    }

    std::vector<u8> GetTlvData(bool setTerminationFlag) override
    {
        std::vector<u8> ret(sizeof(T));
        mTlv.field_0_flags.Set(TLV_Flags::eBit3_End_TLV_List, setTerminationFlag);
        memcpy(ret.data(), &mTlv, sizeof(T));
        return ret;
    }

    TlvTypes TlvType() const
    {
        return mType;
    }

protected:
    const TlvTypes mType = {};
    T mTlv = {};
};
