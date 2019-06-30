#pragma once

#include "Throwable.hpp"

class Bone : public BaseThrowable
{
public:
    EXPORT Bone* ctor_4112C0(FP xpos, FP ypos, __int16 countId);

    virtual void VThrow_49E460(FP /*velX*/, FP /*velY*/) override
    {
        // TODO
    }

    virtual BOOL VCanThrow_49E350() override
    {
        // TODO
        return 0;
    }

    virtual BOOL VIsFalling_49E330() override
    {
        // TOOD
        return 0;
    }

    virtual void VTimeToExplodeRandom_411490() override
    {
        // TODO
    }
private:
    EXPORT Bone* vdtor_411580(signed int flags);

    EXPORT void dtor_4115B0();

    EXPORT void AddToPlatform_412310();


    /*
    Bone__vOnTrapDoorOpen_412490
    Bone__vThrow_411670
    Bone__vCanThrow_411530
    Bone__vIsFalling_411510
    Rock__vTimeToExplodeRandom_411490
    Bone__vGetCount_412500
    BaseThrowable__vToDead_4114B0
    Bone__vsub_411560 // State is 4, eaten?
    */
private:
    __int16 field_11C_state;
    __int16 field_11E;
    FP field_120;
    FP field_124;
    int field_128;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
};
ALIVE_ASSERT_SIZEOF(Bone, 0x134);

struct Path_BoneBag : public Path_TLV
{
    __int16 field_10_side;
    unsigned __int16 field_12_x_vel;
    unsigned __int16 field_14_y_vel;
    __int16 field_16_scale;
    __int16 field_18_num_bones;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoneBag, 0x1C);

class BoneBag : public BaseAliveGameObject
{
public:
    EXPORT BoneBag* ctor_4125C0(Path_BoneBag* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_4127C0(flags);
    }

    virtual void VUpdate() override
    {
        // TODO
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_412BF0();
    }

private:
    EXPORT void vScreenChanged_412BF0();
    EXPORT BoneBag* vdtor_4127C0(signed int flags);
    EXPORT void dtor_4127F0();

private:
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E_count;
    __int16 field_120;
    __int16 field_122;
    int field_124;
    int field_128;
};
ALIVE_ASSERT_SIZEOF(BoneBag, 0x12C);
