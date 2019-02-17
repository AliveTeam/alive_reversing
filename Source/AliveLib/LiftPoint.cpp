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
#include "Sfx.hpp"
#include "Events.hpp"

struct LiftPointData
{
    int field_0_maxW_platform;
    int field_4_maxH_platform;
    int field_8_platfom_frame_table_offset;
    int field_C_lift_wheel_frame_table_offset;
    int field_10_pulley_frame_table_offset;
    int field_14_maxW_lift_wheel_and_pulley;
    int field_18_maxH_lift_wheel_and_pulley;
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

void PlatformBase::sub_4974E0()
{
    NOT_IMPLEMENTED();
}

LiftPoint* LiftPoint::ctor_461030(Path_LiftPoint* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(&field_13C_lift_wheel, 0x544290); // gVtbl_animation_2a_544290;
    SetVTable(&field_1D4_pulley_anim, 0x544290); // gVtbl_animation_2a_544290;
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

    const LiftPointData& rPlatformData = sLiftPointData_545AC8[static_cast<DWORD>(gMap_5C3030.sCurrentLevelId_5C3030)];
    AddDynamicCollision_4971C0(
        rPlatformData.field_0_maxW_platform,
        rPlatformData.field_4_maxH_platform,
        static_cast<WORD>(rPlatformData.field_8_platfom_frame_table_offset),
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
    const LiftPointData& rLiftWheelData = sLiftPointData_545AC8[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)];
    if (field_13C_lift_wheel.Init_40A030(
        rLiftWheelData.field_C_lift_wheel_frame_table_offset,
        gObjList_animations_5C1A24,
        this,
        static_cast<WORD>(rLiftWheelData.field_14_maxW_lift_wheel_and_pulley),
        static_cast<WORD>(rLiftWheelData.field_18_maxH_lift_wheel_and_pulley),
        ppPulleyAnim,
        1u,
        0,
        0))
    {
        if (pTlv->field_18_scale)
        {
            field_13C_lift_wheel.field_C_render_layer = 6;
            field_13C_lift_wheel.field_14_scale = field_CC_sprite_scale;
        }
        else
        {
            field_13C_lift_wheel.field_C_render_layer = 25;
            field_13C_lift_wheel.field_14_scale = field_CC_sprite_scale;
        }

        BYTE** ppAbeLiftRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeliftResID, TRUE, FALSE);

        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

        field_12C &= ~1u;

        field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
        field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
        field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
        field_280_flags.Clear(LiftFlags::eBit5);

        field_274_ppRes = ppAbeLiftRes;

        field_13C_lift_wheel.field_9_g = static_cast<BYTE>(field_D2_g);
        field_13C_lift_wheel.field_8_r = static_cast<BYTE>(field_D4_b);
        field_13C_lift_wheel.field_A_b = static_cast<BYTE>(field_D0_r);
        field_13C_lift_wheel.field_B_render_mode = 0;

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

        field_280_flags.Clear(LiftFlags::eBit4_bHasPulley);
        CreatePulleyIfExists_462C80();

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

void LiftPoint::VUpdate()
{
    vUpdate_461AE0();
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

            field_13C_lift_wheel.field_8_r = static_cast<BYTE>(r);
            field_13C_lift_wheel.field_9_g = static_cast<BYTE>(g);
            field_13C_lift_wheel.field_A_b = static_cast<BYTE>(b);

            if (gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eNecrum_2 && Is_In_Current_Camera_424A70() == Map::CameraPos::eCamCurrent)
            {
                field_13C_lift_wheel.vRender_40B820(
                    FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + (FP_FromInteger(3) * field_CC_sprite_scale)),
                    FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + (FP_FromInteger(-5) * field_CC_sprite_scale)),
                    pOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_13C_lift_wheel.Get_Frame_Rect_409E10(&frameRect);
                pScreenManager_5BB5F4->InvalidateRect_40EC90(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h,
                    pScreenManager_5BB5F4->field_3A_idx);
            }

            if (field_280_flags.Get(LiftFlags::eBit4_bHasPulley))
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    FP_FromInteger(field_26C_pulley_xpos),
                    FP_FromInteger(field_26E_pulley_ypos),
                    0))
                {
                    r = field_D0_r;
                    g = field_D2_g;
                    b = field_D4_b;

                    ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                        field_26C_pulley_xpos,
                        field_26E_pulley_ypos,
                        field_D6_scale,
                        &r,
                        &g,
                        &b);

                    field_1D4_pulley_anim.field_8_r = static_cast<BYTE>(r);
                    field_1D4_pulley_anim.field_9_g = static_cast<BYTE>(g);
                    field_1D4_pulley_anim.field_A_b = static_cast<BYTE>(b);

                    field_1D4_pulley_anim.vRender_40B820(
                        FP_GetExponent(FP_FromInteger(field_26C_pulley_xpos) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                        FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                        pOt,
                        0,
                        0);

                    PSX_RECT frameRect = {};
                    field_1D4_pulley_anim.Get_Frame_Rect_409E10(&frameRect);
                    pScreenManager_5BB5F4->InvalidateRect_40EC90(
                        frameRect.x,
                        frameRect.y,
                        frameRect.w,
                        frameRect.h,
                        pScreenManager_5BB5F4->field_3A_idx);
                }
            }

            // The base animation is the actual lift/platform itself
            BaseAnimatedWithPhysicsGameObject::VRender(pOt);

            if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eNecrum_2 && Is_In_Current_Camera_424A70() == Map::CameraPos::eCamCurrent)
            {
                field_13C_lift_wheel.vRender_40B820(
                    FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + (FP_FromInteger(3) * field_CC_sprite_scale)),
                    FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + (FP_FromInteger(-5) *field_CC_sprite_scale)),
                    pOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                field_13C_lift_wheel.Get_Frame_Rect_409E10(&frameRect);
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

void LiftPoint::vUpdate_461AE0()
{
    NOT_IMPLEMENTED();

    if (field_12C & 1)
    {
        if (field_280_flags.Get(LiftFlags::eBit5))
        {
            bool v33 = field_270 == field_BC_ypos;
            FP v44 = field_270 - field_BC_ypos;
            FP v32 = v44;

            FP v45 = {};
            FP* v34 = nullptr;
            if (v44 < FP_FromInteger(0) || v33)
            {
                v45 = -v44;
                v34 = &v45;
            }
            else
            {
                v34 = &v44;
            }

            FP v35 = *v34;

            if (v35 >= FP_FromInteger(2) * field_CC_sprite_scale)
            {
                if (v32 >= FP_FromInteger(0))
                {
                    field_C8_vely = FP_FromInteger(2) * field_CC_sprite_scale;
                }
                else
                {
                    field_C8_vely = -(FP_FromInteger(2) * field_CC_sprite_scale);
                }
                field_12C |= 1u;
            }
            else
            {
                field_BC_ypos = field_270;
                field_C8_vely = FP_FromInteger(0);
                field_12C &= ~1u;
                field_280_flags.Clear(LiftFlags::eBit5);
                SFX_Play_46FA90(0x1Eu, 0, 0x10000);
                SFX_Play_46FBA0(0x1Eu, 80, -2000, 0x10000);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
        }
        else
        {
            field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
            const FP lineY = FP_FromInteger(field_124_pCollisionLine->field_0_rect.y);
            Path_TLV* pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                nullptr,
                field_B8_xpos,
                lineY,
                field_B8_xpos,
                (field_CC_sprite_scale * FP_FromInteger(30)) + lineY);

            if (pTlvIter)
            {
                while (pTlvIter->field_4_type != Path_LiftPoint::kType)
                {
                    pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                        pTlvIter,
                        field_B8_xpos,
                        lineY,
                        field_B8_xpos,
                        lineY + (field_CC_sprite_scale *  FP_FromInteger(30)));

                    if (!pTlvIter)
                    {
                        break;
                    }
                }
            }

            Path_LiftPoint* pLiftTlv = static_cast<Path_LiftPoint*>(pTlvIter);
            if (pLiftTlv)
            {
                field_130_lift_point_stop_type = pLiftTlv->field_16_lift_point_stop_type;

                field_280_flags.Clear(LiftFlags::eBit8_bIgnoreLiftMover);
                if (pLiftTlv->field_1A_bIgnore_lift_mover & 1)
                {
                    field_280_flags.Set(LiftFlags::eBit8_bIgnoreLiftMover);
                }

                // TODO: Wtf ??
                if (pLiftTlv->field_0_flags.Raw().all != ~0xF && pLiftTlv->field_12_bstart_point)
                {
                    field_280_flags.Set(LiftFlags::eBit6);
                }
                else
                {
                    field_280_flags.Clear(LiftFlags::eBit6);
                }
            }

            if (pLiftTlv)
            {
                sub_461000(pLiftTlv);
                // TODO
                //field_270 = (pLiftTlv->field_8_top_left.field_2_y + 65535 * field_120) << 16;
            }
            else
            {
                field_280_flags.Clear(LiftFlags::eBit6);
                field_270 = FP_FromInteger(0);
                field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
            }

            const FP v10 = field_270 - field_BC_ypos;
            const FP kMinus25Scaled = field_CC_sprite_scale * FP_FromInteger(-25);
            const FP k30Scaled = field_CC_sprite_scale * FP_FromInteger(30);

            switch (field_130_lift_point_stop_type)
            {
            case LiftPointStopType::eTopFloor_0:
                if (field_C8_vely >= FP_FromInteger(0))
                {
                    if (field_C8_vely != FP_FromInteger(0) || v10 <= kMinus25Scaled || v10 >= k30Scaled)
                    {
                        pLiftTlv->field_1_unknown = 1;
                        field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
                        field_27C_pTlv = -1;
                    }
                    else
                    {
                        field_280_flags.Set(LiftFlags::eBit5);
                        field_12C |= 1u;
                        field_BC_ypos = field_270 - v10;

                        pLiftTlv->field_1_unknown = 3;

                        field_27C_pTlv = sPath_dword_BB47C0->sub_4DB7C0(pLiftTlv);
                        pLiftTlv->field_10_id = field_278_lift_point_id;
                        field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                    }
                }
                else if (field_C8_vely + lineY <= FP_FromInteger(pLiftTlv->field_8_top_left.field_2_y))
                {
                    /* TODO
                    ((void(*)(LiftPoint *, int, Path_LiftPoint *))field_0_VTbl->VRock.Rock__vnull_411490)(
                        this,
                        field_280_flags & 1,
                        pLiftTlv);
                    */
                    field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                }
                break;

            case LiftPointStopType::eBottomFloor_1:
                if (field_C8_vely <= FP_FromInteger(0))
                {
                    if (field_C8_vely != FP_FromInteger(0) || v10 <= kMinus25Scaled || v10 >= k30Scaled)
                    {
                        pLiftTlv->field_1_unknown = 1;

                        field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
                        field_27C_pTlv = -1;
                    }
                    else
                    {
                        field_12C |= 1u;
                        field_280_flags.Set(LiftFlags::eBit5);
                        field_BC_ypos = field_270 - v10;

                        pLiftTlv->field_1_unknown = 3;

                        field_27C_pTlv = sPath_dword_BB47C0->sub_4DB7C0(pLiftTlv);
                        pLiftTlv->field_10_id = field_278_lift_point_id;
                        field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                    }
                }
                else if (lineY + field_C8_vely >= FP_FromInteger(pLiftTlv->field_8_top_left.field_2_y))
                {
                    /* TODO
                    ((void(*)(LiftPoint *, int, Path_LiftPoint *))field_0_VTbl->VRock.Rock__vnull_411490)(
                        this,
                        (LOBYTE(field_280_flags) >> 2) & 1,
                        pLiftTlv);
                    */
                    field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                }
                break;

            case LiftPointStopType::eMiddleFloor_2:
                if (v10 <= kMinus25Scaled || v10 >= k30Scaled)
                {
                    pLiftTlv->field_1_unknown = 1;
                    field_27C_pTlv = -1;
                }
                else
                {
                    if (field_280_flags.Get(LiftFlags::eBit7))
                    {
                        /* TODO
                        ((void(*)(LiftPoint *, int, Path_LiftPoint *))field_0_VTbl->VRock.Rock__vnull_411490)(
                            this,
                            ((unsigned __int8)field_280_flags >> 1) & 1,
                            pLiftTlv);
                        */
                        field_280_flags.Clear(LiftFlags::eBit7);
                    }

                    if (field_C8_vely == FP_FromInteger(0))
                    {
                        field_12C |= 1u;
                        field_280_flags.Set(LiftFlags::eBit5);
                        field_BC_ypos = field_270 - v10;
                    }

                    pLiftTlv->field_1_unknown = 3;
                    field_27C_pTlv = sPath_dword_BB47C0->sub_4DB7C0(pLiftTlv);;
                    pLiftTlv->field_10_id = field_278_lift_point_id;
                    field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor);
                }
                break;

            case LiftPointStopType::eStartPointOnly_4:
                if (pLiftTlv)
                {
                    pLiftTlv->field_1_unknown = 1;
                    field_27C_pTlv = -1;
                }
                field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
                field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
                field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
                break;
            default:
                break;
            }
        }

        field_B8_xpos += field_C4_velx;
        field_BC_ypos += field_C8_vely;

        if (field_124_pCollisionLine)
        {
            sub_4974E0();
        }

        sub_497600(field_C4_velx);
    }
    
    Rope* pRope2 = static_cast<Rope*>(sObjectIds_5C1B70.Find_449CF0(field_134_rope2_id));
    pRope2->field_106_bottom = FP_GetExponent((FP_FromInteger(field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * field_CC_sprite_scale)));

    Rope* pRope1 = static_cast<Rope*>(sObjectIds_5C1B70.Find_449CF0(field_138_rope1_id));
    pRope1->field_106_bottom = FP_GetExponent((FP_FromInteger(field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * field_CC_sprite_scale)));

    if (field_280_flags.Get(LiftPoint::eBit4_bHasPulley))
    {
        pRope2->field_102_top = FP_GetExponent((FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * field_CC_sprite_scale)));
        pRope1->field_102_top = FP_GetExponent((FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * field_CC_sprite_scale)));
    }

    const FP v39 = field_BC_ypos * FP_FromDouble(1.5);
    const FP v40 = FP_FromRaw(FP_GetExponent(v39 * field_CC_sprite_scale) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);
    pRope2->field_BC_ypos = FP_NoFractional(v40 + (FP_FromInteger(25) * field_CC_sprite_scale) + field_BC_ypos + FP_FromInteger(pRope2->field_F6_rope_length));
    pRope1->field_BC_ypos = FP_NoFractional((FP_FromInteger(25) * field_CC_sprite_scale) + field_BC_ypos + FP_FromInteger(pRope1->field_F6_rope_length) - v40);

    field_13C_lift_wheel.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_1D4_pulley_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);

    if (field_C8_vely == FP_FromInteger(0))
    {
        // Wheels are stopped if not moving
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    }
    else if (field_C8_vely > FP_FromInteger(0))
    {
        // Pulley/lift wheels spin opposite ways for up/down
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
        field_1D4_pulley_anim.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
    }
    else
    {
        field_13C_lift_wheel.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
        field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
    }

    if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eNecrum_2 ||
        gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMudomoVault_3 ||
        gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMudomoVault_Ender_11 ||
        gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMudancheeVault_4 ||
        gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eMudancheeVault_Ender_7)
    {
        if (field_13C_lift_wheel.field_92_current_frame == 1 && field_13C_lift_wheel.field_4_flags.Get(AnimFlags::eBit2_Animate))
        {
            SFX_Play_46FA90(0x1Fu, 0, 0x10000);
        }
    }
    else if (field_13C_lift_wheel.field_92_current_frame == 1 && field_13C_lift_wheel.field_4_flags.Get(AnimFlags::eBit2_Animate) && sGnFrame_5C1B84 & 1)
    {
        SFX_Play_46FA90(0x1Fu, 0, 0x10000);
    }

    if ((field_C2_lvl_number != gMap_5C3030.sCurrentLevelId_5C3030 || field_C0_path_number != gMap_5C3030.sCurrentPathId_5C3032 || Event_Get_422C00(kEventDeathReset)) 
        && field_118_count <= 0)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void LiftPoint::sub_497600(FP /*xVelocity*/)
{
    NOT_IMPLEMENTED();
}

void CCSTD LiftPoint::sub_461000(Path_TLV* pTlv)
{
    pTlv->field_1_unknown &= ~3;
    pTlv->field_1_unknown |= 1;
}

void LiftPoint::CreatePulleyIfExists_462C80()
{
    Path_TLV* pFound = nullptr;

    const PathData* pPathData = sPath_dword_BB47C0->field_C_pPathData;
    __int16 yCamIdx = FP_GetExponent(field_BC_ypos) / pPathData->field_C_grid_height;
    // If we are in the top row of cameras then there can't be a pulley in the screen above because there are no more screens above!
    while (yCamIdx >= 0)
    {
        const __int16 xCamIdx = (FP_GetExponent(field_B8_xpos) / pPathData->field_A_grid_width) - gMap_5C3030.field_D0_cam_x_idx;
        // Keep looking up 1 camera for any camera that has TLVs in it.
        Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(xCamIdx, yCamIdx - gMap_5C3030.field_D2_cam_y_idx);
        while (pTlvIter)
        {
            if (pTlvIter->field_4_type == Path_Pulley::kType)
            {
                const FP left = FP_FromInteger(field_124_pCollisionLine->field_0_rect.x) + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                if (left <= FP_FromInteger(pTlvIter->field_8_top_left.field_0_x))
                {
                    const FP right = FP_FromInteger(field_124_pCollisionLine->field_0_rect.w) - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                    if (FP_FromInteger(pTlvIter->field_8_top_left.field_0_x) <= right)
                    {
                        pFound = pTlvIter;
                        break;
                    }
                }
            }
            pTlvIter = Path::TLV_Next_Of_Type_4DB720(pTlvIter, Path_Pulley::kType);
        }

        if (pFound)
        {
            // Found a pulley, stop searching
            break;
        }

        // Go 1 more camera up
        yCamIdx--;
    }

    if (!pFound)
    {
        return;
    }

    // Calculate pulley position
    const FP k13_scaled = FP_FromInteger(13) / field_CC_sprite_scale;
    const FP kM10_scaled = FP_FromInteger(-10) / field_CC_sprite_scale;

    field_26C_pulley_xpos = FP_GetExponent(((kM10_scaled + k13_scaled) / FP_FromInteger(2)) + FP_NoFractional(field_B8_xpos));
    field_26E_pulley_ypos = pFound->field_8_top_left.field_2_y;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPulleyResID);
    const LiftPointData& data = sLiftPointData_545AC8[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)];
    field_1D4_pulley_anim.Init_40A030(
        data.field_10_pulley_frame_table_offset,
        gObjList_animations_5C1A24,
        this,
        static_cast<short>(data.field_14_maxW_lift_wheel_and_pulley),
        static_cast<short>(data.field_18_maxH_lift_wheel_and_pulley),
        ppRes,
        1,
        0,
        0);

    field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

    field_280_flags.Set(LiftFlags::eBit4_bHasPulley);

    field_1D4_pulley_anim.field_C_render_layer = field_20_animation.field_C_render_layer;
    field_1D4_pulley_anim.field_14_scale = field_CC_sprite_scale;

    field_1D4_pulley_anim.field_8_r = static_cast<BYTE>(field_D0_r);
    field_1D4_pulley_anim.field_9_g = static_cast<BYTE>(field_D2_g);
    field_1D4_pulley_anim.field_A_b = static_cast<BYTE>(field_D4_b);

    field_1D4_pulley_anim.field_B_render_mode = 0;

    // Set the top of the ropes to be the bottom of the pulley
    Rope* pRope1 = static_cast<Rope*>(sObjectIds_5C1B70.Find_449CF0(field_134_rope2_id));
    Rope* pRope2 = static_cast<Rope*>(sObjectIds_5C1B70.Find_449CF0(field_138_rope1_id));

    pRope1->field_102_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * field_CC_sprite_scale));
    pRope2->field_102_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * field_CC_sprite_scale));
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

    field_13C_lift_wheel.vCleanUp_40C630();

    if (field_280_flags.Get(LiftFlags::eBit4_bHasPulley))
    {
        field_1D4_pulley_anim.vCleanUp_40C630();
    }

    ResourceManager::FreeResource_49C330(field_274_ppRes);

    dtor_4973E0();
}
