#include "stdafx.h"
#include "Leaf.hpp"
#include "Function.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x563aa4, u8, sLeafRandIdx_563AA4, 8);

Leaf::Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mBaseAnimatedWithPhysicsGameObject_RGB.SetRGB(100, 100, 100);

    const AnimRecord& rec = AnimRec(AnimId::Well_Leaf);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

    if (scale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

    mBaseAnimatedWithPhysicsGameObject_VelX = xVel * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    mBaseAnimatedWithPhysicsGameObject_VelY = yVel * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    sLeafRandIdx_563AA4++;

    field_F4_bHitSomething &= ~1u;

    s16 randLeftVol = Math_RandomRange(19, 24);
    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5))
    {
        randLeftVol -= 7;
    }

    const s16 randRightVol = Math_RandomRange(-900, -700);
    SFX_Play_Pitch(SoundEffect::Leaf_22, (3 * randLeftVol) / 4, randRightVol);
    SetUpdateDelay(1);
}

void Leaf::VUpdate()
{
    mBaseAnimatedWithPhysicsGameObject_VelY += FP_FromDouble(0.5);

    mBaseAnimatedWithPhysicsGameObject_VelX = mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(0.8);
    mBaseAnimatedWithPhysicsGameObject_VelY = mBaseAnimatedWithPhysicsGameObject_VelY * FP_FromDouble(0.8);

    const s32 randX = (sRandomBytes_546744[sLeafRandIdx_563AA4++] - 127);
    mBaseAnimatedWithPhysicsGameObject_VelX += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * (FP_FromInteger(randX) / FP_FromInteger(64)));

    const s32 randY = (sRandomBytes_546744[sLeafRandIdx_563AA4++] - 127);
    mBaseAnimatedWithPhysicsGameObject_VelY += (mBaseAnimatedWithPhysicsGameObject_SpriteScale * (FP_FromInteger(randY) / FP_FromInteger(64)));

    const FP x2 = mBaseAnimatedWithPhysicsGameObject_VelX + mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP y2 = mBaseAnimatedWithPhysicsGameObject_VelY + mBaseAnimatedWithPhysicsGameObject_YPos;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions->Raycast(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        x2,
        y2,
        &pLine,
        &hitX,
        &hitY,
        kFgOrBgFloor);

    // Hit the floor, die but only if in background..
    if (bCollision && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(0.5) && pLine->mLineType == eLineTypes::eFloor_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (field_F4_bHitSomething & 1 || !bCollision || ((mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) || pLine->mLineType != eLineTypes::eBackgroundFloor_4) && (mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(1.0) || pLine->mLineType != eLineTypes::eFloor_0)))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = x2;
        mBaseAnimatedWithPhysicsGameObject_YPos = y2;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
        mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(0);

        field_F4_bHitSomething |= 1u;

        mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    // Out of the camera, die
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

void Leaf::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
