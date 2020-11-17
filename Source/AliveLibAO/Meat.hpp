#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Throwable.hpp"

namespace AO {

struct Path_MeatSack : public Path_TLV
{
    __int16 field_18_side;
    unsigned __int16 field_1A_x_vel;
    __int16 field_1C_y_vel;
    unsigned __int16 field_1E_scale;
    __int16 field_20_amount_of_meat;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_MeatSack, 0x24);


class MeatSack : public BaseAliveGameObject
{
public:
    EXPORT MeatSack* ctor_4390F0(Path_MeatSack* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT MeatSack* Vdtor_439550(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_439540();

    EXPORT BaseGameObject* dtor_439250();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4392C0();

    int field_10C_tlvInfo;
    __int16 field_110;
    __int16 field_112_num_items;
    __int16 field_114;
    __int16 field_116;
    FP field_118_velX;
    FP field_11C_velY;
};
ALIVE_ASSERT_SIZEOF(MeatSack, 0x120);


class Meat : public BaseThrowable
{
public:
    EXPORT Meat* ctor_438550(FP xpos, FP ypos, __int16 count);

    EXPORT BaseGameObject* dtor_438660();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Meat* Vdtor_4390D0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_438E70();

    virtual void VThrow(FP velX, FP velY) override;

    EXPORT void VThrow_4386E0(FP velX, FP velY);
   
    virtual __int16 VCanThrow() override;

    EXPORT __int16 VCanThrow_4390B0();

    virtual BOOL VCanEatMe();

    EXPORT BOOL VCanEatMe_4390C0();

    EXPORT __int16 VIsFalling_4573B0();

    virtual __int16 VIsFalling() override;

    virtual void VTimeToExplodeRandom() override;

    EXPORT void InTheAir_438720();

    virtual void VUpdate() override;

    EXPORT void VUpdate_438A20();

    EXPORT __int16 OnCollision_438D80(BaseAliveGameObject* pObj);

    EXPORT void AddToPlatform_438EA0();

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_438FD0();

    virtual __int16 VGetCount() override;

    EXPORT __int16 VGetCount_439020();

    __int16 field_110_state;
    __int16 field_112;
    FP field_114_xpos;
    FP field_118_ypos;
    int field_11C_timer;
    int field_120_deadtimer;
    PathLine* field_124_pLine;
};
ALIVE_ASSERT_SIZEOF(Meat, 0x128);


}

