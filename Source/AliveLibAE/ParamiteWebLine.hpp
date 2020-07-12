#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Path.hpp"

struct Path_ParamiteWebLine : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ParamiteWebLine, 0x14);

class ParamiteWebLine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParamiteWebLine* ctor_4E1FC0(Path_ParamiteWebLine* pTlv, int tlvInfo);

    EXPORT void Wobble_4E29D0(short ypos);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual PSX_RECT* vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx) override;

    virtual void VRender(int** pOrderingTable) override;

private:
    EXPORT ParamiteWebLine* vdtor_4E2460(signed int flags);

    EXPORT void dtor_4E2490();

    EXPORT void vUpdate_4E2A50();

    EXPORT PSX_RECT* vGetBoundingRect_4E2B40(PSX_RECT* pRect, int idx);

    EXPORT void vRender_4E2530(int **ppOt);
private:

    EXPORT void vScreenChanged_4E2BC0();

    __int16 field_F4_anim_segment_count;
    __int16 field_F6_piece_length;
    __int16 field_F8_top;
    __int16 field_FA_bottom;
    AnimationUnknown* field_FC_pRes;
    int field_100_tlv_info;
    __int16 field_104_wobble_idx;
    __int16 field_106_wobble_pos;
    Animation field_108_anim_flare;
    __int16 field_1A0_pulse_position;
    __int16 field_1A2_pulse_position_speed;
    __int16 field_1A4_delay_counter;
    __int16 field_1A6_padding;
    __int16 field_1A8_padding;
    __int16 field_1AA_padding;
};
ALIVE_ASSERT_SIZEOF(ParamiteWebLine, 0x1AC);
