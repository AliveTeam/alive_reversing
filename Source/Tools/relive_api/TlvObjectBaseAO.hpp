#pragma once

#include "TlvObjectBase.hpp"
#include "TlvObjectBaseMacros.hpp"

#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/Map.hpp"

template<class T>
class TlvObjectBaseAO : public TlvObjectBase
{
public:
    // Ctor used only to get "typeName"
    TlvObjectBaseAO(AO::TlvTypes tlvType, const std::string& typeName)
        : TlvObjectBase(typeName), mType(tlvType), mBase(&mTlv)
    {

    }

    ~TlvObjectBaseAO() { }

    TlvObjectBaseAO(TypesCollectionBase& globalTypes, AO::TlvTypes tlvType, const std::string& typeName, AO::Path_TLV* pTlv)
        : TlvObjectBase(typeName), mType(tlvType), mBase(&mTlv)
    {
        mTlv.field_4_type.mType = mType;
        mTlv.field_2_length = sizeof(T);
        COPY_TLV();

        if (mBase->field_14_bottom_right.field_0_x - mBase->field_10_top_left.field_0_x < 0 ||
            mBase->field_14_bottom_right.field_2_y - mBase->field_10_top_left.field_2_y < 0)
        {
            // Sanity check on the data - passed on all OG data, left for any bad/corrupted lvls
            abort();
        }

        ADD("xpos", mBase->field_10_top_left.field_0_x);
        ADD("ypos", mBase->field_10_top_left.field_2_y);

        mBase->field_14_bottom_right.field_0_x -= mBase->field_10_top_left.field_0_x;
        mBase->field_14_bottom_right.field_2_y -= mBase->field_10_top_left.field_2_y;

        ADD("width", mBase->field_14_bottom_right.field_0_x);
        ADD("height", mBase->field_14_bottom_right.field_2_y);
    }

    void InstanceFromJsonBase(jsonxx::Object& obj) override
    {
        mStructTypeName = obj.get<std::string>("name");

        mBase->field_14_bottom_right.field_0_x += mBase->field_10_top_left.field_0_x;
        mBase->field_14_bottom_right.field_2_y += mBase->field_10_top_left.field_2_y;

        mBase->field_C_sound_pos.field_0_x = mBase->field_10_top_left.field_0_x;
        mBase->field_C_sound_pos.field_2_y = mBase->field_10_top_left.field_2_y;
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
        mTlv.field_0_flags.Set(AO::TLV_Flags::eBit3_End_TLV_List, setTerminationFlag);
        memcpy(ret.data(), &mTlv, sizeof(T));
        return ret;
    }

    AO::TlvTypes TlvType() const
    {
        return mType;
    }

protected:
    const AO::TlvTypes mType = {};
    T mTlv = {};
    AO::Path_TLV* mBase = nullptr;
};
