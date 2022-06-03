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
    SetType(AETypes::eBaseBomb_46);

    const AnimRecord& rec = AnimRec(AnimId::Explosion_Mine);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, BaseGameObject::Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId), 1, 1);

    mAnim.mAnimFlags.Clear(AnimFlags::eBit18_IsLastFrame); // Double Check
    mAnim.mAnimFlags.Set(AnimFlags::eBit24);

    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mAnim.mBlue = 128;
    mAnim.mGreen = 128;
    mAnim.mRed = 128;

    mScale = scale;

    if (scale == FP_FromDouble(1.0))
    {
        mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    mApplyShadows &= ~1;
    mSpriteScale = scale * FP_FromDouble(2.75);
    mXPos = x;
    mYPos = y;

    ae_new<ScreenShake>(true, false);

    ae_new<ParticleBurst>(
        mXPos,
        mYPos,
        35,
        mScale,
        BurstType::eFallingRocks_0,
        13);

    PSX_RECT damageRect = {
        FP_GetExponent(FP_FromInteger(-10) * mScale),
        FP_GetExponent(FP_FromInteger(-10) * mScale),
        FP_GetExponent(FP_FromInteger(10) * mScale),
        FP_GetExponent(FP_FromInteger(10) * mScale)};
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

    Event_Broadcast(kEventShooting, this);
    Event_Broadcast(kEventLoudNoise, this);
    Event_Broadcast(kEventSuspiciousNoise, this);

    switch (this->mAnim.field_92_current_frame)
    {
        case 0:
            rect.x = FP_GetExponent(FP_FromInteger(-30) * mScale);
            rect.w = FP_GetExponent(FP_FromInteger(30) * mScale);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * mScale);
            rect.h = FP_GetExponent(FP_FromInteger(20) * mScale);
            DealDamageRect(&rect);
            break;

        case 1:
            rect.x = FP_GetExponent(FP_FromInteger(-50) * mScale);
            rect.w = FP_GetExponent(FP_FromInteger(50) * mScale);
            rect.y = FP_GetExponent(FP_FromInteger(-30) * mScale);
            rect.h = FP_GetExponent(FP_FromInteger(30) * mScale);
            DealDamageRect(&rect);
            break;

        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-80) * mScale);
            rect.w = FP_GetExponent(FP_FromInteger(80) * mScale);
            rect.y = FP_GetExponent(FP_FromInteger(-40) * mScale);
            rect.h = FP_GetExponent(FP_FromInteger(40) * mScale);
            DealDamageRect(&rect);
            break;

        case 3:
        {
            ae_new<ParticleBurst>(
                mXPos,
                mYPos,
                20,
                mSpriteScale,
                BurstType::eBigRedSparks_3,
                13);

            ae_new<Flash>(Layer::eLayer_Above_FG1_39, static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(255), 1, TPageAbr::eBlend_3, 1);

            rect.x = FP_GetExponent(FP_FromInteger(-113) * mScale);
            rect.w = FP_GetExponent(FP_FromInteger(113) * mScale);
            rect.y = FP_GetExponent(FP_FromInteger(-50) * mScale);
            rect.h = FP_GetExponent(FP_FromInteger(50) * mScale);
            DealDamageRect(&rect);
            break;
        }

        case 4:
        {
            ae_new<Flash>(Layer::eLayer_Above_FG1_39, static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(255), 1, TPageAbr::eBlend_1, 1);
            break;
        }

        case 7:
        {
            ae_new<ParticleBurst>(
                mXPos,
                mYPos,
                20u,
                mSpriteScale,
                BurstType::eBigRedSparks_3,
                13);

            ae_new<Flash>(Layer::eLayer_Above_FG1_39, static_cast<u8>(255), static_cast<u8>(255), static_cast<u8>(255), 1, TPageAbr::eBlend_3, 1);
            break;
        }

        default:
            break;
    }

    if (mAnim.field_92_current_frame == 3)
    {
        const AnimRecord& rec = AnimRec(AnimId::Explosion_Mine);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        if (ppRes)
        {
            Particle* pParticle = ae_new<Particle>(
                mXPos,
                mYPos,
                rec.mFrameTableOffset,
                rec.mMaxW,
                rec.mMaxH,
                ppRes);
            if (pParticle)
            {
                pParticle->mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                pParticle->mApplyShadows &= ~1;
                pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                pParticle->mSpriteScale = mSpriteScale * FP_FromDouble(0.7);
            }
        }
    }

    if (mAnim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted)) // Animation ended
    {
        // Time to die
        mGameObjectFlags.Set(Options::eDead);
    }
}
