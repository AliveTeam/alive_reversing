#include "stdafx.h"
#include "Bullet.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "Slig.hpp"

Bullet* Bullet::ctor_414540(BaseAliveGameObject* pParent, BulletType type, FP xpos, FP ypos, FP xDist, int a7, FP scale, __int16 a9)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x54446C);
    field_4_typeId = Types::eBullet_15;
    field_20_type = type;
    field_28_xpos = xpos;
    field_2C_ypos = ypos;
    field_3A_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_40_pParent = pParent;
    field_38_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_3C_scale = scale;
    field_34 = a7;
    field_44 = a9;
    field_30 = xDist;
    field_22 = 0;
    return this;
}

BaseGameObject* Bullet::VDestructor(signed int flags)
{
    return vdtor_4145E0(flags);
}

Bullet* Bullet::vdtor_4145E0(signed int flags)
{
    BaseGameObject_dtor_4DBEC0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

BaseAliveGameObject* Bullet::ShootObject_414630(PSX_RECT* pRect)
{
    if (!gBaseAliveGameObjects_5C1B7C)
    {
        return nullptr;
    }

    BaseAliveGameObject* pObjectToShoot = nullptr;
    for (int i= 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
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
                    if ((field_20_type == BulletType::Type_0 || field_20_type == BulletType::Type_1) &&
                        (pObj->field_4_typeId == Types::eSlig_125 && pObj->field_106_current_motion != eSligMotions::M_Possess_37_4B72C0 ||
                            pObj->field_4_typeId == Types::eFlyingSlig_54 ||
                            pObj->field_4_typeId == Types::eCrawlingSlig_26 ||
                            pObj->field_4_typeId == Types::eGlukkon_67 ||
                            pObj->field_4_typeId == Types::eMudokon_110 ||
                            pObj->field_4_typeId == Types::eType_Abe_69 ||
                            pObj->field_4_typeId == Types::eSlog_126 ||
                            pObj->field_4_typeId == Types::eGreeter_64) ||

                            pObj->field_4_typeId == Types::eMudokon_110||
                            pObj->field_4_typeId == Types::eType_Abe_69 ||
                            pObj->field_4_typeId == Types::eMineCar_89 ||
                            (pObj->field_4_typeId == Types::eSlig_125 && sControlledCharacter_5C1B8C == pObj) ||
                            (pObj->field_4_typeId == Types::eFlyingSlig_54 && sControlledCharacter_5C1B8C == pObj) ||
                            (pObj->field_4_typeId == Types::eCrawlingSlig_26 && sControlledCharacter_5C1B8C == pObj) ||
                            (pObj->field_4_typeId == Types::eGlukkon_67 && sControlledCharacter_5C1B8C == pObj))
                    {
                        PSX_RECT bRect = {};
                        pObj->vGetBoundingRect_424FD0(&bRect, 1);

                        if (pRect->x <= bRect.w &&
                            pRect->w >= bRect.x &&
                            pRect->h >= bRect.y &&
                            pRect->y <= bRect.h)
                        {
                            if ((field_20_type == BulletType::Type_3 || field_20_type == BulletType::Type_1) && field_40_pParent->field_D6_scale < pObj->field_D6_scale ||
                                (field_20_type == BulletType::Type_2 || field_20_type == BulletType::Type_0) && field_40_pParent->field_D6_scale == pObj->field_D6_scale)
                            {
                                if (pObj->field_4_typeId != Types::eGlukkon_67 || FP_Abs(pObj->field_B8_xpos - field_28_xpos) >= ScaleToGridSize_4498B0(field_3C_scale))
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
