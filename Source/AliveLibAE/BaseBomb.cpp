#include "stdafx.h"
#include "BaseBomb.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/VRam.hpp"
#include "Game.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "ScreenShake.hpp"
#include "ParticleBurst.hpp"
#include "Flash.hpp"
#include "../relive_lib/Particle.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"

BaseBomb::BaseBomb(FP x, FP y, s32 /*unused*/, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBaseBomb);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Explosion_Mine));
    Animation_Init(GetAnimRes(AnimId::Explosion_Mine));

    mAnim.mFlags.Clear(AnimFlags::eBit18_IsLastFrame); // Double Check
    mAnim.mFlags.Set(AnimFlags::eBit24);

    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mAnim.mBlue = 128;
    mAnim.mGreen = 128;
    mAnim.mRed = 128;

    mBombSpriteScale = scale;

    if (scale == FP_FromDouble(1.0))
    {
        mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mSpriteScale = scale * FP_FromDouble(2.75);
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

void BaseBomb::VUpdate()
{
    PSX_RECT rect = {};

    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    switch (this->mAnim.mCurrentFrame)
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
                mSpriteScale,
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
                mSpriteScale,
                BurstType::eBigRedSparks_3,
                13);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, TPageAbr::eBlend_3, 1);
            break;
        }

        default:
            break;
    }

    if (mAnim.mCurrentFrame == 3)
    {
        const AnimRecord& rec = AnimRec(AnimId::Explosion_Mine);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        if (ppRes)
        {
            Particle* pParticle = relive_new Particle(
                mXPos,
                mYPos,
                AnimId::Explosion_Mine,
                ppRes);
            if (pParticle)
            {
                pParticle->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
                pParticle->mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
                pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                pParticle->mSpriteScale = mSpriteScale * FP_FromDouble(0.7);
            }
        }
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted)) // Animation ended
    {
        // Time to die
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BaseBomb::DealDamageRect(const PSX_RECT* pRect)
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
                    if (mSpriteScale == (pObj->mSpriteScale * FP_FromDouble(2.75)))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }
}
