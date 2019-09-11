#include "stdafx.h"
#include "SecurityDoor.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"

EXPORT __int16 CC Code_Length_4C9DB0(unsigned int code)
{
    __int16 i = 0;
    for (i = 0; code; ++i)
    {
        code /= 10u;
    }
    return i;
}

const int dword_560F10[12] =
{
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
    0,
    0
};

EXPORT int CC Code_Convert_4C9DF0(unsigned __int16 code1, unsigned __int16 code2)
{
    if (code2)
    {
        return code2 + code1 * dword_560F10[Code_Length_4C9DB0(code2)];
    }
    else
    {
        return code1;
    }
}

SecurityDoor* SecurityDoor::ctor_4ABFC0(Path_SecurityDoor* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    field_104_event_idx = -1;
    field_118_max_idx = 0;
    field_11A_event_idx = -1;

    SetVTable(this, 0x547028);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 6027);
    Animation_Init_424E10(1512, 70, 19u, ppRes, 1, 1u);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_F4_tlvInfo = tlvInfo;

    field_20_animation.field_C_render_layer = 22;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_FA_id = pTlv->field_12_id;
    field_FC_code_converted = Code_Convert_4C9DF0(pTlv->field_14_code_1, pTlv->field_16_code2);
    field_100_code_len = Code_Length_4C9DB0(field_FC_code_converted);
    field_11C_top_left = pTlv->field_8_top_left;
    field_120_bottom_right = pTlv->field_C_bottom_right;
    field_B8_xpos = FP_FromInteger(pTlv->field_18_xpos);
    field_BC_ypos = FP_FromInteger(pTlv->field_1A_ypos);

    PSX_Point point = {};
    gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&point);

    if (field_B8_xpos > FP_FromInteger(0))
    {
        field_B8_xpos -= FP_FromInteger(point.field_0_x);
    }
    else
    {
        field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    }

    if (field_BC_ypos > FP_FromInteger(0))
    {
        field_BC_ypos -= FP_FromInteger(point.field_2_y);
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
        field_124_timer = sGnFrame_5C1B84 + 10;
    }

    return this;
}

void SecurityDoor::VScreenChanged()
{
    vScreenChanged_4AC970();
}

void SecurityDoor::VUpdate()
{
    // todo 0x4AC380
}

BaseGameObject* SecurityDoor::VDestructor(signed int flags)
{
    return vdtor_4AC230(flags);
}

void SecurityDoor::dtor_4AC260()
{
    SetVTable(this, 0x547028);
    if (field_F8_state != 1)
    {
        field_F8_state = 0;
    }
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, field_F8_state + 1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void SecurityDoor::vScreenChanged_4AC970()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

SecurityDoor* SecurityDoor::vdtor_4AC230(signed int flags)
{
    dtor_4AC260();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

__int16 SecurityDoor::IsPlayerNear_4AC300()
{
    const short xpos = FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos);
    const short ypos = FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos);

    if (xpos < field_11C_top_left.field_0_x || xpos > field_120_bottom_right.field_0_x)
    {
        return 0;
    }

    if (ypos >= field_11C_top_left.field_2_y && ypos <= field_120_bottom_right.field_2_y)
    {
        return 1;
    }

    return 0;
}
