#include "stdafx.h"
#include "ZapSpark.hpp"
#include "Math.hpp"
#include "../MapWrapper.hpp"
#include "../../AliveLibAE/stdlib.hpp"
#include "../FixedPoint.hpp"
#include "../GameType.hpp"

void ZapSpark::LoadAnimationsAO()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle_Small));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle));
}

void ZapSpark::LoadAnimationsAE()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AE_ZapSpark));
}

ZapSpark::ZapSpark(FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eZapSpark);

    FP spriteScaleAddition;
    if (GetGameType() == GameType::eAe)
    {
        LoadAnimationsAE();
        Animation_Init(GetAnimRes(AnimId::AE_ZapSpark));
        spriteScaleAddition = FP_FromDouble(0.7);
    }
    else
    {
        LoadAnimationsAO();
        Animation_Init(GetAnimRes(AnimId::ChantOrb_Particle_Small));
        spriteScaleAddition = FP_FromDouble(0.2);
    }

    SetApplyShadowZoneColour(false);

    GetAnimation().SetRenderLayer(Layer::eLayer_FG1_37);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);

    GetAnimation().SetRGB(80, 80, 80);

    SetSpriteScale(scale * ((FP_FromInteger(Math_NextRandom() % 6) / FP_FromInteger(10)) + spriteScaleAddition));
    mSparkTimer = Math_RandomRange(0, 16);

    mXPos = xpos;
    mYPos = ypos;

    mVelX = FP_FromInteger(Math_RandomRange(-8, 8));
    mVelY = FP_FromInteger(Math_RandomRange(-6, -3));
}

void ZapSpark::VUpdate()
{
    if (mSparkTimer > 0)
    {
        mSparkTimer--;
    }

    if (mSparkTimer == 0)
    {
        const AnimId anim = GetGameType() == GameType::eAe ? AnimId::AE_ZapSpark : AnimId::ChantOrb_Particle;
        GetAnimation().Set_Animation_Data(GetAnimRes(anim));
        mSparkTimer = -1;
    }

    mVelY += FP_FromDouble(0.8);

    mVelX *= FP_FromDouble(0.9);
    mVelY *= FP_FromDouble(0.9);

    mVelX += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    mVelY += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    mXPos += mVelX;
    mYPos += mVelY;

    if (!GetMap().Is_Point_In_Current_Camera(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos,
        0))
    {
        SetDead(true);
    }
}

void ZapSpark::VScreenChanged()
{
    SetDead(true);
}
