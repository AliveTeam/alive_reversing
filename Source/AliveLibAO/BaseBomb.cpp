#include "stdafx_ao.h"
#include "Function.hpp"
#include "BaseBomb.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ParticleBurst.hpp"
#include "ScreenShake.hpp"
#include "Midi.hpp"
#include "Events.hpp"
#include "Flash.hpp"
#include "Particle.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

ALIVE_VAR(1, 0x4FFA4C, short, word_4FFA4C, 0);

void BaseBomb::VUpdate_417580()
{
    PSX_RECT rect = {};

    Event_Broadcast_417220(kEvent_2, this);
    Event_Broadcast_417220(kEvent_14, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);

    switch (field_10_anim.field_92_current_frame)
    {
    case 0:
        rect.x = FP_GetExponent(FP_FromInteger(-30) * field_E4_scale);
        rect.w = FP_GetExponent(FP_FromInteger(30) * field_E4_scale);
        rect.y = FP_GetExponent(FP_FromInteger(-20) * field_E4_scale);
        rect.h = FP_GetExponent(FP_FromInteger(20) * field_E4_scale);
        DealDamageRect_417A50(&rect);
        break;

    case 1:
        rect.x = FP_GetExponent(FP_FromInteger(-50) * field_E4_scale);
        rect.w = FP_GetExponent(FP_FromInteger(50) * field_E4_scale);
        rect.y = FP_GetExponent(FP_FromInteger(-30) * field_E4_scale);
        rect.h = FP_GetExponent(FP_FromInteger(30) * field_E4_scale);
        DealDamageRect_417A50(&rect);
        break;

    case 2:
        rect.x = FP_GetExponent(FP_FromInteger(-80) * field_E4_scale);
        rect.w = FP_GetExponent(FP_FromInteger(80) * field_E4_scale);
        rect.y = FP_GetExponent(FP_FromInteger(-40) * field_E4_scale);
        rect.h = FP_GetExponent(FP_FromInteger(40) * field_E4_scale);
        DealDamageRect_417A50(&rect);
        break;

    case 3:
    {
        ParticleBurst* pParticleBurst = ao_new<ParticleBurst>();
        if (pParticleBurst)
        {
            pParticleBurst->ctor_40D0F0(
                field_A8_xpos,
                field_AC_ypos,
                20,
                field_BC_sprite_scale,
                BurstType::eType_3);
        }


        Flash* pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(39, 255u, 255u, 255u, 1, 3u, 1);
        }

        rect.x = FP_GetExponent(FP_FromInteger(-113) * field_E4_scale);
        rect.w = FP_GetExponent(FP_FromInteger(113) * field_E4_scale);
        rect.y = FP_GetExponent(FP_FromInteger(-50) * field_E4_scale);
        rect.h = FP_GetExponent(FP_FromInteger(50) * field_E4_scale);
        DealDamageRect_417A50(&rect);
        break;
    }

    case 4:
    {
        Flash* pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(39, 255u, 255u, 255u, 1, 1u, 1);
        }
    }

    case 7:
    {
        ParticleBurst* pParticleBurst = ao_new<ParticleBurst>();
        if (pParticleBurst)
        {
            pParticleBurst->ctor_40D0F0(
                field_A8_xpos,
                field_AC_ypos,
                20,
                field_BC_sprite_scale,
                BurstType::eType_3);
        }

        Flash* pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(39, 255u, 255u, 255u, 1, 3u, 1);
        }
        break;
    }

    default:
        break;
    }

    if (field_10_anim.field_92_current_frame == 3)
    {
        BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBgexpldResID, 1, 0);
        if (ppRes)
        {
            Particle* pParticle = ao_new<Particle>();
            if (pParticle)
            {
                pParticle->ctor_478880(
                    field_A8_xpos,
                    field_AC_ypos,
                    51600,
                    214,
                    49,
                    ppRes);
            }
            else
            {
                pParticle = nullptr;
            }
            pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            pParticle->field_CC_bApplyShadows &= ~1u;
            pParticle->field_10_anim.field_B_render_mode = 1;
            pParticle->field_BC_sprite_scale = field_BC_sprite_scale * FP_FromDouble(0.7);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void BaseBomb::VUpdate()
{
    VUpdate_417580();
}

void BaseBomb::DealDamageRect_417A50(const PSX_RECT* pRect)
{
    if (gBaseAliveGameObjects_4FC8A0)
    {
        short min_w_x = pRect->w;
        if (pRect->x <= pRect->w)
        {
            min_w_x = pRect->x;
        }

        auto min_x_w = pRect->w;
        if (pRect->w <= pRect->x)
        {
            min_x_w = pRect->x;
        }

        auto min_y_h = pRect->h;
        if (pRect->y <= pRect->h)
        {
            min_y_h = pRect->y;
        }

        short min_h_y = pRect->h;
        if (pRect->h <= pRect->y)
        {
            min_h_y = pRect->y;
        }

        auto right = FP_GetExponent(field_A8_xpos) + min_x_w;
        auto left = FP_GetExponent(field_A8_xpos) + min_w_x;
        auto top = FP_GetExponent(field_AC_ypos) + min_y_h;
        auto bottom = FP_GetExponent(field_AC_ypos) + min_h_y;

        if ((abs(left) & 1023) < 256)
        {
            left -= 656;
        }

        if ((abs(right) & 1023) > 624)
        {
            right += 656;
        }

        if (top % 480 < 120)
        {
            top -= 240;
        }

        if (bottom % 480 > 360)
        {
            bottom += 240;
        }

        for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            const short obj_xpos = FP_GetExponent(pObj->field_A8_xpos);
            if (obj_xpos >= left && obj_xpos <= right)
            {
                const short obj_ypos = FP_GetExponent(pObj->field_AC_ypos);
                if (obj_ypos >= top &&
                    obj_ypos <= bottom &&
                    field_BC_sprite_scale == (pObj->field_BC_sprite_scale * FP_FromDouble(2.75)))
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
    }
}

BaseBomb* BaseBomb::ctor_4173A0(FP xpos, FP ypos, int /*unused*/, FP scale)
{
    ctor_417C10();
    SetVTable(this, 0x4BAA00);
    field_4_typeId = Types::eBaseBomb_30;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBgexpldResID, 1, 0);
    Animation_Init_417FD0(51600, 214, 49, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);

    field_10_anim.field_B_render_mode = 1;

    field_10_anim.field_A_b = 128;
    field_10_anim.field_9_g = 128;
    field_10_anim.field_8_r = 128;

    field_E4_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 36;
    }
    else
    {
        field_10_anim.field_C_layer = 17;
    }

    field_CC_bApplyShadows &= ~1u;
    field_BC_sprite_scale = scale * FP_FromDouble(2.75);

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    auto pScreenShake = ao_new<ScreenShake>();
    if (pScreenShake)
    {
        pScreenShake->ctor_4624D0(1);
    }

    auto pParticleBurst = ao_new<ParticleBurst>();
    if (pParticleBurst)
    {
        pParticleBurst->ctor_40D0F0(
            field_A8_xpos,
            field_AC_ypos,
            35,
            field_E4_scale,
            BurstType::eType_0);
    }

    PSX_RECT damageRect =
    {
        FP_GetExponent(FP_FromInteger(-10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(-10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_E4_scale)
    };
    DealDamageRect_417A50(&damageRect);

    word_4FFA4C = 0;
    SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_21, 1, 1);

    return this;
}

BaseGameObject* BaseBomb::VDestructor(signed int flags)
{
    dtor_417D10();

    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

}

