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
    SetType(ReliveTypes::eBaseBomb);

    const AnimRecord& rec = AnimRec(AnimId::Explosion_Mine);
    Animation_Init(AnimId::Explosion_Mine, BaseGameObject::Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId));

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit18_IsLastFrame); // Double Check
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit24);

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;

    mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = 128;
    mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = 128;
    mBaseAnimatedWithPhysicsGameObject_Anim.mRed = 128;

    field_f4_scale = scale;

    if (scale == FP_FromDouble(1.0))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale * FP_FromDouble(2.75);
    mBaseAnimatedWithPhysicsGameObject_XPos = x;
    mBaseAnimatedWithPhysicsGameObject_YPos = y;

    relive_new ScreenShake(true, false);

    relive_new ParticleBurst(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
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

    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    switch (this->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame)
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
            relive_new ParticleBurst(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                20,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                BurstType::eBigRedSparks_3,
                13);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, 1, TPageAbr::eBlend_3, 1);

            rect.x = FP_GetExponent(FP_FromInteger(-113) * field_f4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(113) * field_f4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-50) * field_f4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(50) * field_f4_scale);
            DealDamageRect(&rect);
            break;
        }

        case 4:
        {
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, 1, TPageAbr::eBlend_1, 1);
            break;
        }

        case 7:
        {
            relive_new ParticleBurst(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                20u,
                mBaseAnimatedWithPhysicsGameObject_SpriteScale,
                BurstType::eBigRedSparks_3,
                13);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, 1, TPageAbr::eBlend_3, 1);
            break;
        }

        default:
            break;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 3)
    {
        const AnimRecord& rec = AnimRec(AnimId::Explosion_Mine);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        if (ppRes)
        {
            Particle* pParticle = relive_new Particle(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                AnimId::Explosion_Mine,
                ppRes);
            if (pParticle)
            {
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                pParticle->mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
                pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(0.7);
            }
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted)) // Animation ended
    {
        // Time to die
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

void BaseBomb::DealDamageRect(const PSX_RECT* pRect)
{
    if (gBaseAliveGameObjects_5C1B7C)
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

        const auto right = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) + min_x_w;
        const auto left = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos) + min_w_x;
        const auto top = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) + min_y_h;
        const auto bottom = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos) + min_h_y;

        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            const auto objXPos = FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos);
            const auto objYPos = FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos);

            if (objXPos >= right && objXPos <= left)
            {
                if (objYPos >= top && objYPos <= bottom)
                {
                    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == (pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(2.75)))
                    {
                        pObj->VTakeDamage(this);
                    }
                }
            }
        }
    }
}
