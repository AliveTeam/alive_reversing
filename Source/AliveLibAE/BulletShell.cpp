#include "stdafx.h"
#include "BulletShell.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Sfx.hpp"

ALIVE_VAR(1, 0xBAF7E0, s16, sShellCount_BAF7E0, 0);

BulletShell::BulletShell(FP xpos, FP ypos, s16 direction, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    sShellCount_BAF7E0++;

    if (sShellCount_BAF7E0 >= 11)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        SetType(ReliveTypes::eNone);
        const AnimRecord& rec = AnimRec(AnimId::Bullet_Shell);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

        if (scale == FP_FromInteger(1))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        mApplyShadows &= ~1u;
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, direction & 1);

        field_FC_hitCount = 0;

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
        field_100_speed = FP_FromInteger(1);
    }
}

BulletShell::~BulletShell()
{
    sShellCount_BAF7E0--;
}

void BulletShell::VUpdate()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
    mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;

    mBaseAnimatedWithPhysicsGameObject_VelY += field_100_speed;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos - mBaseAnimatedWithPhysicsGameObject_VelY,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale != 0 ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls)
        == 1)
    {
        if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eFloor_0 ||
            BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eBackgroundFloor_4)
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

            s16 volume = 19 * (3 - field_FC_hitCount);
            if (volume <= 19)
            {
                volume = 19;
            }

            SFX_Play_Mono(SoundEffect::BulletShell_6, volume);
            field_FC_hitCount++;
        }
    }

    if (!gMap.Is_Point_In_Current_Camera_4810D0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_FC_hitCount >= 3)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}
