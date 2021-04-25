#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct Path_MeatSaw : public Path_TLV
{
    s16 field_18_scale_background;
    s16 field_1A_min_time_off1;
    s16 field_1C_max_time_off1;
    s16 field_1E_max_rise_time;
    s16 field_20_id;
    s16 field_22_type;
    s16 field_24_speed;
    s16 field_26_start_state;
    s16 field_28_off_speed;
    s16 field_2A_min_time_off2;
    s16 field_2C_max_time_off2;
    s16 field_2E_inital_position;
};
ALIVE_ASSERT_SIZEOF(Path_MeatSaw, 0x30);

enum class MeatSawStates : s16
{
    eIdle_0 = 0,
    eGoingDown_1 = 1,
    eGoingUp_2 = 2
};

class MeatSaw : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MeatSaw* ctor_439570(Path_MeatSaw* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_4398F0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT MeatSaw* Vdtor_43A0B0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_43A060();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4399D0();

    EXPORT void GrindUpObjects_439CD0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_439F50(PrimHeader** ppOt);

    s32 field_D4_padding[4];
    MeatSawStates field_E4_state;
    u16 field_E6_max_rise_time;
    s16 field_E8_speed2;
    s16 field_EA_speed1;
    s16 field_EC_off_speed;
    u16 field_EE_switch_id;
    s16 field_F0_switch_value;
    s16 field_F2_switch_value;
    s16 field_F4;
    s16 field_F6_min_time_off1;
    s16 field_F8_field_1C_max_time_off1;
    s16 field_FA_min_time_off2;
    s16 field_FC_max_time_off2;
    s16 field_FE_padding;
    s32 field_100_tlvInfo;
    s32 field_104_idle_timer;
    s32 field_108_SFX_timer;
    s32 field_10C_FrameCount;
    Animation field_110_anim;
    u16 field_1A8_flags;
    s16 field_1AA_padding;
};
ALIVE_ASSERT_SIZEOF(MeatSaw, 0x1AC);


}

