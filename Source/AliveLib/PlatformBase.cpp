#include "stdafx.h"
#include "PlatformBase.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

BaseGameObject* PlatformBase::VDestructor(signed int flags)
{
    return vdtor_4974B0(flags);
}

void PlatformBase::VAdd(BaseAliveGameObject* pObj)
{
    vAddCount_4975B0(pObj);
}

void PlatformBase::VRemove(BaseAliveGameObject* pObj)
{
    vRemoveCount_4975E0(pObj);
}

void PlatformBase::AddDynamicCollision_4971C0(int maxW, int maxH, unsigned __int16 frameTableOffset, BYTE** ppAnimData, Path_TLV* pTlv, Map* /*pMap*/, int tlvInfo)
{
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_128_tlvInfo = tlvInfo;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_118_count = 0;
    Animation_Init_424E10(
        maxW,
        maxH,
        frameTableOffset,
        ppAnimData,
        1,
        1);

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 25;
        field_D6_scale = 1;
    }
    else
    {
        field_20_animation.field_C_render_layer = 6;
        field_D6_scale = 0;
    }

    FrameInfoHeader* pFrameHeader = field_20_animation.Get_FrameHeader_40B730(0);
    field_BC_ypos += FP_NoFractional(FP_FromInteger(-pFrameHeader->field_8_data.points[1].y) * field_CC_sprite_scale);
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);

    field_124_pCollisionLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line_417FA0(
        pTlv->field_8_top_left.field_0_x,
        pTlv->field_8_top_left.field_2_y,
        pTlv->field_C_bottom_right.field_0_x,
        pTlv->field_8_top_left.field_2_y,
        32);

    field_11C = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - field_B8_xpos);
    field_11E = FP_GetExponent(FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - field_B8_xpos);
    field_120 = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - field_BC_ypos);
    field_122 = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_2_y) - field_BC_ypos);

    if (!ObjList_5C1B78->Push_Back(this))
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }
}

void PlatformBase::dtor_4973E0()
{
    SetVTable(this, 0x546890); // vTbl_PlatformBase_546890
    ObjList_5C1B78->Remove_Item(this);

    if (field_124_pCollisionLine)
    {
        if (gMap_5C3030.sCurrentLevelId_5C3030 == field_C2_lvl_number && gMap_5C3030.sCurrentPathId_5C3032 == field_C0_path_number)
        {
            Rect_Clear_418040(&field_124_pCollisionLine->field_0_rect);
        }
    }

    dtor_4080B0();
}

void PlatformBase::SyncCollisionLinePosition_4974E0()
{
    field_124_pCollisionLine->field_0_rect.x = FP_GetExponent(FP_FromInteger(field_11C) + field_B8_xpos);
    field_124_pCollisionLine->field_0_rect.w = FP_GetExponent(FP_FromInteger(field_11E) + field_B8_xpos);
    field_124_pCollisionLine->field_0_rect.y = FP_GetExponent(field_BC_ypos + FP_FromInteger(field_120));
    field_124_pCollisionLine->field_0_rect.h = FP_GetExponent(field_BC_ypos + FP_FromInteger(field_122));
}

void PlatformBase::vRemoveCount_4975E0(BaseAliveGameObject* /*pObj*/)
{
    --field_118_count;
}

EXPORT void PlatformBase::vAddCount_4975B0(BaseAliveGameObject* /*pObj*/)
{
    ++field_118_count;
    if (field_124_pCollisionLine)
    {
        SyncCollisionLinePosition_4974E0();
    }
}

PlatformBase* PlatformBase::vdtor_4974B0(signed int flags)
{
    dtor_4973E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}
