#include "stdafx_ao.h"
#include "Function.hpp"
#include "Grenade.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5076EC, short, gInfiniteGrenades_5076EC, 0);

void Grenade::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_41F920();
}

void Grenade::VUpdate()
{
    VUpdate_41F240();
}

void Grenade::VScreenChanged()
{
    VScreenChanged_41F720();
}

Grenade* Grenade::ctor_41EBD0(FP xpos, FP ypos, __int16 numGrenades)
{
    ctor_401090();
    field_10E_bDead = 0;
    SetVTable(this, 0x4BB0A0);
    field_4_typeId = Types::eGrenade_40;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6005, 1, 0);
    Animation_Init_417FD0(
        1132,
        17,
        11,
        ppRes,
        1);

    field_6_flags.Clear(Options::eInteractive_Bit8);
    field_10_anim.field_B_render_mode = 0;

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_A8_xpos = xpos;
    field_120 = xpos;

    field_AC_ypos = ypos;
    field_124 = ypos;

    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_10C_count = numGrenades;

    if (numGrenades > 0)
    {
        field_110_state = 0;
    }
    else
    {
        field_110_state = 3;
        field_112_explode_timer = 90;
    }

    field_118 = 0;
    field_11C = 0;
    return this;
}

BaseGameObject* Grenade::dtor_41ECD0()
{
    SetVTable(this, 0x4BB0A0);

    if (field_11C)
    {
        field_11C->field_C_refCount--;
    }

    if (!gInfiniteGrenades_5076EC && !field_10E_bDead)
    {
        if (field_F8_pLiftPoint)
        {
            field_F8_pLiftPoint->VRemove(this);
            field_F8_pLiftPoint->field_C_refCount--;
            field_F8_pLiftPoint = nullptr;
        }

        if (gpThrowableArray_50E26C)
        {
            __int16 count = 0;
            if (field_10C_count >= 1u)
            {
                count = field_10C_count;
            }
            else
            {
                count = 1;
            }
            gpThrowableArray_50E26C->Remove_4540D0(count);
        }
    }
    return dtor_401000();
}

BaseGameObject* Grenade::VDestructor(signed int flags)
{
    return Vdtor_41F9F0(flags);
}

Grenade* Grenade::Vdtor_41F9F0(signed int flags)
{
    dtor_41ECD0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Grenade::AddToPlatform_41F7C0()
{
    NOT_IMPLEMENTED();
}

void Grenade::VTimeToExplodeRandom_41F9B0()
{
    field_112_explode_timer -= Math_NextRandom() % 16;
}

void Grenade::VThrow_41ED90(FP velX, FP velY)
{
    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_B4_velx = velX;
    field_B8_vely = velY;

    if (field_10C_count == 0)
    {
        field_110_state = 4;
    }
}

void Grenade::VScreenChanged_41F720()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Grenade::VUpdate_41F240()
{
    NOT_IMPLEMENTED();
}

void Grenade::VOnTrapDoorOpen_41F920()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove_451680(this);
        field_F8_pLiftPoint->field_C_refCount--;

        if (field_110_state == 1 || field_110_state == 2)
        {
            field_110_state = 0;
        }
        else if (field_110_state != 6)
        {
            field_110_state = 4;
        }
    }
}

signed __int16 Grenade::InTheAir_41EF10()
{
    NOT_IMPLEMENTED();
}

signed __int16 Grenade::OnCollision_BounceOff_41F650(BYTE* /*a2*/)
{
    NOT_IMPLEMENTED();
}

signed __int16 Grenade::BlowUp_41EDD0()
{
    const short timer = field_112_explode_timer--;
    if (!(timer % 16))
    {
        SFX_Play_43AD70(SoundEffect::GreenTick_3, 0, 0);
    }

    if (timer)
    {
        return 0;
    }

    auto pExplosion = ao_new<Explosion>();
    if (pExplosion)
    {
        pExplosion->ctor_458B80(
            field_A8_xpos,
            field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(5)),
            field_BC_sprite_scale);
    }

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_11C = pExplosion;
    pExplosion->field_C_refCount++;
    field_110_state = 6;

    auto pGibs = ao_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_407B20(5, field_A8_xpos, field_AC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_BC_sprite_scale);
    }
    return 1;
}

__int16 Grenade::VCanThrow_453EC0()
{
    return FALSE;
}

__int16 Grenade::VIsFalling_453ED0()
{
    return FALSE;
}

END_NS_AO
