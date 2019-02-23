#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class SwitchOp : __int16;

struct Path_Switch : public Path_TLV
{
    static constexpr unsigned short kType = 17;
    SwitchOp field_10_target_action;
    __int16 field_12_scale;
    __int16 field_14_on_sound;
    __int16 field_16_off_sound;
    __int16 field_18_sound_direction;
    __int16 field_1A_trigger_id;
    __int16 field_1C_persist_offscreen;
    __int16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Switch, 0x20);

class Switch : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual __int16 Vsub_4D6050(__int16 a2);
    EXPORT Switch* ctor_4D5860(Path_Switch* pTlv, DWORD tlvInfo);
private:
    EXPORT void dtor_4D5B00();
    EXPORT Switch* vdtor_4D5AD0(signed int flags);
    EXPORT void vUpdate_4D5C00();
    EXPORT void vScreenChanged_4D5B90();
    EXPORT __int16 vsub_4D6050( __int16 a2);
private:
    int field_E4[4];
    __int16 field_F4_trigger_id;
    __int16 field_F6; // pad
    // TODO: Make enum
    __int16 field_F8_state;
    //__int16 field_FA; // pad
    int field_FC_tlvInfo;
    __int16 field_100_flags;
    SwitchOp field_102_target_action;
    __int16 field_104_on_sound;
    __int16 field_106_off_sound;
    __int16 field_108_sound_direction;
    //__int16 field_10A; // pad
};
ALIVE_ASSERT_SIZEOF(Switch, 0x10C);
