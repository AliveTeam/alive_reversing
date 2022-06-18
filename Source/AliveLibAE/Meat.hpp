#pragma once

#include "Throwable.hpp"

enum class MeatStates : s16
{
    eCreated_0 = 0,
    eIdle_1 = 1,
    eBeingThrown_2 = 2,
    eBecomeAPickUp_3 = 3,
    eWaitForPickUp_4 = 4,
    eFall_5 = 5,
};

struct Meat_SaveState final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    FP field_18_sprite_scale;
    s16 field_1C_path_number;
    LevelIds field_1E_lvl_number;
    enum MeatStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8
    };
    BitField16<MeatStateFlags> field_20_flags;
    s16 field_22_pad;
    s32 field_24_base_id;
    s16 field_28_line_type;
    s16 field_2A_count;
    MeatStates field_2C_state;
    s16 field_2E_pad;
    FP field_30_xpos;
    FP field_34_ypos;
    s32 field_38_savedfield12C; // TODO: Figure out what field_12C is. -- Nemin (5/7/2020)
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Meat_SaveState, 60);

class Meat final : public BaseThrowable
{
public:
    Meat(FP xpos, FP ypos, s16 count);
    ~Meat();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual Bool32 VCanThrow() override;
    virtual Bool32 VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;
    virtual s16 VGetCount() override;
    virtual Bool32 VCanEatMe();
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pBuffer);


private:
    void AddToPlatform();
    void InTheAir();
    s16 OnCollision(BaseGameObject* pHit);

private:
    MeatStates field_11C_state = MeatStates::eCreated_0;
    s16 field_11E_padding = 0;
    FP field_120_xpos = {};
    FP field_124_ypos = {};
    s32 field_128_timer = 0;
    s32 field_12C_deadtimer = 0;

public:
    PathLine* field_130_pLine = nullptr;
};
ALIVE_ASSERT_SIZEOF(Meat, 0x134);

struct Path_MeatSack final : public Path_TLV
{
    XDirection_short field_10_meat_fall_direction;
    u16 field_12_xVel;
    u16 field_14_yVel;
    Scale_short field_16_scale;
    s16 field_18_amount_of_meat;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSack, 0x1C);

class MeatSack final : public BaseAliveGameObject
{
public:
    MeatSack(Path_MeatSack* pTlv, s32 tlvInfo);
    ~MeatSack();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    s32 field_118_tlvInfo = 0;
    s16 field_11C_bDoMeatSackIdleAnim = 0;
    s16 field_11E_amount_of_meat = 0;
    s16 field_120_bPlayWobbleSound = 0;
    s16 field_122_always_0 = 0;
    FP field_124_velX = {};
    FP field_128_velY = {};
};
ALIVE_ASSERT_SIZEOF(MeatSack, 0x12C);
