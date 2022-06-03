#include "stdafx_ao.h"
#include "Function.hpp"
#include "BaseBomb.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ParticleBurst.hpp"
#include "ScreenShake.hpp"
#include "Midi.hpp"
#include "Events.hpp"
#include "Flash.hpp"
#include "Particle.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

ALIVE_VAR(1, 0x4FFA4C, s16, word_4FFA4C, 0);

void BaseBomb::VUpdate()
{
    PSX_RECT rect = {};

    Event_Broadcast(kEvent_2, this);
    Event_Broadcast(kEvent_14, this);
    Event_Broadcast(kEventSuspiciousNoise_10, this);

    switch (mAnim.field_92_current_frame)
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
            ao_new<ParticleBurst>(
                mXPos,
                mYPos,
                20,
                mSpriteScale,
                BurstType::eBigRedSparks_3);

            ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);

            rect.x = FP_GetExponent(FP_FromInteger(-113) * mScale);
            rect.w = FP_GetExponent(FP_FromInteger(113) * mScale);
            rect.y = FP_GetExponent(FP_FromInteger(-50) * mScale);
            rect.h = FP_GetExponent(FP_FromInteger(50) * mScale);
            DealDamageRect(&rect);
            break;
        }

        case 4:
        {
            ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u, 1, TPageAbr::eBlend_1, 1);
            break;
        }

        case 7:
        {
            ao_new<ParticleBurst>(
                mXPos,
                mYPos,
                20,
                mSpriteScale,
                BurstType::eBigRedSparks_3);

            ao_new<Flash>(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        default:
            break;
    }

    if (mAnim.field_92_current_frame == 3)
    {
        const AnimRecord& rec = AO::AnimRec(AnimId::Explosion_Mine);
        u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
        if (ppRes)
        {
            Particle* pParticle = ao_new<Particle>(
                mXPos,
                mYPos,
                rec.mFrameTableOffset,
                rec.mMaxW,
                rec.mMaxH,
                ppRes);
            if (pParticle)
            {
                pParticle->mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
                pParticle->mApplyShadows &= ~1u;
                pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
                pParticle->mSpriteScale = mSpriteScale * FP_FromDouble(0.7);
            }
        }
    }

    if (mAnim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BaseBomb::DealDamageRect(const PSX_RECT* pRect)
{
    if (gBaseAliveGameObjects_4FC8A0)
    {
        s16 min_w_x = pRect->w;
        if (pRect->x <= pRect->w)
        {
            min_w_x = pRect->x;
        }

        auto min_x_w = pRect->w;
        if (pRect->w <= pRect->x)
        {
            min_x_w = pRect->x;
        }

        auto min_y_h = pRect->h;
        if (pRect->y <= pRect->h)
        {
            min_y_h = pRect->y;
        }

        s16 min_h_y = pRect->h;
        if (pRect->h <= pRect->y)
        {
            min_h_y = pRect->y;
        }

        auto right = FP_GetExponent(mXPos) + min_x_w;
        auto left = FP_GetExponent(mXPos) + min_w_x;
        auto top = FP_GetExponent(mYPos) + min_y_h;
        auto bottom = FP_GetExponent(mYPos) + min_h_y;

        if ((abs(left) & 1023) < 256)
        {
            left -= 656;
        }

        if ((abs(right) & 1023) > 624)
        {
            right += 656;
        }

        if (top % 480 < 120)
        {
            top -= 240;
        }

        if (bottom % 480 > 360)
        {
            bottom += 240;
        }

        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            const s16 obj_xpos = FP_GetExponent(pObj->mXPos);
            if (obj_xpos >= left && obj_xpos <= right)
            {
                const s16 obj_ypos = FP_GetExponent(pObj->mYPos);
                if (obj_ypos >= top && obj_ypos <= bottom && mSpriteScale == (pObj->mSpriteScale * FP_FromDouble(2.75)))
                {
                    pObj->VTakeDamage(this);
                }
            }
        }
    }
}

BaseBomb::BaseBomb(FP xpos, FP ypos, s32 /*unused*/, FP scale)
{
    mTypeId = Types::eBaseBomb_30;

    const AnimRecord& rec = AO::AnimRec(AnimId::Explosion_Mine);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mAnim.mAnimFlags.Clear(AnimFlags::eBit18_IsLastFrame);

    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mAnim.mBlue = 128;
    mAnim.mGreen = 128;
    mAnim.mRed = 128;

    mScale = scale;

    if (scale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    mApplyShadows &= ~1u;
    mSpriteScale = scale * FP_FromDouble(2.75);

    mXPos = xpos;
    mYPos = ypos;

    ao_new<ScreenShake>(1);

    ao_new<ParticleBurst>(
        mXPos,
        mYPos,
        35,
        mScale,
        BurstType::eFallingRocks_0);

    PSX_RECT damageRect = {
        FP_GetExponent(FP_FromInteger(-10) * mScale),
        FP_GetExponent(FP_FromInteger(-10) * mScale),
        FP_GetExponent(FP_FromInteger(10) * mScale),
        FP_GetExponent(FP_FromInteger(10) * mScale)};
    DealDamageRect(&damageRect);

    word_4FFA4C = 0;
    SND_SEQ_PlaySeq_4775A0(SeqId::eExplosion1_21, 1, 1);
}

} // namespace AO
