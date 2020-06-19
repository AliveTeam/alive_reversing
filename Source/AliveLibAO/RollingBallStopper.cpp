#include "stdafx_ao.h"
#include "RollingBallStopper.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "ResourceManager.hpp"
#include "FixedPoint.hpp"
#include "Sfx.hpp"

START_NS_AO

EXPORT RollingBallStopper* RollingBallStopper::ctor_43BCE0(Path_RollingBallStopper* pTlv, int /*tlvInfo*/)
{
    NOT_IMPLEMENTED();

    ctor_401090();

    SetVTable(this, 0x4BBAC8);
    field_4_typeId = Types::eRollingBallStopper_60;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2003, TRUE, FALSE);
    Animation_Init_417FD0(1256, 106, 73, ppRes, 1);
    field_10_anim.field_C_layer = 37;
    
    field_114_switch_on_id = pTlv->field_18_id_on;

    if (pTlv->field_1A_scale == 1)
    {
        field_BC_scale = FP_FromDouble(0.5);
        field_C6 = 0;
    }

    field_116_switch_id_off = pTlv->field_1C_id_off;

    /*
    if (pTlv->field_1E_direction == 0)
    {
        LOBYTE(field_10_anim.field_4_flags) |= 0x10u;
    }

    field_A8_xpos = pTlv->field_C_bottom_right.field_0_x << 16;
    field_B4_velx = -1441792;
    field_AC_ypos = pTlv->field_C_bottom_right.field_2_y << 16;
    
    bXFlip = (LOBYTE(field_10_anim.field_4_flags) >> 4) & 1;
    if (bXFlip)
    {
        field_B4_velx = 0x160000;
    }

    flags_byte1 = *(_WORD*)&pTlv->field_0_flags & 0xFF00;
    field_B8_vely = 0;
    field_10C_tlvInfo = tlvInfo;

    if (flags_byte1 == 0x100)
    {
        field_AC_ypos += Math_FixedPoint_Multiply_451040(field_BC_scale,  0x460000);
        if (bXFlip)
        {
            scaledVal = Math_FixedPoint_Multiply_451040(field_BC_scale, 0x230000);
            field_112_state = 2;
            field_A8_xpos += scaledVal;
        }
        else
        {
            field_112_state = 2;
            field_A8_xpos = field_A8_xpos - Math_FixedPoint_Multiply_451040(field_BC_scale, 0x230000);
        }
    }
    else
    {
        field_112_state = 0;
        sSwitchStates_505568[pTlv->field_1C_id_off] = 0;
        sSwitchStates_505568[pTlv->field_18_id_on] = 0;
    }

    const auto xpos_1 = field_A8_xpos;
    sub_401D30(1);
    field_A8_xpos = xpos_1;
    
    if ((field_10_anim.field_4_flags >> 4) & 1)
    {
        scaled = Grid_Scale_41FA30(field_BC_scale);
        x1_fp = Math_FP_450FB0(scaled, 0x20000) + (xpos_1 / 0x10000 << 16);
    }
    else
    {
        scaled2 = Grid_Scale_41FA30(field_BC_scale);
        x1_fp = (xpos_1 / 0x10000 << 16) - Math_FP_450FB0(scaled2, 0x20000);
    }

    x1 = x1_fp / 0x10000;
    y1 = field_AC_ypos / 0x10000;
    if (field_BC_scale == 0x10000)
    {
        pNewLine = sCollisions_DArray_504C6C->Add_Dynamic_Collision_Line_40C8A0(x1, y1 - 70, x1, y1, 1);
    }
    else
    {
        pNewLine = sCollisions_DArray_504C6C->Add_Dynamic_Collision_Line_40C8A0(x1, y1 - 35, x1, y1, 5);
    }
    field_118_pLine = pNewLine;
     */
    return this;
}

BaseGameObject* RollingBallStopper::Vdtor_43C160(signed int flags)
{
    dtor_43C0A0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* RollingBallStopper::dtor_43C0A0()
{
    SetVTable(this, 0x4BBAC8);

    if (field_112_state != 0)
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, 1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, 0, 0, 0);
    }

    if (field_118_pLine)
    {
        Rect_Clear_40C920(field_118_pLine);
    }

    return dtor_401000();
}

void RollingBallStopper::VScreenChanged_43C130()
{
    if (field_112_state == 1)
    {
        SwitchStates_Set(field_116_switch_id_off, 1);
    }
    field_6_flags.Set(Options::eDead_Bit3);
}

void RollingBallStopper::VUpdate_43BF70()
{
    switch (field_112_state)
    {
    case 0:
        if (SwitchStates_Get(field_114_switch_on_id))
        {
            Rect_Clear_40C920(field_118_pLine);
            field_118_pLine = nullptr;
            field_112_state = 1;
            SFX_Play_43AE60(33u, 100, -2400, 0);
            SFX_Play_43AE60(35u, 80, -800, 0);
        }
        break;

    case 1:
        field_B8_vely += (field_BC_scale * FP_FromInteger(25));
        if (field_B8_vely <= (field_BC_scale * FP_FromInteger(70)))
        {
            field_A8_xpos += field_B4_velx;
            field_AC_ypos += (field_BC_scale * FP_FromInteger(25));
        }
        else
        {
            field_112_state = 2;
            SwitchStates_Set(field_116_switch_id_off, 1);
        }
        break;
    }
}

BaseGameObject* RollingBallStopper::VDestructor(signed int flags)
{
    return Vdtor_43C160(flags);
}

void RollingBallStopper::VScreenChanged()
{
    VScreenChanged_43C130();
}

void RollingBallStopper::VUpdate()
{
    VUpdate_43BF70();
}

END_NS_AO
