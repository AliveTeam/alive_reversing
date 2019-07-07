#pragma once

#include "Throwable.hpp"

struct Bone_SaveState
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

    enum BoneStateFlags
    {
        eBit1_bRender = 0x1,
        eBit2_bDrawable = 0x2,
        eBit3_bLoop = 0x4,
        eBit4_bInteractive = 0x8,
        eBit5_Unknown = 0x10,
    };

    BitField16<BoneStateFlags> field_20_flags;
    __int16 field_22;
    int field_24_base_id;
    __int16 field_28_line_type;
    __int16 field_2A_count;
    __int16 field_2C_state;
    __int16 field_2E;
    FP field_30;
    FP field_34;
    int field_38;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Bone_SaveState, 0x3C);

class Bone : public BaseThrowable
{
public:
    EXPORT Bone* ctor_4112C0(FP xpos, FP ypos, __int16 countId);

    virtual void VScreenChanged() override;
    virtual void VThrow_49E460(FP velX, FP velY) override;
    virtual void VOnTrapDoorOpen() override;
    virtual BOOL VCanThrow_49E350() override;
    virtual BOOL VIsFalling_49E330() override;
    virtual void VTimeToExplodeRandom_411490() override;
    virtual BOOL VCanBeEaten_411560();
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    EXPORT static int CC CreateFromSaveState_412C10(const BYTE* pData);

private:
    EXPORT Bone* vdtor_411580(signed int flags);
    EXPORT void dtor_4115B0();
    EXPORT void AddToPlatform_412310();
    EXPORT void vThrow_411670(FP velX, FP velY);
    EXPORT void vOnTrapDoorOpen_412490();
    EXPORT BOOL vIsFalling_411510();
    EXPORT BOOL vCanThrow_411530();
    EXPORT __int16 OnCollision_412140(BaseAnimatedWithPhysicsGameObject* pObj);
    EXPORT void vScreenChanged_4122D0();
    EXPORT BOOL vCanBeEaten_411560();
    EXPORT int vGetSaveState_412ED0(Bone_SaveState* pState);

private:
    __int16 field_11C_state;
    __int16 field_11E;
    FP field_120;
    FP field_124;
    int field_128;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
};
ALIVE_ASSERT_SIZEOF(Bone, 0x134);

struct Path_BoneBag : public Path_TLV
{
    __int16 field_10_side;
    unsigned __int16 field_12_x_vel;
    unsigned __int16 field_14_y_vel;
    __int16 field_16_scale;
    __int16 field_18_num_bones;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoneBag, 0x1C);

class BoneBag : public BaseAliveGameObject
{
public:
    EXPORT BoneBag* ctor_4125C0(Path_BoneBag* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

private:
    EXPORT void vScreenChanged_412BF0();
    EXPORT BoneBag* vdtor_4127C0(signed int flags);
    EXPORT void dtor_4127F0();

    EXPORT void vUpdate_412880();

private:
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E_count;
    __int16 field_120;
    __int16 field_122;
    FP field_124_velX;
    FP field_128_velY;
};
ALIVE_ASSERT_SIZEOF(BoneBag, 0x12C);
