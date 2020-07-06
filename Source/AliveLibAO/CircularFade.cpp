#include "stdafx_ao.h"
#include "Function.hpp"
#include "CircularFade.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"

START_NS_AO

void CircularFade::VRender_47A080(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

char CircularFade::Vsub_479FE0(unsigned __int8 direction, char destroyOnDone)
{
    field_E4_flags.Set(Flags::eBit1_FadeIn, direction);

    field_E4_flags.Clear(Flags::eBit2_Done);
    field_E4_flags.Clear(Flags::eBit4_NeverSet);

    field_E4_flags.Set(Flags::eBit3_DestroyOnDone, destroyOnDone);


    if (field_E4_flags.Get(Flags::eBit1_FadeIn))
    {
        field_1AA_speed = 12;
    }
    else
    {
        field_1AA_speed = -12;
    }
    return static_cast<char>(field_E4_flags.Raw().all);
}

void CircularFade::VUpdate_47A030()
{
    if ((!field_E4_flags.Get(Flags::eBit4_NeverSet) && !field_E4_flags.Get(Flags::eBit2_Done)))
    {
        field_1A8_fade_colour += field_1AA_speed;
        if (field_E4_flags.Get(Flags::eBit1_FadeIn))
        {
            if (field_1A8_fade_colour > 255)
            {
                field_1A8_fade_colour = 255;
            }
        }
        else if (field_1A8_fade_colour < 0)
        {
            field_1A8_fade_colour = 0;
        }
    }
}

void CircularFade::VRender(int** ppOt)
{
    VRender_47A080(ppOt);
}

void CircularFade::VUpdate()
{
    VUpdate_47A030();
}

BaseGameObject* CircularFade::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void CircularFade::VScreenChanged()
{
    // Empty
}

CircularFade* CircularFade::ctor_479E20(FP xpos, FP ypos, FP scale, __int16 direction, char destroyOnDone)
{
    ctor_417C10();
    SetVTable(this, 0x4BCE38);

    if (direction)
    {
        field_1A8_fade_colour = 0;
    }
    else
    {
        field_1A8_fade_colour = 255;
    }

    // NOTE: Inlined
    Vsub_479FE0(static_cast<char>(direction), destroyOnDone);

    const BYTE fade_rgb = static_cast<BYTE>((field_1A8_fade_colour * 60) / 100);
    field_C4_b = fade_rgb;
    field_C2_g = fade_rgb;
    field_C0_r = fade_rgb;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 316, 1, 0);
    Animation_Init_417FD0(2472, 57, 32, ppRes, 1);

    field_CC_bApplyShadows &= ~1u;

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
    field_BC_sprite_scale.fpValue = scale.fpValue * 2;
    field_10_anim.field_14_scale.fpValue = scale.fpValue * 2;

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    field_10_anim.field_B_render_mode = 2;
    field_10_anim.field_C_layer = 40;
    field_C0_r = field_1A8_fade_colour;
    field_C2_g = field_1A8_fade_colour;
    field_C4_b = field_1A8_fade_colour;

    Init_SetTPage_495FB0(field_188, 0, 0, PSX_getTPage_4965D0(2, 2, 0, 0));
    Init_SetTPage_495FB0(&field_188[1], 0, 0, PSX_getTPage_4965D0(2, 2, 0, 0));

    return this;
}

int CircularFade::Vsub_47A4C0()
{
    return field_E4_flags.Get(Flags::eBit2_Done);
}

CircularFade* CC Make_Circular_Fade_447640(FP xpos, FP ypos, FP scale, __int16 direction, char destroyOnDone)
{
    auto pCircularFade = ao_new<CircularFade>();
    if (pCircularFade)
    {
        pCircularFade->ctor_479E20(xpos, ypos, scale, direction, destroyOnDone);
    }
    return pCircularFade;
}

END_NS_AO
