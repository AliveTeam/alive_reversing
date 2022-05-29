#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_MeatSaw final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_switch_min_time_off;
    s16 field_1C_switch_max_time_off;
    s16 field_1E_max_rise_time;
    s16 field_20_switch_id;
    enum class Type : s16
    {
        eAutomaticPersistOffscreen_0 = 0,
        eAutomatic_1 = 1,
        eSwitchId_2 = 2
    };
    Type field_22_type;
    s16 field_24_speed;
    enum class StartState : s16
    {
        eOff_0 = 0,
        eOn_1 = 1
    };
    StartState field_26_start_state;
    s16 field_28_off_speed;
    s16 field_2A_automatic_min_time_off;
    s16 field_2C_automatic_max_time_off;
    s16 field_2E_inital_position;
};
ALIVE_ASSERT_SIZEOF(Path_MeatSaw, 0x30);

enum class MeatSawStates : s16
{
    eIdle_0 = 0,
    eGoingDown_1 = 1,
    eGoingUp_2 = 2
};

class MeatSaw final : public BaseAnimatedWithPhysicsGameObject
{
public:
    MeatSaw(Path_MeatSaw* pTlv, s32 tlvInfo);
    ~MeatSaw();

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
    s16 field_F6_switch_min_time_off;
    s16 field_F8_switch_max_time_off;
    s16 field_FA_automatic_min_time_off;
    s16 field_FC_automatic_max_time_off;
    s16 field_FE_padding;
    s32 field_100_tlvInfo;
    s32 field_104_idle_timer;
    s32 field_108_SFX_timer;
    s32 field_10C_FrameCount;
    Animation field_110_anim;

    enum flags_1A8
    {
        eBit1_ResetOffscreen = 0x1,
        eBit2_SwitchIdMeatSaw = 0x2,
        eBit3_AutomaticMeatSawIsDown = 0x4
    };
    BitField16<flags_1A8> field_1A8_flags;
    s16 field_1AA_padding;
};
ALIVE_ASSERT_SIZEOF(MeatSaw, 0x1AC);


} // namespace AO
