#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_MeatSaw;
}

namespace AO {

enum class MeatSawStates : s16
{
    eIdle_0 = 0,
    eGoingDown_1 = 1,
    eGoingUp_2 = 2
};

class MeatSaw final : public BaseAnimatedWithPhysicsGameObject
{
public:
    MeatSaw(relive::Path_MeatSaw* pTlv, const Guid& tlvId);
    ~MeatSaw();


    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    void LoadAnimations();
    void GrindUpObjects_439CD0();

    MeatSawStates field_E4_state = MeatSawStates::eIdle_0;
    u16 field_E6_max_rise_time = 0;
    s16 field_E8_speed2 = 0;
    s16 field_EA_speed1 = 0;
    s16 field_EC_off_speed = 0;
    u16 field_EE_switch_id = 0;
    s32 field_F0_switch_value = 0;
    s32 field_F2_switch_value = 0;
    s16 field_F4 = 0;
    s16 field_F6_switch_min_time_off = 0;
    s16 field_F8_switch_max_time_off = 0;
    s16 field_FA_automatic_min_time_off = 0;
    s16 field_FC_automatic_max_time_off = 0;
    Guid field_100_tlvInfo;
    s32 field_104_idle_timer = 0;
    s32 field_108_SFX_timer = 0;
    s32 field_10C_FrameCount = 0;
    Animation field_110_anim;
    bool mResetOffscreen = false;
    bool mUsesSwitchId = false;
    bool mAutomaticMeatSawIsDown = false;
};


} // namespace AO
