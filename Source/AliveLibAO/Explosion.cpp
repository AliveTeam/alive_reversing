#include "stdafx_ao.h"
#include "Function.hpp"
#include "Explosion.hpp"
#include "ScreenShake.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"
#include "Events.hpp"
#include "ParticleBurst.hpp"
#include "Flash.hpp"
#include "Particle.hpp"
#include "Map.hpp"

START_NS_AO

void Explosion::VUpdate()
{
    VUpdate_458D00();
}

void Explosion::VScreenChanged()
{
    VScreenChanged_459470();
}

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

    SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_21, 1, 1);

    return this;
}


void Explosion::VScreenChanged_459470()
{
    if (gMap_507BA8.field_28_cd_or_overlay_num != gMap_507BA8.GetOverlayId_4440B0())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Explosion::VUpdate_458D00()
{
    Event_Broadcast_417220(kEvent_2, this);
    Event_Broadcast_417220(kEvent_14, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    PSX_RECT rect = {};

    switch (field_10_anim.field_92_current_frame)
    {
    case 2:
        rect.x = FP_GetExponent(FP_FromInteger(-20) * field_E4_scale);
        rect.w = FP_GetExponent(FP_FromInteger(20) * field_E4_scale);
        rect.y = FP_GetExponent(FP_FromInteger(-20) * field_E4_scale);
        rect.h = FP_GetExponent(FP_FromInteger(10) * field_E4_scale);
        DealBlastDamage_459160(&rect);
        break;

    case 3:
    {
        auto pParticleBurst = ao_new<ParticleBurst>();
        if (pParticleBurst)
        {
            pParticleBurst->ctor_40D0F0(field_A8_xpos, field_AC_ypos, 20, field_BC_sprite_scale, BurstType::eType_3);
        }

        auto pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(39, 255u, 255u, 255u, 1, 3u, 1);
        }
        break;
    }

    case 4:
    {
        auto pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(39, 255u, 255u, 255u, 1, 1u, 1);
        }

        rect.x = FP_GetExponent(FP_FromInteger(-38) * field_E4_scale);
        rect.w = FP_GetExponent(FP_FromInteger(38) * field_E4_scale);
        rect.y = FP_GetExponent(FP_FromInteger(-38) * field_E4_scale);
        rect.h = FP_GetExponent(FP_FromInteger(19) * field_E4_scale);
        DealBlastDamage_459160(&rect);
        break;
    }

    case 6:
        rect.x = FP_GetExponent(FP_FromInteger(-60) * field_E4_scale);
        rect.w = FP_GetExponent(FP_FromInteger(60) * field_E4_scale);
        rect.y = FP_GetExponent(FP_FromInteger(-60) * field_E4_scale);
        rect.h = FP_GetExponent(FP_FromInteger(30) * field_E4_scale);
        DealBlastDamage_459160(&rect);
        break;

    case 8:
    {
        auto pParticleBurst = ao_new<ParticleBurst>();
        if (pParticleBurst)
        {
            pParticleBurst->ctor_40D0F0(field_A8_xpos, field_AC_ypos, 20, field_BC_sprite_scale, BurstType::eType_3);
        }

        auto pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(39, 255u, 255u, 255u, 1, 3u, 1);
        }
        break;
    }

    default:
        break;
    }

    if (field_10_anim.field_92_current_frame > 9)
    {
        field_BC_sprite_scale -= FP_FromDouble(0.2);
    }

    if (field_10_anim.field_92_current_frame == 1)
    {
        const auto GetLoadedResource = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kUnknownResID_301, 1, 0);
        if (GetLoadedResource)
        {
            auto pParticle = ao_new<Particle>();
            if (pParticle)
            {
                pParticle->ctor_478880(field_A8_xpos, field_AC_ypos, 27376, 200, 91, GetLoadedResource);
            }
            else
            {
                pParticle = nullptr;
            }

            // TODO: crashes if the "else" path gets hit
            if (pParticle->field_6_flags.Get(BaseGameObject::eListAddFailed_Bit1))
            {
                pParticle->field_6_flags.Set(Options::eDead_Bit3);
            }

            pParticle->field_CC_bApplyShadows &= ~1u;
            pParticle->field_10_anim.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            pParticle->field_10_anim.field_B_render_mode = 1;
            pParticle->field_BC_sprite_scale = field_BC_sprite_scale * FP_FromDouble(0.25);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
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
