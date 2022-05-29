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

ALIVE_VAR(1, 0x4FFA4C, s16, word_4FFA4C, 0);

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
            ao_new<ParticleBurst>(
                field_A8_xpos,
                field_AC_ypos,
                20,
                field_BC_sprite_scale,
                BurstType::eBigRedSparks_3);

            ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);

            rect.x = FP_GetExponent(FP_FromInteger(-113) * field_E4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(113) * field_E4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-50) * field_E4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(50) * field_E4_scale);
            DealDamageRect_417A50(&rect);
            break;
        }

        case 4:
        {
            ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u, 1, TPageAbr::eBlend_1, 1);
            break;
        }

        case 7:
        {
            ao_new<ParticleBurst>(
                field_A8_xpos,
                field_AC_ypos,
                20,
                field_BC_sprite_scale,
                BurstType::eBigRedSparks_3);

            ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        default:
            break;
    }

    if (field_10_anim.field_92_current_frame == 3)
    {
        const AnimRecord& rec = AO::AnimRec(AnimId::Explosion_Mine);
        u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
        if (ppRes)
        {
            Particle* pParticle = ao_new<Particle>();
            if (pParticle)
            {
                pParticle->ctor_478880(
                    field_A8_xpos,
                    field_AC_ypos,
                    rec.mFrameTableOffset,
                    rec.mMaxW,
                    rec.mMaxH,
                    ppRes);
            }
            else
            {
                pParticle = nullptr;
            }
            pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            pParticle->field_CC_bApplyShadows &= ~1u;
            pParticle->field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;
            pParticle->field_BC_sprite_scale = field_BC_sprite_scale * FP_FromDouble(0.7);
        }
    }

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        mFlags.Set(BaseGameObject::eDead);
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
        s16 min_w_x = pRect->w;
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

        s16 min_h_y = pRect->h;
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

        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            const s16 obj_xpos = FP_GetExponent(pObj->field_A8_xpos);
            if (obj_xpos >= left && obj_xpos <= right)
            {
                const s16 obj_ypos = FP_GetExponent(pObj->field_AC_ypos);
                if (obj_ypos >= top && obj_ypos <= bottom && field_BC_sprite_scale == (pObj->field_BC_sprite_scale * FP_FromDouble(2.75)))
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
    }
}

BaseBomb::BaseBomb(FP xpos, FP ypos, s32 /*unused*/, FP scale)
{
    field_4_typeId = Types::eBaseBomb_30;

    const AnimRecord& rec = AO::AnimRec(AnimId::Explosion_Mine);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);

    field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;

    field_10_anim.field_A_b = 128;
    field_10_anim.field_9_g = 128;
    field_10_anim.field_8_r = 128;

    field_E4_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = Layer::eLayer_Foreground_36;
    }
    else
    {
        field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
    }

    field_CC_bApplyShadows &= ~1u;
    field_BC_sprite_scale = scale * FP_FromDouble(2.75);

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    ao_new<ScreenShake>(1);

    ao_new<ParticleBurst>(
        field_A8_xpos,
        field_AC_ypos,
        35,
        field_E4_scale,
        BurstType::eFallingRocks_0);

    PSX_RECT damageRect = {
        FP_GetExponent(FP_FromInteger(-10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(-10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_E4_scale)};
    DealDamageRect_417A50(&damageRect);

    word_4FFA4C = 0;
    SND_SEQ_PlaySeq_4775A0(SeqId::eExplosion1_21, 1, 1);
}

} // namespace AO
