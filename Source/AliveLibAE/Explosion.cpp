#include "stdafx.h"
#include "Explosion.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "Map.hpp"
#include "Particle.hpp"
#include "Flash.hpp"
#include "ScreenShake.hpp"
#include "Gibs.hpp"
#include "Events.hpp"
#include "ParticleBurst.hpp"
#include "Sound/Midi.hpp"
#include "stdlib.hpp"
#include "Slig.hpp"
#include "ExplosionSet.hpp"

Explosion* Explosion::ctor_4A1200(FP xpos, FP ypos, FP scale, s16 bSmall)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x546CB8);
    SetType(AETypes::eExplosion_109);

    field_F4_bSmall = bSmall;
    if (field_F4_bSmall)
    {
        const AnimRecord& rec = AnimRec(AnimId::Explosion_Small);
        u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    }
    else
    {
        const AnimRecord& rec = AnimRec(AnimId::Explosion);
        u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    }

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
    field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
    field_F8_scale = scale;
    field_D6_scale = scale == FP_FromInteger(1);
    field_CC_sprite_scale = scale * FP_FromInteger(2);

    if (field_F4_bSmall)
    {
        field_FC_explosion_size = scale * FP_FromDouble(0.5);
    }
    else
    {
        field_FC_explosion_size = scale;
    }

    field_DC_bApplyShadows &= ~1u;
    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    auto pScreenShake = ae_new<ScreenShake>();
    if (pScreenShake)
    {
        pScreenShake->ctor_4ACF70(bEnabled_5C1BB6 ? 0 : 1, field_F4_bSmall);
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(FP_FromInteger(-10) * field_FC_explosion_size);
    rect.y = FP_GetExponent(FP_FromInteger(-10) * field_FC_explosion_size);
    rect.w = FP_GetExponent(FP_FromInteger(10) * field_FC_explosion_size);
    rect.h = FP_GetExponent(FP_FromInteger(10) * field_FC_explosion_size);

    DealBlastDamage_4A1BD0(&rect);

    SND_SEQ_PlaySeq_4CA960(SeqId::Explosion1_14, 1, 1);

    return this;
}

void Explosion::dtor_4A14F0()
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BaseGameObject* Explosion::VDestructor(s32 flags)
{
    return vdtor_4A14C0(flags);
}

Explosion* Explosion::vdtor_4A14C0(s32 flags)
{
    dtor_4A14F0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Explosion::VUpdate()
{
    vUpdate_4A1510();
}

void Explosion::vUpdate_4A1510()
{
    Event_Broadcast_422BC0(kEventShooting, this);
    Event_Broadcast_422BC0(kEventLoudNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

    PSX_RECT rect = {};

    switch (field_20_animation.field_92_current_frame)
    {
        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-20) * field_FC_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(20) * field_FC_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * field_FC_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(10) * field_FC_explosion_size);
            DealBlastDamage_4A1BD0(&rect);
            break;

        case 4:
        {
            auto pFlash = ae_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u, 1, TPageAbr::eBlend_1, 1);
            }
            rect.x = FP_GetExponent(FP_FromInteger(-38) * field_FC_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(38) * field_FC_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-38) * field_FC_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(19) * field_FC_explosion_size);
            DealBlastDamage_4A1BD0(&rect);
        }
        break;

        case 3:
        case 6:
            rect.x = FP_GetExponent(FP_FromInteger(-60) * field_FC_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(60) * field_FC_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-60) * field_FC_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(30) * field_FC_explosion_size);
            DealBlastDamage_4A1BD0(&rect);
            break;

        case 8:
        {
            auto pParticleBurst = ae_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(field_B8_xpos, field_BC_ypos, field_F4_bSmall ? 6 : 20, field_F8_scale, BurstType::eBigRedSparks_3, field_F4_bSmall ? 11 : 13);
            }

            auto pFlash = ae_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u, 1, TPageAbr::eBlend_3, 1);
            }
            break;
        }

        default:
            break;
    }

    if (field_20_animation.field_92_current_frame > 9)
    {
        if (field_F4_bSmall)
        {
            field_CC_sprite_scale -= FP_FromDouble(0.066);
        }
        else
        {
            field_CC_sprite_scale -= FP_FromDouble(0.2);
        }
    }

    if (field_20_animation.field_92_current_frame == 1)
    {
        u8** ppRes = field_F4_bSmall ? Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID) : Add_Resource_4DC130(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
        if (ppRes)
        {
            auto pParticle = ae_new<Particle>();
            if (pParticle)
            {
                const AnimRecord& rec = field_F4_bSmall ? AnimRec(AnimId::Explosion_Small) : AnimRec(AnimId::Explosion);
                pParticle->ctor_4CC4C0(field_B8_xpos, field_BC_ypos, rec.mFrameTableOffset,
                                       202, // Same size for both explosions for some reason
                                       91,  // ^^^
                                       ppRes);

                if (pParticle->field_6_flags.Get(BaseGameObject::eListAddFailed_Bit1))
                {
                    pParticle->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }

                pParticle->field_DC_bApplyShadows &= ~1u;
                pParticle->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;

                if (field_20_animation.field_92_current_frame == 3)
                {
                    pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                    pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.5);
                }
                else
                {
                    pParticle->field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                    pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.25);
                }
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Explosion::VScreenChanged()
{
    vScreenChanged_4A1EE0();
}

void Explosion::vScreenChanged_4A1EE0()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Explosion::DealBlastDamage_4A1BD0(PSX_RECT* pRect)
{
    if (!gBaseAliveGameObjects_5C1B7C)
    {
        return;
    }

    PSX_RECT expandedRect = {};
    expandedRect.x = std::min(pRect->x, pRect->w);
    expandedRect.w = std::max(pRect->x, pRect->w);

    expandedRect.y = std::min(pRect->y, pRect->h);
    expandedRect.h = std::max(pRect->y, pRect->h);

    expandedRect.x += FP_GetExponent(field_B8_xpos);
    expandedRect.w += FP_GetExponent(field_B8_xpos);

    expandedRect.y += FP_GetExponent(field_BC_ypos);
    expandedRect.h += FP_GetExponent(field_BC_ypos);

    for (s32 idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            PSX_RECT boundRect = {};
            pObj->vGetBoundingRect_424FD0(&boundRect, 1);

            if (PSX_Rects_overlap_no_adjustment(&boundRect, &expandedRect) && field_D6_scale == pObj->field_D6_scale)
            {
                pObj->VTakeDamage_408730(this);
            }
        }
    }

    auto pTlv = static_cast<Path_Slig*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        expandedRect.x,
        expandedRect.y,
        expandedRect.w,
        expandedRect.h,
        TlvTypes::Slig_15));

    if (pTlv)
    {
        if (!pTlv->field_0_flags.Get(TLV_Flags::eBit2_Destroyed) && pTlv->field_12_start_state == Path_Slig::StartState::Sleeping_2)
        {
            pTlv->field_0_flags.Set(TLV_Flags::eBit2_Destroyed);

            const CameraPos dir = gMap_5C3030.GetDirection_4811A0(
                gMap_5C3030.field_0_current_level,
                gMap_5C3030.field_2_current_path,
                FP_FromInteger(pTlv->field_8_top_left.field_0_x),
                FP_FromInteger(pTlv->field_8_top_left.field_2_y));

            if (dir == CameraPos::eCamLeft_3)
            {
                auto pGibs = ae_new<Gibs>();
                if (pGibs)
                {
                    pGibs->ctor_40FB40(GibType::Slig_1, field_B8_xpos + FP_FromInteger(656), field_BC_ypos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 0);
                }
            }
            else if (dir == CameraPos::eCamRight_4)
            {
                auto pGibs = ae_new<Gibs>();
                if (pGibs)
                {
                    pGibs->ctor_40FB40(GibType::Slig_1, field_B8_xpos - FP_FromInteger(656), field_BC_ypos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 0);
                }
            }

            Stop_slig_sounds_4CBA70(dir, 0);
        }
    }
}
