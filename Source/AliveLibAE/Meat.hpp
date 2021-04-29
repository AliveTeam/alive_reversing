#pragma once

#include "Throwable.hpp"

enum class MeatStates : s16
{
    State_0 = 0,
    State_1_Idle = 1,
    State_2_BeingThrown = 2,
    State_3_BecomeAPickUp = 3,
    State_4_WaitForPickUp = 4,
    State_5_Fall = 5,
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
    EXPORT Meat* ctor_4694A0(FP xpos, FP ypos, s16 count);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VThrow_49E460(FP velX, FP velY) override;
    virtual BOOL VCanThrow_49E350() override;
    virtual BOOL VIsFalling_49E330() override;
    virtual void VTimeToExplodeRandom_411490() override;
    virtual s16 VGetCount_448080() override;
    virtual BOOL VCanEatMe_4696A0();
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    EXPORT static s32 CC CreateFromSaveState_46A9E0(const u8* pBuffer);


private:
    EXPORT void vScreenChanged_46A130();
    EXPORT void AddToPlatform_46A170();
    EXPORT void vOnTrapDoorOpen_46A2E0();
    EXPORT BOOL vIsFalling_469660();
    EXPORT BOOL vCanThrow_469680();
    EXPORT void dtor_4696F0();
    EXPORT Meat* vdtor_4696C0(s32 flags);
    EXPORT void vThrow_469790(FP velX, FP velY);
    EXPORT s16 vGetCount_46A350();
    EXPORT void InTheAir_4697E0();
    EXPORT s16 OnCollision_469FF0(BaseGameObject* pHit);
    EXPORT void vUpdate_469BA0();
    EXPORT s32 vGetSaveState_46AC40(Meat_SaveState* pState);
    EXPORT BOOL vCanEatMe_4696A0();

private:
    MeatStates field_11C_state;
    s16 field_11E;
    FP field_120_xpos;
    FP field_124_ypos;
    s32 field_128_timer;
    s32 field_12C_deadtimer;

public:
    PathLine* field_130_pLine;
};
ALIVE_ASSERT_SIZEOF(Meat, 0x134);

struct Path_MeatSack final : public Path_TLV
{
    XDirection_short field_10_meat_fall_direction;
    u16 field_12_xVel;
    u16 field_14_yVel;
    Scale_short field_16_scale;
    s16 field_18_meat_amount;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MeatSack, 0x1C);

class MeatSack final : public BaseAliveGameObject
{
public:
    EXPORT MeatSack* ctor_46A410(Path_MeatSack* pTlv, s32 tlvInfo);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    EXPORT MeatSack* vdtor_46A5E0(s32 flags);
    EXPORT void dtor_46A610();
    EXPORT void vScreenChanged_46A9C0();
    EXPORT void vUpdate_46A6A0();


private:
    s32 field_118_tlvInfo;
    s16 field_11C;
    s16 field_11E_meat_amount;
    s16 field_120;
    s16 field_122;
    FP field_124_velX;
    FP field_128_velY;
};
ALIVE_ASSERT_SIZEOF(MeatSack, 0x12C);
