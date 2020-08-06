#include "stdafx_ao.h"
#include "Function.hpp"
#include "Meat.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "ThrowableArray.hpp"

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

Meat* Meat::ctor_438550(FP xpos, FP ypos, __int16 count)
{
    ctor_401090();
    SetVTable(this, 0x4BB8B0);

    field_10E_bDead = 0;

    field_4_typeId = Types::eMeat_54;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2023, 1, 0);
    Animation_Init_417FD0(
        500,
        18,
        9,
        ppRes,
        1);

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    field_114_xpos = xpos;
    field_118_ypos = ypos;

    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_11C_timer = 0;
    field_6_flags.Clear(Options::eInteractive_Bit8);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_120_deadtimer = gnFrameCount_507670 + 600;
    field_124_pLine = 0;
    field_10C_count = count;
    field_110_state = 0;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* Meat::dtor_438660()
{
    SetVTable(this, 0x4BB8B0);
    if (!field_10E_bDead)
    {
        if (gpThrowableArray_50E26C)
        {
            gpThrowableArray_50E26C->Remove_4540D0(field_10C_count >= 1u ? field_10C_count : 1);
        }
    }
    return dtor_401000();
}

BaseGameObject* Meat::VDestructor(signed int flags)
{
    return Vdtor_4390D0(flags);
}

Meat* Meat::Vdtor_4390D0(signed int flags)
{
    dtor_438660();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Meat::VScreenChanged()
{
    VScreenChanged_438E70();
}

void Meat::VScreenChanged_438E70()
{
    if (gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path ||
        gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Meat::VThrow(FP velX, FP velY)
{
    VThrow_4386E0(velX, velY);
}

void Meat::VThrow_4386E0(FP velX, FP velY)
{
    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_B4_velx = velX;
    field_B8_vely = velY;

    if (field_10C_count == 0)
    {
        field_110_state = 2;
    }
    else
    {
        field_110_state = 1;
    }
}

__int16 Meat::VCanThrow()
{
    return VCanThrow_4390B0();
}

__int16 Meat::VCanThrow_4390B0()
{
    return field_110_state == 2;
}

BOOL Meat::VCanEatMe()
{
    return VCanEatMe_4390C0();
}

BOOL Meat::VCanEatMe_4390C0()
{
    return field_110_state != 0;
}

END_NS_AO
