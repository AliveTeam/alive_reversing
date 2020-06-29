#include "stdafx_ao.h"
#include "Function.hpp"
#include "Explosion.hpp"
#include "ScreenShake.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"

START_NS_AO

Explosion* Explosion::ctor_458B80(FP xpos, FP ypos, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BC218);
    field_4_typeId = Types::eExplosion_74;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 1, 0);
    Animation_Init_417FD0(27376, 200, 91, ppRes, 1);
    
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
    field_10_anim.field_B_render_mode = 1;
    field_E4_scale = scale;

    field_BC_sprite_scale = scale * FP_FromInteger(2);
    field_CC_bApplyShadows &= ~1u;
    field_AC_ypos = ypos;
    field_A8_xpos = xpos;

    auto pScreenShake = ao_new<ScreenShake>();
    if (pScreenShake)
    {
        pScreenShake->ctor_4624D0(TRUE);
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(FP_FromInteger(-10) * scale);
    rect.y = FP_GetExponent(FP_FromInteger(10) * scale);
    rect.w = FP_GetExponent(FP_FromInteger(-10) * scale);
    rect.h = FP_GetExponent(FP_FromInteger(10) * scale);

    DealBlastDamage_459160(&rect);

    SND_SEQ_PlaySeq_4775A0(21u, 1, 1);

    return this;
}

void Explosion::DealBlastDamage_459160(PSX_RECT* /*pRect*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* Explosion::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

END_NS_AO
