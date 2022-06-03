#include "stdafx.h"
#include "Sparks.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

Sparks::Sparks(FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eSparks_22);

    const AnimRecord& rec = AnimRec(AnimId::Sparks);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mApplyShadows &= ~1u;

    mAnim.mRenderLayer = Layer::eLayer_FG1_37;
    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mAnim.mBlue = 80;
    mAnim.mGreen = 80;
    mAnim.mRed = 80;

    mSpriteScale = scale * (((FP_FromInteger(Math_NextRandom() % 6)) / FP_FromInteger(10)) + FP_FromDouble(0.7));
    field_FA_16_random = Math_RandomRange(0, 16);

    mXPos = xpos;
    mYPos = ypos;

    mVelX = FP_FromInteger(Math_RandomRange(-8, 8));
    mVelY = FP_FromInteger(Math_RandomRange(-6, -3));
}

void Sparks::VUpdate()
{
    if (field_FA_16_random > 0)
    {
        field_FA_16_random--;
    }

    if (field_FA_16_random == 0)
    {
        const AnimRecord& animRec = AnimRec(AnimId::Sparks);
        mAnim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        field_FA_16_random = -1;
    }

    mVelY += FP_FromDouble(0.8);

    mVelX *= FP_FromDouble(0.9);
    mVelY *= FP_FromDouble(0.9);

    mVelX += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    mVelY += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    mYPos += mVelY;
    mXPos += mVelX;

    if (!gMap.Is_Point_In_Current_Camera_4810D0(mLvlNumber, mPathNumber, mXPos, mYPos, 0))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Sparks::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}
