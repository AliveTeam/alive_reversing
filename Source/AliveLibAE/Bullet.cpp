#include "stdafx.h"
#include "Abe.hpp"
#include "Bullet.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"
#include "Game.hpp"
#include "Slig.hpp"
#include "Spark.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "ScreenManager.hpp"

Bullet* Bullet::ctor_414540(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, s32 unused, FP scale, s16 numberOfBullets)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x54446C);
    SetType(AETypes::eBullet_15);
    field_20_type = type;
    field_28_xpos = xpos;
    field_2C_ypos = ypos;
    field_3A_path = gMap_5C3030.field_2_current_path;
    field_40_pParent = pParent;
    field_38_level = gMap_5C3030.field_0_current_level;
    field_3C_scale = scale;
    field_34_unused = unused;
    field_44_number_of_bullets = numberOfBullets;
    field_30_x_distance = xDist;
    field_22_unused = 0;
    return this;
}

BaseGameObject* Bullet::VDestructor(s32 flags)
{
    return vdtor_4145E0(flags);
}

Bullet* Bullet::vdtor_4145E0(s32 flags)
{
    BaseGameObject_dtor_4DBEC0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Bullet::VUpdate()
{
    vUpdate_413560();
}

void Bullet::vUpdate_413560()
{
    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_38_level, field_3A_path, field_28_xpos, field_2C_ypos, 0) && !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_38_level, field_3A_path, field_28_xpos + FP_FromInteger(10), field_2C_ypos, 0) && !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_38_level, field_3A_path, field_28_xpos - FP_FromInteger(10), field_2C_ypos, 0))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    const s16 volume = field_3C_scale != FP_FromDouble(0.5) ? 75 : 50;

    switch (field_20_type)
    {
        case BulletType::eSligPossessedOrUnderGlukkonCommand_0:
        case BulletType::eNormalBullet_2:
        {
            s32 randomW = FP_GetExponent(FP_FromInteger(Math_RandomRange_496AB0(1, 5)) * field_3C_scale);
            const FP randomHeight = FP_FromInteger(Math_RandomRange_496AB0(1, 5)) * field_3C_scale;

            PSX_RECT shootRect = {};
            if (field_30_x_distance > FP_FromInteger(0))
            {
                shootRect.x = FP_GetExponent(field_28_xpos);
                shootRect.w = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + FP_FromInteger(640));
            }
            else
            {
                shootRect.x = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
                shootRect.w = FP_GetExponent(field_28_xpos);
            }

            shootRect.y = FP_GetExponent(field_2C_ypos - FP_FromInteger(5));
            shootRect.h = FP_GetExponent(field_2C_ypos + FP_FromInteger(5)); // TODO: Check correct

            if (sControlledCharacter_5C1B8C->Type() == AETypes::eAbe_69)
            {
                shootRect.y = FP_GetExponent(field_2C_ypos - FP_FromInteger(10));
            }

            BaseAliveGameObject* pShotObj = ShootObject_414630(&shootRect);

            const s16 vol = field_3C_scale != FP_FromDouble(0.5) ? 90 : 60;

            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                    field_28_xpos,
                    field_2C_ypos - (FP_FromInteger(10) * field_3C_scale),
                    field_30_x_distance + field_28_xpos,
                    field_2C_ypos - (FP_FromInteger(10) * field_3C_scale),
                    &field_24_pLine,
                    &hitX,
                    &hitY,
                    field_3C_scale > FP_FromDouble(0.5) ? 0x0F : 0xF0)
                == 1)
            {
                if (pShotObj)
                {
                    if (FP_Abs(pShotObj->field_B8_xpos - field_28_xpos) <= FP_Abs(hitX - field_28_xpos))
                    {
                        if (pShotObj->Type() == AETypes::eMineCar_89 || pShotObj->Type() == AETypes::eGreeter_64)
                        {
                            if (pShotObj->Type() == AETypes::eGreeter_64)
                            {
                                randomW = FP_GetExponent(FP_FromInteger(randomW) + (FP_FromInteger(14) * field_3C_scale));
                            }

                            if (field_30_x_distance <= FP_FromInteger(0))
                            {
                                auto pSpark = ae_new<Spark>();
                                if (pSpark)
                                {
                                    pSpark->ctor_4CBBB0(
                                        pShotObj->field_B8_xpos + (field_3C_scale * FP_FromInteger(30)) - FP_FromInteger(randomW),
                                        field_2C_ypos + FP_NoFractional(randomHeight),
                                        field_3C_scale, 6, -76, 76, SparkType::eSmallChantParticle_0);
                                }
                                New_Smoke_Particles_426C70(
                                    pShotObj->field_B8_xpos + (field_3C_scale * FP_FromInteger(30)) - FP_FromInteger(randomW),
                                    field_2C_ypos + FP_NoFractional(randomHeight),
                                    field_3C_scale, 3, 128u, 128u, 128u);
                            }
                            else
                            {
                                auto pSpark = ae_new<Spark>();
                                if (pSpark)
                                {
                                    pSpark->ctor_4CBBB0(
                                        pShotObj->field_B8_xpos + FP_FromInteger(randomW) - (field_3C_scale * FP_FromInteger(30)),
                                        field_2C_ypos + FP_NoFractional(randomHeight),
                                        field_3C_scale, 6, 50, 205, SparkType::eSmallChantParticle_0);
                                }
                                New_Smoke_Particles_426C70(
                                    pShotObj->field_B8_xpos + FP_FromInteger(randomW) - (field_3C_scale * FP_FromInteger(30)),
                                    field_2C_ypos + FP_NoFractional(randomHeight),
                                    field_3C_scale, 3, 128u, 128u, 128u);
                            }

                            if (Math_RandomRange_496AB0(0, 100) < 90)
                            {
                                SFX_Play_46FA90(static_cast<u8>(Math_RandomRange_496AB0(SoundEffect::Bullet1_0, SoundEffect::Bullet2_1)), volume);
                            }
                        }

                        if (pShotObj->VTakeDamage_408730(this))
                        {
                            if (pShotObj->Type() != AETypes::eGreeter_64 && pShotObj->Type() != AETypes::eMineCar_89)
                            {
                                PlayBulletSounds(vol);
                            }
                        }
                        field_6_flags.Set(BaseGameObject::eDead_Bit3);
                        return;
                    }
                }

                if (field_30_x_distance <= FP_FromInteger(0))
                {
                    auto pSpark = ae_new<Spark>();
                    if (pSpark)
                    {
                        pSpark->ctor_4CBBB0(
                            hitX - (field_3C_scale * FP_FromInteger(6)),
                            (FP_FromInteger(10) * field_3C_scale) + hitY,
                            field_3C_scale, 6, -76, 76, SparkType::eSmallChantParticle_0);
                    }
                    New_Smoke_Particles_426C70(hitX - (field_3C_scale * FP_FromInteger(6)), hitY, field_3C_scale, 3, 128u, 128u, 128u);
                }
                else
                {
                    auto pSpark = ae_new<Spark>();
                    if (pSpark)
                    {
                        pSpark->ctor_4CBBB0(
                            hitX + (field_3C_scale * FP_FromInteger(7)),
                            (FP_FromInteger(10) * field_3C_scale) + hitY,
                            field_3C_scale, 6, 50, 205, SparkType::eSmallChantParticle_0);
                    }
                    New_Smoke_Particles_426C70(hitX + (field_3C_scale * FP_FromInteger(7)), hitY, field_3C_scale, 3, 128u, 128u, 128u);
                }

                if (Math_RandomRange_496AB0(0, 100) < 90)
                {
                    SFX_Play_46FA90(static_cast<u8>(Math_RandomRange_496AB0(SoundEffect::Bullet1_0, SoundEffect::Bullet2_1)), volume);
                }
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            if (pShotObj)
            {
                if (pShotObj->Type() == AETypes::eMineCar_89 || pShotObj->Type() == AETypes::eGreeter_64)
                {
                    if (pShotObj->Type() == AETypes::eGreeter_64)
                    {
                        randomW = FP_GetExponent(FP_FromInteger(randomW) + (FP_FromInteger(14) * field_3C_scale));
                    }

                    if (field_30_x_distance <= FP_FromInteger(0))
                    {
                        auto pSpark = ae_new<Spark>();
                        if (pSpark)
                        {
                            pSpark->ctor_4CBBB0(
                                (field_3C_scale * FP_FromInteger(30)) + pShotObj->field_B8_xpos - FP_FromInteger(randomW),
                                field_2C_ypos + FP_NoFractional(randomHeight),
                                field_3C_scale, 6, -76, 76, SparkType::eSmallChantParticle_0);
                        }

                        New_Smoke_Particles_426C70(
                            (field_3C_scale * FP_FromInteger(30)) + pShotObj->field_B8_xpos - FP_FromInteger(randomW),
                            field_2C_ypos + FP_NoFractional(randomHeight),
                            field_3C_scale, 3, 128u, 128u, 128u);
                    }
                    else
                    {
                        auto pSparkMem = ae_new<Spark>();
                        if (pSparkMem)
                        {
                            pSparkMem->ctor_4CBBB0(
                                FP_FromInteger(randomW) + pShotObj->field_B8_xpos - (field_3C_scale * FP_FromInteger(30)),
                                field_2C_ypos + FP_NoFractional(randomHeight),
                                field_3C_scale, 6, 50, 205, SparkType::eSmallChantParticle_0);
                        }

                        New_Smoke_Particles_426C70(
                            FP_FromInteger(randomW) + pShotObj->field_B8_xpos - (field_3C_scale * FP_FromInteger(30)),
                            field_2C_ypos + FP_NoFractional(randomHeight),
                            field_3C_scale, 3, 128u, 128u, 128u);
                    }

                    if (Math_RandomRange_496AB0(0, 100) < 90)
                    {
                        SFX_Play_46FA90(static_cast<u8>(Math_RandomRange_496AB0(SoundEffect::Bullet1_0, SoundEffect::Bullet2_1)), volume);
                    }
                }

                if (pShotObj->VTakeDamage_408730(this))
                {
                    if (pShotObj->Type() != AETypes::eGreeter_64 && pShotObj->Type() != AETypes::eMineCar_89)
                    {
                        PlayBulletSounds(vol);
                    }
                }
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            if (Math_RandomRange_496AB0(0, 100) < 70)
            {
                SFX_Play_46FA90(static_cast<u8>(Math_RandomRange_496AB0(SoundEffect::Bullet1_0, SoundEffect::Bullet2_1)), volume);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        case BulletType::ePossessedSligZBullet_1:
        {
            const s32 xSnapped = SnapToXGrid_449930(FP_FromInteger(1), FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos));
            PSX_RECT rect = {};
            rect.x = static_cast<s16>(xSnapped - 25);
            rect.w = static_cast<s16>(xSnapped - 25 + 50);
            rect.y = FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
            rect.h = static_cast<s16>(rect.y + 240);
            BaseAliveGameObject* pShootObj = ShootObject_414630(&rect);
            if (pShootObj)
            {
                if (pShootObj->VTakeDamage_408730(this))
                {
                    if (pShootObj->Type() != AETypes::eGreeter_64 && pShootObj->Type() != AETypes::eMineCar_89)
                    {
                        PlayBulletSounds(90);
                        field_6_flags.Set(BaseGameObject::eDead_Bit3);
                        return;
                    }
                }
            }

            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                    sControlledCharacter_5C1B8C->field_B8_xpos,
                    FP_FromInteger(rect.y),
                    sControlledCharacter_5C1B8C->field_B8_xpos,
                    FP_FromInteger(rect.h),
                    &field_24_pLine, &hitX, &hitY, 0x400)
                == 1)
            {
                auto pSpark = ae_new<Spark>();
                if (pSpark)
                {
                    pSpark->ctor_4CBBB0(hitX, hitY, FP_FromInteger(1), 9, -31, 159, SparkType::eSmallChantParticle_0);
                }
                New_Smoke_Particles_426C70(hitX, hitY, FP_FromInteger(1), 3, 128u, 128u, 128u);
            }
            SFX_Play_46FA90(static_cast<u8>(Math_RandomRange_496AB0(SoundEffect::Bullet1_0, SoundEffect::Bullet2_1)), 75);
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        case BulletType::eZBullet_3:
        {
            FP rectXPos = {};
            // TODO: Check field_44_number_of_bullets << 20 is FP_FromInt * 16.
            if (field_28_xpos >= sControlledCharacter_5C1B8C->field_B8_xpos)
            {
                const FP doubleVelX = (sControlledCharacter_5C1B8C->field_C4_velx * FP_FromInteger(2));
                rectXPos = (FP_FromInteger(field_44_number_of_bullets * 16)) + sControlledCharacter_5C1B8C->field_B8_xpos - doubleVelX;
            }
            else
            {
                const FP doubleVelX = (sControlledCharacter_5C1B8C->field_C4_velx * FP_FromInteger(2));
                rectXPos = sControlledCharacter_5C1B8C->field_B8_xpos - doubleVelX - (FP_FromInteger(field_44_number_of_bullets * 16));
            }

            PSX_RECT rect = {};
            sControlledCharacter_5C1B8C->vGetBoundingRect_424FD0(&rect, 1);
            rect.x = FP_GetExponent(rectXPos);
            rect.y = rect.h - 10;
            rect.w = rect.x + 2;
            rect.h = rect.h;

            BaseAliveGameObject* pShootObj = ShootObject_414630(&rect);
            if (pShootObj && pShootObj->VTakeDamage_408730(this) && pShootObj->Type() != AETypes::eGreeter_64 && pShootObj->Type() != AETypes::eMineCar_89)
            {
                PlayBulletSounds(90);
            }
            else
            {
                FP hitX = {};
                FP hitY = {};
                if (sCollisions_DArray_5C1128->Raycast_417A60(
                        field_28_xpos,
                        field_2C_ypos,
                        rectXPos,
                        sActiveHero_5C1B68->field_BC_ypos + FP_FromInteger(10),
                        &field_24_pLine, &hitX, &hitY, 1024)
                    == 1)
                {
                    auto pSpark = ae_new<Spark>();
                    if (pSpark)
                    {
                        pSpark->ctor_4CBBB0(hitX, hitY, FP_FromInteger(1), 9, -31, 159, SparkType::eSmallChantParticle_0);
                    }
                    New_Smoke_Particles_426C70(hitX, hitY, FP_FromInteger(1), 3, 128u, 128u, 128u);
                }
                SFX_Play_46FA90(static_cast<u8>(Math_RandomRange_496AB0(SoundEffect::Bullet1_0, SoundEffect::Bullet2_1)), 75);
            }
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
        }

        default:
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return;
    }
}

bool Bullet::InZBulletCover(FP xpos, FP ypos, const PSX_RECT& objRect)
{
    Path_TLV* pZCover = nullptr;
    while (1)
    {
        // Go to the next entry (or first if first call).
        pZCover = sPath_dword_BB47C0->TLV_Get_At_4DB290(
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
        if (pZCover->field_4_type == TlvTypes::ZSligCover_50)
        {
            // Within Z Cover?
            if (objRect.x >= pZCover->field_8_top_left.field_0_x && objRect.x <= pZCover->field_C_bottom_right.field_0_x && objRect.y >= pZCover->field_8_top_left.field_2_y && objRect.y <= pZCover->field_C_bottom_right.field_2_y && objRect.w >= pZCover->field_8_top_left.field_0_x && objRect.w <= pZCover->field_C_bottom_right.field_0_x && objRect.h >= pZCover->field_8_top_left.field_2_y && objRect.h <= pZCover->field_C_bottom_right.field_2_y)
            {
                return true;
            }
        }
    }
    return false;
}

BaseAliveGameObject* Bullet::ShootObject_414630(PSX_RECT* pRect)
{
    if (!gBaseAliveGameObjects_5C1B7C)
    {
        return nullptr;
    }

    BaseAliveGameObject* pObjectToShoot = nullptr;
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj != field_40_pParent)
        {
            if (pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_38_level, field_3A_path, pObj->field_B8_xpos, pObj->field_BC_ypos, 1))
                {
                    if (((field_20_type == BulletType::eSligPossessedOrUnderGlukkonCommand_0 || field_20_type == BulletType::ePossessedSligZBullet_1) && ((pObj->Type() == AETypes::eSlig_125 && pObj->field_106_current_motion != eSligMotions::M_Possess_37_4B72C0) || pObj->Type() == AETypes::eFlyingSlig_54 || pObj->Type() == AETypes::eCrawlingSlig_26 || pObj->Type() == AETypes::eGlukkon_67 || pObj->Type() == AETypes::eMudokon_110 || pObj->Type() == AETypes::eAbe_69 || pObj->Type() == AETypes::eSlog_126 || pObj->Type() == AETypes::eGreeter_64)) ||

                        pObj->Type() == AETypes::eMudokon_110 || pObj->Type() == AETypes::eAbe_69 || pObj->Type() == AETypes::eMineCar_89 || (pObj->Type() == AETypes::eSlig_125 && sControlledCharacter_5C1B8C == pObj) || (pObj->Type() == AETypes::eFlyingSlig_54 && sControlledCharacter_5C1B8C == pObj) || (pObj->Type() == AETypes::eCrawlingSlig_26 && sControlledCharacter_5C1B8C == pObj) || (pObj->Type() == AETypes::eGlukkon_67 && sControlledCharacter_5C1B8C == pObj))
                    {
                        PSX_RECT bRect = {};
                        pObj->vGetBoundingRect_424FD0(&bRect, 1);

                        if (pRect->x <= bRect.w && pRect->w >= bRect.x && pRect->h >= bRect.y && pRect->y <= bRect.h)
                        {
                            if (((field_20_type == BulletType::eZBullet_3 || field_20_type == BulletType::ePossessedSligZBullet_1) && field_40_pParent->field_D6_scale < pObj->field_D6_scale) || ((field_20_type == BulletType::eNormalBullet_2 || field_20_type == BulletType::eSligPossessedOrUnderGlukkonCommand_0) && field_40_pParent->field_D6_scale == pObj->field_D6_scale))
                            {
                                if (pObj->Type() != AETypes::eGlukkon_67 || FP_Abs(pObj->field_B8_xpos - field_28_xpos) >= ScaleToGridSize_4498B0(field_3C_scale))
                                {
                                    if (!pObjectToShoot)
                                    {
                                        pObjectToShoot = pObj;
                                    }
                                    else if (FP_Abs(pObj->field_B8_xpos - field_28_xpos) < FP_Abs(pObjectToShoot->field_B8_xpos - field_28_xpos))
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
    SFX_Play_46FBA0(SoundEffect::AirStream_23, volume, 2000);
    SFX_Play_46FBA0(SoundEffect::MeatBounce_36, volume, Math_RandomRange_496AB0(300, 700));
    SFX_Play_46FBA0(SoundEffect::KillEffect_64, volume, Math_RandomRange_496AB0(900, 1400));
}
