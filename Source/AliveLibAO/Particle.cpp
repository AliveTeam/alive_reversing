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
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);

    auto pParticle = relive_new Particle(xpos, ypos, AnimId::DeathFlare_2, ppRes);
    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
    pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(2) * scale;

    if (scale == FP_FromInteger(1))
    {
        pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Above_FG1_39;
    }
    else
    {
        pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);

    return pParticle;
}

void New_Smoke_Particles_419A80(FP xpos, FP ypos, FP scale, s16 count, s16 type)
{
    FP velYCounter = {};
    for (s32 i = 0; i < count; i++)
    {
        FP randX = (FP_FromInteger(Math_RandomRange(-3, 3)) * scale) + xpos;
        FP particleY = (FP_FromInteger(6 * (i + 1) / 2 * (1 - 2 * (i % 2))) * scale) + ypos;
        const AnimRecord& rec = AO::AnimRec(AnimId::SquibSmoke_Particle);
        u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
        auto pParticle = relive_new Particle(randX, particleY, AnimId::SquibSmoke_Particle, ppRes);
        if (pParticle)
        {
            pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_3;

            // TODO: type enum
            if (type == 1)
            {
                pParticle->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(32, 128, 32);
            }
            else
            {
                pParticle->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(128, 128, 128);
            }

            pParticle->mBaseAnimatedWithPhysicsGameObject_VelX = (scale * FP_FromInteger(Math_RandomRange(-10, 10))) / FP_FromInteger(10);
            pParticle->mBaseAnimatedWithPhysicsGameObject_VelY = ((scale * velYCounter) * FP_FromInteger(Math_RandomRange(50, 50))) / FP_FromInteger(100);
            pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

            if (scale == FP_FromInteger(1))
            {
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
            }
            else
            {
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
            }

            pParticle->field_E4_scale_amount = scale * FP_FromDouble(0.03);
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameDelay = static_cast<u16>((i + 3) / 2);
            if (Math_NextRandom() < 127)
            {
                pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
        }
        velYCounter -= FP_FromInteger(1);
    }
}

void New_Chant_Particle_4198E0(FP xpos, FP ypos, FP scale, Layer layer)
{
    const AnimRecord& orbRec = AO::AnimRec(AnimId::ChantOrb_Particle);
    auto pParticle = relive_new Particle(xpos, ypos, AnimId::ChantOrb_Particle, ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, orbRec.mResourceId, 1, 0));
    if (pParticle)
    {
        pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
        pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;

        if (layer != Layer::eLayer_0)
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = layer;
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
    }
}

void New_Shiny_Particle_4199A0(FP xpos, FP ypos, FP scale, Layer layer)
{
    const AnimRecord& orbRec = AO::AnimRec(AnimId::ChantOrb_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, orbRec.mResourceId, 1, 0);
    auto pParticle = relive_new Particle(xpos, ypos, AnimId::ChantOrb_Particle, ppRes);
    if (pParticle)
    {
        pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
        pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
        pParticle->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(100, 100, 100);

        if (layer != Layer::eLayer_0)
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = layer;
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
    }
}

void New_ShootingZFire_Particle_419810(FP xpos, FP ypos, FP scale)
{
    const AnimRecord& rec = AO::AnimRec(AnimId::ShootingZFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    auto pParticle = relive_new Particle(xpos, ypos, AnimId::ShootingZFire_Particle, ppRes);
    if (pParticle)
    {
        pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);

        pParticle->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(55, 55, 55);

        pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
    }
}

void New_ShootingFire_Particle_419720(FP xpos, FP ypos, s8 direction, FP scale)
{
    const AnimRecord& rec = AO::AnimRec(AnimId::ShootingFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    auto pParticle = relive_new Particle(xpos, ypos, AnimId::ShootingFire_Particle, ppRes);
    if (pParticle)
    {
        pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
        pParticle->mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(55, 55, 55);
        pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, direction & 1);
        pParticle->mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
    }
}

Particle::Particle(FP xpos, FP ypos, AnimId animId, u8** ppAnimData)
{
    mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(128, 128, 128);

    mBaseGameObjectTypeId = ReliveTypes::eParticle;

    Animation_Init(animId, ppAnimData, 1);

    if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    field_E4_scale_amount = FP_FromInteger(0);
}

void Particle::VUpdate()
{
    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    mBaseAnimatedWithPhysicsGameObject_SpriteScale += field_E4_scale_amount;

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
