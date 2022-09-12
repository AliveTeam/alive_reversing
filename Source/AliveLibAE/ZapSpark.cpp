#include "stdafx.h"
#include "ZapSpark.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"

void ZapSpark::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::AE_ZapSpark));
}

ZapSpark::ZapSpark(FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eZapSpark);

    LoadAnimations();

    const AnimRecord& rec = AnimRec(AnimId::AE_ZapSpark);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::AE_ZapSpark, ppRes);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mAnim.mRenderLayer = Layer::eLayer_FG1_37;
    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mAnim.mBlue = 80;
    mAnim.mGreen = 80;
    mAnim.mRed = 80;

    mSpriteScale = scale * (((FP_FromInteger(Math_NextRandom() % 6)) / FP_FromInteger(10)) + FP_FromDouble(0.7));
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
        mAnim.Set_Animation_Data(GetAnimRes(AnimId::AE_ZapSpark));
        mSparkTimer = -1;
    }

    mVelY += FP_FromDouble(0.8);

    mVelX *= FP_FromDouble(0.9);
    mVelY *= FP_FromDouble(0.9);

    mVelX += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    mVelY += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    mYPos += mVelY;
    mXPos += mVelX;

    if (!gMap.Is_Point_In_Current_Camera(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos,
        0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ZapSpark::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
