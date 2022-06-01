#pragma once

#include "Throwable.hpp"
#include "BitField.hpp"

enum class LevelIds : s16;

enum class RockStates : s16
{
    eNone_0 = 0,
    eFallingOutOfRockSack_1 = 1,
    eRolling_2 = 2,
    eOnGround_3 = 3,
    eBouncing_4 = 4,
    eFallingOutOfWorld_5 = 5,
};

struct RockSaveState final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_obj_id;
    FP field_8_xpos;
    FP field_C_ypos;
    FP field_10_velx;
    FP field_14_vely;
    FP field_18_sprite_scale;
    s16 field_1C_path_number;
    LevelIds field_1E_lvl_number;
    enum RockStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8
    };
    BitField16<RockStateFlags> field_20_flags;
    s16 field_22_padding;
    s32 field_24_id;
    s16 field_28_line_type;
    s16 field_2A_count;
    RockStates field_2C_state;
    s16 field_2E_volume;
    FP field_30_xpos;
    FP field_34_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(RockSaveState, 0x38);

class Rock final : public BaseThrowable
{
public:
    Rock(FP xpos, FP ypos, s16 count);
    ~Rock();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VThrow_49E460(FP velX, FP velY) override;
    virtual Bool32 VCanThrow_49E350() override;
    virtual Bool32 VIsFalling_49E330() override;
    virtual void VTimeToExplodeRandom_411490() override;

    static s32 CreateFromSaveState(const u8* pData);

private:
    void vScreenChanged_49F030();
    s32 vGetSaveState_49F9A0(RockSaveState* pState);
    Bool32 vIsFalling_49E330();
    Bool32 vCanThrow_49E350();
    void vThrow_49E460(FP velX, FP velY);
    void InTheAir_49E4B0();
    s16 OnCollision_49EF10(BaseAliveGameObject* pObj);
    void vUpdate_49E9F0();

    void BounceHorizontally( FP hitX, FP hitY );

private:
    RockStates field_11C_state;
    s16 field_11E_volume;
    FP field_120_xpos;
    FP field_124_ypos;
    s32 field_128_shimmer_timer;
};
ALIVE_ASSERT_SIZEOF(Rock, 0x12C);
