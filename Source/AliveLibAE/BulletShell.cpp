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
        mGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        SetType(AETypes::eNone_0);
        const AnimRecord& rec = AnimRec(AnimId::Bullet_Shell);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        mSpriteScale = scale;

        if (scale == FP_FromInteger(1))
        {
            mAnim.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        mApplyShadows &= ~1u;
        mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, direction & 1);

        field_FC_hitCount = 0;

        mXPos = xpos;
        mYPos = ypos;

        if (direction)
        {
            mVelX = FP_FromInteger(Math_RandomRange(-6, -3));
        }
        else
        {
            mVelX = FP_FromInteger(Math_RandomRange(3, 6));
        }
        mVelY = FP_FromInteger(Math_RandomRange(-4, -1));
        field_100_speed = FP_FromInteger(1);
    }
}

BulletShell::~BulletShell()
{
    sShellCount_BAF7E0--;
}

void BulletShell::VUpdate()
{
    if (mGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return;
    }

    mXPos += mVelX;
    mYPos += mVelY;

    mVelY += field_100_speed;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            mXPos,
            mYPos - mVelY,
            mXPos,
            mYPos,
            &mCollisionLine,
            &hitX,
            &hitY,
            mScale != 0 ? 0x0F : 0xF0)
        == 1)
    {
        if (mCollisionLine->field_8_type == eLineTypes::eFloor_0 ||
            mCollisionLine->field_8_type == eLineTypes::eBackgroundFloor_4)
        {
            mYPos = hitY - FP_FromInteger(1);
            mVelY = -(mVelY * FP_FromDouble(0.3));
            mVelX = (mVelX * FP_FromDouble(0.3));

            if (mVelX < FP_FromInteger(0) && mVelX > FP_FromInteger(-1))
            {
                mVelX = FP_FromInteger(-1);
            }
            else if (mVelX > FP_FromInteger(0) && mVelX < FP_FromInteger(1))
            {
                mVelX = FP_FromInteger(1);
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

    if (!gMap.Is_Point_In_Current_Camera_4810D0(mLvlNumber, mPathNumber, mXPos, mYPos, 0))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_FC_hitCount >= 3)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}
