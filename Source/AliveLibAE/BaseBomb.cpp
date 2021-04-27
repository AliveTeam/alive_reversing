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

void BaseBomb::VUpdate()
{
    vUpdate_424180();
}


BaseGameObject* BaseBomb::VDestructor(s32 flags)
{
    return vdtor_424130(flags);
}

BaseBomb* BaseBomb::ctor_423E70(FP x, FP y, s32 /*unused*/, FP scale)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x544C54);
    field_4_typeId = AETypes::eBaseBomb_46;

    const AnimRecord& rec = AnimRec(AnimId::Explosion_Mine);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, BaseGameObject::Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId), 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame); // Double Check
    field_20_animation.field_4_flags.Set(AnimFlags::eBit24);

    field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;

    field_20_animation.field_A_b = 128;
    field_20_animation.field_9_g = 128;
    field_20_animation.field_8_r = 128;

    field_f4_scale = scale;

    if (scale == FP_FromDouble(1.0))
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_36;
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_17;
    }

    field_DC_bApplyShadows &= ~1;
    field_CC_sprite_scale = scale * FP_FromDouble(2.75);
    field_B8_xpos = x;
    field_BC_ypos = y;

    auto pScreenShake = ae_new<ScreenShake>();
    if (pScreenShake)
    {
        pScreenShake->ctor_4ACF70(1, 0);
    }

    auto pParticleBurst = ae_new<ParticleBurst>();
    if (pParticleBurst)
    {
        pParticleBurst->ctor_41CF50(
            field_B8_xpos,
            field_BC_ypos,
            35,
            field_f4_scale,
            BurstType::eFallingRocks_0,
            13);
    }

    PSX_RECT damageRect = {
        FP_GetExponent(FP_FromInteger(-10) * field_f4_scale),
        FP_GetExponent(FP_FromInteger(-10) * field_f4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_f4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_f4_scale)};
    DealDamageRect_4247A0(&damageRect);

    // alternate between Explosion1 and Explosion2 on each call
    static s32 staticIdFlip = 0;
    const s16 explosionSeqId = static_cast<s16>(staticIdFlip ? SeqId::Explosion1_14 : SeqId::Explosion2_15);
    SND_SEQ_PlaySeq_4CA960(explosionSeqId, 1, 1);
    staticIdFlip = !staticIdFlip;

    return this;
}

void BaseBomb::vUpdate_424180()
{
    PSX_RECT rect = {};

    Event_Broadcast_422BC0(kEventShooting, this);
    Event_Broadcast_422BC0(kEventLoudNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    switch (this->field_20_animation.field_92_current_frame)
    {
        case 0:
            rect.x = FP_GetExponent(FP_FromInteger(-30) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(30) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(20) * field_f4_scale);
            DealDamageRect_4247A0(&rect);
            break;

        case 1:
            rect.x = FP_GetExponent(FP_FromInteger(-50) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(50) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-30) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(30) * field_f4_scale);
            DealDamageRect_4247A0(&rect);
            break;

        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-80) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(80) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-40) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(40) * field_f4_scale);
            DealDamageRect_4247A0(&rect);
            break;

        case 3:
        {
            ParticleBurst* pParticleBurst = ae_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    field_B8_xpos,
                    field_BC_ypos,
                    0x14u,
                    field_CC_sprite_scale,
                    BurstType::eBigRedSparks_3,
                    13);
            }

            Flash* pFlash = ae_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(Layer::eLayer_39, 255, 255, 255, 1, TPageAbr::eBlend_3, 1);
            }

            rect.x = FP_GetExponent(FP_FromInteger(-113) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(113) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-50) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(50) * field_f4_scale);
            DealDamageRect_4247A0(&rect);
            break;
        }

        case 4:
        {
            Flash* pFlash = ae_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(Layer::eLayer_39, 255, 255, 255, 1, TPageAbr::eBlend_1, 1);
            }
            break;
        }

        case 7:
        {
            ParticleBurst* pParticleBurst = ae_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(
                    field_B8_xpos,
                    field_BC_ypos,
                    20u,
                    field_CC_sprite_scale,
                    BurstType::eBigRedSparks_3,
                    13);
            }

            Flash* pFlash = ae_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(Layer::eLayer_39, 255, 255, 255, 1, TPageAbr::eBlend_3, 1);
            }
            break;
        }

        default:
            break;
    }

    if (field_20_animation.field_92_current_frame == 3)
    {
        u8** ppRes = Add_Resource_4DC130(
            ResourceManager::Resource_Animation,
            ResourceID::kBgexpldResID);
        if (ppRes)
        {
            Particle* pParticle = ae_new<Particle>();
            if (pParticle)
            {
                pParticle->ctor_4CC4C0(
                    field_B8_xpos,
                    field_BC_ypos,
                    51588,
                    214,
                    49,
                    ppRes);

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
        field_6_flags.Set(Options::eDead_Bit3);
    }
}


BaseBomb* BaseBomb::vdtor_424130(s32 flags)
{
    dtor_424160();

    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void BaseBomb::dtor_424160()
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}
