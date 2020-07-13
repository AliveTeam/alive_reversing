#include "stdafx_ao.h"
#include "Function.hpp"
#include "Shrykull.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Abe.hpp"
#include "ZapLine.hpp"
#include "Shadow.hpp"

START_NS_AO

BaseGameObject* Shrykull::dtor_463990()
{
    SetVTable(this, 0x4BC9B8);

    if (field_118)
    {
        field_118->field_C_refCount--;
        field_118->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_11C)
    {
        field_11C->field_C_refCount--;
    }

    return dtor_401000();
}

Shrykull* Shrykull::Vdtor_464300(signed int flags)
{
    dtor_463990();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* Shrykull::VDestructor(signed int flags)
{
    return Vdtor_464300(flags);
}

void Shrykull::VScreenChanged_464280()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Shrykull::VScreenChanged()
{
    VScreenChanged_464280();
}

Shrykull* Shrykull::ctor_463880()
{
    ctor_401090();
    field_6_flags.Set(Options::eCanExplode_Bit7);
    SetVTable(this, 0x4BC9B8);
    field_4_typeId = Types::eShrykull_85;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 121, 1, 0);
    Animation_Init_417FD0(
        92004,
        121,
        79,
        ppRes,
        1);
    field_118 = 0;
    field_11C = 0;

    field_A8_xpos = sActiveHero_507678->field_A8_xpos;
    field_AC_ypos = sActiveHero_507678->field_AC_ypos;
    field_BC_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;
    field_C6_scale = sActiveHero_507678->field_C6_scale;
    field_10C_state = 0;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, sActiveHero_507678->field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX));

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }
    field_122_bUnknown = 0;
    return this;
}

void Shrykull::VOnThrowableHit(BaseGameObject*)
{
    // Empty
}

END_NS_AO

