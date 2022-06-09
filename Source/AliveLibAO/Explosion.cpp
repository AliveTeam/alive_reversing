#include "stdafx_ao.h"
#include "Explosion.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Particle.hpp"
#include "Flash.hpp"
#include "ScreenShake.hpp"
#include "Gibs.hpp"
#include "Events.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "AmbientSound.hpp"
#include "Slig.hpp"
#include "ResourceManager.hpp"
#undef min
#undef max

namespace AO {

Explosion::Explosion(FP xpos, FP ypos, FP exposion_size)
{
    mBaseGameObjectTypeId = ReliveTypes::eExplosion;
    const AnimRecord rec = AO::AnimRec(AnimId::Explosion);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit18_IsLastFrame);
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
    field_E4_explosion_size = exposion_size;

    mBaseAnimatedWithPhysicsGameObject_SpriteScale = exposion_size * FP_FromInteger(2);
    mApplyShadows &= ~1u;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;

    relive_new ScreenShake(TRUE);

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(FP_FromInteger(-10) * exposion_size);
    rect.y = FP_GetExponent(FP_FromInteger(-10) * exposion_size);
    rect.w = FP_GetExponent(FP_FromInteger(10) * exposion_size);
    rect.h = FP_GetExponent(FP_FromInteger(10) * exposion_size);

    DealBlastDamage(&rect);

    SND_SEQ_PlaySeq_4775A0(SeqId::eExplosion1_21, 1, 1);
}

void Explosion::VUpdate()
{
    Event_Broadcast(kEventShooting, this);
    Event_Broadcast(kEventLoudNoise, this);
    Event_Broadcast(kEventSuspiciousNoise, this);

    PSX_RECT rect = {};

    switch (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame)
    {
        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-20) * field_E4_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(20) * field_E4_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * field_E4_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(10) * field_E4_explosion_size);
            DealBlastDamage(&rect);
            break;

        case 3:
        {
            relive_new ParticleBurst(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 20, mBaseAnimatedWithPhysicsGameObject_SpriteScale, BurstType::eBigRedSparks_3);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        case 4:
        {
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u, 1, TPageAbr::eBlend_1, 1);

            rect.x = FP_GetExponent(FP_FromInteger(-38) * field_E4_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(38) * field_E4_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-38) * field_E4_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(19) * field_E4_explosion_size);
            DealBlastDamage(&rect);
            break;
        }

        case 6:
            rect.x = FP_GetExponent(FP_FromInteger(-60) * field_E4_explosion_size);
            rect.w = FP_GetExponent(FP_FromInteger(60) * field_E4_explosion_size);
            rect.y = FP_GetExponent(FP_FromInteger(-60) * field_E4_explosion_size);
            rect.h = FP_GetExponent(FP_FromInteger(30) * field_E4_explosion_size);
            DealBlastDamage(&rect);
            break;

        case 8:
        {
            relive_new ParticleBurst(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 20, mBaseAnimatedWithPhysicsGameObject_SpriteScale, BurstType::eBigRedSparks_3);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        default:
            break;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame > 9)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale -= FP_FromDouble(0.2);
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 1)
    {
        const AnimRecord& rec = AO::AnimRec(AnimId::Explosion);
        const auto ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
        if (ppRes)
        {
            auto pParticle = relive_new Particle(mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
            if (pParticle)
            {
                if (pParticle->mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
                {
                    pParticle->mBaseGameObjectFlags.Set(Options::eDead);
                }

                pParticle->mApplyShadows &= ~1u;
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
                pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.25);
            }
            else
            {
                pParticle = nullptr;
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
    if (!gBaseAliveGameObjects_4FC8A0)
    {
        return;
    }

    PSX_RECT expandedRect = {};
    expandedRect.x = std::min(pRect->x, pRect->w);
    expandedRect.w = std::max(pRect->x, pRect->w);

    expandedRect.y = std::min(pRect->y, pRect->h);
    expandedRect.h = std::max(pRect->y, pRect->h);

    expandedRect.x += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    expandedRect.y += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);

    expandedRect.w += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos);
    expandedRect.h += FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);

    if ((expandedRect.x % 1024) < 256)
    {
        expandedRect.x -= 656;
    }
    if ((expandedRect.w % 1024) > 624)
    {
        expandedRect.w += 656;
    }
    if (expandedRect.y % 480 < 120)
    {
        expandedRect.y -= 240;
    }
    if (expandedRect.h % 480 > 360)
    {
        expandedRect.h += 240;
    }

    for (s32 idx = 0; idx < gBaseAliveGameObjects_4FC8A0->Size(); idx++)
    {
        auto pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseGameObjectFlags.Get(Options::eIsBaseAliveGameObject_Bit6))
        {
            PSX_RECT rect = {};
            pObj->VGetBoundingRect(&rect, 1);

            if (PSX_Rects_overlap_no_adjustment(&rect, &expandedRect) && field_E4_explosion_size == pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale)
            {
                pObj->VTakeDamage(this);
            }
        }
    }

    auto pTlv = static_cast<Path_Slig*>(gMap.TLV_Get_At_446260(
        expandedRect.x,
        expandedRect.y,
        expandedRect.w,
        expandedRect.h,
        TlvTypes::Slig_24));

    if (pTlv)
    {
        if (!pTlv->field_0_flags.Get(TLV_Flags::eBit2_Destroyed) && pTlv->field_1A_start_state == Path_Slig::StartState::Sleeping_2)
        {
            pTlv->field_0_flags.Set(TLV_Flags::eBit2_Destroyed);
            const CameraPos dir = gMap.GetDirection_444A40(
                gMap.mCurrentLevel,
                gMap.mCurrentPath,
                FP_FromInteger(pTlv->field_10_top_left.field_0_x),
                FP_FromInteger(pTlv->field_10_top_left.field_2_y));

            if (dir == CameraPos::eCamLeft_3)
            {
                relive_new Gibs(GibType::Slig_1, mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(656), mBaseAnimatedWithPhysicsGameObject_YPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1));
            }
            else if (dir == CameraPos::eCamRight_4)
            {
                relive_new Gibs(GibType::Slig_1, mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(656), mBaseAnimatedWithPhysicsGameObject_YPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1));
            }
            Stop_slig_sounds(dir, 0);
        }
    }
}

} // namespace AO
