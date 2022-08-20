#include "stdafx_ao.h"
#include "Function.hpp"
#include "BaseBomb.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ParticleBurst.hpp"
#include "ScreenShake.hpp"
#include "Midi.hpp"
#include "../relive_lib/Events.hpp"
#include "Flash.hpp"
#include "Particle.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

ALIVE_VAR(1, 0x4FFA4C, s16, word_4FFA4C, 0);

void BaseBomb::VUpdate()
{
    PSX_RECT rect = {};

    EventBroadcast(kEventShooting, this);
    EventBroadcast(kEventLoudNoise, this);
    EventBroadcast(kEventSuspiciousNoise, this);

    switch (mAnim.mCurrentFrame)
    {
        case 0:
            rect.x = FP_GetExponent(FP_FromInteger(-30) * field_E4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(30) * field_E4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * field_E4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(20) * field_E4_scale);
            DealDamageRect(&rect);
            break;

        case 1:
            rect.x = FP_GetExponent(FP_FromInteger(-50) * field_E4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(50) * field_E4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-30) * field_E4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(30) * field_E4_scale);
            DealDamageRect(&rect);
            break;

        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-80) * field_E4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(80) * field_E4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-40) * field_E4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(40) * field_E4_scale);
            DealDamageRect(&rect);
            break;

        case 3:
        {
            relive_new ParticleBurst(
                mXPos,
                mYPos,
                20,
                mSpriteScale,
                BurstType::eBigRedSparks_3);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);

            rect.x = FP_GetExponent(FP_FromInteger(-113) * field_E4_scale);
            rect.w = FP_GetExponent(FP_FromInteger(113) * field_E4_scale);
            rect.y = FP_GetExponent(FP_FromInteger(-50) * field_E4_scale);
            rect.h = FP_GetExponent(FP_FromInteger(50) * field_E4_scale);
            DealDamageRect(&rect);
            break;
        }

        case 4:
        {
            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u, TPageAbr::eBlend_1, 1);
            break;
        }

        case 7:
        {
            relive_new ParticleBurst(
                mXPos,
                mYPos,
                20,
                mSpriteScale,
                BurstType::eBigRedSparks_3);

            relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
            break;
        }

        default:
            break;
    }

    if (mAnim.mCurrentFrame == 3)
    {
        const AnimRecord& rec = AO::AnimRec(AnimId::Explosion_Mine);
        u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
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

    if (mAnim.mFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BaseBomb::DealDamageRect(const PSX_RECT* pRect)
{
    if (gBaseAliveGameObjects)
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

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
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
    mBaseGameObjectTypeId = ReliveTypes::eBaseBomb;

    const AnimRecord rec = AO::AnimRec(AnimId::Explosion_Mine);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::Explosion_Mine, ppRes);

    mAnim.mFlags.Clear(AnimFlags::eBit18_IsLastFrame);

    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mAnim.mBlue = 128;
    mAnim.mGreen = 128;
    mAnim.mRed = 128;

    field_E4_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mSpriteScale = scale * FP_FromDouble(2.75);

    mXPos = xpos;
    mYPos = ypos;

    relive_new ScreenShake(1);

    relive_new ParticleBurst(
        mXPos,
        mYPos,
        35,
        field_E4_scale,
        BurstType::eFallingRocks_0);

    PSX_RECT damageRect = {
        FP_GetExponent(FP_FromInteger(-10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(-10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_E4_scale),
        FP_GetExponent(FP_FromInteger(10) * field_E4_scale)};
    DealDamageRect(&damageRect);

    word_4FFA4C = 0;
    SND_SEQ_PlaySeq_4775A0(SeqId::eExplosion1_21, 1, 1);
}

} // namespace AO
