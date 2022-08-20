#include "stdafx_ao.h"
#include "Abe.hpp"
#include "Bullet.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Spark.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Elum.hpp"
#include "Math.hpp"
#include "ZapSpark.hpp"

namespace AO {

Bullet::Bullet(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, FP scale, s32 numberOfBullets)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eBullet;
    mBulletType = type;
    mXPos = xpos;
    mYPos = ypos;
    mBulletPath = gMap.mCurrentPath;
    mBulletParent = pParent;
    mBulletLevel = gMap.mCurrentLevel;
    mSpriteScale = scale;
    mNumberOfBullets = static_cast<s16>(numberOfBullets);
    mXDistance = xDist;
}

void Bullet::VUpdate()
{
    if (!gMap.Is_Point_In_Current_Camera(
            mBulletLevel,
            mBulletPath,
            mXPos,
            mYPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }
    const s16 volume = mSpriteScale != FP_FromDouble(0.5) ? 75 : 50;

    switch (mBulletType)
    {
        case BulletType::ePossessedSlig_0:
        case BulletType::eNormalBullet_1:
        {
            FP hitX = {};
            FP hitY = {};
            PSX_RECT shootRect = {};
            if (mXDistance > FP_FromInteger(0))
            {
                shootRect.x = FP_GetExponent(mXPos);
                shootRect.w = FP_GetExponent(mXPos) + 640;
            }
            else
            {
                shootRect.x = FP_GetExponent(mXPos) - 640;
                shootRect.w = FP_GetExponent(mXPos);
            }

            shootRect.y = FP_GetExponent(mYPos - FP_FromInteger(15));
            shootRect.h = FP_GetExponent(mYPos + FP_FromInteger(5));

            BaseAliveGameObject* pShotObj = ShootObject(&shootRect);
            if (sCollisions->Raycast(
                    mXPos,
                    mYPos,
                    mXDistance + mXPos,
                    mYPos,
                    &mLine,
                    &hitX,
                    &hitY,
                    mSpriteScale != FP_FromDouble(0.5) ? kFgWallsOrFloor : kBgWallsOrFloor))
            {
                FP distHit = {};
                FP distShot = {};
                if (pShotObj)
                {
                    distHit = FP_Abs(hitX - mXPos);
                    distShot = FP_Abs(pShotObj->mXPos - mXPos);
                }

                if (!pShotObj || (distShot > distHit))
                {
                    if (mXDistance <= FP_FromInteger(0))
                    {
                        relive_new Spark(hitX, hitY, mSpriteScale, 6u, -76, 76);
                    }
                    else
                    {
                        relive_new Spark(hitX, hitY, mSpriteScale, 6u, 50, 205);
                    }
                    New_Smoke_Particles_419A80(hitX, hitY, mSpriteScale, 3, 0);
                    if (Math_RandomRange(0, 100) < 90 || Math_RandomRange(0, 128) >= 64)
                    {
                        SfxPlayMono(SoundEffect::Bullet2_2, volume, 0);
                    }
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    return;
                }
            }
            else
            {
                if (!pShotObj)
                {
                    if (Math_RandomRange(0, 100) < 70)
                    {
                        SfxPlayMono(SoundEffect::Bullet2_2, volume, 0);
                    }
                    if (Math_RandomRange(0, 128) < 64)
                    {
                        SfxPlayMono(SoundEffect::Bullet1_1, volume, 0);
                    }
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    return;
                }
            }

            if (pShotObj->VTakeDamage(this))
            {
                mSpriteScale == FP_FromInteger(1) ? PlayBulletSounds(90) : PlayBulletSounds(60);
            }
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            return;
        }
        case BulletType::eZBullet_2:
        {
            FP distX_2 = {};
            FP distX_1 = {};
            FP hitX = {};
            FP hitY = {};
            PSX_RECT shootRect = {};
            if (sControlledCharacter == gElum)
            {
                distX_1 = sActiveHero->mXPos;
                distX_2 = gElum->mVelX * FP_FromInteger(4);
            }
            else
            {
                if (mXPos >= sActiveHero->mXPos)
                {
                    distX_1 = sActiveHero->mXPos + FP_FromInteger(mNumberOfBullets * 16);
                }
                else
                {
                    distX_1 = sActiveHero->mXPos - FP_FromInteger(mNumberOfBullets * 16);
                }
                distX_2 = sActiveHero->mVelX * FP_FromInteger(2);
            }

            shootRect.x = FP_GetExponent(distX_1 - distX_2);
            shootRect.w = shootRect.x + 2;
            shootRect.y = FP_GetExponent(sActiveHero->mYPos)
                        + sActiveHero->mAnim.Get_FrameHeader(-1)->field_8_data.points[2].y //or points 3?!
                        - 10;
            shootRect.h = shootRect.y + 10;

            BaseAliveGameObject* pShotObj = ShootObject(&shootRect);
            if (pShotObj && pShotObj->VTakeDamage(this))
            {
                PlayBulletSounds(90);
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }

            if (sCollisions->Raycast(
                    mXPos,
                    mYPos,
                    distX_1 - distX_2,
                    sActiveHero->mYPos + FP_FromInteger(10),
                    &mLine,
                    &hitX,
                    &hitY,
                    CollisionMask(eBulletWall_10)))
            {
                relive_new Spark(hitX, hitY, FP_FromInteger(1), 9u, -31, 159);
                New_Smoke_Particles_419A80(hitX, hitY, FP_FromInteger(1), 3, 0);
            }

            if (Math_RandomRange(0, 128) < 64)
            {
                SfxPlayMono(SoundEffect::Bullet1_1, 75, 0);
            }
            else
            {
                SfxPlayMono(SoundEffect::Bullet2_2, 75, 0);
            }
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
}

bool Bullet::InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect)
{
    Path_TLV* pZCover = nullptr;
    while (1)
    {
        pZCover = gMap.TLV_Get_At_446060(
            pZCover,
            xpos,
            ypos,
            xpos,
            ypos);

        if (!pZCover)
        {
            break;
        }

        if (pZCover->mTlvType32.mType != TlvTypes::ZSligCover_83)
        {
            continue;
        }

        if (objRect.x >= pZCover->mTopLeft.x && objRect.x <= pZCover->mBottomRight.x && objRect.y >= pZCover->mTopLeft.y && objRect.y <= pZCover->mBottomRight.y && objRect.w >= pZCover->mTopLeft.x && objRect.w <= pZCover->mBottomRight.x && objRect.h >= pZCover->mTopLeft.y && objRect.h <= pZCover->mBottomRight.y)
        {
            return true;
        }
    }
    return false;
}

BaseAliveGameObject* Bullet::ShootObject(PSX_RECT* pRect)
{
    if (!gBaseAliveGameObjects)
    {
        return nullptr;
    }

    BaseAliveGameObject* pObjectToShoot = nullptr;
    for (s32 idx = 0; idx < gBaseAliveGameObjects->Size(); idx++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(idx);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != mBulletParent)
        {
            if (pObjIter->mAnim.mFlags.Get(AnimFlags::eBit3_Render))
            {
                if ((mBulletType == BulletType::ePossessedSlig_0
                     && (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSlig
                         || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eMudokon
                         || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eAbe
                         || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSlog))

                    || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eMudokon
                    || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eAbe
                    || (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSlig && sControlledCharacter == pObjIter))
                {
                    const PSX_RECT bRect = pObjIter->VGetBoundingRect();
                    if (pRect->x <= bRect.w && pRect->w >= bRect.x && pRect->h >= bRect.y && pRect->y <= bRect.h)
                    {
                        if (mBulletType == BulletType::eZBullet_2 || mBulletParent->mSpriteScale == pObjIter->mSpriteScale)
                        {
                            if (pObjectToShoot)
                            {
                                if (FP_Abs(pObjIter->mXPos - mXPos) < FP_Abs(pObjectToShoot->mXPos - mXPos))
                                {
                                    pObjectToShoot = pObjIter;
                                }
                            }
                            else
                            {
                                pObjectToShoot = pObjIter;
                            }
                        }
                    }
                }
            }
        }
    }
    return pObjectToShoot;
}

void Bullet::PlayBulletSounds(s16 volume)
{
    SFX_Play_Pitch(SoundEffect::AirStream_28, volume, 2000, 0);
    SFX_Play_Pitch(SoundEffect::MeatBounce_43, volume, Math_RandomRange(300, 700), 0);
    SFX_Play_Pitch(SoundEffect::KillEffect_78, volume, Math_RandomRange(900, 1400), 0);
}

} // namespace AO
