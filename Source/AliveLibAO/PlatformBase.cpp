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

void PlatformBase::AddDynamicCollision_4512C0(int maxW, int maxH, int frameTableOffset, BYTE** ppAnimData, Path_TLV* pTlv, Map* pMap, int tlvInfo)
{
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    field_128_tlvInfo = tlvInfo;

    field_124_pMap = pMap;
    
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);

    field_10C = 0;
    field_C8_yOffset = 0;
  
    field_110 = 0;
    field_114_count = 0;

    Animation_Init_417FD0(
        maxW,
        maxH,
        frameTableOffset,
        ppAnimData,
        1);

    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 25;
        field_C6_scale = 1;
    }
    else
    {
        field_10_anim.field_C_layer = 6;
        field_C6_scale = 0;
    }

   FrameInfoHeader* pFrameHeader = field_10_anim.Get_FrameHeader_403A00(0);
   field_AC_ypos += FP_NoFractional(FP_FromInteger(-pFrameHeader->field_8_data.points[1].y) * field_BC_sprite_scale);
   field_A8_xpos = FP_FromInteger((pTlv->field_10_top_left.field_0_x + pTlv->field_14_bottom_right.field_0_x) / 2);

    field_120_pCollisionLine = sCollisions_DArray_504C6C->Add_Dynamic_Collision_Line_40C8A0(
        pTlv->field_10_top_left.field_0_x,
        pTlv->field_10_top_left.field_2_y,
        pTlv->field_14_bottom_right.field_0_x,
        pTlv->field_10_top_left.field_2_y,
        32);

    field_118_x_offset = FP_GetExponent(FP_FromInteger(pTlv->field_10_top_left.field_0_x) - field_A8_xpos);
    field_11A_width_offset = FP_GetExponent(FP_FromInteger(pTlv->field_14_bottom_right.field_0_x) - field_A8_xpos);
    field_11C_y_offset = FP_GetExponent(FP_FromInteger(pTlv->field_10_top_left.field_2_y) - field_AC_ypos);
    field_11E_height_offset = FP_GetExponent(FP_FromInteger(pTlv->field_10_top_left.field_2_y) - field_AC_ypos);

    if (!ObjListPlatforms_50766C->Push_Back(this))
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }

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
    field_120_pCollisionLine->field_0_rect.x = FP_GetExponent(field_A8_xpos + FP_FromInteger(field_118_x_offset));
    field_120_pCollisionLine->field_0_rect.w = FP_GetExponent(field_A8_xpos + FP_FromInteger(field_11A_width_offset));
    field_120_pCollisionLine->field_0_rect.y = FP_GetExponent(field_AC_ypos + FP_FromInteger(field_11C_y_offset));
    field_120_pCollisionLine->field_0_rect.h = FP_GetExponent(field_AC_ypos + FP_FromInteger(field_11E_height_offset));
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
        field_120_pCollisionLine->field_0_rect.x = FP_GetExponent(field_A8_xpos + FP_FromInteger(field_118_x_offset));
        field_120_pCollisionLine->field_0_rect.w = FP_GetExponent(field_A8_xpos + FP_FromInteger(field_11A_width_offset));
        field_120_pCollisionLine->field_0_rect.y = FP_GetExponent(field_AC_ypos + FP_FromInteger(field_11C_y_offset));
        field_120_pCollisionLine->field_0_rect.h = FP_GetExponent(field_AC_ypos + FP_FromInteger(field_11E_height_offset));
    }
}

END_NS_AO

