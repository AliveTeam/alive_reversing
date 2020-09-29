#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class SwitchState : __int16
{
    eWaiting_0 = 0,
    ePulled_1 = 1,
    eFinished_2 = 2
};

enum class SwitchSoundType : __int16
{
    eWell_1 = 1,
    eUnknown_2 = 2,
    eDoor_3 = 3,
    eElectricWall_4 = 4,
    eSecurityOrb_5 = 5,
    eLift_6 = 6
};

enum class SwitchOp : __int16;

struct Path_Switch : public Path_TLV
{
    static constexpr unsigned short kType = 17; // NOTE: Is this meant to be a TlvType?
    SwitchOp field_10_target_action;
    __int16 field_12_scale;
    SwitchSoundType field_14_on_sound;
    SwitchSoundType field_16_off_sound;
    __int16 field_18_sound_direction;
    __int16 field_1A_trigger_id;
    __int16 field_1C_persist_offscreen;
    __int16 field_1E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Switch, 0x20);

class Switch : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual __int16 VPull_4D6050(__int16 bLeftDirection);
    EXPORT Switch* ctor_4D5860(Path_Switch* pTlv, DWORD tlvInfo);
private:
    EXPORT void dtor_4D5B00();
    EXPORT Switch* vdtor_4D5AD0(signed int flags);
    EXPORT void vUpdate_4D5C00();
    EXPORT void vScreenChanged_4D5B90();
    EXPORT __int16 vPull_4D6050( __int16 bLeftDirection);
private:
    __int16 field_F4_trigger_id;
    __int16 field_F6_padding;
    SwitchState field_F8_state;
    int field_FC_tlvInfo;
    __int16 field_100_flags;
    SwitchOp field_102_target_action;
    SwitchSoundType field_104_on_sound;
    SwitchSoundType field_106_off_sound;
    __int16 field_108_sound_direction;
};
ALIVE_ASSERT_SIZEOF(Switch, 0x10C);
