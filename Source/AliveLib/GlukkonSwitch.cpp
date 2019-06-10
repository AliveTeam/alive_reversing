#include "stdafx.h"
#include "GlukkonSwitch.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

GlukkonSwitch* GlukkonSwitch::ctor_444E60(Path_GlukkonSwitch* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    field_100_last_event_idx = -1;
    field_114 = 0;
    field_116 = -1;

    SetVTable(this, 0x545534);
    field_4_typeId = Types::eHelpPhone_68;
    
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 6027);
    Animation_Init_424E10(1512, 70, 19u, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_F4_tlvInfo = tlvInfo;
    field_20_animation.field_C_render_layer = 22;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }

    field_FA_ok_id = pTlv->field_12_ok_id;
    field_FC_fail_id = pTlv->field_14_fail_id;
    field_118_top_left = pTlv->field_8_top_left;
    field_11C_bottom_right = pTlv->field_C_bottom_right;
    
    field_B8_xpos = FP_FromInteger(pTlv->field_16_xpos);
    field_BC_ypos = FP_FromInteger(pTlv->field_18_ypos);

    PSX_Point pos = {};
    gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&pos);
    if (field_B8_xpos > FP_FromInteger(0))
    {
        field_B8_xpos -= FP_FromInteger(pos.field_0_x);
    }
    else
    {
        field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    }

    if (field_BC_ypos > FP_FromInteger(0))
    {
        field_BC_ypos -= FP_FromInteger(pos.field_2_y);
    }
    else
    {
        field_BC_ypos = FP_FromInteger((pTlv->field_8_top_left.field_2_y + pTlv->field_C_bottom_right.field_2_y) / 2);
    }

    if (pTlv->field_1_unknown)
    {
        field_F8_state = pTlv->field_1_unknown - 1;
    }
    else
    {
        field_F8_state = 0;
    }

    if (field_F8_state != 1)
    {
        field_120_timer = sGnFrame_5C1B84 + 10;
    }

    return this;
}

BaseGameObject* GlukkonSwitch::VDestructor(signed int flags)
{
    return vdtor_4450C0(flags);
}

void GlukkonSwitch::VScreenChanged()
{
    vScreenChange_4456D0();
}

void GlukkonSwitch::dtor_4450F0()
{
    SetVTable(this, 0x545534);
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

GlukkonSwitch* GlukkonSwitch::vdtor_4450C0(signed int flags)
{
    dtor_4450F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void GlukkonSwitch::vScreenChange_4456D0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}
