#include "stdafx_ao.h"
#include "Function.hpp"
#include "Sparks.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

namespace AO {

Sparks::Sparks(FP xpos, FP ypos, FP scale)
{
    mTypeId = Types::eSpark_15;

    const AnimRecord& rec = AO::AnimRec(AnimId::Zap_Sparks);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mAnim.mBlue = 80;
    mAnim.mGreen = 80;
    mAnim.mRed = 80;

    mApplyShadows &= ~1u;

    mAnim.mRenderLayer = Layer::eLayer_FG1_37;
    mAnim.mRenderMode = TPageAbr::eBlend_1;

    mSpriteScale = scale * ((FP_FromInteger(Math_NextRandom() % 6) / FP_FromInteger(10)) + FP_FromDouble(0.2));
    field_EA_random = Math_RandomRange_450F20(0, 16);

    mXPos = xpos;
    mYPos = ypos;

    mVelX = FP_FromInteger(Math_RandomRange_450F20(-8, 8));
    mVelY = FP_FromInteger(Math_RandomRange_450F20(-6, -3));
}

void Sparks::VUpdate()
{
    if (field_EA_random > 0)
    {
        field_EA_random--;
    }

    if (field_EA_random == 0)
    {
        mAnim.Set_Animation_Data(1492, 0);
        field_EA_random = -1;
    }

    mVelY += FP_FromDouble(0.8);

    mVelX = mVelX * FP_FromDouble(0.9);
    mVelY = mVelY * FP_FromDouble(0.9);

    mVelX += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    mVelY += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    mXPos += mVelX;
    mYPos += mVelY;

    if (!gMap.Is_Point_In_Current_Camera_4449C0(
            mLvlNumber,
            mPathNumber,
            mXPos,
            mYPos,
            0))
        mGameObjectFlags.Set(BaseGameObject::eDead);
}

void Sparks::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

} // namespace AO
