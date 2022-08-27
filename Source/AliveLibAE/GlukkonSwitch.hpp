#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_GlukkonSwitch final : public Path_TLV
{
    enum class Scale : s16
    {
        eHalf_0 = 0,
        eFull_1 = 1,
    };
    Scale mScale;
    s16 mOkSwitchId;
    s16 mFailSwitchId;
    u16 mXPos;
    u16 mYPos;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GlukkonSwitch, 0x1C);

class GlukkonSwitch final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    GlukkonSwitch(relive::Path_GlukkonSwitch* pTlv, s32 tlvInfo);
    ~GlukkonSwitch();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 PlayerNearMe();

private:
    s32 field_F4_tlvInfo = 0;
    s16 field_F8_state = 0;
    s16 field_FA_ok_switch_id = 0;
    s16 field_FC_fail_switch_id = 0;
    s16 field_FE = 0;
    s32 field_100_last_event_idx = 0;
    s16 field_104 = 0;
    s16 field_106 = 0;
    s16 field_108 = 0;
    s16 field_10A = 0;
    s16 field_10C = 0;
    s16 field_10E = 0;
    s16 field_110 = 0;
    s16 field_112 = 0;
    s16 field_114 = 0;
    s16 field_116 = 0;
    PSX_Point field_118_top_left = {};
    PSX_Point field_11C_bottom_right = {};
    s32 field_120_timer = 0;
};
ALIVE_ASSERT_SIZEOF(GlukkonSwitch, 0x124);
