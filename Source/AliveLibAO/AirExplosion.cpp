#include "stdafx_ao.h"
#include "AirExplosion.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "ScreenShake.hpp"
#include "Gibs.hpp"
#include "../relive_lib/Events.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "AmbientSound.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

#undef min
#undef max

#include <algorithm>

namespace AO {

AirExplosion::AirExplosion(FP xpos, FP ypos, FP explosion_size)
    : BaseAnimatedWithPhysicsGameObject(0),
    mExplosionSize(explosion_size)
{
    SetType(ReliveTypes::eAirExplosion);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AirExplosion));
    Animation_Init(GetAnimRes(AnimId::AirExplosion));

    GetAnimation().SetIsLastFrame(false);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);

    SetSpriteScale(explosion_size * FP_FromInteger(2));
    SetApplyShadowZoneColour(false);
    mXPos = xpos;
    mYPos = ypos;

    relive_new ScreenShake(true);

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(FP_FromInteger(-10) * explosion_size);
    rect.y = FP_GetExponent(FP_FromInteger(-10) * explosion_size);
    rect.w = FP_GetExponent(FP_FromInteger(10) * explosion_size);
    rect.h = FP_GetExponent(FP_FromInteger(10) * explosion_size);

    DealBlastDamage(&rect);

    SND_SEQ_PlaySeq(SeqId::eExplosion1_21, 1, 1);
}

void AirExplosion::VUpdate()
{
    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    PSX_RECT rect = {};

    switch (GetAnimation().GetCurrentFrame())
    {
        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-20) * mExplosionSize);
            rect.w = FP_GetExponent(FP_FromInteger(20) * mExplosionSize);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * mExplosionSize);
            rect.h = FP_GetExponent(FP_FromInteger(10) * mExplosionSize);
            DealBlastDamage(&rect);
            break;

        case 3:
        {
            relive_new ParticleBurst(mXPos, mYPos, 20, GetSpriteScale(), BurstType::eBigRedSparks_3);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        case 4:
        {
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u, relive::TBlendModes::eBlend_1, 1);

            rect.x = FP_GetExponent(FP_FromInteger(-38) * mExplosionSize);
            rect.w = FP_GetExponent(FP_FromInteger(38) * mExplosionSize);
            rect.y = FP_GetExponent(FP_FromInteger(-38) * mExplosionSize);
            rect.h = FP_GetExponent(FP_FromInteger(19) * mExplosionSize);
            DealBlastDamage(&rect);
            break;
        }

        case 6:
            rect.x = FP_GetExponent(FP_FromInteger(-60) * mExplosionSize);
            rect.w = FP_GetExponent(FP_FromInteger(60) * mExplosionSize);
            rect.y = FP_GetExponent(FP_FromInteger(-60) * mExplosionSize);
            rect.h = FP_GetExponent(FP_FromInteger(30) * mExplosionSize);
            DealBlastDamage(&rect);
            break;

        case 8:
        {
            relive_new ParticleBurst(mXPos, mYPos, 20, GetSpriteScale(), BurstType::eBigRedSparks_3);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        default:
            break;
    }

    if (GetAnimation().GetCurrentFrame() > 9)
    {
        SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.2));
    }

    if (GetAnimation().GetCurrentFrame() == 1)
    {
        auto pParticle = relive_new Particle(mXPos, mYPos, GetAnimRes(AnimId::AirExplosion));
        if (pParticle)
        {
            if (pParticle->GetListAddFailed())
            {
                pParticle->SetDead(true);
            }

            pParticle->SetApplyShadowZoneColour(false);
            pParticle->GetAnimation().SetFlipX(false);
            pParticle->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
            pParticle->SetSpriteScale(GetSpriteScale() * FP_FromDouble(0.25));
        }
        else
        {
            pParticle = nullptr;
        }
    }

    if (GetAnimation().GetForwardLoopCompleted())
    {
        SetDead(true);
    }
}

void AirExplosion::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

void AirExplosion::DealBlastDamage(PSX_RECT* pRect)
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
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetIsBaseAliveGameObject())
        {
            const PSX_RECT boundRect = pObj->VGetBoundingRect();
            if (PSX_Rects_overlap_no_adjustment(&boundRect, &expandedRect) && mExplosionSize == pObj->GetSpriteScale())
            {
                pObj->VTakeDamage(this);
            }
        }
    }

    auto pTlv = static_cast<relive::Path_Slig*>(gMap.VTLV_Get_At(
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
