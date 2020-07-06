#pragma once

#include "Throwable.hpp"
#include "BitField.hpp"

enum class LevelIds : __int16;

enum class RockStates : __int16
{
    eState_0 = 0,
    eState_1_FallToTheVoid = 1,
    eState_2 = 2,
    eState_3 = 3,
    eState_4 = 4,
    eState_5 = 5,
};

struct RockSaveState
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
    enum RockStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8
    };
    BitField16<RockStateFlags> field_20_flags;
    __int16 field_22_pad;
    int field_24_id;
    __int16 field_28_line_type;
    __int16 field_2A_count;
    RockStates field_2C_state;
    __int16 field_2E_state;
    FP field_30_xpos;
    FP field_34_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(RockSaveState, 0x38);

class Rock : public BaseThrowable
{
public:
    EXPORT Rock* ctor_49E150(FP xpos, FP ypos, __int16 count);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual void VThrow_49E460(FP velX, FP velY) override;
    virtual BOOL VCanThrow_49E350() override;
    virtual BOOL VIsFalling_49E330() override;
    virtual void VTimeToExplodeRandom_411490() override;

    static int CC CreateFromSaveState_49F720(const BYTE* pData);

private:
    EXPORT void vScreenChanged_49F030();
    EXPORT int vGetSaveState_49F9A0(RockSaveState* pState);
    EXPORT BOOL vIsFalling_49E330();
    EXPORT BOOL vCanThrow_49E350();
    EXPORT Rock* vdtor_49E370(signed int flags);
    EXPORT void dtor_49E3A0();
    EXPORT void vThrow_49E460(FP velX, FP velY);
    EXPORT void InTheAir_49E4B0();
    EXPORT __int16 OnCollision_49EF10(BaseAliveGameObject* pObj);
    EXPORT void vUpdate_49E9F0();

private:
    RockStates field_11C_state;
    __int16 field_11E_vol;
    FP field_120_xpos;
    FP field_124_ypos;
    int field_128_timer;
};
ALIVE_ASSERT_SIZEOF(Rock, 0x12C);

struct Path_RockSack : public Path_TLV
{
    __int16 field_10_side;
    unsigned __int16 field_12_x_vel;
    unsigned __int16 field_14_y_vel;
    __int16 field_16_scale;
    __int16 field_18_num_rocks;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_RockSack, 0x1C);

class RockSack : public BaseAliveGameObject
{
public:
    EXPORT RockSack* ctor_49F100(Path_RockSack* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    EXPORT RockSack* vdtor_49F2E0(signed int flags);
    EXPORT void dtor_49F310();
    EXPORT void vScreenChanged_49F700();
    EXPORT void vUpdate_49F3A0();

private:
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E_num_rocks;
    __int16 field_120_can_play_wobble_sound;
    __int16 field_122_force_wobble_sound; // Makes sure RockSacks play the sound when you first encounter them. Unused afterwards. -- Nemin (6/7/2020)
    FP field_124_x_vel;
    FP field_128_y_vel;
};
ALIVE_ASSERT_SIZEOF(RockSack, 0x12C);
