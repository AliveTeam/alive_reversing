#pragma once

#include "Throwable.hpp"

enum class BoneStates : s16
{
    eSpawned_0 = 0,
    eAirborne_1 = 1,
    eCollided_2 = 2,
    eOnGround_3 = 3,
    eEdible_4 = 4,
    eFalling_5 = 5
};

struct Bone_SaveState final
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

    enum BoneStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8,
        eBit5_bHitObject = 0x10,
    };

    BitField16<BoneStateFlags> field_20_flags;
    s16 field_22_padding;
    s32 field_24_base_id;
    s16 field_28_line_type;
    s16 field_2A_count;
    BoneStates field_2C_state;
    s16 field_2E_volume_modifier;
    FP field_30_xpos;
    FP field_34_ypos;
    s32 field_38_time_to_live;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Bone_SaveState, 0x3C);

class Bone final : public BaseThrowable
{
public:
    Bone(FP xpos, FP ypos, s16 countId);
    ~Bone();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VThrow_49E460(FP velX, FP velY) override;
    virtual void VOnTrapDoorOpen() override;
    virtual Bool32 VCanThrow_49E350() override;
    virtual Bool32 VIsFalling_49E330() override;
    virtual void VTimeToExplodeRandom_411490() override;
    virtual Bool32 VCanBeEaten_411560();
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VGetCount_448080() override;
    static s32 CreateFromSaveState(const u8* pData);

private:
    void AddToPlatform_412310();
    void vThrow_411670(FP velX, FP velY);
    void vOnTrapDoorOpen_412490();
    Bool32 vIsFalling_411510();
    Bool32 vCanThrow_411530();
    s16 OnCollision_412140(BaseAnimatedWithPhysicsGameObject* pObj);
    Bool32 vCanBeEaten_411560();
    s32 vGetSaveState_412ED0(Bone_SaveState* pState);
    void InTheAir_4116C0();
    s16 vGetCount_412500();

private:
    BoneStates field_11C_state;
    s16 field_11E_volume_modifier;
    FP field_120_xpos;
    FP field_124_ypos;
    s32 field_128_shine_timer;
    s32 field_12C_time_to_live;
    s16 field_130_hit_object;
    s16 field_132_padding;
};
ALIVE_ASSERT_SIZEOF(Bone, 0x134);

struct Path_BoneBag final : public Path_TLV
{
    XDirection_short field_10_bone_fall_direction;
    u16 field_12_x_vel;
    u16 field_14_y_vel;
    Scale_short field_16_scale;
    s16 field_18_bone_amount;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoneBag, 0x1C);

class BoneBag final : public BaseAliveGameObject
{
public:
    BoneBag(Path_BoneBag* pTlv, s32 tlvInfo);
    ~BoneBag();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
 
private:
    s32 field_118_tlvInfo;
    s16 field_11C_bIs_hit;
    s16 field_11E_count;
    s16 field_120_allow_sound;
    s16 field_122_force_play_sound;
    FP field_124_velX;
    FP field_128_velY;
};
ALIVE_ASSERT_SIZEOF(BoneBag, 0x12C);
