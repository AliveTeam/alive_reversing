#include "stdafx_ao.h"
#include "AirExplosion.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "../relive_lib/GameObjects/ScreenShake.hpp"
#include "../relive_lib/GameObjects/Gibs.hpp"
#include "../relive_lib/Events.hpp"
#include "ParticleBurst.hpp"
#include "Midi.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "AmbientSound.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../AliveLibAE/ExplosionSet.hpp"
#include "../relive_lib/GameType.hpp"

#undef min
#undef max

#include <algorithm>

namespace AO {

AirExplosion::AirExplosion(FP xpos, FP ypos, FP explosion_size, bool bSmall)
    : BaseAnimatedWithPhysicsGameObject(0),
    mExplosionSize(explosion_size), 
    mSmallExplosion(bSmall)
{
    SetType(ReliveTypes::eAirExplosion);

    if (mSmallExplosion)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AirExplosion_Small));
        Animation_Init(GetAnimRes(AnimId::AirExplosion_Small));
    }
    else
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AirExplosion));
        Animation_Init(GetAnimRes(AnimId::AirExplosion));
    }

    GetAnimation().SetIsLastFrame(false);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);

    mParticleScale = explosion_size;
    SetScale(explosion_size == FP_FromInteger(1) ? Scale::Fg : Scale::Bg);
    SetSpriteScale(explosion_size * FP_FromInteger(2));

    if (mSmallExplosion)
    {
        mExplosionSize = explosion_size * FP_FromDouble(0.5);
    }

    SetApplyShadowZoneColour(false);
    mXPos = xpos;
    mYPos = ypos;

    relive_new ScreenShake(gExplosionSetEnabled ? false : true, mSmallExplosion);

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(FP_FromInteger(-10) * mExplosionSize);
    rect.y = FP_GetExponent(FP_FromInteger(-10) * mExplosionSize);
    rect.w = FP_GetExponent(FP_FromInteger(10) * mExplosionSize);
    rect.h = FP_GetExponent(FP_FromInteger(10) * mExplosionSize);

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
            // TODO: AE has a rev bug and should've done what AO does
            // in the else block but we have to keep the bug until we do a new
            // recording for AE
            if (GetGameType() == GameType::eAe)
            {
                rect.x = FP_GetExponent(FP_FromInteger(-60) * mExplosionSize);
                rect.w = FP_GetExponent(FP_FromInteger(60) * mExplosionSize);
                rect.y = FP_GetExponent(FP_FromInteger(-60) * mExplosionSize);
                rect.h = FP_GetExponent(FP_FromInteger(30) * mExplosionSize);
                DealBlastDamage(&rect);
            }
            else
            {
                relive_new ParticleBurst(mXPos, mYPos, mSmallExplosion ? 6 : 20, mParticleScale, BurstType::eBigRedSparks, mSmallExplosion ? 11 : 13, false);
                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, relive::TBlendModes::eBlend_3, 1);
            }
            break;
        }

        case 4:
        {
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, relive::TBlendModes::eBlend_1, 1);
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
            relive_new ParticleBurst(mXPos, mYPos, mSmallExplosion ? 6 : 20, mParticleScale, BurstType::eBigRedSparks, mSmallExplosion ? 11 : 13, false);
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, relive::TBlendModes::eBlend_3, 1);
            break;
        }

        default:
            break;
    }

    if (GetAnimation().GetCurrentFrame() > 9)
    {
        if (mSmallExplosion)
        {
            SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.066));
        }
        else
        {
            SetSpriteScale(GetSpriteScale() - FP_FromDouble(0.2));
        }
    }

    if (GetAnimation().GetCurrentFrame() == 1)
    {
        const bool explosionSizeHack = GetGameType() == GameType::eAe;
        const AnimId explosionId = mSmallExplosion ? AnimId::AirExplosion_Small : AnimId::AirExplosion;
        auto pParticle = relive_new Particle(mXPos, mYPos, GetAnimRes(explosionId), explosionSizeHack);

        if (pParticle)
        {
            if (pParticle->GetListAddFailed())
            {
                pParticle->SetDead(true);
            }

            pParticle->SetApplyShadowZoneColour(false);
            pParticle->GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);

            pParticle->GetAnimation().SetFlipX(false);
            pParticle->SetSpriteScale(GetSpriteScale() * FP_FromDouble(0.25));
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

    if (GetGameType() == GameType::eAo)
    {
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
                relive_new Gibs(GibType::eSlig, mXPos + FP_FromInteger(656), mYPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), false);
            }
            else if (dir == CameraPos::eCamRight_4)
            {
                relive_new Gibs(GibType::eSlig, mXPos - FP_FromInteger(656), mYPos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), false);
            }

            AO::Stop_slig_sounds(dir, 0);
        }
    }
}

} // namespace AO
