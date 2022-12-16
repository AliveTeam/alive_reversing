#include "stdafx.h"
#include "GroundExplosion.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Sound/Midi.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "../relive_lib/Flash.hpp"
#include "../relive_lib/Particle.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/IBaseAliveGameObject.hpp"

GroundExplosion::GroundExplosion(FP x, FP y, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eGroundExplosion);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::GroundExplosion));
    Animation_Init(GetAnimRes(AnimId::GroundExplosion));

    GetAnimation().SetIsLastFrame(false); // Double Check

    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

    GetAnimation().SetRGB(128, 128, 128);

    mBombSpriteScale = scale;

    if (scale == FP_FromDouble(1.0))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
    }

    SetApplyShadowZoneColour(false);
    SetSpriteScale(scale * FP_FromDouble(2.75));

    mXPos = x;
    mYPos = y;

    relive_new ScreenShake(true, false);

    relive_new ParticleBurst(
        mXPos,
        mYPos,
        35,
        mBombSpriteScale,
        BurstType::eFallingRocks_0,
        13);

    PSX_RECT damageRect = {
        FP_GetExponent(FP_FromInteger(-10) * mBombSpriteScale),
        FP_GetExponent(FP_FromInteger(-10) * mBombSpriteScale),
        FP_GetExponent(FP_FromInteger(10) * mBombSpriteScale),
        FP_GetExponent(FP_FromInteger(10) * mBombSpriteScale)};
    DealDamageRect(&damageRect);

    // alternate between Explosion1 and Explosion2 on each call
    static s32 staticIdFlip = 0;
    const s16 explosionSeqId = static_cast<s16>(staticIdFlip ? SeqId::Explosion1_14 : SeqId::Explosion2_15);
    SND_SEQ_PlaySeq(explosionSeqId, 1, 1);
    staticIdFlip = !staticIdFlip;
}

void GroundExplosion::VUpdate()
{
    PSX_RECT rect = {};

    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    switch (this->GetAnimation().GetCurrentFrame())
    {
        case 0:
            rect.x = FP_GetExponent(FP_FromInteger(-30) * mBombSpriteScale);
            rect.w = FP_GetExponent(FP_FromInteger(30) * mBombSpriteScale);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * mBombSpriteScale);
            rect.h = FP_GetExponent(FP_FromInteger(20) * mBombSpriteScale);
            DealDamageRect(&rect);
            break;

        case 1:
            rect.x = FP_GetExponent(FP_FromInteger(-50) * mBombSpriteScale);
            rect.w = FP_GetExponent(FP_FromInteger(50) * mBombSpriteScale);
            rect.y = FP_GetExponent(FP_FromInteger(-30) * mBombSpriteScale);
            rect.h = FP_GetExponent(FP_FromInteger(30) * mBombSpriteScale);
            DealDamageRect(&rect);
            break;

        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-80) * mBombSpriteScale);
            rect.w = FP_GetExponent(FP_FromInteger(80) * mBombSpriteScale);
            rect.y = FP_GetExponent(FP_FromInteger(-40) * mBombSpriteScale);
            rect.h = FP_GetExponent(FP_FromInteger(40) * mBombSpriteScale);
            DealDamageRect(&rect);
            break;

        case 3:
        {
            relive_new ParticleBurst(
                mXPos,
                mYPos,
                20,
                GetSpriteScale(),
                BurstType::eBigRedSparks_3,
                13);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, TPageAbr::eBlend_3, 1);

            rect.x = FP_GetExponent(FP_FromInteger(-113) * mBombSpriteScale);
            rect.w = FP_GetExponent(FP_FromInteger(113) * mBombSpriteScale);
            rect.y = FP_GetExponent(FP_FromInteger(-50) * mBombSpriteScale);
            rect.h = FP_GetExponent(FP_FromInteger(50) * mBombSpriteScale);
            DealDamageRect(&rect);
            break;
        }

        case 4:
        {
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, TPageAbr::eBlend_1, 1);
            break;
        }

        case 7:
        {
            relive_new ParticleBurst(
                mXPos,
                mYPos,
                20u,
                GetSpriteScale(),
                BurstType::eBigRedSparks_3,
                13);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, TPageAbr::eBlend_3, 1);
            break;
        }

        default:
            break;
    }

    if (GetAnimation().GetCurrentFrame() == 3)
    {
        Particle* pParticle = relive_new Particle(
            mXPos,
            mYPos,
            GetAnimRes(AnimId::GroundExplosion));
        if (pParticle)
        {
            pParticle->GetAnimation().SetFlipX(true);
            pParticle->SetApplyShadowZoneColour(false);
            pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
            pParticle->SetSpriteScale(GetSpriteScale() * FP_FromDouble(0.7));
        }
    }

    if (GetAnimation().GetForwardLoopCompleted()) // Animation ended
    {
        // Time to die
        SetDead(true);
    }
}

void GroundExplosion::DealDamageRect(const PSX_RECT* pRect)
{
    if (gBaseAliveGameObjects)
    {
        auto min_x_w = pRect->w;
        if (pRect->x <= pRect->w)
        {
            min_x_w = pRect->x;
        }

        auto min_w_x = pRect->w;
        if (pRect->w <= pRect->x)
        {
            min_w_x = pRect->x;
        }

        auto min_y_h = pRect->h;
        if (pRect->y <= pRect->h)
        {
            min_y_h = pRect->y;
        }

        auto min_h_y = pRect->h;
        if (pRect->h <= pRect->y)
        {
            min_h_y = pRect->y;
        }

        const auto right = FP_GetExponent(mXPos) + min_x_w;
        const auto left = FP_GetExponent(mXPos) + min_w_x;
        const auto top = FP_GetExponent(mYPos) + min_y_h;
        const auto bottom = FP_GetExponent(mYPos) + min_h_y;

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            const auto objXPos = FP_GetExponent(pObj->mXPos);
            const auto objYPos = FP_GetExponent(pObj->mYPos);

            if (objXPos >= right && objXPos <= left)
            {
                if (objYPos >= top && objYPos <= bottom)
                {
                    if (GetSpriteScale() == (pObj->GetSpriteScale() * FP_FromDouble(2.75)))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }
}
