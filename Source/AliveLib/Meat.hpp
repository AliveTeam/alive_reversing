#pragma once

#include "Throwable.hpp"

class Meat : public BaseThrowable
{
public:
    EXPORT Meat* ctor_4694A0(FP xpos, FP ypos, __int16 count);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VScreenChanged() override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VThrow_49E460(FP velX, FP velY) override;

    virtual BOOL VCanThrow_49E350() override;

    virtual BOOL VIsFalling_49E330() override;

    virtual void VTimeToExplodeRandom_411490() override;

    virtual __int16 VGetCount_448080() override;

private:
    EXPORT void vScreenChanged_46A130();

    EXPORT void AddToPlatform_46A170();

    EXPORT void vOnTrapDoorOpen_46A2E0();

    EXPORT BOOL vIsFalling_469660();

    EXPORT BOOL vCanThrow_469680();

    EXPORT void dtor_4696F0();

    EXPORT Meat* vdtor_4696C0(signed int flags);

    EXPORT void vThrow_469790(FP velX, FP velY);

    EXPORT __int16 vGetCount_46A350();

    EXPORT void InTheAir_4697E0();

    /*
    Meat__vOnTrapDoorOpen_46A2E0
    Meat__vThrow_469790
    Meat__vCanThrow_469680
    Meat__vIsFalling_469660
    Rock__vTimeToExplodeRandom_411490
    Meat__vGetCount_46A350
    BaseThrowable__vToDead_4114B0
    Meat__vsub_4696A0 // State not 0, eaten?
    */
private:
    __int16 field_11C_state;
    __int16 field_11E;
    FP field_120_xpos;
    FP field_124_ypos;
    int field_128;
    int field_12C;
    PathLine* field_130_pLine;
};
ALIVE_ASSERT_SIZEOF(Meat, 0x134);

struct Path_MeatSack : public Path_TLV
{
    __int16 field_10_side;
    unsigned __int16 field_12_xVel;
    unsigned __int16 field_14_yVel;
    __int16 field_16_scale;
    __int16 field_18_num_items;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSack, 0x1C);

class MeatSack : public BaseAliveGameObject
{
public:
    EXPORT MeatSack* ctor_46A410(Path_MeatSack* pTlv, int tlvInfo);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
private:
    EXPORT MeatSack* vdtor_46A5E0(signed int flags);
    EXPORT void dtor_46A610();
    EXPORT void vScreenChanged_46A9C0();
    EXPORT void vUpdate_46A6A0();

private:
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E_num_items;
    __int16 field_120;
    __int16 field_122;
    FP field_124_velX;
    FP field_128_velY;
};
ALIVE_ASSERT_SIZEOF(MeatSack, 0x12C);
