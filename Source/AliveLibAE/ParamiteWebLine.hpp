#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"

struct Path_ParamiteWebLine final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ParamiteWebLine, 0x14);

class ParamiteWebLine final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ParamiteWebLine(Path_ParamiteWebLine* pTlv, s32 tlvInfo);
    ~ParamiteWebLine();

    void Wobble_4E29D0(s16 ypos);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    

    virtual PSX_RECT* vGetBoundingRect_424FD0(PSX_RECT* pRect, s32 pointIdx) override;

    virtual void VRender(PrimHeader** ppOt) override;

private:
    ParamiteWebLine* vdtor_4E2460(s32 flags);

    void dtor_4E2490();

    void vUpdate_4E2A50();

    PSX_RECT* vGetBoundingRect_4E2B40(PSX_RECT* pRect, s32 idx);

    void vRender_4E2530(PrimHeader** ppOt);

private:
    void vScreenChanged_4E2BC0();

    s16 field_F4_anim_segment_count;
    s16 field_F6_piece_length;
    s16 field_F8_top;
    s16 field_FA_bottom;
    AnimationUnknown* field_FC_pRes;
    s32 field_100_tlv_info;
    s16 field_104_wobble_idx;
    s16 field_106_wobble_pos;
    Animation field_108_anim_flare;
    s16 field_1A0_pulse_position;
    s16 field_1A2_pulse_position_speed;
    s16 field_1A4_delay_counter;
    s16 field_1A6_padding;
    s16 field_1A8_padding;
    s16 field_1AA_padding;
};
ALIVE_ASSERT_SIZEOF(ParamiteWebLine, 0x1AC);
