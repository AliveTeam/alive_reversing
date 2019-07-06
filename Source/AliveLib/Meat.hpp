#pragma once

#include "Throwable.hpp"

enum class MeatStates : __int16
{
    State_0 = 0,
    State_1_Idle = 1,
    State_2_BeingThrown = 2,
    State_3_BecomeAPickUp = 3,
    State_4_WaitForPickUp = 4,
    State_5_Fall = 5,
};

struct Meat_SaveState
{
    Types field_0_type;
    __int16 field_2_pad;
    int field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    FP field_18_sprite_scale;
    __int16 field_1C_path_number;
    LevelIds field_1E_lvl_number;
    enum MeatStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8
    };
    BitField16<MeatStateFlags> field_20_flags;
    __int16 field_22_pad;
    int field_24_base_id;
    __int16 field_28_line_type;
    __int16 field_2A_count;
    MeatStates field_2C_state;
    __int16 field_2E_pad;
    FP field_30_xpos;
    FP field_34_ypos;
    int field_38;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Meat_SaveState, 60);

class Meat : public BaseThrowable
{
public:
    EXPORT Meat* ctor_4694A0(FP xpos, FP ypos, __int16 count);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VThrow_49E460(FP velX, FP velY) override;
    virtual BOOL VCanThrow_49E350() override;
    virtual BOOL VIsFalling_49E330() override;
    virtual void VTimeToExplodeRandom_411490() override;
    virtual __int16 VGetCount_448080() override;
    virtual BOOL VCanEatMe_4696A0();
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    EXPORT static int CC CreateFromSaveState_46A9E0(const BYTE* pBuffer);


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
    EXPORT __int16 OnCollision_469FF0(BaseGameObject* pHit);
    EXPORT void vUpdate_469BA0();
    EXPORT int vGetSaveState_46AC40(Meat_SaveState* pState);
    EXPORT BOOL vCanEatMe_4696A0();

private:
    MeatStates field_11C_state;
    __int16 field_11E;
    FP field_120_xpos;
    FP field_124_ypos;
    int field_128_timer;
    int field_12C;
public:
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
