#include "stdafx_ao.h"
#include "Function.hpp"
#include "Bat.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"

START_NS_AO

Bat* Bat::ctor_4046E0(Path_Bat* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BA0E8);

    field_4_typeId = Types::eBat_6;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 63, 1, 0);
    if (!ppRes)
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return this;
    }

    Animation_Init_417FD0(6560, 48, 17, ppRes, 1);
    field_10_anim.field_C_layer = 33;

    FP hitX = {};
    FP hitY = {};
    sCollisions_DArray_504C6C->RayCast_40C410(
        FP_FromInteger(pTlv->field_10_top_left.field_0_x),
        FP_FromInteger(pTlv->field_10_top_left.field_2_y),
        FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
        &field_E4_pLine,
        &hitX,
        &hitY,
        0x100);

    field_F0_tlvInfo = tlvInfo;

    field_A8_xpos = FP_FromInteger(field_E4_pLine->field_0_rect.x);
    field_AC_ypos = FP_FromInteger(field_E4_pLine->field_0_rect.y);

    field_EC = pTlv->field_18_ticks_before_moving;
    field_E8_speed = pTlv->field_1A_speed << 8;

    if (pTlv->field_1C_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = 6;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = 25;
    }

    field_F4_state = 0;
    field_10C = 0;
    field_F6_attack_duration = pTlv->field_1E_attack_duration;

    return this;
}

BaseGameObject* Bat::dtor_404870()
{
    SetVTable(this, 0x4BA0E8);
    if (field_10C)
    {
        field_10C->field_C_bCanKill--;
    }
    gMap_507BA8.TLV_Reset_446870(field_F0_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

BaseGameObject* Bat::VDestructor(signed int flags)
{
    return Vdtor_404FF0(flags);
}

void Bat::VScreenChanged()
{
    VScreenChanged_404FE0();
}

void Bat::VScreenChanged_404FE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

Bat* Bat::Vdtor_404FF0(signed int flags)
{
    dtor_404870();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
