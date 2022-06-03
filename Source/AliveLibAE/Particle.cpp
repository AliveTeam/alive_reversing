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

    mBlue = 128;
    mGreen = 128;
    mRed = 128;

    Animation_Init(animFrameTableOffset, static_cast<s16>(maxW), static_cast<s16>(maxH), ppAnimData, 1, 1);

    if (mGameObjectFlags.Get(Options::eListAddFailed_Bit1))
    {
        mGameObjectFlags.Set(Options::eDead);
    }

    mYPos = ypos;
    mXPos = xpos;
    field_F4_scale_amount = FP_FromInteger(0);
}

void Particle::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;
    mSpriteScale += field_F4_scale_amount;

    if (mAnim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mGameObjectFlags.Set(Options::eDead);
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

    pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;
    pParticle->mSpriteScale = FP_FromRaw(scale.fpValue * 2);

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

Particle* New_Orb_Particle(FP xpos, FP ypos, FP velX, FP velY, FP scale, Layer layer, u8 r, u8 b, u8 g)
{
    const AnimRecord& orbRec = AnimRec(AnimId::ChantOrb_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, orbRec.mResourceId, 0, 0);
    auto pParticle = ae_new<Particle>(xpos, ypos, orbRec.mFrameTableOffset, orbRec.mMaxW, orbRec.mMaxH, ppRes);
    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->mRed = r;
    pParticle->mBlue = g;
    pParticle->mGreen = b;

    pParticle->mVelY = velY;
    pParticle->mVelX = velX;

    pParticle->mApplyShadows &= ~1u;

    pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;

    if (layer != Layer::eLayer_0)
    {
        pParticle->mAnim.mRenderLayer = layer;
    }
    else
    {
        if (scale == FP_FromInteger(1))
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }
    }

    pParticle->mSpriteScale = scale;

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
            pParticle->mAnim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);
            pParticle->mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->mAnim.mRenderMode = TPageAbr::eBlend_3;

            pParticle->mRed = r;
            pParticle->mGreen = g;
            pParticle->mBlue = b;

            pParticle->mVelX = (scale * FP_FromInteger(Math_RandomRange(-10, 10))) / FP_FromInteger(10);
            pParticle->mVelY = ((scale * velYCounter) * FP_FromInteger(Math_RandomRange(50, 50))) / FP_FromInteger(100);
            pParticle->mSpriteScale = scale;

            if (scale == FP_FromInteger(1))
            {
                pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
            }
            else
            {
                pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
            }

            pParticle->field_F4_scale_amount = scale * FP_FromDouble(0.03);
            pParticle->mAnim.field_10_frame_delay = static_cast<u16>((i + 3) / 2);
            if (Math_NextRandom() < 127)
            {
                pParticle->mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
        }
        velYCounter -= FP_FromInteger(1);
    }
}

void New_RandomizedChant_Particle(BaseAliveGameObject* pObj)
{
    const FP ypos = pObj->mYPos - (pObj->mSpriteScale * FP_FromInteger(Math_RandomRange(30, 60)));
    const FP xpos = (pObj->mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 20))) + pObj->mXPos;
    New_TintChant_Particle(xpos, ypos, pObj->mSpriteScale, Layer::eLayer_0);
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

void New_ShootingFire_Particle(FP xpos, FP ypos, s8 direction, FP scale)
{
    const AnimRecord& shootingFireRec = AnimRec(AnimId::ShootingFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, shootingFireRec.mResourceId, 0, 0);
    auto pParticle = ae_new<Particle>(xpos, ypos, shootingFireRec.mFrameTableOffset, shootingFireRec.mMaxW, shootingFireRec.mMaxH, ppRes);
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
