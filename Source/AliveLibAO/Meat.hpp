#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Throwable.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_MeatSack final : public Path_TLV
{
    XDirection_short field_18_meat_fall_direction;
    u16 field_1A_x_vel;
    s16 field_1C_y_vel;
    Scale_short field_1E_scale;
    s16 field_20_amount_of_meat;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_MeatSack, 0x24);


class MeatSack final : public BaseAliveGameObject
{
public:
    MeatSack(Path_MeatSack* pTlv, s32 tlvInfo);
    ~MeatSack();
    
    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_439540();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4392C0();

    s32 field_10C_tlvInfo;
    s16 field_110_bDoMeatSackIdleAnim;
    s16 field_112_num_items;
    s16 field_114_bPlayWobbleSound;
    s16 field_116_always_0;
    FP field_118_velX;
    FP field_11C_velY;
};
ALIVE_ASSERT_SIZEOF(MeatSack, 0x120);


class Meat final : public BaseThrowable
{
public:
    Meat(FP xpos, FP ypos, s16 count);
    ~Meat();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_438E70();

    virtual void VThrow(FP velX, FP velY) override;

    EXPORT void VThrow_4386E0(FP velX, FP velY);

    virtual s16 VCanThrow() override;

    EXPORT s16 VCanThrow_4390B0();

    virtual Bool32 VCanEatMe();

    EXPORT Bool32 VCanEatMe_4390C0();

    EXPORT s16 VIsFalling_4573B0();

    virtual s16 VIsFalling() override;

    virtual void VTimeToExplodeRandom() override;

    EXPORT void InTheAir_438720();

    virtual void VUpdate() override;

    EXPORT void VUpdate_438A20();

    EXPORT s16 OnCollision_438D80(BaseAliveGameObject* pObj);

    EXPORT void AddToPlatform_438EA0();

    virtual void VOnTrapDoorOpen() override;

    EXPORT void VOnTrapDoorOpen_438FD0();

    virtual s16 VGetCount() override;

    EXPORT s16 VGetCount_439020();

    s16 field_110_state;
    s16 field_112;
    FP field_114_xpos;
    FP field_118_ypos;
    s32 field_11C_timer;
    s32 field_120_deadtimer;
    PathLine* field_124_pLine;
};
ALIVE_ASSERT_SIZEOF(Meat, 0x128);


} // namespace AO
