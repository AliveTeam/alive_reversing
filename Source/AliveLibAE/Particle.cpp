#include "stdafx.h"
#include "Particle.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"

Particle::Particle(FP xpos, FP ypos, s32 animFrameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eParticle_134);

    ResourceManager::Inc_Ref_Count_49C310(ppAnimData);

    field_10_resources_array.Push_Back(ppAnimData);

    field_D4_b = 128;
    field_D2_g = 128;
    field_D0_r = 128;

    Animation_Init(animFrameTableOffset, static_cast<s16>(maxW), static_cast<s16>(maxH), ppAnimData, 1, 1);

    if (mBaseGameObjectFlags.Get(Options::eListAddFailed_Bit1))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    field_F4_scale_amount = FP_FromInteger(0);
}

void Particle::VUpdate()
{
    mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
    mBaseAnimatedWithPhysicsGameObject_YPos += field_C8_vely;
    field_CC_sprite_scale += field_F4_scale_amount;

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

Particle* New_DestroyOrCreateObject_Particle(FP xpos, FP ypos, FP scale)
{
    const AnimRecord& rec = AnimRec(AnimId::DeathFlare_2);
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, rec.mResourceId, FALSE, FALSE);
    auto pParticle = ae_new<Particle>(xpos, ypos, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);

    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->field_20_animation.mRenderMode = TPageAbr::eBlend_1;
    pParticle->field_CC_sprite_scale = FP_FromRaw(scale.fpValue * 2);

    if (scale == FP_FromInteger(1))
    {
        pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Above_FG1_39;
    }
    else
    {
        pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    pParticle->mApplyShadows &= ~1u;

    return pParticle;
}

Particle* New_Orb_Particle(FP xpos, FP ypos, FP velX, FP velY, FP scale, Layer layer, u8 r, u8 b, u8 g)
{
    const AnimRecord& orbRec = AnimRec(AnimId::ChantOrb_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, orbRec.mResourceId, 0, 0);
    auto pParticle = ae_new<Particle>(xpos, ypos, orbRec.mFrameTableOffset, orbRec.mMaxW, orbRec.mMaxH, ppRes);
    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->field_D0_r = r;
    pParticle->field_D4_b = g;
    pParticle->field_D2_g = b;

    pParticle->field_C8_vely = velY;
    pParticle->field_C4_velx = velX;

    pParticle->mApplyShadows &= ~1u;

    pParticle->field_20_animation.mRenderMode = TPageAbr::eBlend_1;

    if (layer != Layer::eLayer_0)
    {
        pParticle->field_20_animation.mRenderLayer = layer;
    }
    else
    {
        if (scale == FP_FromInteger(1))
        {
            pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
    }

    pParticle->field_CC_sprite_scale = scale;

    return pParticle;
}

Particle* New_TintChant_Particle(FP xpos, FP ypos, FP scale, Layer layer)
{
    return New_Orb_Particle(xpos, ypos, FP_FromInteger(0), FP_FromInteger(0), scale, layer, 128u, 128u, 128u);
}

// Fart/dust cloud particle spawner
void New_Smoke_Particles(FP xpos, FP ypos, FP scale, s16 count, u8 r, u8 g, u8 b)
{
    FP velYCounter = {};
    for (s32 i = 0; i < count; i++)
    {
        FP randX = (FP_FromInteger(Math_RandomRange(-3, 3)) * scale) + xpos;
        FP particleY = (FP_FromInteger(6 * (i + 1) / 2 * (1 - 2 * (i % 2))) * scale) + ypos;
        const AnimRecord& squibSmokeRec = AnimRec(AnimId::SquibSmoke_Particle);
        u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, squibSmokeRec.mResourceId, 0, 0);
        auto pParticle = ae_new<Particle>(randX, particleY, squibSmokeRec.mFrameTableOffset, squibSmokeRec.mMaxW, squibSmokeRec.mMaxH, ppRes);
        if (pParticle)
        {
            pParticle->mApplyShadows &= ~1u;
            pParticle->field_20_animation.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
            pParticle->field_20_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->field_20_animation.mRenderMode = TPageAbr::eBlend_3;

            pParticle->field_D0_r = r;
            pParticle->field_D2_g = g;
            pParticle->field_D4_b = b;

            pParticle->field_C4_velx = (scale * FP_FromInteger(Math_RandomRange(-10, 10))) / FP_FromInteger(10);
            pParticle->field_C8_vely = ((scale * velYCounter) * FP_FromInteger(Math_RandomRange(50, 50))) / FP_FromInteger(100);
            pParticle->field_CC_sprite_scale = scale;

            if (scale == FP_FromInteger(1))
            {
                pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_36;
            }
            else
            {
                pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
            }

            pParticle->field_F4_scale_amount = scale * FP_FromDouble(0.03);
            pParticle->field_20_animation.field_10_frame_delay = static_cast<u16>((i + 3) / 2);
            if (Math_NextRandom() < 127)
            {
                pParticle->field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
        }
        velYCounter -= FP_FromInteger(1);
    }
}

void New_RandomizedChant_Particle(BaseAliveGameObject* pObj)
{
    const FP ypos = pObj->mBaseAnimatedWithPhysicsGameObject_YPos - (pObj->field_CC_sprite_scale * FP_FromInteger(Math_RandomRange(30, 60)));
    const FP xpos = (pObj->field_CC_sprite_scale * FP_FromInteger(Math_RandomRange(-20, 20))) + pObj->mBaseAnimatedWithPhysicsGameObject_XPos;
    New_TintChant_Particle(xpos, ypos, pObj->field_CC_sprite_scale, Layer::eLayer_0);
}

Particle* New_TintShiny_Particle(FP xpos, FP ypos, FP scale, Layer layer)
{
    return New_Orb_Particle(xpos, ypos, FP_FromInteger(0), FP_FromInteger(0), scale, layer, 100u, 100u, 100u);
}

void New_ShootingZFire_Particle(FP xpos, FP ypos, FP scale)
{
    const AnimRecord& ZFireRec = AnimRec(AnimId::ShootingZFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ZFireRec.mResourceId, 0, 0);
    auto pParticle = ae_new<Particle>(xpos, ypos, ZFireRec.mFrameTableOffset, ZFireRec.mMaxW, ZFireRec.mMaxH, ppRes);
    if (pParticle)
    {
        pParticle->mApplyShadows &= ~1u;
        pParticle->field_D4_b = 55;
        pParticle->field_D2_g = 55;
        pParticle->field_D0_r = 55;
        pParticle->field_20_animation.mRenderMode = TPageAbr::eBlend_1;
        if (scale == FP_FromInteger(1))
        {
            pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
        pParticle->field_CC_sprite_scale = scale;
    }
}

void New_ShootingFire_Particle(FP xpos, FP ypos, s8 direction, FP scale)
{
    const AnimRecord& shootingFireRec = AnimRec(AnimId::ShootingFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, shootingFireRec.mResourceId, 0, 0);
    auto pParticle = ae_new<Particle>(xpos, ypos, shootingFireRec.mFrameTableOffset, shootingFireRec.mMaxW, shootingFireRec.mMaxH, ppRes);
    if (pParticle)
    {
        pParticle->mApplyShadows &= ~1u;
        pParticle->field_D4_b = 55;
        pParticle->field_D2_g = 55;
        pParticle->field_D0_r = 55;
        pParticle->field_20_animation.mRenderMode = TPageAbr::eBlend_1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->field_20_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, direction & 1);
        pParticle->field_CC_sprite_scale = scale;
    }
}
