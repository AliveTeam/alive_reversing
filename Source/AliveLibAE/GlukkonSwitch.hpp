#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_GlukkonSwitch : public Path_TLV
{
    enum class Scale : s16
    {
        eHalf_0 = 0,
        eFull_1 = 1,
    };
    Scale field_10_scale;
    s16 field_12_ok_id;
    s16 field_14_fail_id;
    u16 field_16_xpos;
    u16 field_18_ypos;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GlukkonSwitch, 0x1C);

class GlukkonSwitch : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT GlukkonSwitch* ctor_444E60(Path_GlukkonSwitch* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

private:
    EXPORT void dtor_4450F0();

    EXPORT GlukkonSwitch* vdtor_4450C0(s32 flags);

    EXPORT void vScreenChange_4456D0();

    EXPORT s16 PlayerNearMe_445180();

    EXPORT void vUpdate_445200();

private:
    int field_F4_tlvInfo;
    s16 field_F8_state;
    s16 field_FA_ok_id;
    s16 field_FC_fail_id;
    s16 field_FE;
    int field_100_last_event_idx;
    s16 field_104;
    s16 field_106;
    s16 field_108;
    s16 field_10A;
    s16 field_10C;
    s16 field_10E;
    s16 field_110;
    s16 field_112;
    s16 field_114;
    s16 field_116;
    PSX_Point field_118_top_left;
    PSX_Point field_11C_bottom_right;
    int field_120_timer;
};
ALIVE_ASSERT_SIZEOF(GlukkonSwitch, 0x124);
