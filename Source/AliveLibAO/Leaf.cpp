#include "stdafx_ao.h"
#include "Function.hpp"
#include "Leaf.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"
#include "Map.hpp"

namespace AO {

ALIVE_VAR(1, 0x4D148C, u8, sLeafRandIdx_4D148C, 8);

void Leaf::VUpdate()
{
    mBaseAnimatedWithPhysicsGameObject_VelY += FP_FromDouble(0.5);

    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(0.8);
    mBaseAnimatedWithPhysicsGameObject_VelY = mBaseAnimatedWithPhysicsGameObject_VelY * FP_FromDouble(0.8);

    const s32 randX = sRandomBytes_4BBE30[sLeafRandIdx_4D148C++] - 127;
    mBaseAnimatedWithPhysicsGameObject_VelX += mBaseAnimatedWithPhysicsGameObject_SpriteScale * (FP_FromInteger(randX) / FP_FromInteger(64));

    const s32 randY = sRandomBytes_4BBE30[sLeafRandIdx_4D148C++] - 127;
    mBaseAnimatedWithPhysicsGameObject_VelY += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * (FP_FromInteger(randY) / FP_FromInteger(64)));

    const FP x2 = mBaseAnimatedWithPhysicsGameObject_VelX + mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP y2 = mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions->RayCast(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        x2,
        y2,
        &pLine,
        &hitX,
        &hitY,
        17);

    // Hit the floor, die but only if in background..
    if (bCollision && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5) && pLine->field_8_type == eLineTypes::eFloor_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (field_E4_bHitSomething & 1 || !bCollision || 
        (
        (mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) || pLine->field_8_type != eLineTypes::eBackgroundFloor_4) &&
        (mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1) || pLine->field_8_type != eLineTypes::eFloor_0))
        )
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = x2;
        mBaseAnimatedWithPhysicsGameObject_YPos = y2;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

        field_E4_bHitSomething |= 1;

        mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    // Out of the camera, die
    if (!gMap.Is_Point_In_Current_Camera_4449C0(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Leaf::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

Leaf::Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale)
{
    mBaseAnimatedWithPhysicsGameObject_Red = 100;
    mBaseAnimatedWithPhysicsGameObject_Green = 100;
    mBaseAnimatedWithPhysicsGameObject_Blue = 100;

    const AnimRecord& leafRec = AO::AnimRec(AnimId::Well_Leaf);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, leafRec.mResourceId, 1, 0);
    if (!ppRes)
    {
        return;
    }

    Animation_Init_417FD0(
        leafRec.mFrameTableOffset,
        leafRec.mMaxW,
        leafRec.mMaxH,
        ppRes,
        1);

    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

    mBaseAnimatedWithPhysicsGameObject_VelX = xVel * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    mBaseAnimatedWithPhysicsGameObject_VelY = yVel * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    sLeafRandIdx_4D148C++;

    field_E4_bHitSomething &= ~1u;

    s16 randLeftVol = Math_RandomRange_450F20(19, 24);
    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.4)) // ??
    {
        randLeftVol -= 7;
    }

    const s16 randRightVol = Math_RandomRange_450F20(-900, -700);
    SFX_Play_Pitch(SoundEffect::Leaf_27, (3 * randLeftVol) / 4, randRightVol, nullptr);
}

} // namespace AO
