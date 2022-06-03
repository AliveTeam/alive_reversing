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
    mTypeId = Types::eNone_0;

    const AnimRecord& rec = AO::AnimRec(AnimId::Bullet_Shell);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

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

    field_EC_hitCount = 0;

    mXPos = xpos;
    mYPos = ypos;

    if (direction)
    {
        mVelX = FP_FromInteger(Math_RandomRange_450F20(-6, -3));
    }
    else
    {
        mVelX = FP_FromInteger(Math_RandomRange_450F20(3, 6));
    }
    mVelY = FP_FromInteger(Math_RandomRange_450F20(-4, -1));
    field_F0_speed = FP_FromInteger(1);
}

void BulletShell::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;

    mVelY += field_F0_speed;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast(
            mXPos,
            mYPos - mVelY,
            mXPos,
            mYPos,
            &field_E4_pLine,
            &hitX,
            &hitY,
            mSpriteScale != FP_FromDouble(0.5) ? 7 : 0x70)
        == 1)
    {
        if (field_E4_pLine->field_8_type == eLineTypes ::eFloor_0 ||
            field_E4_pLine->field_8_type == eLineTypes::eBackgroundFloor_4)
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

            s16 volume = 19 * (3 - field_EC_hitCount);
            if (volume <= 19)
            {
                volume = 19;
            }
            SFX_Play_Mono(SoundEffect::BulletShell_7, volume, 0);
            field_EC_hitCount++;
        }
    }

    if (!gMap.Is_Point_In_Current_Camera_4449C0(
            mLvlNumber,
            mPathNumber,
            mXPos,
            mYPos,
            0))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_EC_hitCount >= 3)
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
