#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

enum class SwitchState : s16
{
    eWaiting_0 = 0,
    ePulled_1 = 1,
    eFinished_2 = 2
};

enum class SwitchSoundType : s16
{
    eNone = 0,
    eWell_1 = 1,
    eUnknown_2 = 2,
    eDoor_3 = 3,
    eElectricWall_4 = 4,
    eSecurityOrb_5 = 5,
    eLift_6 = 6
};

enum class SwitchOp : s16;

enum class SwitchSoundDirection : s16
{
    eLeftAndRight_0 = 0,
    eLeft_1 = 1,
    eRight_2 = 2,
};

struct Path_Switch final : public Path_TLV
{
    SwitchOp field_10_target_action;
    Scale_short field_12_scale;
    SwitchSoundType field_14_on_sound;
    SwitchSoundType field_16_off_sound;
    SwitchSoundDirection field_18_sound_direction;
    s16 field_1A_trigger_id;
    Choice_short field_1C_persist_offscreen;
    s16 field_1E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Switch, 0x20);

class Switch final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s16 VPull_4D6050(s16 bLeftDirection);
    EXPORT Switch* ctor_4D5860(Path_Switch* pTlv, u32 tlvInfo);

private:
    EXPORT void dtor_4D5B00();
    EXPORT Switch* vdtor_4D5AD0(s32 flags);
    EXPORT void vUpdate_4D5C00();
    EXPORT void vScreenChanged_4D5B90();
    EXPORT s16 vPull_4D6050(s16 bLeftDirection);

private:
    s16 field_F4_trigger_id;
    s16 field_F6_padding;
    SwitchState field_F8_state;
    s32 field_FC_tlvInfo;
    enum Flags_100
    {
        eBit1_switch_anim_left_direction = 0x1,
        eBit2_persist_offscreen = 0x2,
    };
    BitField16<Flags_100> field_100_flags;
    SwitchOp field_102_target_action;
    SwitchSoundType field_104_on_sound;
    SwitchSoundType field_106_off_sound;
    SwitchSoundDirection field_108_sound_direction;
};
ALIVE_ASSERT_SIZEOF(Switch, 0x10C);
