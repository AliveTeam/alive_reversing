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

Explosion::Explosion(FP xpos, FP ypos, FP scale, bool bSmall)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eExplosion);

    field_F4_bSmall = bSmall;
    if (field_F4_bSmall)
    {
        const AnimRecord& rec = AnimRec(AnimId::Explosion_Small);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(AnimId::Explosion_Small, ppRes);
    }
    else
    {
        const AnimRecord& rec = AnimRec(AnimId::Explosion);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(AnimId::Explosion, ppRes);
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
    field_F8_scale = scale;
    mBaseAnimatedWithPhysicsGameObject_Scale = scale == FP_FromInteger(1) ? Scale::Fg : Scale::Bg;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale * FP_FromInteger(2);

    if (field_F4_bSmall)
    {
        field_FC_explosion_size = scale * FP_FromDouble(0.5);
    }
    else
    {
        field_FC_explosion_size = scale;
    }

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

    relive_new ScreenShake(bEnabled_5C1BB6 ? 0 : 1, field_F4_bSmall);

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(FP_FromInteger(-10) * field_FC_explosion_size);
    rect.y = FP_GetExponent(FP_FromInteger(-10) * field_FC_explosion_size);
    rect.w = FP_GetExponent(FP_FromInteger(10) * field_FC_explosion_size);
    rect.h = FP_GetExponent(FP_FromInteger(10) * field_FC_explosion_size);

    DealBlastDamage(&rect);

    SND_SEQ_PlaySeq(SeqId::Explosion1_14, 1, 1);
}

void Explosion::VUpdate()
{
    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    PSX_RECT rect = {};

    switch (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
    {
        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-20) * field_FC_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(20) * field_FC_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * field_FC_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(10) * field_FC_explosion_size);
            DealBlastDamage(&rect);
            break;

        case 4:
        {
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, 1, TPageAbr::eBlend_1, 1);
            rect.x = FP_GetExponent(FP_FromInteger(-38) * field_FC_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(38) * field_FC_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-38) * field_FC_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(19) * field_FC_explosion_size);
            DealBlastDamage(&rect);
        }
        break;

        case 3:
        case 6:
            rect.x = FP_GetExponent(FP_FromInteger(-60) * field_FC_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(60) * field_FC_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-60) * field_FC_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(30) * field_FC_explosion_size);
            DealBlastDamage(&rect);
            break;

        case 8:
        {
            relive_new ParticleBurst(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, field_F4_bSmall ? 6 : 20, field_F8_scale, BurstType::eBigRedSparks_3, field_F4_bSmall ? 11 : 13);
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, 1, TPageAbr::eBlend_3, 1);
            break;
        }

        default:
            break;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame > 9)
    {
        if (field_F4_bSmall)
        {
            mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.066);
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.2);
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 1)
    {
        u8** ppRes = field_F4_bSmall ? Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID) : Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kExplo2ResID);
        if (ppRes)
        {
            const AnimId explosionId = field_F4_bSmall ? AnimId::Explosion_Small : AnimId::Explosion;
            auto pParticle = relive_new Particle(
                mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos,
                explosionId,
                ppRes,
                true);

            if (pParticle)
            {
                if (pParticle->mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
                {
                    pParticle->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }

                pParticle->mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;

                if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3)
                {
                    pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                    pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.5);
                }
                else
                {
                    pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                    pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.25);
                }
            }
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Explosion::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Explosion::DealBlastDamage(PSX_RECT* pRect)
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

    expandedRect.x += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    expandedRect.w += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);

    expandedRect.y += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);
    expandedRect.h += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);

    for (s32 idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            const PSX_RECT boundRect = pObj->VGetBoundingRect();

            if (PSX_Rects_overlap_no_adjustment(&boundRect, &expandedRect) && mBaseAnimatedWithPhysicsGameObject_Scale == pObj->mBaseAnimatedWithPhysicsGameObject_Scale)
            {
                pObj->VTakeDamage(this);
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
        if (!pTlv->mTlvFlags.Get(TlvFlags::eBit2_Destroyed) && pTlv->field_12_start_state == Path_Slig::StartState::Sleeping_2)
        {
            pTlv->mTlvFlags.Set(TlvFlags::eBit2_Destroyed);

            const CameraPos dir = gMap.GetDirection(
                gMap.mCurrentLevel,
                gMap.mCurrentPath,
                FP_FromInteger(pTlv->mTopLeft.x),
                FP_FromInteger(pTlv->mTopLeft.y));

            if (dir == CameraPos::eCamLeft_3)
            {
                relive_new Gibs(GibType::Slig_1, mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(656), mBaseAnimatedWithPhysicsGameObject_YPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 0);
            }
            else if (dir == CameraPos::eCamRight_4)
            {
                relive_new Gibs(GibType::Slig_1, mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(656), mBaseAnimatedWithPhysicsGameObject_YPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 0);
            }

            Stop_slig_sounds(dir, 0);
        }
    }
}
