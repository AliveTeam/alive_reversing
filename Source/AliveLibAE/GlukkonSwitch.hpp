#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive
{
    struct Path_GlukkonSwitch;
}

class GlukkonSwitch final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    GlukkonSwitch(relive::Path_GlukkonSwitch* pTlv, const Guid& tlvId);
    ~GlukkonSwitch();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 PlayerNearMe();

private:
    Guid field_F4_tlvInfo;
    s16 field_F8_state = 0;
    s16 field_FA_ok_switch_id = 0;
    s16 field_FC_fail_switch_id = 0;
    s32 field_100_last_event_idx = 0;
    s16 field_114 = 0;
    s16 field_116 = 0;
    PSX_Point field_118_top_left = {};
    PSX_Point field_11C_bottom_right = {};
    s32 field_120_timer = 0;
};
