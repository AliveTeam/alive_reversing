#include "stdafx_ao.h"
#include "Function.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "ResourceManager.hpp"
#include "Rope.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "ShadowZone.hpp"
#include "ScreenManager.hpp"

namespace AO {

struct LiftPointCoord
{
    int field_0;
    int field_4;
};

struct LiftPointData
{
    int field_0_maxW_Platform;
    int field_4_maxH_platform;
    int field_8_platform_frame_table_offset;
    int field_C_lift_wheel_frame_table_offset;
    int field_10_pulley_frame_table_offset;
    int field_14_maxW_lift_wheel_and_pulley;
    int field_18_maxW_lift_wheel_and_pulley;
};

const LiftPointData stru_4BB480[16] =
{
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 1260, 117, 19, 24204, 24240, 124, 37 },
    { 1524, 114, 25, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 1524, 114, 25, 17108, 17144, 62, 46 },
    { 1472, 116, 20, 21900, 21936, 73, 55 },
    { 1472, 116, 20, 21900, 21936, 73, 55 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 1260, 117, 19, 24204, 24240, 124, 37 },
    { 1260, 117, 19, 24204, 24240, 124, 37 },
    { 940, 122, 12, 17108, 17144, 62, 46 },
    { 1472, 116, 20, 21900, 21936, 73, 55 }
};

const LiftPointCoord stru_4BB640[16] =
{
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 2, 1 },
    { 2, 1 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 0, 0 },
    { 2, 1 }
};



LiftPoint* LiftPoint::ctor_434710(Path_LiftPoint* pTlv, Map* pPath, int tlvInfo)
{
    ctor_401090();

    SetVTable(&field_13C_lift_wheel, 0x4BA2B8);
    SetVTable(&field_1D4_pulley_anim, 0x4BA2B8);
    SetVTable(this, 0x4BB6C0);
    field_4_typeId = Types::eLiftPoint_51;

    pTlv->field_1_unknown = 3;

    BYTE** ppLiftRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kLiftResID, 1, 0);
    if (pTlv->field_20_scale)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    const int idx = static_cast<int>(gMap_507BA8.field_0_current_level);
    AddDynamicCollision_4512C0(
        stru_4BB480[idx].field_0_maxW_Platform,
        stru_4BB480[idx].field_4_maxH_platform,
        stru_4BB480[idx].field_8_platform_frame_table_offset,
        ppLiftRes,
        pTlv,
        pPath,
        tlvInfo);

    if (field_BC_sprite_scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 25;
    }
    else
    {
        field_10_anim.field_C_layer = 6;
        field_120_pCollisionLine->field_8_type = 36;
    }
    
    const FP oldX = field_A8_xpos;
    MapFollowMe_401D30(1);
    const FP newX = field_A8_xpos;
    
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    const auto xMovedBy = FP_GetExponent(newX - oldX);
    field_118_x_offset -= xMovedBy;
    field_11A_width_offset -= xMovedBy;

    BYTE** ppLiftWheelRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kPulleyResID, 1, 0);
    if (field_13C_lift_wheel.Init_402D20(
        stru_4BB480[idx].field_C_lift_wheel_frame_table_offset,
        gObjList_animations_505564,
        this,
        static_cast<short>(stru_4BB480[idx].field_14_maxW_lift_wheel_and_pulley),
        static_cast<short>(stru_4BB480[idx].field_18_maxW_lift_wheel_and_pulley),
        ppLiftWheelRes,
        1,
        0,
        0))
    {
        if (pTlv->field_20_scale)
        {
            field_13C_lift_wheel.field_C_layer = 6;
        }
        else
        {
            field_13C_lift_wheel.field_C_layer = 25;
        }

        field_13C_lift_wheel.field_14_scale = field_BC_sprite_scale;

        field_27A_flags.Clear(Flags::eBit6_bMoveToFloorLevel);
        field_27A_flags.Clear(Flags::eBit4_bBottomFloor);
        field_27A_flags.Clear(Flags::eBit3_bMiddleFloor);
        field_27A_flags.Clear(Flags::eBit2_bTopFloor);
 
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit2_Animate);

        field_12C_bMoving &= ~1u;
        field_274_ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeliftResID, 1, 0);
        field_B4_velx = FP_FromInteger(0);
        field_13C_lift_wheel.field_8_r = 128;
        field_13C_lift_wheel.field_9_g = 128;
        field_13C_lift_wheel.field_A_b = 128;
        field_13C_lift_wheel.field_B_render_mode = 0;
        field_10C = 1;

        field_B8_vely = FP_FromInteger(0);
        field_130_stop_type = 4;

        auto pRopeMem = ao_new<Rope>();
        if (pRopeMem)
        {
            pRopeMem->ctor_458520(
                FP_GetExponent(field_A8_xpos + (FP_FromInteger(13) * field_BC_sprite_scale) + FP_FromInteger(stru_4BB640[idx].field_4)),
                0,
                FP_GetExponent(field_AC_ypos + (FP_FromInteger(25) * field_BC_sprite_scale)),
                field_BC_sprite_scale);
            pRopeMem->field_C_refCount++;
            field_138_pRope1 = pRopeMem;
        }

        auto pRopeMem2 = ao_new<Rope>();
        if (pRopeMem2)
        {
            pRopeMem2->ctor_458520(
                FP_GetExponent(field_A8_xpos + (FP_FromInteger(-10) * field_BC_sprite_scale) + FP_FromInteger(stru_4BB640[idx].field_0)),
                0,
                FP_GetExponent(field_AC_ypos + (FP_FromInteger(25) * field_BC_sprite_scale)),
                field_BC_sprite_scale);
            pRopeMem2->field_C_refCount++;
            field_134_pRope2 = pRopeMem2;

        }

        field_134_pRope2->field_F2_bottom = FP_GetExponent((FP_FromInteger(25) * field_BC_sprite_scale) + FP_FromInteger(field_120_pCollisionLine->field_0_rect.y));
        field_138_pRope1->field_F2_bottom = FP_GetExponent((FP_FromInteger(25) * field_BC_sprite_scale) + FP_FromInteger(field_120_pCollisionLine->field_0_rect.y));

        const FP v29 = FP_FromRaw(FP_GetExponent((field_AC_ypos * FP_FromDouble(1.5)) * field_BC_sprite_scale) % FP_FromInteger(field_134_pRope2->field_E6_rope_length).fpValue);
        field_134_pRope2->field_AC_ypos = FP_NoFractional(field_AC_ypos + v29 + (FP_FromInteger(25) * field_BC_sprite_scale) + FP_FromInteger(field_134_pRope2->field_E6_rope_length));
        field_138_pRope1->field_AC_ypos = FP_NoFractional(field_AC_ypos + v29 - (FP_FromInteger(25) * field_BC_sprite_scale) + FP_FromInteger(field_138_pRope1->field_E6_rope_length));

        field_27A_flags.Clear(Flags::eBit5_bHasPulley);

        CreatePulleyIfExists_435AE0(0, 0);

        field_278_point_id = pTlv->field_18_id;
        field_130_stop_type = pTlv->field_1E_lift_point_stop_type;

        switch (field_130_stop_type)
        {
        case 0:
            field_27A_flags.Set(Flags::eBit2_bTopFloor);
            break;

        case 1:
            field_27A_flags.Set(Flags::eBit4_bBottomFloor);
            break;

        case 2:
            field_27A_flags.Set(Flags::eBit3_bMiddleFloor);
            break;
        }

        field_27A_flags.Clear(Flags::eBit8_KeepOnMiddleFloor);
        field_27A_flags.Set(Flags::eBit7_bIgnoreLiftMover);
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }
    return this;
}

void LiftPoint::Move_435740(FP xSpeed, FP ySpeed, int /*not_used*/)
{
    field_B4_velx = xSpeed;
    field_B8_vely = ySpeed;
    if (FP_GetExponent(xSpeed) || FP_GetExponent(ySpeed))
    {
        field_12C_bMoving |= 1u;
    }
}


void LiftPoint::StayOnFloor(__int16 floor, Path_LiftPoint* pLiftTlv)
{
    if (!floor)
    {
        field_AC_ypos = FP_FromInteger(pLiftTlv->field_10_top_left.field_2_y - field_11C_y_offset);
        SFX_Play_43AD70(SoundEffect::LiftStop_35, 0);
        SFX_Play_43AE60(SoundEffect::LiftStop_35, 80, -2000);
    }

    field_12C_bMoving &= ~1u;
    pLiftTlv->field_1_unknown = 3;
    pLiftTlv->field_18_id = field_278_point_id;
    field_B8_vely = FP_FromInteger(0);

    Event_Broadcast_417220(kEventNoise_0, this);
    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
}


void LiftPoint::Sub_Unknown(Path_TLV* pTlv)
{
    pTlv->field_0_flags.Clear(eBit1_Created);
    pTlv->field_0_flags.Clear(eBit2_Unknown);
    pTlv->field_1_unknown |= 1;
}

bool LiftPoint::OnTopFloor() const
{
    return field_27A_flags.Get(Flags::eBit2_bTopFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnBottomFloor() const
{
    return field_27A_flags.Get(Flags::eBit4_bBottomFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnMiddleFloor() const
{
    return field_27A_flags.Get(Flags::eBit3_bMiddleFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
}

bool LiftPoint::OnAnyFloor() const
{
    return OnBottomFloor() || OnTopFloor() || OnMiddleFloor();
}


bool LiftPoint::OnAFloorLiftMoverCanUse() const
{
    return (OnMiddleFloor() && !field_27A_flags.Get(Flags::eBit7_bIgnoreLiftMover)) || OnBottomFloor() || OnTopFloor();
}

void LiftPoint::VUpdate()
{
    VUpdate_434D10();
}

void LiftPoint::VUpdate_434D10()
{
    if (field_12C_bMoving & 1)
    {
        if (field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel))
        {
            const bool bOnFloor = field_270_floorYLevel == field_AC_ypos;
            const FP distToFloor = field_270_floorYLevel - field_AC_ypos;
            FP absDistToFloor = distToFloor;
            if (distToFloor < FP_FromInteger(0) || bOnFloor)
            {
                absDistToFloor = -distToFloor;
            }
            else
            {
                absDistToFloor = distToFloor;
            }

            if (absDistToFloor >= (FP_FromInteger(2) * field_BC_sprite_scale))
            {
                if (distToFloor >= FP_FromInteger(0))
                {
                    field_B8_vely = (FP_FromInteger(2) * field_BC_sprite_scale);
                }
                else
                {
                    field_B8_vely = -(FP_FromInteger(2) * field_BC_sprite_scale);
                }
                field_12C_bMoving |= 1u;
            }
            else
            {
                field_AC_ypos = field_270_floorYLevel;
                field_B8_vely = FP_FromInteger(0);
                field_12C_bMoving &= ~1u;
                field_27A_flags.Clear(Flags::eBit6_bMoveToFloorLevel);
                SFX_Play_43AD70(SoundEffect::LiftStop_35, 0);
                SFX_Play_43AE60(SoundEffect::LiftStop_35, 80, -2000);
                Event_Broadcast_417220(kEventNoise_0, this);
                Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
            }
        }
        else
        {
            field_130_stop_type = 4;
            const FP lineY = FP_FromInteger(field_120_pCollisionLine->field_0_rect.y);

            Path_LiftPoint* pLiftTlv = nullptr;
            Path_TLV* pTlvIter = gMap_507BA8.TLV_Get_At_446060(
                nullptr,
                field_A8_xpos,
                lineY,
                field_A8_xpos,
                (field_BC_sprite_scale * FP_FromInteger(30)) + lineY);
            while (pTlvIter)
            {
                if (pTlvIter->field_4_type == TlvTypes::LiftPoint_8)
                {
                    pLiftTlv = static_cast<Path_LiftPoint*>(pTlvIter);
                    field_130_stop_type = pLiftTlv->field_1E_lift_point_stop_type;
                    break;
                }
                pTlvIter = gMap_507BA8.TLV_Get_At_446060(
                    pTlvIter,
                    field_A8_xpos,
                    lineY,
                    field_A8_xpos,
                    lineY + (field_BC_sprite_scale * FP_FromInteger(30)));
            }

            if (pLiftTlv && pLiftTlv->field_1A_bstart_point)
            {
                field_27A_flags.Set(Flags::eBit7_bIgnoreLiftMover);
            }
            else
            {
                field_27A_flags.Clear(Flags::eBit7_bIgnoreLiftMover);
            }

            if (pLiftTlv)
            {
                Sub_Unknown(pLiftTlv);

                field_270_floorYLevel = FP_FromInteger(pTlvIter->field_10_top_left.field_2_y + -field_11C_y_offset);
            }
            else
            {
                field_270_floorYLevel = FP_FromInteger(0);
                field_130_stop_type = 4;
            }

            const auto distanceToFloor = field_270_floorYLevel - field_AC_ypos;
            const FP kMinus25Scaled = (field_BC_sprite_scale * FP_FromInteger(-25));
            const FP k30Scaled = (field_BC_sprite_scale * FP_FromInteger(30));

            switch (field_130_stop_type)
            {
            case 0:
                if (field_B8_vely >= FP_FromInteger(0))
                {
                    if (field_B8_vely != FP_FromInteger(0) || (distanceToFloor <= kMinus25Scaled) || distanceToFloor >= k30Scaled)
                    {
                        pLiftTlv->field_1_unknown = 1;
                        field_27A_flags.Clear(Flags::eBit2_bTopFloor);
                    }
                    else
                    {
                        field_27A_flags.Set(Flags::eBit6_bMoveToFloorLevel);
                        field_12C_bMoving |= 1u;
                        field_AC_ypos = field_270_floorYLevel - distanceToFloor;
                        
                        pLiftTlv->field_1_unknown = 3;

                        pLiftTlv->field_18_id = field_278_point_id;
                        field_27A_flags.Set(Flags::eBit2_bTopFloor);
                    }
                }
                else if (field_B8_vely + lineY <= FP_FromInteger(pTlvIter->field_10_top_left.field_2_y))
                {
                    StayOnFloor(field_27A_flags.Get(Flags::eBit2_bTopFloor), pLiftTlv);

                    field_27A_flags.Set(Flags::eBit2_bTopFloor);
                }
                break;

            case 1:
                if (field_B8_vely <= FP_FromInteger(0))
                {
                    if (field_B8_vely != FP_FromInteger(0) || (distanceToFloor <= kMinus25Scaled) || distanceToFloor >= k30Scaled)
                    {
                        pLiftTlv->field_1_unknown = 1;
                        field_27A_flags.Clear(Flags::eBit4_bBottomFloor);
                    }
                    else
                    {
                        field_12C_bMoving |= 1u;
                        field_27A_flags.Set(Flags::eBit6_bMoveToFloorLevel);
                        field_AC_ypos = field_270_floorYLevel - distanceToFloor;

                        pLiftTlv->field_1_unknown = 3;

                        pLiftTlv->field_18_id = field_278_point_id;
                        field_27A_flags.Set(Flags::eBit4_bBottomFloor);
                    }
                }
                else if (field_B8_vely + lineY >= FP_FromInteger(pTlvIter->field_10_top_left.field_2_y))
                {
                    StayOnFloor(field_27A_flags.Get(Flags::eBit4_bBottomFloor), pLiftTlv);
                    field_27A_flags.Set(Flags::eBit4_bBottomFloor);
                }
                break;

            case 2:
                if (distanceToFloor <= kMinus25Scaled || distanceToFloor >= k30Scaled)
                {
                    pLiftTlv->field_1_unknown = 1;
                }
                else
                {
                    if (field_27A_flags.Get(Flags::eBit8_KeepOnMiddleFloor))
                    {
                        StayOnFloor(field_27A_flags.Get(Flags::eBit3_bMiddleFloor), pLiftTlv);
                        field_27A_flags.Clear(Flags::eBit8_KeepOnMiddleFloor);
                    }

                    if (field_B8_vely == FP_FromInteger(0))
                    {
                        field_12C_bMoving |= 1u;
                        field_27A_flags.Set(Flags::eBit6_bMoveToFloorLevel);
                        field_AC_ypos = field_270_floorYLevel - distanceToFloor;
                    }

                    pLiftTlv->field_1_unknown = 3;
                    pLiftTlv->field_18_id = field_278_point_id;
                    field_27A_flags.Set(Flags::eBit3_bMiddleFloor);
                }
                break;

            case 4:
                if (pLiftTlv)
                {
                    pLiftTlv->field_1_unknown = 1;
                }

                field_27A_flags.Clear(Flags::eBit2_bTopFloor);
                field_27A_flags.Clear(Flags::eBit3_bMiddleFloor);
                field_27A_flags.Clear(Flags::eBit4_bBottomFloor);
                break;

            default:
                break;
            }
        }

        field_A8_xpos += field_B4_velx;
        field_AC_ypos += field_B8_vely;

        if (field_120_pCollisionLine)
        {
            SyncCollisionLinePosition_451540();
        }
        KeepThingsOnPlatform_451690(field_B4_velx);
    }

    const FP FP_25xScale = FP_FromInteger(25) * field_BC_sprite_scale;
    const FP FP_m19xScale = FP_FromInteger(-19) * field_BC_sprite_scale;

    Rope* pRope2 = field_134_pRope2;
    Rope* pRope1 = field_138_pRope1;

    const FP rope2_rope_length = FP_FromInteger(pRope2->field_E6_rope_length);
    const FP rope1_rope_length = FP_FromInteger(pRope1->field_E6_rope_length);

    const FP pColliRectY = FP_FromInteger(field_120_pCollisionLine->field_0_rect.y);

    pRope2->field_F2_bottom = FP_GetExponent(pColliRectY + FP_25xScale);
    pRope1->field_F2_bottom = FP_GetExponent(pColliRectY + FP_25xScale);

    if (field_27A_flags.Get(Flags::eBit5_bHasPulley))
    {
        const FP pulley_ypos = FP_FromInteger(field_26E_pulley_ypos);

        pRope2->field_EE_top = FP_GetExponent(pulley_ypos + FP_m19xScale);
        pRope1->field_EE_top = FP_GetExponent(pulley_ypos + FP_m19xScale);
    }

    const FP new_ypos = (field_AC_ypos * FP_FromDouble(1.5));
    const FP remaining_rope = FP_FromRaw(FP_GetExponent(new_ypos * field_BC_sprite_scale) % rope2_rope_length.fpValue);

    pRope2->field_AC_ypos = FP_NoFractional((field_AC_ypos + remaining_rope) + (FP_25xScale + rope2_rope_length));
    pRope1->field_AC_ypos = FP_NoFractional((field_AC_ypos + FP_25xScale) + (rope1_rope_length - remaining_rope));


    field_13C_lift_wheel.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_1D4_pulley_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);

    if (field_B8_vely == FP_FromInteger(0))
    {
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    }
    else if (field_B8_vely > FP_FromInteger(0))
    {
        field_13C_lift_wheel.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
        field_1D4_pulley_anim.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
    }
    else if (field_B8_vely < FP_FromInteger(0))
    {
        field_13C_lift_wheel.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
        field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
    }

    if (field_13C_lift_wheel.field_92_current_frame == 1)
    {
        if (field_13C_lift_wheel.field_4_flags.Get(AnimFlags::eBit2_Animate))
        {
            SFX_Play_43AD70(SoundEffect::WheelSqueak_36, 0);
        }
    }

    if (field_B2_lvl_number != gMap_507BA8.field_0_current_level ||
        field_B0_path_number != gMap_507BA8.field_2_current_path ||
        Event_Get_417250(kEventDeathReset_4))
    {
        if (field_114_count <= 0)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

void LiftPoint::VRender(int** pOrderingTable)
{
    VRender_435780(pOrderingTable);
}

void LiftPoint::VRender_435780(int** ppOt)
{
    PSX_Point mapCoord = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&mapCoord);
    if (field_B2_lvl_number == gMap_507BA8.field_0_current_level)
    {
        if (field_B0_path_number == gMap_507BA8.field_2_current_path)
        {
            if (field_A8_xpos >= FP_FromInteger(mapCoord.field_0_x) && field_A8_xpos <= FP_FromInteger(mapCoord.field_0_x + 1024))
            {
                BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

                __int16 liftWheelR = field_C0_r;
                __int16 liftWheelG = field_C2_g;
                __int16 liftWheelB = field_C4_b;

                PSX_RECT boundingRect = {};
                VGetBoundingRect(&boundingRect, 1);

                ShadowZone::ShadowZones_Calculate_Colour_435FF0(
                    FP_GetExponent(field_A8_xpos),
                    (boundingRect.y + boundingRect.h) / 2,
                    field_C6_scale,
                    &liftWheelR,
                    &liftWheelG,
                    &liftWheelB);

                field_13C_lift_wheel.field_8_r = static_cast<BYTE>(liftWheelR);
                field_13C_lift_wheel.field_9_g = static_cast<BYTE>(liftWheelG);
                field_13C_lift_wheel.field_A_b = static_cast<BYTE>(liftWheelB);

                const FP xOff = (FP_FromInteger(3) * field_BC_sprite_scale);
                const FP yOff = (FP_FromInteger(-5) * field_BC_sprite_scale);

                field_13C_lift_wheel.VRender_403AE0(
                    FP_GetExponent(field_A8_xpos
                        + xOff
                        + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
                    FP_GetExponent(field_AC_ypos
                        + yOff
                        + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
                    ppOt,
                    0,
                    0);

                PSX_RECT liftWheelRect = {};
                field_13C_lift_wheel.Get_Frame_Rect_402B50(&liftWheelRect);
                pScreenManager_4FF7C8->InvalidateRect_406E40(
                    liftWheelRect.x,
                    liftWheelRect.y,
                    liftWheelRect.w,
                    liftWheelRect.h,
                    pScreenManager_4FF7C8->field_2E_idx);

                if (field_27A_flags.Get(Flags::eBit5_bHasPulley))
                {
                    const FP pulley_xpos = FP_FromInteger(field_26C_pulley_xpos);
                    const FP pulley_ypos = FP_FromInteger(field_26E_pulley_ypos);
                    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                        field_B2_lvl_number,
                        field_B0_path_number,
                        pulley_xpos,
                        pulley_ypos,
                        0))
                    {
                        __int16 r = field_C0_r;
                        __int16 g = field_C2_g;
                        __int16 b = field_C4_b;

                        ShadowZone::ShadowZones_Calculate_Colour_435FF0(
                            field_26C_pulley_xpos,
                            field_26E_pulley_ypos,
                            field_C6_scale,
                            &r,
                            &g,
                            &b);
  
                        field_1D4_pulley_anim.field_8_r = static_cast<BYTE>(r);
                        field_1D4_pulley_anim.field_9_g = static_cast<BYTE>(g);
                        field_1D4_pulley_anim.field_A_b = static_cast<BYTE>(b);

                        field_1D4_pulley_anim.VRender_403AE0(
                            FP_GetExponent(pulley_xpos + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
                            FP_GetExponent(pulley_ypos + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
                            ppOt,
                            0,
                            0);
                        PSX_RECT pulleyRect = {};
                        field_1D4_pulley_anim.Get_Frame_Rect_402B50(&pulleyRect);
                        pScreenManager_4FF7C8->InvalidateRect_406E40(
                            pulleyRect.x,
                            pulleyRect.y,
                            pulleyRect.w,
                            pulleyRect.h,
                            pScreenManager_4FF7C8->field_2E_idx);
                    }
                }
            }
        }
    }
}

void LiftPoint::VScreenChanged()
{
    VScreenChanged_435CC0();
}

void LiftPoint::VScreenChanged_435CC0()
{
    if (!field_27A_flags.Get(Flags::eBit5_bHasPulley))
    {
        CreatePulleyIfExists_435AE0(0, -1);
    }

    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

BaseGameObject* LiftPoint::dtor_4355E0()
{
    SetVTable(this, 0x4BB6C0);

    if (field_6_flags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        return dtor_451490();
    }

    if (field_134_pRope2)
    {
        field_134_pRope2->field_C_refCount--;
        field_134_pRope2->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_138_pRope1)
    {
        field_138_pRope1->field_C_refCount--;
        field_138_pRope1->field_6_flags.Set(Options::eDead_Bit3);
    }

    field_134_pRope2 = nullptr;
    field_138_pRope1 = nullptr;

    gMap_507BA8.TLV_Reset_446870(field_128_tlvInfo, -1, 0, 0);

    auto pLiftPointTlv = gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(FP_FromInteger(field_120_pCollisionLine->field_0_rect.y) + FP_FromInteger(field_120_pCollisionLine->field_0_rect.y)),
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent((field_BC_sprite_scale * FP_FromInteger(30)) + FP_FromInteger(field_120_pCollisionLine->field_0_rect.y)),
        TlvTypes::LiftPoint_8);

    if (pLiftPointTlv)
    {
        pLiftPointTlv->field_1_unknown &= ~3u;
    }

    field_13C_lift_wheel.VCleanUp_403F40();

    if (field_27A_flags.Get(Flags::eBit5_bHasPulley))
    {
        field_1D4_pulley_anim.VCleanUp_403F40();
    }

    ResourceManager::FreeResource_455550(field_274_ppRes);
    return dtor_451490();
}

BaseGameObject* LiftPoint::VDestructor(signed int flags)
{
    return Vdtor_435D10(flags);
}

void LiftPoint::CreatePulleyIfExists_435AE0(short camX, short camY)
{
    auto pTlv = gMap_507BA8.Get_First_TLV_For_Offsetted_Camera_4463B0(camX, camY);
    if (pTlv)
    {
        while (1)
        {
            field_26E_pulley_ypos = pTlv->field_C_sound_pos.field_2_y;
            field_26C_pulley_xpos = pTlv->field_C_sound_pos.field_0_x;

            if (pTlv->field_4_type == TlvTypes::Pulley_35)
            {
                if (field_120_pCollisionLine->field_0_rect.x <= pTlv->field_C_sound_pos.field_0_x &&
                    pTlv->field_C_sound_pos.field_0_x <= field_120_pCollisionLine->field_0_rect.w)
                {
                    break;
                }
            }

            pTlv = gMap_507BA8.TLV_Get_At_446060(pTlv, FP_FromInteger(-1), FP_FromInteger(-1), FP_FromInteger(-1), FP_FromInteger(-1));
            if (!pTlv)
            {
                return;
            }
        }

        const FP k13_scaled = FP_FromInteger(13) * field_BC_sprite_scale;
        const FP kM10_scaled = FP_FromInteger(-10) * field_BC_sprite_scale;

        field_26C_pulley_xpos = FP_GetExponent(((k13_scaled + kM10_scaled) / FP_FromInteger(2)) + FP_NoFractional(field_A8_xpos));

        BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kPulleyResID, 1, 0);
        const int idx = static_cast<int>(gMap_507BA8.field_0_current_level);

        field_1D4_pulley_anim.Init_402D20(
            stru_4BB480[idx].field_10_pulley_frame_table_offset,
            gObjList_animations_505564,
            this,
            static_cast<short>(stru_4BB480[idx].field_14_maxW_lift_wheel_and_pulley),
            static_cast<short>(stru_4BB480[idx].field_18_maxW_lift_wheel_and_pulley),
            ppRes,
            1,
            0,
            0);

        field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        field_1D4_pulley_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);

        field_27A_flags.Set(Flags::eBit5_bHasPulley);

        field_1D4_pulley_anim.field_8_r = 128;
        field_1D4_pulley_anim.field_9_g = 128;
        field_1D4_pulley_anim.field_A_b = 128;

        field_1D4_pulley_anim.field_C_layer = field_10_anim.field_C_layer;
        field_1D4_pulley_anim.field_14_scale = field_BC_sprite_scale;
        field_1D4_pulley_anim.field_B_render_mode = 0;

        field_134_pRope2->field_EE_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * field_1D4_pulley_anim.field_14_scale));
        field_138_pRope1->field_EE_top = FP_GetExponent(FP_FromInteger(field_26E_pulley_ypos) + (FP_FromInteger(-19) * field_1D4_pulley_anim.field_14_scale));
    }
}

LiftPoint* LiftPoint::Vdtor_435D10(signed int flags)
{
    dtor_4355E0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

}
