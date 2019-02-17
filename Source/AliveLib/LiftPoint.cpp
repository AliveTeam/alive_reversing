#include "stdafx.h"
#include "LiftPoint.hpp"
#include "Function.hpp"
#include "Rope.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "ObjectIds.hpp"
#include "Abe.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"

struct LiftPointData
{
    int field_0_maxW;
    int field_4_maxH;
    int field_8_platfom_animation_offset;
    int field_C_pulley_animation_offset;
    int field_10_frameTableOffset;
    int field_14_maxW;
    int field_18_maxH;
};
ALIVE_ASSERT_SIZEOF(LiftPointData, 0x1C);

const LiftPointData sLiftPointData_545AC8[18] =
{
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1312, 135, 17, 8664, 8700, 69, 34 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 1936, 136, 31, 4760, 4784, 47, 24 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
};

const TintEntry sLiftTints_55BF50[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 107u, 107u, 107u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};

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
        field_6_flags.Set(Options::eListAddFailed);
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

LiftPoint* LiftPoint::ctor_461030(Path_LiftPoint* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(&field_13C_pulleyAnim, 0x544290); // gVtbl_animation_2a_544290;
    SetVTable(&field_1D4_anim2, 0x544290); // gVtbl_animation_2a_544290;
    SetVTable(this, 0x545CC0); // vTbl_LiftPoint_545CC0

    field_C_objectId = tlvInfo;
    field_4_typeId = Types::eLiftPoint_78;

    pTlv->field_1_unknown = 3;

    field_27C_pTlv = sPath_dword_BB47C0->sub_4DB7C0(pTlv);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kLiftResID);
    if (pTlv->field_18_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    const LiftPointData& rData = sLiftPointData_545AC8[static_cast<DWORD>(gMap_5C3030.sCurrentLevelId_5C3030)];
    AddDynamicCollision_4971C0(
        rData.field_0_maxW,
        rData.field_4_maxH,
        static_cast<WORD>(rData.field_8_platfom_animation_offset),
        ppRes,
        pTlv,
        &gMap_5C3030,
        tlvInfo);

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 25;
    }
    else
    {
        field_20_animation.field_C_render_layer = 6;
        field_124_pCollisionLine->field_8_type = 36; // TODO: Type
    }

    SetTint_425600(sLiftTints_55BF50, gMap_5C3030.sCurrentLevelId_5C3030);

    const FP oldX = field_B8_xpos;
    MapFollowMe_408D10(TRUE);
    const short xSnapDelta = FP_GetExponent(field_B8_xpos - oldX);
    field_11C -= xSnapDelta;
    field_11E -= xSnapDelta;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);


    BYTE** ppPulleyAnim = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPulleyResID);
    const LiftPointData& rPulleyData = sLiftPointData_545AC8[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)];
    if (field_13C_pulleyAnim.Init_40A030(
        rPulleyData.field_C_pulley_animation_offset,
        gObjList_animations_5C1A24,
        this,
        static_cast<WORD>(rPulleyData.field_14_maxW),
        static_cast<WORD>(rPulleyData.field_18_maxH),
        ppPulleyAnim,
        1u,
        0,
        0))
    {
        if (pTlv->field_18_scale)
        {
            field_13C_pulleyAnim.field_C_render_layer = 6;
            field_13C_pulleyAnim.field_14_scale = field_CC_sprite_scale;
        }
        else
        {
            field_13C_pulleyAnim.field_C_render_layer = 25;
            field_13C_pulleyAnim.field_14_scale = field_CC_sprite_scale;
        }

        BYTE** ppAbeLiftRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeliftResID, TRUE, FALSE);

        field_13C_pulleyAnim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_13C_pulleyAnim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_13C_pulleyAnim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

        field_12C &= ~1u;

        field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
        field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
        field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
        field_280_flags.Clear(LiftFlags::eBit5);

        field_274_ppRes = ppAbeLiftRes;

        field_13C_pulleyAnim.field_9_g = static_cast<BYTE>(field_D2_g);
        field_13C_pulleyAnim.field_8_r = static_cast<BYTE>(field_D4_b);
        field_13C_pulleyAnim.field_A_b = static_cast<BYTE>(field_D0_r);
        field_13C_pulleyAnim.field_B_render_mode = 0;

        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);

        const FP k25 = FP_FromInteger(25);
        const FP k13 = FP_FromInteger(13);
        const FP km10 = FP_FromInteger(-10);


        auto pRope1 = alive_new<Rope>();
        if (pRope1)
        {
            pRope1->ctor_4A0A70(
                FP_GetExponent((k13 * field_CC_sprite_scale + field_B8_xpos)),
                0, // Start at the very top of the screen
                FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos),
                field_CC_sprite_scale);

            field_138_rope1_id = pRope1->field_8_object_id;
        }

        auto pRope2 = alive_new<Rope>();
        if (pRope2)
        {
            pRope2->ctor_4A0A70(
                FP_GetExponent((km10 * field_CC_sprite_scale) + field_B8_xpos),
                0, // Start at the very top of the screen
                FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos),
                field_CC_sprite_scale);
            field_134_rope2_id = pRope2->field_8_object_id;
        }

        pRope2->field_106_bottom = FP_GetExponent((k25 * field_CC_sprite_scale) + FP_FromInteger(field_124_pCollisionLine->field_0_rect.y));
        pRope1->field_106_bottom = FP_GetExponent((k25 * field_CC_sprite_scale) + FP_FromInteger(field_124_pCollisionLine->field_0_rect.y));

        const FP v28 = field_BC_ypos * FP_FromDouble(1.5);
        const FP v29 = FP_FromRaw(FP_GetExponent(v28 * field_CC_sprite_scale) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);

        pRope2->field_BC_ypos = FP_FromInteger(FP_GetExponent(v29 + (k25* field_CC_sprite_scale) + field_BC_ypos + FP_FromInteger(pRope2->field_F6_rope_length)));
        pRope1->field_BC_ypos = FP_FromInteger(FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos + FP_FromInteger(pRope1->field_F6_rope_length) - v29));

        field_280_flags.Clear(LiftFlags::eBit4);
        sub_462C80();

        field_278_lift_point_id = static_cast<char>(pTlv->field_10_id);
        field_130_lift_point_stop_type = pTlv->field_16_lift_point_stop_type;

        switch (field_130_lift_point_stop_type)
        {
        case LiftPointStopType::eTopFloor_0:
            field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
            break;

        case LiftPointStopType::eBottomFloor_1:
            field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
            break;

        case LiftPointStopType::eMiddleFloor_2:
            field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor);
            break;

        default:
        case LiftPointStopType::eMiddleLockFloor_3:  // Not used ??
        case LiftPointStopType::eStartPointOnly_4:
            field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
            break;
        }

        field_280_flags.Clear(LiftFlags::eBit7);
        field_280_flags.Set(LiftFlags::eBit6);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed);
    }
    return this;
}

BaseGameObject* LiftPoint::VDestructor(signed int flags)
{
    return vdtor_4619D0(flags);
}

void LiftPoint::VRender(int** pOrderingTable)
{
    vRender_462730(pOrderingTable);
}

void LiftPoint::VScreenChanged()
{
    vScreenChanged_463020();
}

void LiftPoint::vsub_461870()
{
    field_280_flags.Set(LiftFlags::eBit7);
}

BOOL LiftPoint::vsub_461890()
{
    return field_280_flags.Get(LiftFlags::eBit1_bTopFloor) && !(field_280_flags.Get(LiftFlags::eBit5));
}

BOOL LiftPoint::vsub_4618C0()
{
    return field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor) && !(field_280_flags.Get(LiftFlags::eBit5));
}

BOOL LiftPoint::vsub_4618F0()
{
    return field_280_flags.Get(LiftFlags::eBit3_bBottomFloor) && !(field_280_flags.Get(LiftFlags::eBit5));
}

int LiftPoint::vsub_4619B0()
{
    return field_280_flags.Get(LiftFlags::eBit5);
}

EXPORT void LiftPoint::vMove_4626A0(FP xSpeed, FP ySpeed, int /*not_used*/)
{
    field_C4_velx = xSpeed * field_CC_sprite_scale;
    field_C8_vely = ySpeed * field_CC_sprite_scale;

    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        field_12C |= 1u;
    }
}

void LiftPoint::vRender_462730(int** pOt)
{
    // Renders the pulley, lift platform and lift platform wheel

    // In the current level/map?
    if (field_C2_lvl_number == gMap_5C3030.sCurrentLevelId_5C3030 &&
        field_C0_path_number == gMap_5C3030.sCurrentPathId_5C3032)
    {
        // Within the current camera X bounds?
        PSX_Point camPos = {};
        gMap_5C3030.GetCurrentCamCoords_480680(&camPos);

        if (field_B8_xpos >= FP_FromInteger(camPos.field_0_x) && field_B8_xpos <= FP_FromInteger(camPos.field_0_x + 640))
        {
            short r = field_D0_r;
            short g = field_D2_g;
            short b = field_D4_b;

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                FP_GetExponent(field_B8_xpos),
                (bRect.h + bRect.y) / 2,
                field_D6_scale,
                &r,
                &g,
                &b);

            field_13C_pulleyAnim.field_8_r = static_cast<BYTE>(r);
            field_13C_pulleyAnim.field_9_g = static_cast<BYTE>(g);
            field_13C_pulleyAnim.field_A_b = static_cast<BYTE>(b);

            if (gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eNecrum_2 && Is_In_Current_Camera_424A70() == Map::CameraPos::eCamCurrent)
            {
                field_13C_pulleyAnim.vRender_40B820(
                    FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + (FP_FromInteger(3) * field_CC_sprite_scale)),
                    FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + (FP_FromInteger(-5) * field_CC_sprite_scale)),
                    pOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_13C_pulleyAnim.Get_Frame_Rect_409E10(&frameRect);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h,
                    pScreenManager_5BB5F4->field_3A_idx);
            }

            if (field_280_flags.Get(LiftFlags::eBit4))
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    FP_FromInteger(field_26C),
                    FP_FromInteger(field_26E),
                    0))
                {
                    r = field_D0_r;
                    g = field_D2_g;
                    b = field_D4_b;

                    ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                        field_26C,
                        field_26E,
                        field_D6_scale,
                        &r,
                        &g,
                        &b);

                    field_1D4_anim2.field_8_r = static_cast<BYTE>(r);
                    field_1D4_anim2.field_9_g = static_cast<BYTE>(g);
                    field_1D4_anim2.field_A_b = static_cast<BYTE>(b);

                    field_1D4_anim2.vRender_40B820(
                        FP_GetExponent(FP_FromInteger(field_26C) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                        FP_GetExponent(FP_FromInteger(field_26E) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                        pOt,
                        0,
                        0);

                    PSX_RECT frameRect = {};
                    field_1D4_anim2.Get_Frame_Rect_409E10(&frameRect);
                    pScreenManager_5BB5F4->InvalidateRect_40EC90(
                        frameRect.x,
                        frameRect.y,
                        frameRect.w,
                        frameRect.h,
                        pScreenManager_5BB5F4->field_3A_idx);
                }
            }

            BaseAnimatedWithPhysicsGameObject::VRender(pOt);

            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eNecrum_2 && Is_In_Current_Camera_424A70() == Map::CameraPos::eCamCurrent)
            {
                field_13C_pulleyAnim.vRender_40B820(
                    FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + (FP_FromInteger(3) * field_CC_sprite_scale)),
                    FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + (FP_FromInteger(-5) *field_CC_sprite_scale)),
                    pOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_13C_pulleyAnim.Get_Frame_Rect_409E10(&frameRect);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h,
                    pScreenManager_5BB5F4->field_3A_idx);
            }
        }
    }
}

void LiftPoint::sub_462C80()
{
    NOT_IMPLEMENTED();
}

void LiftPoint::vScreenChanged_463020()
{
    if (sActiveHero_5C1B68)
    {
        FP xd = FP_Abs(field_B8_xpos - sActiveHero_5C1B68->field_B8_xpos);
        if (xd <= FP_FromInteger(375))
        {
            if (field_274_ppRes == nullptr)
            {
                field_274_ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeliftResID, TRUE, FALSE);
            }
        }
        else
        {
            ResourceManager::FreeResource_49C330(field_274_ppRes);
            field_274_ppRes = nullptr;
        }
    }

    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

LiftPoint* LiftPoint::vdtor_4619D0(signed int flags)
{
    dtor_4624E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void LiftPoint::dtor_4624E0()
{
    SetVTable(this, 0x545CC0); // vTbl_LiftPoint_545CC0

    BaseGameObject* pRope2 = sObjectIds_5C1B70.Find_449CF0(field_134_rope2_id);
    BaseGameObject* pRope1 = sObjectIds_5C1B70.Find_449CF0(field_138_rope1_id);
    if (pRope2)
    {
        pRope2->field_6_flags.Set(BaseGameObject::eDead);
        field_134_rope2_id = -1;
    }

    if (pRope1)
    {
        pRope1->field_6_flags.Set(BaseGameObject::eDead);
        field_138_rope1_id = -1;
    }

    Path::TLV_Reset_4DB8E0(field_128_tlvInfo, -1, 0, 0);

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_CC_sprite_scale * FP_FromInteger(30)),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(FP_FromInteger(field_124_pCollisionLine->field_0_rect.y) + (field_CC_sprite_scale * FP_FromInteger(30))),
        Path_LiftPoint::kType);

    if (pTlv)
    {
        pTlv->field_1_unknown &= ~3;
    }

    field_13C_pulleyAnim.vCleanUp_40C630();

    if (field_280_flags.Get(LiftFlags::eBit4))
    {
        field_1D4_anim2.vCleanUp_40C630();
    }

    ResourceManager::FreeResource_49C330(field_274_ppRes);

    dtor_4973E0();
}
