#include "stdafx_ao.h"
#include "Function.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

START_NS_AO

void PlatformBase::VAdd(BaseAliveGameObject* pObj)
{
    VAdd_4515D0(pObj);
}

void PlatformBase::VRemove(BaseAliveGameObject* pObj)
{
    VRemove_451680(pObj);
}

void PlatformBase::VRemove_451680(BaseAliveGameObject* /*pObj*/)
{
    field_114_count--;
}

void PlatformBase::AddDynamicCollision_4512C0(int /*maxW*/, int /*maxH*/, int /*frameTableOffset*/, BYTE** /*ppAnimData*/, Path_TLV* /*pTlv*/, Map*  /*pMap*/, int /*tlvInfo*/)
{
    NOT_IMPLEMENTED();
}

BaseGameObject* PlatformBase::dtor_451490()
{
    SetVTable(this, 0x4BBF30);

    ObjListPlatforms_50766C->Remove_Item(this);

    if (field_120_pCollisionLine)
    {
        if (gMap_507BA8.field_0_current_level == field_B2_lvl_number &&
            gMap_507BA8.field_2_current_path == field_B0_path_number)
        {
            Rect_Clear_40C920(&field_120_pCollisionLine->field_0_rect);
        }
    }

    return dtor_401000();
}

void PlatformBase::SyncCollisionLinePosition_451540()
{
    field_120_pCollisionLine->field_0_rect.x = FP_GetExponent(field_A8_xpos + FP_FromInteger(field_118));
    field_120_pCollisionLine->field_0_rect.w = FP_GetExponent(field_A8_xpos + FP_FromInteger(field_11A));
    field_120_pCollisionLine->field_0_rect.y = FP_GetExponent(field_AC_ypos + FP_FromInteger(field_11C));
    field_120_pCollisionLine->field_0_rect.h = FP_GetExponent(field_AC_ypos + FP_FromInteger(field_11E));
}

void PlatformBase::KeepThingsOnPlatform_451690(FP xpos)
{
    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_F8_pLiftPoint == this)
        {
            pObjIter->field_A8_xpos += xpos;
            pObjIter->field_AC_ypos = FP_FromInteger(field_120_pCollisionLine->field_0_rect.y);
        }
    }
}

BaseGameObject* PlatformBase::Vdtor_4516F0(signed int flags)
{
    dtor_451490();

    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* PlatformBase::VDestructor(signed int flags)
{
    return Vdtor_4516F0(flags);
}

void PlatformBase::VAdd_4515D0(BaseAliveGameObject* /*pObj*/)
{
    field_114_count++;
    field_110 = 1;

    if (field_120_pCollisionLine)
    {
        field_120_pCollisionLine->field_0_rect.x = FP_GetExponent(field_A8_xpos + FP_FromInteger(field_118));
        field_120_pCollisionLine->field_0_rect.w = FP_GetExponent(field_A8_xpos + FP_FromInteger(field_11A));
        field_120_pCollisionLine->field_0_rect.y = FP_GetExponent(field_AC_ypos + FP_FromInteger(field_11C));
        field_120_pCollisionLine->field_0_rect.h = FP_GetExponent(field_AC_ypos + FP_FromInteger(field_11E));
    }
}

END_NS_AO

