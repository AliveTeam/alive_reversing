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
#include "Grid.hpp"

struct LiftPointData final
{
    AnimId field_0_platform_anim_id;
    s32 field_4_maxW_platform;
    s32 field_8_maxH_platform;
    AnimId field_C_lift_bottom_wheel_anim_id;
    AnimId field_10_lift_top_wheel_anim_id;
    s32 field_14_maxW_lift_wheels;
    s32 field_18_maxH_lift_wheels;
};
ALIVE_ASSERT_SIZEOF(LiftPointData, 0x1C);

const LiftPointData sLiftPointData_545AC8[18] = {
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Necrum, 135, 17, AnimId::LiftBottomWheel_Necrum, AnimId::LiftTopWheel_Necrum, 69, 34},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24},
    {AnimId::LiftPlatform_Mines, 136, 31, AnimId::LiftBottomWheel_Mines, AnimId::LiftTopWheel_Mines, 47, 24}};

const TintEntry sLiftTints_55BF50[18] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 107u, 107u, 107u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

LiftPoint::LiftPoint(Path_LiftPoint* pTlv, s32 tlvInfo)
{
    field_C_objectId = tlvInfo;
    SetType(AETypes::eLiftPoint_78);

    pTlv->field_1_tlv_state = 3;

    field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr_4DB7C0(pTlv);

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kLiftResID);
    if (pTlv->field_18_scale != Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    const LiftPointData& rPlatformData = sLiftPointData_545AC8[static_cast<u32>(gMap.mCurrentLevel)];
    const AnimRecord& platformRec = AnimRec(rPlatformData.field_0_platform_anim_id);
    AddDynamicCollision_4971C0(
        platformRec.mFrameTableOffset,
        platformRec.mMaxW,
        static_cast<u16>(platformRec.mMaxH),
        ppRes,
        pTlv,
        &gMap,
        tlvInfo);

    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_25;
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_Half_6;
        field_124_pCollisionLine->field_8_type = eLineTypes::eUnknown_36;
    }

    SetTint_425600(sLiftTints_55BF50, gMap.mCurrentLevel);

    const FP oldX = field_B8_xpos;
    MapFollowMe(TRUE);
    const s16 xSnapDelta = FP_GetExponent(field_B8_xpos - oldX);
    field_11C_x_offset -= xSnapDelta;
    field_11E_width_offset -= xSnapDelta;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);


    u8** ppLiftWheels = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kLiftWheelsResID);
    const LiftPointData& rLiftWheelData = sLiftPointData_545AC8[static_cast<s32>(gMap.mCurrentLevel)];
    const AnimRecord& bottomWheelRec = AnimRec(rLiftWheelData.field_C_lift_bottom_wheel_anim_id);
    if (field_13C_lift_wheel.Init_40A030(
            bottomWheelRec.mFrameTableOffset,
            gObjList_animations_5C1A24,
            this,
            static_cast<u16>(bottomWheelRec.mMaxW),
            static_cast<u16>(bottomWheelRec.mMaxH),
            ppLiftWheels,
            1u,
            0,
            0))
    {
        if (pTlv->field_18_scale != Scale_short::eFull_0)
        {
            field_13C_lift_wheel.field_C_render_layer = Layer::eLayer_BeforeShadow_Half_6;
            field_13C_lift_wheel.field_14_scale = field_CC_sprite_scale;
        }
        else
        {
            field_13C_lift_wheel.field_C_render_layer = Layer::eLayer_BeforeShadow_25;
            field_13C_lift_wheel.field_14_scale = field_CC_sprite_scale;
        }

        u8** ppAbeLiftRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, TRUE, FALSE);

        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

        field_12C_bMoving &= ~1u;

        field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
        field_280_flags.Clear(LiftFlags::eBit2_bMiddleFloor);
        field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
        field_280_flags.Clear(LiftFlags::eBit5_bMoveToFloorLevel);

        field_274_ppRes = ppAbeLiftRes;

        field_13C_lift_wheel.field_9_g = static_cast<u8>(field_D2_g);
        field_13C_lift_wheel.field_8_r = static_cast<u8>(field_D4_b);
        field_13C_lift_wheel.field_A_b = static_cast<u8>(field_D0_r);
        field_13C_lift_wheel.field_B_render_mode = TPageAbr::eBlend_0;

        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);

        const FP k25 = FP_FromInteger(25);
        const FP k13 = FP_FromInteger(13);
        const FP km10 = FP_FromInteger(-10);


        auto pRope1 = ae_new<Rope>(FP_GetExponent((k13 * field_CC_sprite_scale + field_B8_xpos)),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos),
                                   field_CC_sprite_scale);
        if (pRope1)
        {
            field_138_rope1_id = pRope1->field_8_object_id;
        }

        auto pRope2 = ae_new<Rope>(FP_GetExponent((km10 * field_CC_sprite_scale) + field_B8_xpos),
                                   0, // Start at the very top of the screen
                                   FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos),
                                   field_CC_sprite_scale);
        if (pRope2)
        {
            field_134_rope2_id = pRope2->field_8_object_id;
        }

        pRope2->field_106_bottom = FP_GetExponent((k25 * field_CC_sprite_scale) + FP_FromInteger(field_124_pCollisionLine->field_0_rect.y));
        pRope1->field_106_bottom = FP_GetExponent((k25 * field_CC_sprite_scale) + FP_FromInteger(field_124_pCollisionLine->field_0_rect.y));

        const FP v28 = field_BC_ypos * FP_FromDouble(1.5);
        const FP v29 = FP_FromRaw(FP_GetExponent(v28 * field_CC_sprite_scale) % FP_FromInteger(pRope2->field_F6_rope_length).fpValue);

        pRope2->field_BC_ypos = FP_FromInteger(FP_GetExponent(v29 + (k25 * field_CC_sprite_scale) + field_BC_ypos + FP_FromInteger(pRope2->field_F6_rope_length)));
        pRope1->field_BC_ypos = FP_FromInteger(FP_GetExponent((k25 * field_CC_sprite_scale) + field_BC_ypos + FP_FromInteger(pRope1->field_F6_rope_length) - v29));

        field_280_flags.Clear(LiftFlags::eBit4_bHasPulley);
        CreatePulleyIfExists_462C80();

        field_278_lift_point_id = static_cast<s8>(pTlv->field_10_lift_point_id);
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
            case LiftPointStopType::eMiddleLockFloor_3: // Not used ??
            case LiftPointStopType::eStartPointOnly_4:
                field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
                break;
        }

        field_280_flags.Clear(LiftFlags::eBit7_KeepOnMiddleFloor);
        field_280_flags.Set(LiftFlags::eBit6);
    }
    else
    {
        mFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void LiftPoint::VRender(PrimHeader** ppOt)
{
    vRender_462730(ppOt);
}

void LiftPoint::VUpdate()
{
    vUpdate_461AE0();
}

void LiftPoint::VScreenChanged()
{
    vScreenChanged_463020();
}

s32 LiftPoint::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_4637D0(reinterpret_cast<LiftPoint_State*>(pSaveBuffer));
}

static void LoadLiftResourceBans(const char_type* pRopeBanName, const char_type* pLiftBanName)
{
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kRopesResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170(pRopeBanName, nullptr);
    }
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kLiftResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170(pLiftBanName, nullptr);
    }
}

s32 LiftPoint::CreateFromSaveState(const u8* pData)
{
    const LiftPoint_State* pState = reinterpret_cast<const LiftPoint_State*>(pData);

    Path_LiftPoint* pTlv = static_cast<Path_LiftPoint*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_C_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABELIFT.BAN", nullptr);
    }

    switch (gMap.mCurrentLevel)
    {
        case LevelIds::eMines_1:
            LoadLiftResourceBans("ROPES.BAN", "MILIFT.BND");
            break;

        case LevelIds::eNecrum_2:
            LoadLiftResourceBans("NECROPE.BAN", "NELIFT.BND");
            break;

        case LevelIds::eMudomoVault_3:
        case LevelIds::eMudomoVault_Ender_11:
            LoadLiftResourceBans("NECROPE.BAN", "PVLIFT.BND");
            break;

        case LevelIds::eMudancheeVault_4:
        case LevelIds::eMudancheeVault_Ender_7:
            LoadLiftResourceBans("NECROPE.BAN", "SVLIFT.BND");
            break;

        case LevelIds::eFeeCoDepot_5:
        case LevelIds::eFeeCoDepot_Ender_12:
            LoadLiftResourceBans("ROPES.BAN", "FDLIFT.BND");
            break;

        case LevelIds::eBarracks_6:
        case LevelIds::eBarracks_Ender_13:
            LoadLiftResourceBans("ROPES.BAN", "BALIFT.BND");
            break;

        case LevelIds::eBrewery_9:
        case LevelIds::eBrewery_Ender_10:
            LoadLiftResourceBans("ROPES.BAN", "BRLIFT.BND");
            break;

        default:
            LoadLiftResourceBans("ROPES.BAN", "BWLIFT.BND");
            break;
    }

    auto pLiftPoint = ae_new<LiftPoint>(pTlv, pState->field_C_tlvInfo);
    pLiftPoint->field_B8_xpos = pState->field_4_xpos;
    pLiftPoint->field_BC_ypos = pState->field_8_ypos;
    pLiftPoint->SyncCollisionLinePosition_4974E0();

    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->field_134_rope2_id, AETypes::eLiftRope_108));
    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(pLiftPoint->field_138_rope1_id, AETypes::eLiftRope_108));

    pRope2->field_106_bottom = FP_GetExponent(FP_FromInteger(pLiftPoint->field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * pLiftPoint->field_CC_sprite_scale));
    pRope1->field_106_bottom = FP_GetExponent(FP_FromInteger(pLiftPoint->field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * pLiftPoint->field_CC_sprite_scale));

    if (pLiftPoint->field_280_flags.Get(LiftPoint::eBit4_bHasPulley))
    {
        pRope2->field_102_top = FP_GetExponent(FP_FromInteger(pLiftPoint->field_26E_pulley_ypos) + FP_FromInteger(-19) * pLiftPoint->field_CC_sprite_scale);
        pRope1->field_102_top = FP_GetExponent(FP_FromInteger(pLiftPoint->field_26E_pulley_ypos) + FP_FromInteger(-19) * pLiftPoint->field_CC_sprite_scale);
    }

    pLiftPoint->field_128_tlvInfo = pState->field_C_tlvInfo;
    pLiftPoint->field_27C_pTlv = pState->field_10_pTlv;
    pLiftPoint->field_270_floorYLevel = pState->field_14_floorYLevel;
    pLiftPoint->field_130_lift_point_stop_type = pState->field_18_lift_point_stop_type;

    if (pState->field_1A.Get(LiftPoint_State::eBit1_bMoving))
    {
        pLiftPoint->field_12C_bMoving |= 1;
    }
    else
    {
        pLiftPoint->field_12C_bMoving &= ~1;
    }

    pLiftPoint->field_280_flags.Set(LiftFlags::eBit1_bTopFloor, pState->field_1A.Get(LiftPoint_State::eBit2_bTopFloor));
    pLiftPoint->field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor, pState->field_1A.Get(LiftPoint_State::eBit3_bMiddleFloor));
    pLiftPoint->field_280_flags.Set(LiftFlags::eBit3_bBottomFloor, pState->field_1A.Get(LiftPoint_State::eBit4_bBottomFloor));
    pLiftPoint->field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel, pState->field_1A.Get(LiftPoint_State::eBit5_bMoveToFloorLevel));
    pLiftPoint->field_280_flags.Set(LiftFlags::eBit6, pState->field_1A.Get(LiftPoint_State::eBit6));
    pLiftPoint->field_280_flags.Set(LiftFlags::eBit7_KeepOnMiddleFloor, pState->field_1A.Get(LiftPoint_State::eBit7_KeepOnMiddleFloor));


    if (pState->field_10_pTlv == pState->field_C_tlvInfo)
    {
        return sizeof(LiftPoint_State);
    }

    pTlv->field_1_tlv_state = 1;
    if (pState->field_10_pTlv == -1)
    {
        return sizeof(LiftPoint_State);
    }

    Path_TLV* pTlv2 = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_10_pTlv);
    pTlv2->field_1_tlv_state = 3;
    return sizeof(LiftPoint_State);
}

void LiftPoint::vKeepOnMiddleFloor_461870()
{
    field_280_flags.Set(LiftFlags::eBit7_KeepOnMiddleFloor);
}

Bool32 LiftPoint::vOnTopFloor_461890()
{
    return field_280_flags.Get(LiftFlags::eBit1_bTopFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

Bool32 LiftPoint::vOnMiddleFloor_4618C0()
{
    return field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

Bool32 LiftPoint::vOnBottomFloor_4618F0()
{
    return field_280_flags.Get(LiftFlags::eBit3_bBottomFloor) && !(field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
}

Bool32 LiftPoint::vOnAnyFloor_461920()
{
    return vOnBottomFloor_4618F0() || vOnTopFloor_461890() || vOnMiddleFloor_4618C0();
}

Bool32 LiftPoint::vOnAFloorLiftMoverCanUse_461960()
{
    // Top or bottom floor can still be activated by the lift mover?
    return (vOnMiddleFloor_4618C0() && !field_280_flags.Get(LiftFlags::eBit8_bIgnoreLiftMover)) || vOnBottomFloor_4618F0() || vOnTopFloor_461890();
}

Bool32 LiftPoint::vMovingToFloorLevel_4619B0()
{
    return field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel);
}

void LiftPoint::vMove_4626A0(FP xSpeed, FP ySpeed, s32 /*not_used*/)
{
    field_C4_velx = xSpeed * field_CC_sprite_scale;
    field_C8_vely = ySpeed * field_CC_sprite_scale;

    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        field_12C_bMoving |= 1u;
    }
}

void LiftPoint::vRender_462730(PrimHeader** ppOt)
{
    // Renders the pulley, lift platform and lift platform wheel

    // In the current level/map?
    if (field_C2_lvl_number == gMap.mCurrentLevel && field_C0_path_number == gMap.mCurrentPath)
    {
        // Within the current camera X bounds?
        PSX_Point camPos = {};
        gMap.GetCurrentCamCoords_480680(&camPos);

        if (field_B8_xpos >= FP_FromInteger(camPos.field_0_x) && field_B8_xpos <= FP_FromInteger(camPos.field_0_x + 640))
        {
            s16 r = field_D0_r;
            s16 g = field_D2_g;
            s16 b = field_D4_b;

            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            ShadowZone::ShadowZones_Calculate_Colour_463CE0(
                FP_GetExponent(field_B8_xpos),
                (bRect.h + bRect.y) / 2,
                field_D6_scale,
                &r,
                &g,
                &b);

            field_13C_lift_wheel.field_8_r = static_cast<u8>(r);
            field_13C_lift_wheel.field_9_g = static_cast<u8>(g);
            field_13C_lift_wheel.field_A_b = static_cast<u8>(b);

            if (gMap.mCurrentLevel != LevelIds::eNecrum_2 && Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
            {
                field_13C_lift_wheel.vRender_40B820(
                    FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + (FP_FromInteger(3) * field_CC_sprite_scale)),
                    FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + (FP_FromInteger(-5) * field_CC_sprite_scale)),
                    ppOt,
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
                if (gMap.Is_Point_In_Current_Camera_4810D0(
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

                    field_1D4_pulley_anim.field_8_r = static_cast<u8>(r);
                    field_1D4_pulley_anim.field_9_g = static_cast<u8>(g);
                    field_1D4_pulley_anim.field_A_b = static_cast<u8>(b);

                    field_1D4_pulley_anim.vRender_40B820(
                        FP_GetExponent(FP_FromInteger(field_26C_pulley_xpos) - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
                        FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
                        ppOt,
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
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

            if (gMap.mCurrentLevel == LevelIds::eNecrum_2 && Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
            {
                field_13C_lift_wheel.vRender_40B820(
                    FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x + (FP_FromInteger(3) * field_CC_sprite_scale)),
                    FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + (FP_FromInteger(-5) * field_CC_sprite_scale)),
                    ppOt,
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
    if (field_12C_bMoving & 1)
    {
        if (field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel))
        {
            const bool bOnFloor = field_270_floorYLevel == field_BC_ypos;
            const FP distToFloor = field_270_floorYLevel - field_BC_ypos;

            FP absDistToFloor = {};
            if (distToFloor < FP_FromInteger(0) || bOnFloor)
            {
                absDistToFloor = -distToFloor;
            }
            else
            {
                absDistToFloor = distToFloor;
            }

            if (absDistToFloor >= FP_FromInteger(2) * field_CC_sprite_scale)
            {
                if (distToFloor >= FP_FromInteger(0))
                {
                    field_C8_vely = FP_FromInteger(2) * field_CC_sprite_scale;
                }
                else
                {
                    field_C8_vely = -(FP_FromInteger(2) * field_CC_sprite_scale);
                }
                field_12C_bMoving |= 1u;
            }
            else
            {
                field_BC_ypos = field_270_floorYLevel;
                field_C8_vely = FP_FromInteger(0);
                field_12C_bMoving &= ~1u;
                field_280_flags.Clear(LiftFlags::eBit5_bMoveToFloorLevel);
                SFX_Play_46FA90(SoundEffect::LiftStop_30, 0);
                SFX_Play(SoundEffect::LiftStop_30, 80, -2000);
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
                while (pTlvIter->field_4_type.mType != TlvTypes::LiftPoint_7)
                {
                    pTlvIter = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                        pTlvIter,
                        field_B8_xpos,
                        lineY,
                        field_B8_xpos,
                        lineY + (field_CC_sprite_scale * FP_FromInteger(30)));

                    if (!pTlvIter)
                    {
                        break;
                    }
                }
            }

            // TODO: Bugged if lift point not found - last TLV will get casted which could be anything
            auto pLiftTlv = static_cast<Path_LiftPoint*>(pTlvIter);
            if (pLiftTlv)
            {
                field_130_lift_point_stop_type = pLiftTlv->field_16_lift_point_stop_type;

                field_280_flags.Clear(LiftFlags::eBit8_bIgnoreLiftMover);
                if (pLiftTlv->field_1A_bIgnore_lift_mover == Choice_short::eYes_1)
                {
                    field_280_flags.Set(LiftFlags::eBit8_bIgnoreLiftMover);
                }

                if (pLiftTlv->field_1_tlv_state != static_cast<u8>(pLiftTlv->field_12_bStart_point))
                {
                    field_280_flags.Set(LiftFlags::eBit6);
                }
                else
                {
                    field_280_flags.Clear(LiftFlags::eBit6);
                }
            }

            // TODO: Also bugged because will always be true
            if (pLiftTlv)
            {
                sub_461000(pLiftTlv);
                field_270_floorYLevel = FP_FromInteger(pLiftTlv->field_8_top_left.field_2_y + -field_120_y_offset);
            }
            else
            {
                field_280_flags.Clear(LiftFlags::eBit6);
                field_270_floorYLevel = FP_FromInteger(0);
                field_130_lift_point_stop_type = LiftPointStopType::eStartPointOnly_4;
            }

            const FP distanceToFloor = field_270_floorYLevel - field_BC_ypos;
            const FP kMinus25Scaled = field_CC_sprite_scale * FP_FromInteger(-25);
            const FP k30Scaled = field_CC_sprite_scale * FP_FromInteger(30);

            switch (field_130_lift_point_stop_type)
            {
                case LiftPointStopType::eTopFloor_0:
                    if (field_C8_vely >= FP_FromInteger(0))
                    {
                        if (field_C8_vely != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->field_1_tlv_state = 1;
                            field_280_flags.Clear(LiftFlags::eBit1_bTopFloor);
                            field_27C_pTlv = -1;
                        }
                        else
                        {
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            field_12C_bMoving |= 1u;
                            field_BC_ypos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->field_1_tlv_state = 3;

                            field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr_4DB7C0(pLiftTlv);
                            pLiftTlv->field_10_lift_point_id = field_278_lift_point_id;
                            field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                        }
                    }
                    else if (field_C8_vely + lineY <= FP_FromInteger(pLiftTlv->field_8_top_left.field_2_y))
                    {
                        vStayOnFloor_461A00(field_280_flags.Get(LiftFlags::eBit1_bTopFloor), pLiftTlv);
                        field_280_flags.Set(LiftFlags::eBit1_bTopFloor);
                    }
                    break;

                case LiftPointStopType::eBottomFloor_1:
                    if (field_C8_vely <= FP_FromInteger(0))
                    {
                        if (field_C8_vely != FP_FromInteger(0) || distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                        {
                            pLiftTlv->field_1_tlv_state = 1;

                            field_280_flags.Clear(LiftFlags::eBit3_bBottomFloor);
                            field_27C_pTlv = -1;
                        }
                        else
                        {
                            field_12C_bMoving |= 1u;
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            field_BC_ypos = field_270_floorYLevel - distanceToFloor;

                            pLiftTlv->field_1_tlv_state = 3;

                            field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr_4DB7C0(pLiftTlv);
                            pLiftTlv->field_10_lift_point_id = field_278_lift_point_id;
                            field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                        }
                    }
                    else if (lineY + field_C8_vely >= FP_FromInteger(pLiftTlv->field_8_top_left.field_2_y))
                    {
                        vStayOnFloor_461A00(field_280_flags.Get(LiftFlags::eBit3_bBottomFloor), pLiftTlv);
                        field_280_flags.Set(LiftFlags::eBit3_bBottomFloor);
                    }
                    break;

                case LiftPointStopType::eMiddleFloor_2:
                    if (distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                    {
                        pLiftTlv->field_1_tlv_state = 1;
                        field_27C_pTlv = -1;
                    }
                    else
                    {
                        if (field_280_flags.Get(LiftFlags::eBit7_KeepOnMiddleFloor))
                        {
                            vStayOnFloor_461A00(field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor), pLiftTlv);
                            field_280_flags.Clear(LiftFlags::eBit7_KeepOnMiddleFloor);
                        }

                        if (field_C8_vely == FP_FromInteger(0))
                        {
                            field_12C_bMoving |= 1u;
                            field_280_flags.Set(LiftFlags::eBit5_bMoveToFloorLevel);
                            field_BC_ypos = field_270_floorYLevel - distanceToFloor;
                        }

                        pLiftTlv->field_1_tlv_state = 3;
                        field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr_4DB7C0(pLiftTlv);
                        pLiftTlv->field_10_lift_point_id = field_278_lift_point_id;
                        field_280_flags.Set(LiftFlags::eBit2_bMiddleFloor);
                    }
                    break;

                case LiftPointStopType::eStartPointOnly_4:
                    if (pLiftTlv)
                    {
                        pLiftTlv->field_1_tlv_state = 1;
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
            SyncCollisionLinePosition_4974E0();
        }

        MoveObjectsOnLift_497600(field_C4_velx);
    }

    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(field_134_rope2_id, AETypes::eLiftRope_108));
    pRope2->field_106_bottom = FP_GetExponent((FP_FromInteger(field_124_pCollisionLine->field_0_rect.y) + (FP_FromInteger(25) * field_CC_sprite_scale)));

    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(field_138_rope1_id, AETypes::eLiftRope_108));
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

    if (gMap.mCurrentLevel == LevelIds::eNecrum_2 || gMap.mCurrentLevel == LevelIds::eMudomoVault_3 || gMap.mCurrentLevel == LevelIds::eMudomoVault_Ender_11 || gMap.mCurrentLevel == LevelIds::eMudancheeVault_4 || gMap.mCurrentLevel == LevelIds::eMudancheeVault_Ender_7)
    {
        if (field_13C_lift_wheel.field_92_current_frame == 1 && field_13C_lift_wheel.field_4_flags.Get(AnimFlags::eBit2_Animate))
        {
            SFX_Play_46FA90(SoundEffect::WheelSqueak_31, 0);
        }
    }
    else if (field_13C_lift_wheel.field_92_current_frame == 1 && field_13C_lift_wheel.field_4_flags.Get(AnimFlags::eBit2_Animate) && sGnFrame_5C1B84 & 1)
    {
        SFX_Play_46FA90(SoundEffect::WheelSqueak_31, 0);
    }

    if ((field_C2_lvl_number != gMap.mCurrentLevel || field_C0_path_number != gMap.mCurrentPath || Event_Get_422C00(kEventDeathReset))
        && field_118_count <= 0)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void LiftPoint::MoveObjectsOnLift_497600(FP xVelocity)
{
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        BaseGameObject* pObjectsLiftPoint = sObjectIds.Find_449CF0(pObj->field_110_id);
        if (pObjectsLiftPoint == this)
        {
            if (pObj == sControlledCharacter_5C1B8C)
            {
                // Keep the player in the screen
                const FP oldVelY = pObj->field_C8_vely;
                pObj->field_C8_vely = field_C8_vely;
                pObj->SetActiveCameraDelayedFromDir();

                // And keep the old velY
                pObj->field_C8_vely = oldVelY;
            }

            // Do platforms ever move horizontally? This is always 0 it seems
            pObj->field_B8_xpos += xVelocity;

            // Keep ypos on the platform
            pObj->field_BC_ypos = FP_FromInteger(field_124_pCollisionLine->field_0_rect.y);

            if (pObj->mFlags.Get(BaseGameObject::eInteractive_Bit8))
            {
                pObj->field_E4_collection_rect.x = pObj->field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                pObj->field_E4_collection_rect.y = pObj->field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
                pObj->field_E4_collection_rect.w = pObj->field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                pObj->field_E4_collection_rect.h = pObj->field_BC_ypos;
            }
        }
    }
}

void LiftPoint::sub_461000(Path_TLV* pTlv)
{
    pTlv->field_0_flags.Clear(eBit1_Created);
    pTlv->field_0_flags.Clear(eBit2_Destroyed);
    pTlv->field_1_tlv_state |= 1;
}

void LiftPoint::vStayOnFloor_461A00(s16 floor, Path_LiftPoint* pTlv)
{
    if (!floor)
    {
        field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y + -field_120_y_offset);
        SFX_Play_46FA90(SoundEffect::LiftStop_30, 0);
        SFX_Play(SoundEffect::LiftStop_30, 80, -2000);
    }

    field_12C_bMoving &= ~1;
    pTlv->field_1_tlv_state = 3;
    field_27C_pTlv = sPath_dword_BB47C0->TLVInfo_From_TLVPtr_4DB7C0(pTlv);
    pTlv->field_10_lift_point_id = field_278_lift_point_id;
    field_C8_vely = FP_FromInteger(0);

    Event_Broadcast_422BC0(kEventNoise, this);
    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
}

s32 LiftPoint::vGetSaveState_4637D0(LiftPoint_State* pState)
{
    pState->field_0_type = AETypes::eLiftPoint_78;
    pState->field_4_xpos = field_B8_xpos;
    pState->field_8_ypos = field_BC_ypos;
    pState->field_C_tlvInfo = field_128_tlvInfo;
    pState->field_10_pTlv = field_27C_pTlv;
    pState->field_14_floorYLevel = field_270_floorYLevel;
    pState->field_18_lift_point_stop_type = field_130_lift_point_stop_type;


    pState->field_1A.Set(LiftPoint_State::eBit1_bMoving, field_12C_bMoving & 1);
    pState->field_1A.Set(LiftPoint_State::eBit2_bTopFloor, field_280_flags.Get(LiftFlags::eBit1_bTopFloor));
    pState->field_1A.Set(LiftPoint_State::eBit3_bMiddleFloor, field_280_flags.Get(LiftFlags::eBit2_bMiddleFloor));
    pState->field_1A.Set(LiftPoint_State::eBit4_bBottomFloor, field_280_flags.Get(LiftFlags::eBit3_bBottomFloor));
    pState->field_1A.Set(LiftPoint_State::eBit5_bMoveToFloorLevel, field_280_flags.Get(LiftFlags::eBit5_bMoveToFloorLevel));
    pState->field_1A.Set(LiftPoint_State::eBit6, field_280_flags.Get(LiftFlags::eBit6));
    pState->field_1A.Set(LiftPoint_State::eBit7_KeepOnMiddleFloor, field_280_flags.Get(LiftFlags::eBit7_KeepOnMiddleFloor));

    return sizeof(LiftPoint_State);
}

void LiftPoint::CreatePulleyIfExists_462C80()
{
    Path_TLV* pFound = nullptr;

    const PathData* pPathData = sPath_dword_BB47C0->field_C_pPathData;
    s16 yCamIdx = FP_GetExponent(field_BC_ypos) / pPathData->field_C_grid_height;
    // If we are in the top row of cameras then there can't be a pulley in the screen above because there are no more screens above!
    while (yCamIdx >= 0)
    {
        const s16 xCamIdx = (FP_GetExponent(field_B8_xpos) / pPathData->field_A_grid_width) - gMap.field_D0_cam_x_idx;
        // Keep looking up 1 camera for any camera that has TLVs in it.
        Path_TLV* pTlvIter = sPath_dword_BB47C0->Get_First_TLV_For_Offsetted_Camera_4DB610(xCamIdx, yCamIdx - gMap.field_D2_cam_y_idx);
        while (pTlvIter)
        {
            if (pTlvIter->field_4_type == TlvTypes::Pulley_21)
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
            pTlvIter = Path::TLV_Next_Of_Type_4DB720(pTlvIter, TlvTypes::Pulley_21);
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
    const FP k13_scaled = FP_FromInteger(13) * field_CC_sprite_scale;
    const FP kM10_scaled = FP_FromInteger(-10) * field_CC_sprite_scale;

    field_26C_pulley_xpos = FP_GetExponent(((kM10_scaled + k13_scaled) / FP_FromInteger(2)) + FP_NoFractional(field_B8_xpos));
    field_26E_pulley_ypos = pFound->field_8_top_left.field_2_y;

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kLiftWheelsResID);
    const LiftPointData& data = sLiftPointData_545AC8[static_cast<s32>(gMap.mCurrentLevel)];
    const AnimRecord& topWheelRec = AnimRec(data.field_10_lift_top_wheel_anim_id);
    field_1D4_pulley_anim.Init_40A030(
        topWheelRec.mFrameTableOffset,
        gObjList_animations_5C1A24,
        this,
        static_cast<s16>(topWheelRec.mMaxW),
        static_cast<s16>(topWheelRec.mMaxH),
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

    field_1D4_pulley_anim.field_8_r = static_cast<u8>(field_D0_r);
    field_1D4_pulley_anim.field_9_g = static_cast<u8>(field_D2_g);
    field_1D4_pulley_anim.field_A_b = static_cast<u8>(field_D4_b);

    field_1D4_pulley_anim.field_B_render_mode = TPageAbr::eBlend_0;

    // Set the top of the ropes to be the bottom of the pulley
    Rope* pRope1 = static_cast<Rope*>(sObjectIds.Find(field_134_rope2_id, AETypes::eLiftRope_108));
    Rope* pRope2 = static_cast<Rope*>(sObjectIds.Find(field_138_rope1_id, AETypes::eLiftRope_108));

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
                field_274_ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeliftResID, TRUE, FALSE);
            }
        }
        else
        {
            ResourceManager::FreeResource_49C330(field_274_ppRes);
            field_274_ppRes = nullptr;
        }
    }

    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

LiftPoint::~LiftPoint()
{
    BaseGameObject* pRope2 = sObjectIds.Find(field_134_rope2_id, AETypes::eLiftRope_108);
    BaseGameObject* pRope1 = sObjectIds.Find(field_138_rope1_id, AETypes::eLiftRope_108);
    if (pRope2)
    {
        pRope2->mFlags.Set(BaseGameObject::eDead);
        field_134_rope2_id = -1;
    }

    if (pRope1)
    {
        pRope1->mFlags.Set(BaseGameObject::eDead);
        field_138_rope1_id = -1;
    }

    Path::TLV_Reset_4DB8E0(field_128_tlvInfo, -1, 0, 0);

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_CC_sprite_scale * FP_FromInteger(30)),
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(FP_FromInteger(field_124_pCollisionLine->field_0_rect.y) + (field_CC_sprite_scale * FP_FromInteger(30))),
        TlvTypes::LiftPoint_7);

    if (pTlv)
    {
        pTlv->field_0_flags.Clear(eBit1_Created);
        pTlv->field_0_flags.Clear(eBit2_Destroyed);
    }

    field_13C_lift_wheel.vCleanUp_40C630();

    if (field_280_flags.Get(LiftFlags::eBit4_bHasPulley))
    {
        field_1D4_pulley_anim.vCleanUp_40C630();
    }

    ResourceManager::FreeResource_49C330(field_274_ppRes);
}
