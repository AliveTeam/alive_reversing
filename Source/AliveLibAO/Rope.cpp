#include "stdafx_ao.h"
#include "Function.hpp"
#include "Rope.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

START_NS_AO

Rope* Rope::Vdtor_458B60(signed int flags)
{
    dtor_458720();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

EXPORT void CC ClipPoly_Vertically_4584B0(Poly_FT4* /*pPoly*/, int /*minY*/, int /*maxY*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* Rope::dtor_458720()
{
    SetVTable(this, 0x4BC1E0);
    ao_delete_free_447540(field_E8_pRopeRes);
    return dtor_417D10();
}

BaseGameObject* Rope::VDestructor(signed int flags)
{
    return Vdtor_458B60(flags);
}

Rope* Rope::ctor_458520(unsigned __int16 left, __int16 top, unsigned __int16 bottom, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BC1E0);
    field_4_typeId = Types::eRope_73;

    field_C8_yOffset = 0;

    if (scale == FP_FromInteger(1))
    {
        field_E6_rope_length = 15;
    }
    else
    {
        field_E6_rope_length = 7;
    }

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1000, 1, 0);
    switch (gMap_507BA8.field_0_current_level)
    {
    case LevelIds::eRuptureFarms_1:
    case LevelIds::eDesert_8:
    case LevelIds::eDesertTemple_9:
    case LevelIds::eBoardRoom_12:
    case LevelIds::eRuptureFarmsReturn_13:
    case LevelIds::eDesertEscape:
        Animation_Init_417FD0(636, 4, 16, ppRes, 1);
        break;

    default:
        Animation_Init_417FD0(680, 5, 16, ppRes, 1);
        break;
    }
    field_10_anim.field_14_scale = scale;
    field_BC_sprite_scale = scale;
    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 24;
        field_C6_scale = 1;
    }
    else
    {
        field_10_anim.field_C_layer = 5;
        field_C6_scale = 0;
    }

    field_10_anim.field_8_r = 128;
    field_10_anim.field_9_g = 128;
    field_10_anim.field_A_b = 128;

    field_F2_bottom = bottom;
    field_E4_rope_segment_count = 240 / field_E6_rope_length + 1;

    field_A8_xpos = FP_FromInteger(left);
    field_AC_ypos = FP_FromInteger(bottom);
    field_EC_left = left;
    field_F0_right = left;
    field_EE_top = top;

    field_E8_pRopeRes = reinterpret_cast<AnimationUnknown*>(ao_new_malloc_447520(sizeof(AnimationUnknown) * field_E4_rope_segment_count));
    if (field_E8_pRopeRes)
    {
        for (int i = 0; i < field_E4_rope_segment_count; i++)
        {
            // Placement new the items
            AnimationUnknown* pSegment = &field_E8_pRopeRes[i];
            SetVTable(pSegment, 0x4BA470);
            pSegment->field_4_flags.Set(AnimFlags::eBit3_Render);
            pSegment->field_68_anim_ptr = &field_10_anim;
            pSegment->field_C_layer = field_10_anim.field_C_layer;
            pSegment->field_6C_scale = scale;
            pSegment->field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
            pSegment->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        }
    }
    return this;
}

void Rope::VUpdate()
{
    // Empty
}

END_NS_AO

