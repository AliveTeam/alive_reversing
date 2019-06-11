#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_GlukkonSwitch : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_ok_id;
    __int16 field_14_fail_id;
    unsigned __int16 field_16_xpos;
    unsigned __int16 field_18_ypos;
    __int16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_GlukkonSwitch, 0x1C);

class GlukkonSwitch : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT GlukkonSwitch* ctor_444E60(Path_GlukkonSwitch* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

private:
    EXPORT void dtor_4450F0();

    EXPORT GlukkonSwitch* vdtor_4450C0(signed int flags);

    EXPORT void vScreenChange_4456D0();

    EXPORT __int16 PlayerNearMe_445180();

    EXPORT void vUpdate_445200();

private:
    int field_E4_not_used[4];
    int field_F4_tlvInfo;
    __int16 field_F8_state;
    __int16 field_FA_ok_id;
    __int16 field_FC_fail_id;
    __int16 field_FE;
    int field_100_last_event_idx;
    __int16 field_104;
    __int16 field_106;
    __int16 field_108;
    __int16 field_10A;
    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110;
    __int16 field_112;
    __int16 field_114;
    __int16 field_116;
    PSX_Point field_118_top_left;
    PSX_Point field_11C_bottom_right;
    int field_120_timer;
};
ALIVE_ASSERT_SIZEOF(GlukkonSwitch, 0x124);
