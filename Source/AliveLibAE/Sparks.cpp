#include "stdafx.h"
#include "Sparks.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

Sparks::Sparks(FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eSparks);

    const AnimRecord& rec = AnimRec(AnimId::Sparks);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_FG1_37;
    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;

    mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = 80;
    mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = 80;
    mBaseAnimatedWithPhysicsGameObject_Anim.mRed = 80;

    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale * (((FP_FromInteger(Math_NextRandom() % 6)) / FP_FromInteger(10)) + FP_FromDouble(0.7));
    mSparkTimer = Math_RandomRange(0, 16);

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(Math_RandomRange(-8, 8));
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(Math_RandomRange(-6, -3));
}

void Sparks::VUpdate()
{
    if (mSparkTimer > 0)
    {
        mSparkTimer--;
    }

    if (mSparkTimer == 0)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(AnimId::Sparks, nullptr);
        mSparkTimer = -1;
    }

    mBaseAnimatedWithPhysicsGameObject_VelY += FP_FromDouble(0.8);

    mBaseAnimatedWithPhysicsGameObject_VelX *= FP_FromDouble(0.9);
    mBaseAnimatedWithPhysicsGameObject_VelY *= FP_FromDouble(0.9);

    mBaseAnimatedWithPhysicsGameObject_VelX += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    mBaseAnimatedWithPhysicsGameObject_VelY += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;

    if (!gMap.Is_Point_In_Current_Camera(
        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        mBaseAnimatedWithPhysicsGameObject_PathNumber,
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Sparks::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
