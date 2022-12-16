#include "stdafx_ao.h"
#include "Abe.hpp"
#include "Bullet.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Spark.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Particle.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Elum.hpp"
#include "Math.hpp"
#include "../AliveLibAE/FixedPoint.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

Bullet::Bullet(IBaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, FP scale, s32 numberOfBullets)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eBullet);
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
        SetDead(true);
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

            IBaseAliveGameObject* pShotObj = ShootObject(&shootRect);
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
                    New_Smoke_Particles(hitX, hitY, mSpriteScale, 3, RGB16{128, 128, 128});
                    if (Math_RandomRange(0, 100) < 90 || Math_RandomRange(0, 128) >= 64)
                    {
                        SfxPlayMono(relive::SoundEffects::Bullet2, volume);
                    }
                    SetDead(true);
                    return;
                }
            }
            else
            {
                if (!pShotObj)
                {
                    if (Math_RandomRange(0, 100) < 70)
                    {
                        SfxPlayMono(relive::SoundEffects::Bullet2, volume);
                    }
                    if (Math_RandomRange(0, 128) < 64)
                    {
                        SfxPlayMono(relive::SoundEffects::Bullet1, volume);
                    }
                    SetDead(true);
                    return;
                }
            }

            if (pShotObj->VTakeDamage(this))
            {
                mSpriteScale == FP_FromInteger(1) ? PlayBulletSounds(90) : PlayBulletSounds(60);
            }
            SetDead(true);
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
            shootRect.y = static_cast<s16>(FP_GetExponent(sActiveHero->mYPos)
              
                        + sActiveHero->GetAnimation().Get_FrameHeader(-1)->mBoundMin.y // or points 3?!
                        - 10);
            shootRect.h = shootRect.y + 10;

            IBaseAliveGameObject* pShotObj = ShootObject(&shootRect);
            if (pShotObj && pShotObj->VTakeDamage(this))
            {
                PlayBulletSounds(90);
                SetDead(true);
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
                New_Smoke_Particles(hitX, hitY, FP_FromInteger(1), 3, RGB16{ 128, 128, 128 });
            }

            if (Math_RandomRange(0, 128) < 64)
            {
                SfxPlayMono(relive::SoundEffects::Bullet1, 75);
            }
            else
            {
                SfxPlayMono(relive::SoundEffects::Bullet2, 75);
            }
            SetDead(true);
        }
    }
}

bool Bullet::InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect)
{
    relive::Path_TLV* pZCover = nullptr;
    while (1)
    {
        pZCover = gMap.TLV_Get_At(
            pZCover,
            xpos,
            ypos,
            xpos,
            ypos);

        if (!pZCover)
        {
            break;
        }

        if (pZCover->mTlvType != ReliveTypes::eZSligCover)
        {
            continue;
        }

        if (objRect.x >= pZCover->mTopLeftX &&
            objRect.x <= pZCover->mBottomRightX && objRect.y >= pZCover->mTopLeftY && objRect.y <= pZCover->mBottomRightY && objRect.w >= pZCover->mTopLeftX && objRect.w <= pZCover->mBottomRightX && objRect.h >= pZCover->mTopLeftY && objRect.h <= pZCover->mBottomRightY)
        {
            return true;
        }
    }
    return false;
}

IBaseAliveGameObject* Bullet::ShootObject(PSX_RECT* pRect)
{
    if (!gBaseAliveGameObjects)
    {
        return nullptr;
    }

    IBaseAliveGameObject* pObjectToShoot = nullptr;
    for (s32 idx = 0; idx < gBaseAliveGameObjects->Size(); idx++)
    {
        IBaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(idx);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != mBulletParent)
        {
            if (pObjIter->GetAnimation().GetRender())
            {
                if ((mBulletType == BulletType::ePossessedSlig_0
                     && (pObjIter->Type() == ReliveTypes::eSlig
                         || pObjIter->Type() == ReliveTypes::eMudokon
                         || pObjIter->Type() == ReliveTypes::eAbe
                         || pObjIter->Type() == ReliveTypes::eSlog))

                    || pObjIter->Type() == ReliveTypes::eMudokon
                    || pObjIter->Type() == ReliveTypes::eAbe
                    || (pObjIter->Type() == ReliveTypes::eSlig && sControlledCharacter == pObjIter))
                {
                    const PSX_RECT bRect = pObjIter->VGetBoundingRect();
                    if (pRect->x <= bRect.w && pRect->w >= bRect.x && pRect->h >= bRect.y && pRect->y <= bRect.h)
                    {
                        if (mBulletType == BulletType::eZBullet_2 || mBulletParent->GetSpriteScale() == pObjIter->GetSpriteScale())
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
    SFX_Play_Pitch(relive::SoundEffects::AirStream, volume, 2000);
    SFX_Play_Pitch(relive::SoundEffects::MeatBounce, volume, Math_RandomRange(300, 700));
    SFX_Play_Pitch(relive::SoundEffects::KillEffect, volume, Math_RandomRange(900, 1400));
}

} // namespace AO
