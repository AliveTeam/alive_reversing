#include "stdafx_ao.h"
#include "Function.hpp"
#include "ZapSpark.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"

namespace AO {

void ZapSpark::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle_Small));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::ChantOrb_Particle));
}

ZapSpark::ZapSpark(FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eZapSpark);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::ChantOrb_Particle_Small));

    mAnim.mBlue = 80;
    mAnim.mGreen = 80;
    mAnim.mRed = 80;

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mAnim.mRenderLayer = Layer::eLayer_FG1_37;
    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mSpriteScale = scale * ((FP_FromInteger(Math_NextRandom() % 6) / FP_FromInteger(10)) + FP_FromDouble(0.2));
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
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::ChantOrb_Particle));
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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void ZapSpark::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

} // namespace AO
