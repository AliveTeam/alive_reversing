#include "stdafx.h"
#include "Particle.hpp"
//#include "Function.hpp"
#include "ResourceManagerWrapper.hpp"
//#include "stdlib.hpp"
//#include "BaseAliveGameObject.hpp"
#include "GameType.hpp"

// TODO: Make this the only ctor
Particle::Particle(FP xpos, FP ypos, AnimResource& res, bool explosionSizeHack)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eParticle);

    //mAnims.push_back(res); // TODO: Might need this 

    mRGB.SetRGB(128, 128, 128);

    if (!explosionSizeHack)
    {
        Animation_Init(res);
    }
    else
    {
        // TODO: Reimplement whatever this hack did in a sane way

        // AnimId::Explosion_Small and AnimId::AirExplosion have different width/height but for some reason
        // OG inits both with the AnimId::AirExplosion width and height in AirExplosion.cpp
        // TODO
        //Animation_Init(res, 202, 91);

         Animation_Init(res); // Temp fix - don't crush
    }

    if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mXPos = xpos;
    mYPos = ypos;
    field_F4_scale_amount = FP_FromInteger(0);
}

Particle::~Particle()
{

}

void Particle::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;

    mSpriteScale += field_F4_scale_amount;

    if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

Particle* New_DestroyOrCreateObject_Particle(FP xpos, FP ypos, FP scale)
{
    AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2);

    auto pParticle = relive_new Particle(xpos, ypos, ppRes);

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

    pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);

    return pParticle;
}

// Fart/dust cloud particle spawner
void New_Smoke_Particles(FP xpos, FP ypos, FP scale, s16 count, u8 r, u8 g, u8 b)
{
    FP velYCounter = {};
    for (s32 i = 0; i < count; i++)
    {
        FP randX = (FP_FromInteger(Math_RandomRange(-3, 3)) * scale) + xpos;
        FP particleY = (FP_FromInteger(6 * (i + 1) / 2 * (1 - 2 * (i % 2))) * scale) + ypos;
        AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::SquibSmoke_Particle);
        auto pParticle = relive_new Particle(randX, particleY, ppRes);
        if (pParticle)
        {
            pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
            pParticle->mAnim.mFlags.Clear(AnimFlags::eBlending);
            pParticle->mAnim.mFlags.Set(AnimFlags::eSemiTrans);
            pParticle->mAnim.mRenderMode = TPageAbr::eBlend_3;

            pParticle->mRGB.SetRGB(r, g, b);

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
            pParticle->mAnim.mFrameDelay = static_cast<u16>((i + 3) / 2);
            if (Math_NextRandom() < 127)
            {
                pParticle->mAnim.mFlags.Set(AnimFlags::eFlipX);
            }
        }
        velYCounter -= FP_FromInteger(1);
    }
}

Particle* New_Orb_Particle(FP xpos, FP ypos, FP velX, FP velY, FP scale, Layer layer, u8 r, u8 b, u8 g)
{
    AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle);
    auto pParticle = relive_new Particle(xpos, ypos, ppRes);
    if (pParticle)
    {
        pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
        pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;

        pParticle->mRGB.SetRGB(r, g, b);

        pParticle->mVelY = velY;
        pParticle->mVelX = velX;

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
    return pParticle;
}

Particle* New_TintShiny_Particle(FP xpos, FP ypos, FP scale, Layer layer)
{
    return New_Orb_Particle(xpos, ypos, FP_FromInteger(0), FP_FromInteger(0), scale, layer, 100u, 100u, 100u);
}

Particle* New_TintChant_Particle(FP xpos, FP ypos, FP scale, Layer layer)
{
    return New_Orb_Particle(xpos, ypos, FP_FromInteger(0), FP_FromInteger(0), scale, layer, 128u, 128u, 128u);
}

void New_RandomizedChant_Particle(BaseAnimatedWithPhysicsGameObject* pObj)
{
	if (GetGameType() == GameType::eAe)
	{
	    const FP xpos = (pObj->mSpriteScale * FP_FromInteger(Math_RandomRange(-20, 20))) + pObj->mXPos;
	    const FP ypos = pObj->mYPos - (pObj->mSpriteScale * FP_FromInteger(Math_RandomRange(30, 60)));
	    New_TintChant_Particle(xpos, ypos, pObj->mSpriteScale, Layer::eLayer_0);
	}
	else
	{
	 	const auto xpos = pObj->mXPos + pObj->mSpriteScale * FP_FromInteger(40 * Math_NextRandom() / 256 - 20);
	    const auto ypos = pObj->mYPos - (pObj->mSpriteScale * FP_FromInteger(30 * Math_NextRandom() / 256 + 30));
	    New_TintChant_Particle(xpos, ypos, pObj->mSpriteScale, Layer::eLayer_0);	
	}
}

void New_ShootingZFire_Particle(FP xpos, FP ypos, FP scale)
{
    AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::ShootingZFire_Particle);
    auto pParticle = relive_new Particle(xpos, ypos, ppRes);
    if (pParticle)
    {
        pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);

        pParticle->mRGB.SetRGB(55, 55, 55);

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
    AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::ShootingFire_Particle);
    auto pParticle = relive_new Particle(xpos, ypos, ppRes);
    if (pParticle)
    {
        pParticle->mVisualFlags.Clear(BaseAnimatedWithPhysicsGameObject::VisualFlags::eApplyShadowZoneColour);
        pParticle->mRGB.SetRGB(55, 55, 55);
        pParticle->mAnim.mRenderMode = TPageAbr::eBlend_1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->mAnim.mFlags.Set(AnimFlags::eFlipX, direction & 1);
        pParticle->mSpriteScale = scale;
    }
}
