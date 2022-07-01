#include "stdafx_ao.h"
#include "Function.hpp"
#include "BulletShell.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Collisions.hpp"
#include "Sfx.hpp"

namespace AO {

BulletShell::BulletShell(FP xpos, FP ypos, s32 direction, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eNone;

    const AnimRecord rec = AO::AnimRec(AnimId::Bullet_Shell);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

    if (scale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
    }

    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, direction & 1);

    field_EC_hitCount = 0;

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

    if (direction)
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(Math_RandomRange(-6, -3));
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(Math_RandomRange(3, 6));
    }
    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(Math_RandomRange(-4, -1));
    field_F0_speed = FP_FromInteger(1);
}

void BulletShell::VUpdate()
{
    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    mBaseAnimatedWithPhysicsGameObject_VelY += field_F0_speed;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_VelY,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &field_E4_pLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromDouble(0.5) ? 7 : 0x70)
        == 1)
    {
        if (field_E4_pLine->mLineType == eLineTypes ::eFloor_0 ||
            field_E4_pLine->mLineType == eLineTypes::eBackgroundFloor_4)
        {
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY - FP_FromInteger(1);
            mBaseAnimatedWithPhysicsGameObject_VelY = -(mBaseAnimatedWithPhysicsGameObject_VelY * FP_FromDouble(0.3));
            mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(0.3));

            if (mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0) && mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(-1))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(-1);
            }
            else if (mBaseAnimatedWithPhysicsGameObject_VelX > FP_FromInteger(0) && mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(1))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(1);
            }

            s16 volume = 19 * (3 - field_EC_hitCount);
            if (volume <= 19)
            {
                volume = 19;
            }
            SfxPlayMono(SoundEffect::BulletShell_7, volume, 0);
            field_EC_hitCount++;
        }
    }

    if (!gMap.Is_Point_In_Current_Camera(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_EC_hitCount >= 3)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
