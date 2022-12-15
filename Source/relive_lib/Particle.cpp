#include "stdafx.h"
#include "Particle.hpp"
#include "ResourceManagerWrapper.hpp"
#include "GameType.hpp"
#include "../AliveLibAE/FixedPoint.hpp"

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

    if (GetListAddFailed())
    {
        SetDead(true);
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

    SetSpriteScale(GetSpriteScale() + field_F4_scale_amount);

    if (GetAnimation().GetIsLastFrame())
    {
        SetDead(true);
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

    pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    pParticle->SetSpriteScale(FP_FromInteger(2) * scale);

    if (scale == FP_FromInteger(1))
    {
        pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_39);
    }
    else
    {
        pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
    }

    pParticle->SetApplyShadowZoneColour(false);

    return pParticle;
}

// Fart/dust cloud particle spawner
void New_Smoke_Particles(FP xpos, FP ypos, FP scale, s16 count, RGB16 rgb)
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
            pParticle->SetApplyShadowZoneColour(false);
            pParticle->GetAnimation().SetBlending(false);
            pParticle->GetAnimation().SetSemiTrans(true);
            pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_3);

            pParticle->mRGB = rgb;

            pParticle->mVelX = (scale * FP_FromInteger(Math_RandomRange(-10, 10))) / FP_FromInteger(10);
            pParticle->mVelY = ((scale * velYCounter) * FP_FromInteger(Math_RandomRange(50, 50))) / FP_FromInteger(100);
            pParticle->SetSpriteScale(scale);

            if (scale == FP_FromInteger(1))
            {
                pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
            }
            else
            {
                pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
            }

            pParticle->field_F4_scale_amount = scale * FP_FromDouble(0.03);
            pParticle->GetAnimation().SetFrameDelay(static_cast<u16>((i + 3) / 2));
            if (Math_NextRandom() < 127)
            {
                pParticle->GetAnimation().SetFlipX(true);
            }
        }
        velYCounter -= FP_FromInteger(1);
    }
}

Particle* New_Orb_Particle(FP xpos, FP ypos, FP velX, FP velY, FP scale, Layer layer, RGB16 rgb)
{
    AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle);
    auto pParticle = relive_new Particle(xpos, ypos, ppRes);
    if (pParticle)
    {
        pParticle->SetApplyShadowZoneColour(false);
        pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

        pParticle->mRGB = rgb;

        pParticle->mVelY = velY;
        pParticle->mVelX = velX;

        if (layer != Layer::eLayer_0)
        {
            pParticle->GetAnimation().SetRenderLayer(layer);
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
        }
        else
        {
            pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        }

        pParticle->SetSpriteScale(scale);
    }
    return pParticle;
}

Particle* New_TintShiny_Particle(FP xpos, FP ypos, FP scale, Layer layer)
{
    return New_Orb_Particle(xpos, ypos, FP_FromInteger(0), FP_FromInteger(0), scale, layer, RGB16{100, 100, 100});
}

Particle* New_TintChant_Particle(FP xpos, FP ypos, FP scale, Layer layer)
{
    return New_Orb_Particle(xpos, ypos, FP_FromInteger(0), FP_FromInteger(0), scale, layer, RGB16{128, 128, 128});
}

void New_RandomizedChant_Particle(BaseAnimatedWithPhysicsGameObject* pObj)
{
	if (GetGameType() == GameType::eAe)
	{
	    const FP xpos = (pObj->GetSpriteScale() * FP_FromInteger(Math_RandomRange(-20, 20))) + pObj->mXPos;
	    const FP ypos = pObj->mYPos - (pObj->GetSpriteScale() * FP_FromInteger(Math_RandomRange(30, 60)));
	    New_TintChant_Particle(xpos, ypos, pObj->GetSpriteScale(), Layer::eLayer_0);
	}
	else
	{
	 	const auto xpos = pObj->mXPos + pObj->GetSpriteScale() * FP_FromInteger(40 * Math_NextRandom() / 256 - 20);
	    const auto ypos = pObj->mYPos - (pObj->GetSpriteScale() * FP_FromInteger(30 * Math_NextRandom() / 256 + 30));
	    New_TintChant_Particle(xpos, ypos, pObj->GetSpriteScale(), Layer::eLayer_0);	
	}
}

void New_ShootingZFire_Particle(FP xpos, FP ypos, FP scale)
{
    AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::ShootingZFire_Particle);
    auto pParticle = relive_new Particle(xpos, ypos, ppRes);
    if (pParticle)
    {
        pParticle->SetApplyShadowZoneColour(false);

        pParticle->mRGB.SetRGB(55, 55, 55);

        pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

        if (scale == FP_FromInteger(1))
        {
            pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
        }
        else
        {
            pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        }

        pParticle->SetSpriteScale(scale);
    }
}

void New_ShootingFire_Particle(FP xpos, FP ypos, s8 direction, FP scale)
{
    AnimResource ppRes = ResourceManagerWrapper::LoadAnimation(AnimId::ShootingFire_Particle);
    auto pParticle = relive_new Particle(xpos, ypos, ppRes);
    if (pParticle)
    {
        pParticle->SetApplyShadowZoneColour(false);
        pParticle->mRGB.SetRGB(55, 55, 55);
        pParticle->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

        if (scale == FP_FromInteger(1))
        {
            pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);
        }
        else
        {
            pParticle->GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        }

        pParticle->GetAnimation().SetFlipX(direction & 1);
        pParticle->SetSpriteScale(scale);
    }
}
