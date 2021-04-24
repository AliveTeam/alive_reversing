#pragma once

#include "Throwable.hpp"

enum class BoneStates : s16
{
    eState_0_spawned = 0,
    eState_1_airborne = 1,
    eState_2_collided = 2,
    eState_3_on_ground = 3,
    eState_4_edible = 4,
    eState_5_falling = 5,
};

struct Bone_SaveState
{
    AETypes field_0_type;
    s16 field_2_padding;
    int field_4_obj_id;
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
    int field_24_base_id;
    s16 field_28_line_type;
    s16 field_2A_count;
    BoneStates field_2C_state;
    s16 field_2E_volume_modifier;
    FP field_30_xpos;
    FP field_34_ypos;
    int field_38_time_to_live;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Bone_SaveState, 0x3C);

class Bone : public BaseThrowable
{
public:
    EXPORT Bone* ctor_4112C0(FP xpos, FP ypos, s16 countId);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VThrow_49E460(FP velX, FP velY) override;
    virtual void VOnTrapDoorOpen() override;
    virtual BOOL VCanThrow_49E350() override;
    virtual BOOL VIsFalling_49E330() override;
    virtual void VTimeToExplodeRandom_411490() override;
    virtual BOOL VCanBeEaten_411560();
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual s16 VGetCount_448080() override;
    EXPORT static int CC CreateFromSaveState_412C10(const BYTE* pData);

private:
    EXPORT Bone* vdtor_411580(s32 flags);
    EXPORT void dtor_4115B0();
    EXPORT void AddToPlatform_412310();
    EXPORT void vThrow_411670(FP velX, FP velY);
    EXPORT void vOnTrapDoorOpen_412490();
    EXPORT BOOL vIsFalling_411510();
    EXPORT BOOL vCanThrow_411530();
    EXPORT s16 OnCollision_412140(BaseAnimatedWithPhysicsGameObject* pObj);
    EXPORT void vScreenChanged_4122D0();
    EXPORT BOOL vCanBeEaten_411560();
    EXPORT int vGetSaveState_412ED0(Bone_SaveState* pState);
    EXPORT void InTheAir_4116C0();
    EXPORT void vUpdate_411BC0();
    EXPORT s16 vGetCount_412500();

private:
    BoneStates field_11C_state;
    s16 field_11E_volume_modifier;
    FP field_120_xpos;
    FP field_124_ypos;
    int field_128_shine_timer;
    int field_12C_time_to_live;
    s16 field_130_hit_object;
    s16 field_132_padding;
};
ALIVE_ASSERT_SIZEOF(Bone, 0x134);

struct Path_BoneBag : public Path_TLV
{
    XDirection_short field_10_bone_fall_direction;
    u16 field_12_x_vel;
    u16 field_14_y_vel;
    Scale_short field_16_scale;
    s16 field_18_bone_amount;
    s16 field_1A_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoneBag, 0x1C);

class BoneBag : public BaseAliveGameObject
{
public:
    EXPORT BoneBag* ctor_4125C0(Path_BoneBag* pTlv, int tlvInfo);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void vScreenChanged_412BF0();
    EXPORT BoneBag* vdtor_4127C0(s32 flags);
    EXPORT void dtor_4127F0();
    EXPORT void vUpdate_412880();

private:
    int field_118_tlvInfo;
    s16 field_11C_is_hit;
    s16 field_11E_count;
    s16 field_120_allow_sound;
    s16 field_122_force_play_sound;
    FP field_124_velX;
    FP field_128_velY;
};
ALIVE_ASSERT_SIZEOF(BoneBag, 0x12C);
