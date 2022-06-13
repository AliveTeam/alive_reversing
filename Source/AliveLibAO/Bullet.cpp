#include "stdafx_ao.h"
#include "Abe.hpp"
#include "Bullet.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Spark.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "Collisions.hpp"
#include "Elum.hpp"
#include "Math.hpp"
#include "Sparks.hpp"

namespace AO {

Bullet::Bullet(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, s32 unused, FP scale, s32 numberOfBullets)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTypeId = ReliveTypes::eBullet;
    field_10_type = type;
    field_18_xpos = xpos;
    field_1C_ypos = ypos;
    field_2A_path = gMap.mCurrentPath;
    field_30_pParent = pParent;
    field_28_level = gMap.mCurrentLevel;
    field_2C_scale = scale;
    field_24_unused = unused;
    field_34_number_of_bullets = static_cast<s16>(numberOfBullets);
    field_20_x_distance = xDist;
}

void Bullet::VUpdate()
{
    if (!gMap.Is_Point_In_Current_Camera_4449C0(
            field_28_level,
            field_2A_path,
            field_18_xpos,
            field_1C_ypos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }
    const s16 volume = field_2C_scale != FP_FromDouble(0.5) ? 75 : 50;

    switch (field_10_type)
    {
        case BulletType::ePossessedSlig_0:
        case BulletType::eNormalBullet_1:
        {
            FP hitX = {};
            FP hitY = {};
            PSX_RECT shootRect = {};
            if (field_20_x_distance > FP_FromInteger(0))
            {
                shootRect.x = FP_GetExponent(field_18_xpos);
                shootRect.w = FP_GetExponent(field_18_xpos) + 640;
            }
            else
            {
                shootRect.x = FP_GetExponent(field_18_xpos) - 640;
                shootRect.w = FP_GetExponent(field_18_xpos);
            }

            shootRect.y = FP_GetExponent(field_1C_ypos - FP_FromInteger(15));
            shootRect.h = FP_GetExponent(field_1C_ypos + FP_FromInteger(5));

            BaseAliveGameObject* pShotObj = ShootObject(&shootRect);
            if (sCollisions->Raycast(
                    field_18_xpos,
                    field_1C_ypos,
                    field_20_x_distance + field_18_xpos,
                    field_1C_ypos,
                    &field_14_pLine,
                    &hitX,
                    &hitY,
                    field_2C_scale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                FP distHit = {};
                FP distShot = {};
                if (pShotObj)
                {
                    distHit = FP_Abs(hitX - field_18_xpos);
                    distShot = FP_Abs(pShotObj->mBaseAnimatedWithPhysicsGameObject_XPos - field_18_xpos);
                }

                if (!pShotObj || (distShot > distHit))
                {
                    if (field_20_x_distance <= FP_FromInteger(0))
                    {
                        relive_new Spark(hitX, hitY, field_2C_scale, 6u, -76, 76);
                    }
                    else
                    {
                        relive_new Spark(hitX, hitY, field_2C_scale, 6u, 50, 205);
                    }
                    New_Smoke_Particles_419A80(hitX, hitY, field_2C_scale, 3, 0);
                    if (Math_RandomRange(0, 100) < 90 || Math_RandomRange(0, 128) >= 64)
                    {
                        SFX_Play_Mono(SoundEffect::Bullet2_2, volume, 0);
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
                        SFX_Play_Mono(SoundEffect::Bullet2_2, volume, 0);
                    }
                    if (Math_RandomRange(0, 128) < 64)
                    {
                        SFX_Play_Mono(SoundEffect::Bullet1_1, volume, 0);
                    }
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                    return;
                }
            }

            if (pShotObj->VTakeDamage(this))
            {
                field_2C_scale == FP_FromInteger(1) ? PlayBulletSounds(90) : PlayBulletSounds(60);
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
            if (sControlledCharacter_50767C == gElum_507680)
            {
                distX_1 = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos;
                distX_2 = gElum_507680->mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(4);
            }
            else
            {
                if (field_18_xpos >= sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos)
                {
                    distX_1 = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(field_34_number_of_bullets * 16);
                }
                else
                {
                    distX_1 = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_XPos - FP_FromInteger(field_34_number_of_bullets * 16);
                }
                distX_2 = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromInteger(2);
            }

            shootRect.x = FP_GetExponent(distX_1 - distX_2);
            shootRect.w = shootRect.x + 2;
            shootRect.y = FP_GetExponent(sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos)
                        + sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_Anim.Get_FrameHeader(-1)->field_8_data.points[2].y //or points 3?!
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
                    field_18_xpos,
                    field_1C_ypos,
                    distX_1 - distX_2,
                    sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(10),
                    &field_14_pLine,
                    &hitX,
                    &hitY,
                    0x400))
            {
                relive_new Spark(hitX, hitY, FP_FromInteger(1), 9u, -31, 159);
                New_Smoke_Particles_419A80(hitX, hitY, FP_FromInteger(1), 3, 0);
            }

            if (Math_RandomRange(0, 128) < 64)
            {
                SFX_Play_Mono(SoundEffect::Bullet1_1, 75, 0);
            }
            else
            {
                SFX_Play_Mono(SoundEffect::Bullet2_2, 75, 0);
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

        if (pZCover->field_4_type.mType != TlvTypes::ZSligCover_83)
        {
            continue;
        }

        if (objRect.x >= pZCover->field_10_top_left.field_0_x && objRect.x <= pZCover->field_14_bottom_right.field_0_x && objRect.y >= pZCover->field_10_top_left.field_2_y && objRect.y <= pZCover->field_14_bottom_right.field_2_y && objRect.w >= pZCover->field_10_top_left.field_0_x && objRect.w <= pZCover->field_14_bottom_right.field_0_x && objRect.h >= pZCover->field_10_top_left.field_2_y && objRect.h <= pZCover->field_14_bottom_right.field_2_y)
        {
            return true;
        }
    }
    return false;
}

BaseAliveGameObject* Bullet::ShootObject(PSX_RECT* pRect)
{
    if (!gBaseAliveGameObjects_4FC8A0)
    {
        return nullptr;
    }

    BaseAliveGameObject* pObjectToShoot = nullptr;
    for (s32 idx = 0; idx < gBaseAliveGameObjects_4FC8A0->Size(); idx++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(idx);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != field_30_pParent)
        {
            if (pObjIter->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                if ((field_10_type == BulletType::ePossessedSlig_0
                     && (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSlig
                         || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eMudokon
                         || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eAbe
                         || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSlog))

                    || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eMudokon
                    || pObjIter->mBaseGameObjectTypeId == ReliveTypes::eAbe
                    || (pObjIter->mBaseGameObjectTypeId == ReliveTypes::eSlig && sControlledCharacter_50767C == pObjIter))
                {
                    const PSX_RECT bRect = pObjIter->VGetBoundingRect();
                    if (pRect->x <= bRect.w && pRect->w >= bRect.x && pRect->h >= bRect.y && pRect->y <= bRect.h)
                    {
                        if (field_10_type == BulletType::eZBullet_2 || field_30_pParent->mBaseAnimatedWithPhysicsGameObject_SpriteScale == pObjIter->mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                        {
                            if (pObjectToShoot)
                            {
                                if (FP_Abs(pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos - field_18_xpos) < FP_Abs(pObjectToShoot->mBaseAnimatedWithPhysicsGameObject_XPos - field_18_xpos))
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
