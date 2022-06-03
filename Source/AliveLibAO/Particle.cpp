#include "stdafx_ao.h"
#include "Function.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"

namespace AO {

Particle* New_DestroyOrCreateObject_Particle_419D00(FP xpos, FP ypos, FP scale)
{
    const AnimRecord& rec = AO::AnimRec(AnimId::DeathFlare_2);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);

    auto pParticle = ao_new<Particle>(xpos, ypos, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
    pParticle->mSpriteScale = FP_FromInteger(2) * scale;

    if (scale == FP_FromInteger(1))
    {
        pParticle->mAnim.mRenderLayer = Layer::eLayer_Above_FG1_39;
    }
    else
    {
        pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    pParticle->mApplyShadows &= ~1u;

    return pParticle;
}

void New_Smoke_Particles_419A80(FP xpos, FP ypos, FP scale, s16 count, s16 type)
{
    FP velYCounter = {};
    for (s32 i = 0; i < count; i++)
    {
        FP randX = (FP_FromInteger(Math_RandomRange_450F20(-3, 3)) * scale) + xpos;
        FP particleY = (FP_FromInteger(6 * (i + 1) / 2 * (1 - 2 * (i % 2))) * scale) + ypos;
        const AnimRecord& rec = AO::AnimRec(AnimId::SquibSmoke_Particle);
        u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
        auto pParticle = ao_new<Particle>(randX, particleY, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
        if (pParticle)
        {
            pParticle->mApplyShadows &= ~1u;
            pParticle->mAnim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
            pParticle->mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->mAnim.mRenderMode = TPageAbr::eBlend_3;

            if (type == 1)
            {
                pParticle->mRed = 32;
                pParticle->mGreen = 128;
                pParticle->mBlue = 32;
            }
            else
            {
                pParticle->mRed = 128;
                pParticle->mGreen = 128;
                pParticle->mBlue = 128;
            }

            pParticle->mVelX = (scale * FP_FromInteger(Math_RandomRange_450F20(-10, 10))) / FP_FromInteger(10);
            pParticle->mVelY = ((scale * velYCounter) * FP_FromInteger(Math_RandomRange_450F20(50, 50))) / FP_FromInteger(100);
            pParticle->mSpriteScale = scale;

            if (scale == FP_FromInteger(1))
            {
                pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
            }
            else
            {
                pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
            }

            pParticle->field_E4_scale_amount = scale * FP_FromDouble(0.03);
            pParticle->mAnim.field_10_frame_delay = static_cast<u16>((i + 3) / 2);
            if (Math_NextRandom() < 127)
            {
                pParticle->mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
        }
        velYCounter -= FP_FromInteger(1);
    }
}

void New_Chant_Particle_4198E0(FP xpos, FP ypos, FP scale, Layer layer)
{
    const AnimRecord& orbRec = AO::AnimRec(AnimId::ChantOrb_Particle);
    auto pParticle = ao_new<Particle>(xpos, ypos, orbRec.mFrameTableOffset, orbRec.mMaxW, orbRec.mMaxH, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, orbRec.mResourceId, 1, 0));
    if (pParticle)
    {
        pParticle->mApplyShadows &= ~1u;
        pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;

        if (layer != Layer::eLayer_0)
        {
            pParticle->mAnim.mRenderLayer = layer;
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mSpriteScale = scale;
    }
}

void New_Shiny_Particle_4199A0(FP xpos, FP ypos, FP scale, Layer layer)
{
    const AnimRecord& orbRec = AO::AnimRec(AnimId::ChantOrb_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, orbRec.mResourceId, 1, 0);
    auto pParticle = ao_new<Particle>(xpos, ypos, orbRec.mFrameTableOffset, orbRec.mMaxW, orbRec.mMaxH, ppRes);
    if (pParticle)
    {
        pParticle->mApplyShadows &= ~1u;
        pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
        pParticle->mRed = 100;
        pParticle->mGreen = 100;
        pParticle->mBlue = 100;

        if (layer != Layer::eLayer_0)
        {
            pParticle->mAnim.mRenderLayer = layer;
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mSpriteScale = scale;
    }
}

void New_ShootingZFire_Particle_419810(FP xpos, FP ypos, FP scale)
{
    const AnimRecord& rec = AO::AnimRec(AnimId::ShootingZFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    auto pParticle = ao_new<Particle>(xpos, ypos, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
    if (pParticle)
    {
        pParticle->mApplyShadows &= ~1u;

        pParticle->mBlue = 55;
        pParticle->mGreen = 55;
        pParticle->mRed = 55;

        pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mSpriteScale = scale;
    }
}

void New_ShootingFire_Particle_419720(FP xpos, FP ypos, s8 direction, FP scale)
{
    const AnimRecord& rec = AO::AnimRec(AnimId::ShootingFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    auto pParticle = ao_new<Particle>(xpos, ypos, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
    if (pParticle)
    {
        pParticle->mApplyShadows &= ~1u;
        pParticle->mBlue = 55;
        pParticle->mGreen = 55;
        pParticle->mRed = 55;
        pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, direction & 1);
        pParticle->mSpriteScale = scale;
    }
}

Particle::Particle(FP xpos, FP ypos, s32 animFrameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData)
{
    mBlue = 128;
    mGreen = 128;
    mRed = 128;

    mTypeId = Types::eParticle_93;

    Animation_Init_417FD0(
        animFrameTableOffset,
        maxW,
        maxH,
        ppAnimData,
        1);

    if (mGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mXPos = xpos;
    mYPos = ypos;
    field_E4_scale_amount = FP_FromInteger(0);
}

void Particle::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;

    mSpriteScale += field_E4_scale_amount;

    if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
