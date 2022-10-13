#include "stdafx_ao.h"
#include "Explosion.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "../relive_lib/Particle.hpp"
#include "Flash.hpp"
#include "ScreenShake.hpp"
#include "Gibs.hpp"
#include "../relive_lib/Events.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "AmbientSound.hpp"
#include "BaseAliveGameObject.hpp"

#undef min
#undef max

namespace AO {

Explosion::Explosion(FP xpos, FP ypos, FP exposion_size)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eExplosion);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Explosion));
    Animation_Init(GetAnimRes(AnimId::Explosion));

    mAnim.mFlags.Clear(AnimFlags::eIsLastFrame);
    mAnim.mRenderMode = TPageAbr::eBlend_1;
    field_E4_explosion_size = exposion_size;

    mSpriteScale = exposion_size * FP_FromInteger(2);
    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mYPos = ypos;
    mXPos = xpos;

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
    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    PSX_RECT rect = {};

    switch (mAnim.mCurrentFrame)
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
            relive_new ParticleBurst(mXPos, mYPos, 20, mSpriteScale, BurstType::eBigRedSparks_3);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        case 4:
        {
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u, TPageAbr::eBlend_1, 1);

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
            relive_new ParticleBurst(mXPos, mYPos, 20, mSpriteScale, BurstType::eBigRedSparks_3);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        default:
            break;
    }

    if (mAnim.mCurrentFrame > 9)
    {
        mSpriteScale -= FP_FromDouble(0.2);
    }

    if (mAnim.mCurrentFrame == 1)
    {
        auto pParticle = relive_new Particle(mXPos, mYPos, GetAnimRes(AnimId::Explosion));
        if (pParticle)
        {
            if (pParticle->mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
            {
                pParticle->mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            pParticle->mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
            pParticle->mAnim.mFlags.Clear(AnimFlags::eFlipX);
            pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
            pParticle->mSpriteScale = mSpriteScale * FP_FromDouble(0.25);
        }
        else
        {
            pParticle = nullptr;
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eForwardLoopCompleted))
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
    if (!gBaseAliveGameObjects)
    {
        return;
    }

    PSX_RECT expandedRect = {};
    expandedRect.x = std::min(pRect->x, pRect->w);
    expandedRect.w = std::max(pRect->x, pRect->w);

    expandedRect.y = std::min(pRect->y, pRect->h);
    expandedRect.h = std::max(pRect->y, pRect->h);

    expandedRect.x += FP_GetExponent(mXPos);
    expandedRect.y += FP_GetExponent(mYPos);

    expandedRect.w += FP_GetExponent(mXPos);
    expandedRect.h += FP_GetExponent(mYPos);

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

    for (s32 idx = 0; idx < gBaseAliveGameObjects->Size(); idx++)
    {
        auto pObj = gBaseAliveGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseGameObjectFlags.Get(Options::eIsBaseAliveGameObject_Bit6))
        {
            const PSX_RECT rect = pObj->VGetBoundingRect();
            if (PSX_Rects_overlap_no_adjustment(&rect, &expandedRect) && field_E4_explosion_size == pObj->mSpriteScale)
            {
                pObj->VTakeDamage(this);
            }
        }
    }

    auto pTlv = static_cast<relive::Path_Slig*>(gMap.TLV_Get_At(
        expandedRect.x,
        expandedRect.y,
        expandedRect.w,
        expandedRect.h,
        ReliveTypes::eSlig));

    if (pTlv)
    {
        if (!pTlv->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed) && pTlv->mData.mStartState == relive::Path_Slig_Data::StartState::Sleeping)
        {
            pTlv->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);

            const CameraPos dir = gMap.GetDirection(
                gMap.mCurrentLevel,
                gMap.mCurrentPath,
                FP_FromInteger(pTlv->mTopLeftX),
                FP_FromInteger(pTlv->mTopLeftY));

            if (dir == CameraPos::eCamLeft_3)
            {
                relive_new Gibs(GibType::Slig_1, mXPos + FP_FromInteger(656), mYPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1));
            }
            else if (dir == CameraPos::eCamRight_4)
            {
                relive_new Gibs(GibType::Slig_1, mXPos - FP_FromInteger(656), mYPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1));
            }

            AO::Stop_slig_sounds(dir, 0);
        }
    }
}

} // namespace AO
