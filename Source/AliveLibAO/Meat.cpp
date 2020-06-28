#include "stdafx_ao.h"
#include "Function.hpp"
#include "Meat.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"

START_NS_AO

MeatSack* MeatSack::ctor_4390F0(Path_MeatSack* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BB930);

    field_4_typeId = Types::eMeatStack_55;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 4002, 1, 0);
    Animation_Init_417FD0(
        15688,
        92,
        86,
        ppRes,
        1);

    field_10C_tlvInfo = tlvInfo;
    field_CC_bApplyShadows &= ~1u;

    field_110 = 0;

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    field_118_velX = FP_FromRaw(pTlv->field_1A_x_vel << 8);

    // Throw the meat up into the air as it falls from the sack
    field_11C_velY = -FP_FromRaw(pTlv->field_1C_y_vel << 8);

    if (!pTlv->field_18_side)
    {
        field_118_velX = -field_118_velX;
    }

    if (pTlv->field_1E_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 8;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.field_C_layer = 27;
        field_C6_scale = 1;
    }

    field_112_num_items = pTlv->field_20_amount_of_meat;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* MeatSack::dtor_439250()
{
    SetVTable(this, 0x4BB930);
    gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    return dtor_401000();
}

BaseGameObject* MeatSack::VDestructor(signed int flags)
{
    return Vdtor_439550(flags);
}

MeatSack* MeatSack::Vdtor_439550(signed int flags)
{
    dtor_439250();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void MeatSack::VScreenChanged()
{
    VScreenChanged_439540();
}

void MeatSack::VScreenChanged_439540()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

END_NS_AO
