#include "stdafx.h"
#include "InvisibleEffect.hpp"
#include "Function.hpp"
#include "Abe.hpp"
#include "VRam.hpp"
#include "stdlib.hpp"

EXPORT InvisibleEffect* InvisibleEffect::ctor_45F280(Abe* pAbe)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545A60);

    field_4_typeId = BaseGameObject::Types::eInvisibleEffect_75;

    field_44_objId = pAbe->field_8_object_id;

    field_24_pAlloc = reinterpret_cast<WORD*>(malloc_non_zero_4954F0(pAbe->field_20_animation.field_90_pal_depth * sizeof(WORD)));
    Pal_Copy_483560(
        pAbe->field_20_animation.field_8C_pal_vram_xy,
        pAbe->field_20_animation.field_90_pal_depth,
        field_24_pAlloc,
        &field_28);

    field_30_pPalAlloc = reinterpret_cast<WORD*>(malloc_non_zero_4954F0(pAbe->field_20_animation.field_90_pal_depth * sizeof(WORD)));
    Pal_Copy_483560(
        pAbe->field_20_animation.field_8C_pal_vram_xy,
        pAbe->field_20_animation.field_90_pal_depth,
        field_30_pPalAlloc,
        &field_34);

    field_4A_flags = 0;

    if (pAbe->field_20_animation.field_4_flags.Get(AnimFlags::eBit15_bSemiTrans))
    {
        field_4A_flags |= 0x1;
    }
    if (pAbe->field_20_animation.field_4_flags.Get(AnimFlags::eBit16_bBlending))
    {
        field_4A_flags |= 0x2;
    }

    field_4A_flags &= ~4u;
    field_48 = pAbe->field_20_animation.field_B_render_mode;
    field_20_state_or_op = 0;

    return this;
}

EXPORT void InvisibleEffect::dtor_45F410()
{
    SetVTable(this, 0x545A60);

    if (field_24_pAlloc)
    {
        Mem_Free_495560(field_24_pAlloc);
    }

    if (field_30_pPalAlloc)
    {
        Mem_Free_495560(field_30_pPalAlloc);
    }

    BaseGameObject_dtor_4DBEC0();
}

EXPORT void InvisibleEffect::sub_45FA00()
{
    field_4A_flags |= 4u;
    field_1C_update_delay = 1;
    field_20_state_or_op = 1;
}

EXPORT void InvisibleEffect::sub_45FA30()
{
    field_20_state_or_op = 4;
}

EXPORT void InvisibleEffect::sub_45FA50()
{
    field_1C_update_delay = 1;
    field_20_state_or_op = 5;
}

EXPORT void InvisibleEffect::sub_45F9E0()
{
    field_1C_update_delay = 1;
    field_20_state_or_op = 1;
}

EXPORT void InvisibleEffect::vUpdate_45F4A0()
{
    NOT_IMPLEMENTED();
}

EXPORT BaseGameObject* InvisibleEffect::vdtor_45F3E0(signed int flags)
{
    dtor_45F410();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

BaseGameObject* InvisibleEffect::VDestructor(signed int flags)
{
    return vdtor_45F3E0(flags);
}

void InvisibleEffect::VUpdate()
{
    vUpdate_45F4A0();
}

void InvisibleEffect::VScreenChanged()
{
    // Null @ 45F9C0
}
