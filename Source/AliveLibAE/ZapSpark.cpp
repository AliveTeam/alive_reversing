#include "stdafx.h"
#include "ZapSpark.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "../relive_lib/FixedPoint.hpp"

void ZapSpark::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AE_ZapSpark));
}

ZapSpark::ZapSpark(FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eZapSpark);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::AE_ZapSpark));

    SetApplyShadowZoneColour(false);

    GetAnimation().SetRenderLayer(Layer::eLayer_FG1_37);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

    GetAnimation().SetRGB(80, 80, 80);

    SetSpriteScale(scale * ((FP_FromInteger(Math_NextRandom() % 6) / FP_FromInteger(10)) + FP_FromDouble(0.7)));
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
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::AE_ZapSpark));
        mSparkTimer = -1;
    }

    mVelY += FP_FromDouble(0.8);

    mVelX *= FP_FromDouble(0.9);
    mVelY *= FP_FromDouble(0.9);

    mVelX += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    mVelY += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    mXPos += mVelX;
    mYPos += mVelY;

    if (!gMap.Is_Point_In_Current_Camera(
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
