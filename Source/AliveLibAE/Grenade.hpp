#pragma once

#include "Throwable.hpp"

enum class GrenadeStates : s16
{
    eFallingToBeCollected_0 = 0,
    eWaitToBeCollected_1 = 1,
    eDoesNothing_2 = 2,
    eCountingDown_3 = 3,
    eFalling_4 = 4,
    eHitGround_5 = 5,
    eWaitForExplodeEnd_6 = 6,
    eExploded_7 = 7,
    eDoesNothing_8 = 8,
    eFallingBlowUpOnGround_9 = 9,
};

struct Grenade_SaveState final
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

    enum Flags_20
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8,
        eBit5_unused = 0x10,
        eBit6_bExplodeNow = 0x20,
        eBit7_bBlowUpOnCollision = 0x40,
    };
    BitField16<Flags_20> field_20_flags;
    s16 field_22_padding;
    s32 field_24_base_id;
    s16 field_28_line_type;
    s16 field_2A_savedcount;
    GrenadeStates field_2C_state;
    s16 field_2E;
    s16 field_30_explode_timer;
    s16 field_32_padding;
    FP field_34_xpos;
    FP field_38_ypos;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Grenade_SaveState, 0x3C);

class Grenade final : public BaseThrowable
{
public:
    Grenade(FP xpos, FP ypos, s32 numGrenades, bool bBlowUpOnCollision, s32 unused, BaseGameObject* pOwner);
    ~Grenade();

    virtual void VScreenChanged() override;

    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    virtual void VUpdate() override;

    virtual void VOnTrapDoorOpen() override;

    virtual void VThrow_49E460(FP velX, FP velY) override;

    virtual Bool32 VCanThrow_49E350() override;

    virtual Bool32 VIsFalling_49E330() override;

    virtual void VTimeToExplodeRandom_411490() override;


    // Next 2 virtuals are base

    static s32 CreateFromSaveState_449410(const u8* pBuffer);

private:
    s32 GetSaveState_4496B0(Grenade_SaveState* pState);

    void Init_448110(FP xpos, FP ypos);

    void vOnTrapDoorOpen_449390();


    void vThrow_4482E0(FP velX, FP velY);

    Bool32 vCanThrow_49A5F0();

    Bool32 vIsFalling_49A610();

    void vTimeToExplodeRandom_4480A0();

    void BlowUp_4483C0(s16 bSmallExplosion);

    s16 TimeToBlowUp_448350();

    s16 InTheAir_4484F0(s16 blowUpOnFloorTouch);

    void AddToPlatform_449210();

    s16 OnCollision_BounceOff_448F90(BaseGameObject* pHit);

    s16 OnCollision_InstantExplode_4490D0(BaseGameObject* pHit);


private:
    s32 field_11C_explosion_id;
    GrenadeStates field_120_state;
    s16 field_122_explode_timer;
    s16 field_124;
    s16 field_126_input;
    FP field_128_xpos;
    FP field_12C_ypos;
    s16 field_130_unused;
    s16 field_132_bBlowUpOnCollision;
    s16 field_134_bExplodeNow;
    s16 field_136_padding;
    BaseGameObject* field_138_pOwner;
};
ALIVE_ASSERT_SIZEOF(Grenade, 0x13C);
