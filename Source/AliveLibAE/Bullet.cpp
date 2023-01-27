#include "stdafx.h"
#include "Abe.hpp"
#include "Bullet.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Slig.hpp"
#include "Spark.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Grid.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Math.hpp"
#include "Path.hpp"

Bullet::Bullet(IBaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, FP scale, s32 numberOfBullets)
    : BaseGameObject(true, 0),
    mBulletType(type),
    mXPos(xpos),
    mYPos(ypos),
    mBulletParent(pParent),
    mSpriteScale(scale),
    mNumberOfBullets(numberOfBullets),
    mXDistance(xDist)
{
    SetType(ReliveTypes::eBullet);
    mBulletPath = gMap.mCurrentPath;
    mBulletLevel = gMap.mCurrentLevel;
}

void Bullet::VUpdate()
{
    if (!gMap.Is_Point_In_Current_Camera(mBulletLevel, mBulletPath, mXPos, mYPos, 0) && !gMap.Is_Point_In_Current_Camera(mBulletLevel, mBulletPath, mXPos + FP_FromInteger(10), mYPos, 0) && !gMap.Is_Point_In_Current_Camera(mBulletLevel, mBulletPath, mXPos - FP_FromInteger(10), mYPos, 0))
    {
        SetDead(true);
        return;
    }

    const s16 volume = mSpriteScale != FP_FromDouble(0.5) ? 75 : 50;

    switch (mBulletType)
    {
        case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
        case BulletType::eNormalBullet_2:
        {
            s32 randomW = FP_GetExponent(FP_FromInteger(Math_RandomRange(1, 5)) * mSpriteScale);
            const FP randomHeight = FP_FromInteger(Math_RandomRange(1, 5)) * mSpriteScale;

            PSX_RECT shootRect = {};
            if (mXDistance > FP_FromInteger(0))
            {
                shootRect.x = FP_GetExponent(mXPos);
                shootRect.w = FP_GetExponent(gScreenManager->CamXPos() + FP_FromInteger(640));
            }
            else
            {
                shootRect.x = FP_GetExponent(gScreenManager->CamXPos());
                shootRect.w = FP_GetExponent(mXPos);
            }

            shootRect.y = FP_GetExponent(mYPos - FP_FromInteger(5));
            shootRect.h = FP_GetExponent(mYPos + FP_FromInteger(5)); // TODO: Check correct

            if (sControlledCharacter->Type() == ReliveTypes::eAbe)
            {
                shootRect.y = FP_GetExponent(mYPos - FP_FromInteger(10));
            }

            IBaseAliveGameObject* pShotObj = ShootObject(&shootRect);

            const s16 vol = mSpriteScale != FP_FromDouble(0.5) ? 90 : 60;

            FP hitX = {};
            FP hitY = {};
            if (gCollisions->Raycast(
                    mXPos,
                    mYPos - (FP_FromInteger(10) * mSpriteScale),
                    mXDistance + mXPos,
                    mYPos - (FP_FromInteger(10) * mSpriteScale),
                    &mLine,
                    &hitX,
                    &hitY,
                    mSpriteScale > FP_FromDouble(0.5) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls)
                == 1)
            {
                if (pShotObj)
                {
                    if (FP_Abs(pShotObj->mXPos - mXPos) <= FP_Abs(hitX - mXPos))
                    {
                        if (pShotObj->Type() == ReliveTypes::eMineCar || pShotObj->Type() == ReliveTypes::eGreeter)
                        {
                            if (pShotObj->Type() == ReliveTypes::eGreeter)
                            {
                                randomW = FP_GetExponent(FP_FromInteger(randomW) + (FP_FromInteger(14) * mSpriteScale));
                            }

                            if (mXDistance <= FP_FromInteger(0))
                            {
                                relive_new Spark(
                                    pShotObj->mXPos + (mSpriteScale * FP_FromInteger(30)) - FP_FromInteger(randomW),
                                    mYPos + FP_NoFractional(randomHeight),
                                    mSpriteScale, 6, -76, 76, SparkType::eSmallChantParticle_0);
                                New_Smoke_Particles(
                                    pShotObj->mXPos + (mSpriteScale * FP_FromInteger(30)) - FP_FromInteger(randomW),
                                    mYPos + FP_NoFractional(randomHeight),
                                    mSpriteScale, 3, RGB16{ 128, 128, 128 });
                            }
                            else
                            {
                                relive_new Spark(
                                    pShotObj->mXPos + FP_FromInteger(randomW) - (mSpriteScale * FP_FromInteger(30)),
                                    mYPos + FP_NoFractional(randomHeight),
                                    mSpriteScale, 6, 50, 205, SparkType::eSmallChantParticle_0);
                                New_Smoke_Particles(
                                    pShotObj->mXPos + FP_FromInteger(randomW) - (mSpriteScale * FP_FromInteger(30)),
                                    mYPos + FP_NoFractional(randomHeight),
                                    mSpriteScale, 3, RGB16{ 128, 128, 128 });
                            }

                            if (Math_RandomRange(0, 100) < 90)
                            {
                                SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Bullet1, relive::SoundEffects::Bullet2), volume);
                            }
                        }

                        if (pShotObj->VTakeDamage(this))
                        {
                            if (pShotObj->Type() != ReliveTypes::eGreeter && pShotObj->Type() != ReliveTypes::eMineCar)
                            {
                                PlayBulletSounds(vol);
                            }
                        }
                        SetDead(true);
                        return;
                    }
                }

                if (mXDistance <= FP_FromInteger(0))
                {
                    relive_new Spark(
                        hitX - (mSpriteScale * FP_FromInteger(6)),
                        (FP_FromInteger(10) * mSpriteScale) + hitY,
                        mSpriteScale, 6, -76, 76, SparkType::eSmallChantParticle_0);
                    New_Smoke_Particles(hitX - (mSpriteScale * FP_FromInteger(6)), hitY, mSpriteScale, 3, RGB16{ 128, 128, 128 });
                }
                else
                {
                    relive_new Spark(
                        hitX + (mSpriteScale * FP_FromInteger(7)),
                        (FP_FromInteger(10) * mSpriteScale) + hitY,
                        mSpriteScale, 6, 50, 205, SparkType::eSmallChantParticle_0);
                    New_Smoke_Particles(hitX + (mSpriteScale * FP_FromInteger(7)), hitY, mSpriteScale, 3, RGB16{ 128, 128, 128 });
                }

                if (Math_RandomRange(0, 100) < 90)
                {
                    SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Bullet1, relive::SoundEffects::Bullet2), volume);
                }
                SetDead(true);
                return;
            }

            if (pShotObj)
            {
                if (pShotObj->Type() == ReliveTypes::eMineCar || pShotObj->Type() == ReliveTypes::eGreeter)
                {
                    if (pShotObj->Type() == ReliveTypes::eGreeter)
                    {
                        randomW = FP_GetExponent(FP_FromInteger(randomW) + (FP_FromInteger(14) * mSpriteScale));
                    }

                    if (mXDistance <= FP_FromInteger(0))
                    {

                        relive_new Spark(
                            (mSpriteScale * FP_FromInteger(30)) + pShotObj->mXPos - FP_FromInteger(randomW),
                            mYPos + FP_NoFractional(randomHeight),
                            mSpriteScale, 6, -76, 76, SparkType::eSmallChantParticle_0);

                        New_Smoke_Particles(
                            (mSpriteScale * FP_FromInteger(30)) + pShotObj->mXPos - FP_FromInteger(randomW),
                            mYPos + FP_NoFractional(randomHeight),
                            mSpriteScale, 3, RGB16{ 128, 128, 128 });
                    }
                    else
                    {

                        relive_new Spark(
                            FP_FromInteger(randomW) + pShotObj->mXPos - (mSpriteScale * FP_FromInteger(30)),
                            mYPos + FP_NoFractional(randomHeight),
                            mSpriteScale, 6, 50, 205, SparkType::eSmallChantParticle_0);


                        New_Smoke_Particles(
                            FP_FromInteger(randomW) + pShotObj->mXPos - (mSpriteScale * FP_FromInteger(30)),
                            mYPos + FP_NoFractional(randomHeight),
                            mSpriteScale, 3, RGB16{ 128, 128, 128 });
                    }

                    if (Math_RandomRange(0, 100) < 90)
                    {
                        SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Bullet1, relive::SoundEffects::Bullet2), volume);
                    }
                }

                if (pShotObj->VTakeDamage(this))
                {
                    if (pShotObj->Type() != ReliveTypes::eGreeter && pShotObj->Type() != ReliveTypes::eMineCar)
                    {
                        PlayBulletSounds(vol);
                    }
                }
                SetDead(true);
                return;
            }

            if (Math_RandomRange(0, 100) < 70)
            {
                SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Bullet1, relive::SoundEffects::Bullet2), volume);
            }
            SetDead(true);
            return;
        }

        case BulletType::ePossessedSligZBullet_1:
        {
            const s32 xSnapped = SnapToXGrid(FP_FromInteger(1), FP_GetExponent(sControlledCharacter->mXPos));
            PSX_RECT rect = {};
            rect.x = static_cast<s16>(xSnapped - 25);
            rect.w = static_cast<s16>(xSnapped - 25 + 50);
            rect.y = FP_GetExponent(gScreenManager->CamYPos());
            rect.h = static_cast<s16>(rect.y + 240);
            IBaseAliveGameObject* pShootObj = ShootObject(&rect);
            if (pShootObj)
            {
                if (pShootObj->VTakeDamage(this))
                {
                    if (pShootObj->Type() != ReliveTypes::eGreeter && pShootObj->Type() != ReliveTypes::eMineCar)
                    {
                        PlayBulletSounds(90);
                        SetDead(true);
                        return;
                    }
                }
            }

            FP hitX = {};
            FP hitY = {};
            if (gCollisions->Raycast(
                    sControlledCharacter->mXPos,
                    FP_FromInteger(rect.y),
                    sControlledCharacter->mXPos,
                    FP_FromInteger(rect.h),
                    &mLine, &hitX, &hitY, CollisionMask(eBulletWall_10))
                == 1)
            {
                relive_new Spark(hitX, hitY, FP_FromInteger(1), 9, -31, 159, SparkType::eSmallChantParticle_0);
                New_Smoke_Particles(hitX, hitY, FP_FromInteger(1), 3, RGB16{ 128, 128, 128 });
            }
            SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Bullet1, relive::SoundEffects::Bullet2), 75);
            SetDead(true);
            return;
        }

        case BulletType::eZBullet_3:
        {
            FP rectXPos = {};
            // TODO: Check field_44_number_of_bullets << 20 is FP_FromInt * 16.
            if (mXPos >= sControlledCharacter->mXPos)
            {
                const FP doubleVelX = (sControlledCharacter->mVelX * FP_FromInteger(2));
                rectXPos = (FP_FromInteger(mNumberOfBullets * 16)) + sControlledCharacter->mXPos - doubleVelX;
            }
            else
            {
                const FP doubleVelX = (sControlledCharacter->mVelX * FP_FromInteger(2));
                rectXPos = sControlledCharacter->mXPos - doubleVelX - (FP_FromInteger(mNumberOfBullets * 16));
            }

            PSX_RECT rect = sControlledCharacter->VGetBoundingRect();
            rect.x = FP_GetExponent(rectXPos);
            rect.y = rect.h - 10;
            rect.w = rect.x + 2;
            rect.h = rect.h;

            IBaseAliveGameObject* pShootObj = ShootObject(&rect);
            if (pShootObj && pShootObj->VTakeDamage(this) && pShootObj->Type() != ReliveTypes::eGreeter && pShootObj->Type() != ReliveTypes::eMineCar)
            {
                PlayBulletSounds(90);
            }
            else
            {
                FP hitX = {};
                FP hitY = {};
                if (gCollisions->Raycast(
                        mXPos,
                        mYPos,
                        rectXPos,
                        sActiveHero->mYPos + FP_FromInteger(10),
                        &mLine, &hitX, &hitY, CollisionMask(eBulletWall_10))
                    == 1)
                {
                    relive_new Spark(hitX, hitY, FP_FromInteger(1), 9, -31, 159, SparkType::eSmallChantParticle_0);
                    New_Smoke_Particles(hitX, hitY, FP_FromInteger(1), 3, RGB16{ 128, 128, 128 });
                }
                SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Bullet1, relive::SoundEffects::Bullet2), 75);
            }
            SetDead(true);
            return;
        }

        default:
            SetDead(true);
            return;
    }
}

bool Bullet::InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect)
{
    relive::Path_TLV* pZCover = nullptr;
    while (1)
    {
        // Go to the next entry (or first if first call).
        pZCover = gPathInfo->TlvGetAt(
            pZCover,
            xpos,
            ypos,
            xpos,
            ypos);

        // No more TLVs? Then no Z Cover.
        if (!pZCover)
        {
            break;
        }

        // Is it a Z Cover?
        if (pZCover->mTlvType == ReliveTypes::eZSligCover)
        {
            // Within Z Cover?
            if (objRect.x >= pZCover->mTopLeftX && objRect.x <= pZCover->mBottomRightX && objRect.y >= pZCover->mTopLeftY && objRect.y <= pZCover->mBottomRightY && objRect.w >= pZCover->mTopLeftX && objRect.w <= pZCover->mBottomRightX && objRect.h >= pZCover->mTopLeftY && objRect.h <= pZCover->mBottomRightY)
            {
                return true;
            }
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
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != mBulletParent)
        {
            if (pObj->GetAnimation().GetRender())
            {
                if (gMap.Is_Point_In_Current_Camera(mBulletLevel, mBulletPath, pObj->mXPos, pObj->mYPos, 1))
                {
                    if (((mBulletType == BulletType::eSligPossessedOrUnderGlukkonCommand_0 || mBulletType == BulletType::ePossessedSligZBullet_1) && ((pObj->Type() == ReliveTypes::eSlig && pObj->mCurrentMotion != eSligMotions::Motion_37_Possess) || pObj->Type() == ReliveTypes::eFlyingSlig || pObj->Type() == ReliveTypes::eCrawlingSlig || pObj->Type() == ReliveTypes::eGlukkon || pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eAbe || pObj->Type() == ReliveTypes::eSlog || pObj->Type() == ReliveTypes::eGreeter)) ||

                        pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eAbe || pObj->Type() == ReliveTypes::eMineCar || (pObj->Type() == ReliveTypes::eSlig && sControlledCharacter == pObj) || (pObj->Type() == ReliveTypes::eFlyingSlig && sControlledCharacter == pObj) || (pObj->Type() == ReliveTypes::eCrawlingSlig && sControlledCharacter == pObj) || (pObj->Type() == ReliveTypes::eGlukkon && sControlledCharacter == pObj))
                    {
                        const PSX_RECT bRect = pObj->VGetBoundingRect();

                        if (pRect->x <= bRect.w && pRect->w >= bRect.x && pRect->h >= bRect.y && pRect->y <= bRect.h)
                        {
                            if (((mBulletType == BulletType::eZBullet_3 || mBulletType == BulletType::ePossessedSligZBullet_1) && mBulletParent->GetScale() < pObj->GetScale()) || ((mBulletType == BulletType::eNormalBullet_2 || mBulletType == BulletType::eSligPossessedOrUnderGlukkonCommand_0) && mBulletParent->GetScale() == pObj->GetScale()))
                            {
                                if (pObj->Type() != ReliveTypes::eGlukkon || FP_Abs(pObj->mXPos - mXPos) >= ScaleToGridSize(mSpriteScale))
                                {
                                    if (!pObjectToShoot)
                                    {
                                        pObjectToShoot = pObj;
                                    }
                                    else if (FP_Abs(pObj->mXPos - mXPos) < FP_Abs(pObjectToShoot->mXPos - mXPos))
                                    {
                                        pObjectToShoot = pObj;
                                    }
                                }
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
