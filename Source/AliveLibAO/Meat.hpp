#pragma once

#include "../AliveLibCommon/Function.hpp"
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
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSack, 0x24);


class MeatSack final : public BaseAliveGameObject
{
public:
    MeatSack(Path_MeatSack* pTlv, s32 tlvInfo);
    ~MeatSack();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 field_10C_tlvInfo = 0;
    s16 field_110_bDoMeatSackIdleAnim = 0;
    s16 field_112_num_items = 0;
    s16 field_114_bPlayWobbleSound = 0;
    s16 field_116_always_0 = 0;
    FP field_118_velX = FP_FromInteger(0);
    FP field_11C_velY = FP_FromInteger(0);
};
ALIVE_ASSERT_SIZEOF(MeatSack, 0x120);


class Meat final : public BaseThrowable
{
public:
    Meat(FP xpos, FP ypos, s16 count);
    ~Meat();

    virtual void VScreenChanged() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual s16 VCanThrow() override;
    virtual Bool32 VCanEatMe();
    virtual s16 VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;
    virtual void VUpdate() override;
    virtual void VOnTrapDoorOpen() override;
    virtual s16 VGetCount() override;

    void InTheAir();
    void AddToPlatform();
    s16 OnCollision(BaseAliveGameObject* pObj);

    s16 field_110_state = 0;
    s16 field_112 = 0;
    FP field_114_xpos = {};
    FP field_118_ypos = {};
    s32 field_11C_timer = 0;
    s32 field_120_deadtimer = 0;
    PathLine* field_124_pLine = nullptr;
};
ALIVE_ASSERT_SIZEOF(Meat, 0x128);


} // namespace AO
