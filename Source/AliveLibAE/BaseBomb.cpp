#include "stdafx.h"
#include "BaseBomb.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "Flash.hpp"
#include "Particle.hpp"

BaseBomb::BaseBomb(FP x, FP y, s32 /*unused*/, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eBaseBomb_46);

    const AnimRecord& rec = AnimRec(AnimId::Explosion_Mine);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, BaseGameObject::Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId), 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame); // Double Check
    field_20_animation.field_4_flags.Set(AnimFlags::eBit24);

    field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;

    field_20_animation.field_A_b = 128;
    field_20_animation.field_9_g = 128;
    field_20_animation.field_8_r = 128;

    field_f4_scale = scale;

    if (scale == FP_FromDouble(1.0))
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_Foreground_36;
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_Foreground_Half_17;
    }

    field_DC_bApplyShadows &= ~1;
    field_CC_sprite_scale = scale * FP_FromDouble(2.75);
    field_B8_xpos = x;
    field_BC_ypos = y;

    ae_new<ScreenShake>(true, false);

    ae_new<ParticleBurst>(
        field_B8_xpos,
        field_BC_ypos,
        35,
        field_f4_scale,
        BurstType::eFallingRocks_0,
        13);

    PSX_RECT damageRect = {
        FP_GetExponent(FP_FromInteger(-10) * field_f4_scale),
        FP_GetExponent(FP_FromInteger(-10) * field_f4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_f4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_f4_scale)};
    DealDamageRect(&damageRect);

    // alternate between Explosion1 and Explosion2 on each call
    static s32 staticIdFlip = 0;
    const s16 explosionSeqId = static_cast<s16>(staticIdFlip ? SeqId::Explosion1_14 : SeqId::Explosion2_15);
    SND_SEQ_PlaySeq(explosionSeqId, 1, 1);
    staticIdFlip = !staticIdFlip;
}

void BaseBomb::VUpdate()
{
    PSX_RECT rect = {};

    Event_Broadcast(kEventShooting, this);
    Event_Broadcast(kEventLoudNoise, this);
    Event_Broadcast(kEventSuspiciousNoise, this);

    switch (this->field_20_animation.field_92_current_frame)
    {
        case 0:
            rect.x = FP_GetExponent(FP_FromInteger(-30) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(30) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(20) * field_f4_scale);
            DealDamageRect(&rect);
            break;

        case 1:
            rect.x = FP_GetExponent(FP_FromInteger(-50) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(50) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-30) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(30) * field_f4_scale);
            DealDamageRect(&rect);
            break;

        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-80) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(80) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-40) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(40) * field_f4_scale);
            DealDamageRect(&rect);
            break;

        case 3:
        {
            ae_new<ParticleBurst>(
                field_B8_xpos,
                field_BC_ypos,
                20,
                field_CC_sprite_scale,
                BurstType::eBigRedSparks_3,
                13);

            ae_new<Flash>(Layer::eLayer_Above_FG1_39, static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(255), 1, TPageAbr::eBlend_3, 1);

            rect.x = FP_GetExponent(FP_FromInteger(-113) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(113) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-50) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(50) * field_f4_scale);
            DealDamageRect(&rect);
            break;
        }

        case 4:
        {
            ae_new<Flash>(Layer::eLayer_Above_FG1_39, static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(255), 1, TPageAbr::eBlend_1, 1);
            break;
        }

        case 7:
        {
            ae_new<ParticleBurst>(
                field_B8_xpos,
                field_BC_ypos,
                20u,
                field_CC_sprite_scale,
                BurstType::eBigRedSparks_3,
                13);

            ae_new<Flash>(Layer::eLayer_Above_FG1_39, static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(255), 1, TPageAbr::eBlend_3, 1);
            break;
        }

        default:
            break;
    }

    if (field_20_animation.field_92_current_frame == 3)
    {
        const AnimRecord& rec = AnimRec(AnimId::Explosion_Mine);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        if (ppRes)
        {
            Particle* pParticle = ae_new<Particle>(
                field_B8_xpos,
                field_BC_ypos,
                rec.mFrameTableOffset,
                rec.mMaxW,
                rec.mMaxH,
                ppRes);
            if (pParticle)
            {
                pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                pParticle->field_DC_bApplyShadows &= ~1;
                pParticle->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
                pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.7);
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted)) // Animation ended
    {
        // Time to die
        mFlags.Set(Options::eDead);
    }
}
