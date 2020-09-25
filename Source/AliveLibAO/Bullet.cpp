#include "stdafx_ao.h"
#include "Abe.hpp"
#include "Bullet.hpp"
#include "Collisions.hpp"
#include "Elum.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "Sfx.hpp"
#include "Spark.hpp"
#include "Sparks.hpp"

START_NS_AO

EXPORT Bullet* Bullet::ctor_409380(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, int a7, FP scale, __int16 a9)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BA328);
    field_4_typeId = Types::eBullet_10;
    field_10_type = type;
    field_18_xpos = xpos;
    field_1C_ypos = ypos;
    field_2A_path = gMap_507BA8.field_2_current_path;
    field_30_pParent = pParent;
    field_28_level = gMap_507BA8.field_0_current_level;
    field_2C_scale = scale;
    field_24 = a7;
    field_34 = a9;
    field_20 = xDist;
    field_12 = 0;
    return this;
}

BaseGameObject* Bullet::VDestructor(signed int flags)
{
    dtor_487DF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

EXPORT void Bullet::VUpdate_408E30()
{
    if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
        field_28_level,
        field_2A_path,
        field_18_xpos,
        field_1C_ypos,
        0
        ))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }
    const short volume = field_2C_scale != FP_FromDouble(0.5) ? 75 : 50;

    switch (field_10_type)
    {
        case BulletType::Type_0:
        case BulletType::Type_1:
        {
            FP hitX = {};
            FP hitY = {};
            PSX_RECT shootRect = {};
            if (field_20 > FP_FromInteger(0))
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
            BaseAliveGameObject *pShotObj = ShootObject_409400(&shootRect);
            if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_18_xpos,
                    field_1C_ypos,
                    field_20 + field_18_xpos,
                    field_1C_ypos,
                    &field_14_pLine,
                    &hitX,
                    &hitY,
                    field_2C_scale != FP_FromDouble(0.5) ? 7 : 0x70
                ))
            {
                FP distHit = {};
                FP distShot = {};
                if (pShotObj)
                {
                    distHit = FP_Abs(hitX - field_18_xpos);
                    distShot = FP_Abs(pShotObj->field_A8_xpos - field_18_xpos);
                }
                if (!pShotObj || (distShot > distHit))
                {
                    if (field_20 <= FP_FromInteger(0))
                    {
                        auto pSpark = ao_new<Spark>();
                        if (pSpark)
                        {
                            pSpark->ctor_477B70(hitX, hitY, field_2C_scale, 6u, -76, 76);
                        }
                    }
                    else
                    {
                        auto pSpark = ao_new<Spark>();
                        if (pSpark)
                        {
                            pSpark->ctor_477B70(hitX, hitY, field_2C_scale, 6u, 50, 205);
                        }
                    }
                    New_Particles_419A80(hitX, hitY, field_2C_scale, 3, 0);
                    if (Math_RandomRange_450F20(0, 100) >= 90)
                    {
                        field_6_flags.Set(BaseGameObject::eDead_Bit3);
                        return;
                    }
                    if (Math_RandomRange_450F20(0, 128) < 64)
                    {
                        field_6_flags.Set(BaseGameObject::eDead_Bit3);
                        return;
                    }
                    SFX_Play_43AD70(SoundEffect::Unknown_2, volume, 0);
                    field_6_flags.Set(BaseGameObject::eDead_Bit3);
                    return;
                }
            }
            else
            {
                if (!pShotObj)
                {
                    if (Math_RandomRange_450F20(0, 100) >= 70)
                    {
                        field_6_flags.Set(BaseGameObject::eDead_Bit3);
                        return;
                    }
                    if (Math_RandomRange_450F20(0, 128) < 64)
                    {
                        SFX_Play_43AD70(SoundEffect::Unknown_1, volume, 0);
                        field_6_flags.Set(BaseGameObject::eDead_Bit3);
                        return;
                    }
                    SFX_Play_43AD70(SoundEffect::Unknown_2, volume, 0);
                    field_6_flags.Set(BaseGameObject::eDead_Bit3);
                    return;
                }
            }
            if (pShotObj->VTakeDamage(this))
            {
                SFX_Play_43AE60(SoundEffect::AirStream_28, field_2C_scale != FP_FromDouble(0.5) ? 90 : 60, 2000, 0);
                SFX_Play_43AE60(SoundEffect::MeatBounce_43, field_2C_scale != FP_FromDouble(0.5) ? 90 : 60, Math_RandomRange_450F20(300, 700), 0);
                SFX_Play_43AE60(SoundEffect::KillEffect_64, field_2C_scale != FP_FromDouble(0.5) ? 90 : 60, Math_RandomRange_450F20(900, 1400), 0);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        case BulletType::Type_2:
        {
            FP v5 = {};
            FP v7 = {};
            FP hitX = {};
            FP hitY = {};
            PSX_RECT shootRect = {};
            if (sControlledCharacter_50767C == gElum_507680)
            {
                v5 = gElum_507680->field_B4_velx * FP_FromInteger(4);
                v7 = sActiveHero_507678->field_A8_xpos;
            }
            else
            {
                v5 = sActiveHero_507678->field_B4_velx * FP_FromInteger(2);
                if (field_18_xpos >= sActiveHero_507678->field_A8_xpos)
                {
                    v7 = sActiveHero_507678->field_A8_xpos + FP_FromInteger(field_34 << 4);
                }
                else
                {
                    v7 = sActiveHero_507678->field_A8_xpos - FP_FromInteger(field_34 << 4);
                }
            }
            shootRect.x = FP_GetExponent(v7 - v5);
            shootRect.w = shootRect.x + 2;
            shootRect.y = FP_GetExponent(sActiveHero_507678->field_AC_ypos)
                + sActiveHero_507678->field_10_anim.Get_FrameHeader_403A00(-1)->field_8_data.points[2].y //or points 3?!
                - 10;
            shootRect.h = shootRect.y + 10;
            BaseAliveGameObject *pShotObj = ShootObject_409400(&shootRect);
            if (pShotObj->VTakeDamage(this))
            {
                SFX_Play_43AE60(SoundEffect::AirStream_28, 90, 2000, 0);
                SFX_Play_43AE60(SoundEffect::MeatBounce_43, 90, Math_RandomRange_450F20(300, 700), 0);
                SFX_Play_43AE60(SoundEffect::KillEffect_64, 90, Math_RandomRange_450F20(900, 1400), 0);
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }
            if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_18_xpos,
                    field_1C_ypos,
                    v7 - v5,
                    sActiveHero_507678->field_AC_ypos + FP_FromInteger(10),
                    &field_14_pLine,
                    &hitX,
                    &hitY,
                    0x400
                ))
            {
                auto pSpark = ao_new<Spark>();
                if (pSpark)
                {
                    pSpark->ctor_477B70(hitX, hitY, FP_FromInteger(1), 9u, -31, 159);
                }
                New_Particles_419A80(hitX, hitY, FP_FromInteger(1), 3, 0);
            }
            if (Math_RandomRange_450F20(0, 128) < 64)
            {
                SFX_Play_43AD70(SoundEffect::Unknown_1, 75, 0);
            }
            else
            {
                SFX_Play_43AD70(SoundEffect::Unknown_2, 75, 0);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

BaseAliveGameObject* Bullet::ShootObject_409400(PSX_RECT* /*pRect*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

END_NS_AO
