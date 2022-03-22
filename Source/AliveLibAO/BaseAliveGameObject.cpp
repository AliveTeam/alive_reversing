#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Abe.hpp"
#include "PlatformBase.hpp"
#include "Collisions.hpp"
#include "BeeSwarm.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "BirdPortal.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"

namespace AO {

void BaseAliveGameObject_ForceLink()
{ }

ALIVE_VAR(1, 0x4FC8A0, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0, nullptr);

BaseAliveGameObject* BaseAliveGameObject::ctor_401090()
{
    ctor_417C10();

    SetVTable(this, 0x4BA000);

    field_10A_flags.Clear(Flags_10A::e10A_Bit1_Can_Be_Possessed);
    field_10A_flags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    field_10A_flags.Clear(Flags_10A::e10A_Bit3);
    field_10A_flags.Clear(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_10A_flags.Clear(Flags_10A::e10A_Bit5_Electrocuted);
    field_10A_flags.Clear(Flags_10A::e10A_Bit6);

    field_F0_pTlv = nullptr;
    field_F8_pLiftPoint = nullptr;
    field_F4_pLine = nullptr;
    field_100_health = FP_FromInteger(1);
    field_106_shot = 0;
    field_108_bMotionChanged = 0;
    field_EC = 0;
    field_FC_current_motion = 0;
    field_FE_next_motion = 0;
    field_E4_previous_motion = 0;
    field_E6_last_anim_frame = 0;
    field_E8_LastLineYPos = FP_FromInteger(0);
    field_104_pending_resource_count = 0;
    gBaseAliveGameObjects_4FC8A0->Push_Back(this);
    field_6_flags.Set(Options::eIsBaseAliveGameObject_Bit6);
    return this;
}


BaseGameObject* BaseAliveGameObject::dtor_401000()
{
    SetVTable(this, 0x4BA000);
    gBaseAliveGameObjects_4FC8A0->Remove_Item(this);

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }

    if (field_104_pending_resource_count)
    {
        ResourceManager::WaitForPendingResources_41EA60(this);
    }

    return dtor_417D10();
}

BaseGameObject* BaseAliveGameObject::VDestructor(s32 flags)
{
    return Vdtor_402540(flags);
}

BaseGameObject* BaseAliveGameObject::Vdtor_402540(s32 flags)
{
    dtor_401000();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BaseAliveGameObject::VUnPosses()
{
    // Empty
}

void BaseAliveGameObject::VPossessed()
{
    // Empty
}

void BaseAliveGameObject::VSetMotion(s16 state)
{
    VSetMotion_402520(state);
}

void BaseAliveGameObject::VSetXSpawn(s16 camWorldX, s32 screenXPos)
{
    VSetXSpawn_401150(camWorldX, screenXPos);
}

void BaseAliveGameObject::VSetYSpawn(s32 camWorldY, s16 bLeft)
{
    VSetYSpawn_401380(camWorldY, bLeft);
}

void BaseAliveGameObject::VOnPathTransition(s16 camWorldX, s32 camWorldY, CameraPos direction)
{
    VOnPathTransition_401470(camWorldX, camWorldY, direction);
}

s16 BaseAliveGameObject::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_401920(pFrom);
}

s16 BaseAliveGameObject::VTakeDamage_401920(BaseGameObject* /*pFrom*/)
{
    // Defaults to no damage.
    return 0;
}

void BaseAliveGameObject::VOn_TLV_Collision(Path_TLV* /*pTlv*/)
{
    // Empty
}

void BaseAliveGameObject::VCheckCollisionLineStillValid(s32 distance)
{
    VCheckCollisionLineStillValid_401A90(distance);
}

void BaseAliveGameObject::VOnTrapDoorOpen()
{
    // Empty
}

void BaseAliveGameObject::VCheckCollisionLineStillValid_401A90(s32 distance)
{
    if (field_F4_pLine)
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        const s32 mask = field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70;
        if (sCollisions_DArray_504C6C->RayCast_40C410(
                field_A8_xpos,
                field_AC_ypos - FP_FromInteger(distance),
                field_A8_xpos,
                field_AC_ypos + FP_FromInteger(distance),
                &pLine,
                &hitX,
                &hitY,
                mask))
        {
            field_F4_pLine = pLine;
            field_AC_ypos = hitY;
            if (field_F8_pLiftPoint)
            {
                if (pLine->field_8_type == eLineTypes ::eUnknown_32 ||
                    pLine->field_8_type == eLineTypes::eUnknown_36)
                {
                    field_F8_pLiftPoint->field_C_refCount--;
                    field_F8_pLiftPoint = nullptr;

                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);
                    bRect.y += 5;
                    bRect.h += 5;

                    VOnCollisionWith(
                        {bRect.x, bRect.y},
                        {bRect.w, bRect.h},
                        ObjListPlatforms_50766C,
                        1,
                        (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_401C10);
                }
            }
        }
        else
        {
            field_F0_pTlv = gMap_507BA8.TLV_First_Of_Type_In_Camera_4464A0(TlvTypes::StartController_28, 0);
            if (field_F0_pTlv)
            {
                if (sCollisions_DArray_504C6C->RayCast_40C410(
                        field_A8_xpos,
                        FP_FromInteger(field_F0_pTlv->field_10_top_left.field_2_y),
                        field_A8_xpos,
                        FP_FromInteger(field_F0_pTlv->field_14_bottom_right.field_2_y),
                        &pLine,
                        &hitX,
                        &hitY,
                        mask))
                {
                    field_F4_pLine = pLine;
                    field_AC_ypos = hitY;
                }
            }
        }
    }
}

BirdPortal* BaseAliveGameObject::IntoBirdPortal_402350(s16 distance)
{
    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_4_typeId == Types::eBirdPortal_65)
        {
            auto pPortal = static_cast<BirdPortal*>(pObjIter);
            if (pPortal->field_18_xpos >= field_A8_xpos)
            {
                if (pPortal->field_12_side == PortalSide::eLeft_1)
                {
                    if (pPortal->field_18_xpos - field_A8_xpos <= (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(distance)))
                    {
                        if (!field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            if (FP_Abs(field_AC_ypos - pPortal->field_28_ypos) < field_BC_sprite_scale * FP_FromInteger(10))
                            {
                                if (pPortal->VPortalClipper(1))
                                {
                                    field_10_anim.field_C_layer = field_BC_sprite_scale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
                                    return pPortal;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (pPortal->field_12_side == PortalSide::eRight_0)
                {
                    if (field_A8_xpos - pPortal->field_18_xpos <= (ScaleToGridSize_41FA30(field_BC_sprite_scale) * FP_FromInteger(distance)))
                    {
                        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                        {
                            if (FP_Abs(field_AC_ypos - pPortal->field_28_ypos) < (field_BC_sprite_scale * FP_FromInteger(10)))
                            {
                                if (pPortal->VPortalClipper(1))
                                {
                                    field_10_anim.field_C_layer = field_BC_sprite_scale != FP_FromInteger(1) ? Layer::eLayer_InBirdPortal_Half_11 : Layer::eLayer_InBirdPortal_30;
                                    return pPortal;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

s16 BaseAliveGameObject::Check_IsOnEndOfLine_4021A0(s16 direction, s16 dist)
{
    // Check if distance grid blocks from current snapped X is still on the line or not, if not then we are
    // about to head off an edge.
    const FP gridSize = ScaleToGridSize_41FA30(field_BC_sprite_scale);

    FP xLoc = {};
    if (direction == 1)
    {
        xLoc = -(gridSize * FP_FromInteger(dist));
    }
    else
    {
        xLoc = gridSize * FP_FromInteger(dist);
    }

    const s16 xposRounded = FP_GetExponent(field_A8_xpos) & 1023;
    const FP xPosSnapped = FP_FromInteger((FP_GetExponent(field_A8_xpos) & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, xposRounded));
    if (xposRounded < (240 + 16) || xposRounded > (640 - 16))
    {
        return 0;
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_504C6C->RayCast_40C410(
               xLoc + xPosSnapped,
               field_AC_ypos - FP_FromInteger(4),
               xLoc + xPosSnapped,
               field_AC_ypos + FP_FromInteger(4),
               &pLine,
               &hitX,
               &hitY,
               field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70)
        == 0;
}

void BaseAliveGameObject::VOnPathTransition_401470(s16 camWorldX, s32 camWorldY, CameraPos direction)
{
    const FP oldx = field_A8_xpos;
    const FP oldy = field_AC_ypos;

    // TODO: This was probably a rect ??
    s32 xpos = 0;
    s32 ypos = 0;
    s32 height = 0;
    s32 width = 0;

    switch (direction)
    {
        case CameraPos::eCamTop_1:
        {
            xpos = camWorldX + 206;
            width = camWorldX + 674;
            ypos = camWorldY + 70;
            height = camWorldY + 170;

            // Get the fame header for the first frame in the animation and take its height
            const s32 frameH = reinterpret_cast<FrameHeader*>((*field_10_anim.field_20_ppBlock)[field_10_anim.Get_FrameHeader_403A00(-1)->field_0_frame_header_offset])->field_5_height;

            field_A8_xpos = FP_FromInteger(camWorldX + (FP_GetExponent(oldx) % 1024));
            field_AC_ypos = FP_FromInteger(frameH + camWorldY + 356);
            break;
        }

        case CameraPos::eCamBottom_2:
            xpos = camWorldX + 206;
            width = camWorldX + 674;
            ypos = camWorldY + 310;
            height = camWorldY + 410;
            field_A8_xpos = FP_FromInteger(camWorldX + (FP_GetExponent(oldx) % 1024));
            field_AC_ypos = FP_FromInteger(camWorldY + 124);
            break;

        case CameraPos::eCamLeft_3:
            width = camWorldX + 674;
            xpos = camWorldX + 524;
            ypos = camWorldY + 70;
            height = camWorldY + 410;
            field_A8_xpos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, MaxGridBlocks_41FA10(field_BC_sprite_scale) - 1));
            field_AC_ypos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        case CameraPos::eCamRight_4:
            width = camWorldX + 356;
            xpos = camWorldX + 206;
            ypos = camWorldY + 70;
            height = camWorldY + 410;
            field_A8_xpos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, 1));
            field_AC_ypos = FP_FromInteger(camWorldY + (FP_GetExponent(oldy) % 480));
            break;

        default:
            break;
    }

    field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(static_cast<s16>(xpos), static_cast<s16>(ypos), static_cast<s16>(width), static_cast<s16>(height), TlvTypes::StartController_28);
    if (!field_F0_pTlv)
    {
        // Find to find one at position so just try the first one, and for some reason flip the direction ??
        field_F0_pTlv = gMap_507BA8.TLV_First_Of_Type_In_Camera_4464A0(TlvTypes::StartController_28, 0);

        field_B4_velx = -field_B4_velx;

        field_10_anim.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
    }

    if (!field_F0_pTlv)
    {
        LOG_ERROR("Did a path transition to a camera with no start controller");
        ALIVE_FATAL("Did a path transition to a camera with no start controller (add one if this is an edited level)");
    }

    PSX_Point camLoc = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&camLoc);

    field_A8_xpos = FP_FromInteger((field_F0_pTlv->field_14_bottom_right.field_0_x + field_F0_pTlv->field_10_top_left.field_0_x) / 2);
    field_AC_ypos = FP_FromInteger(field_F0_pTlv->field_10_top_left.field_2_y);

    field_A8_xpos = FP_FromInteger(camLoc.field_0_x + SnapToXGrid_41FAA0(field_BC_sprite_scale, FP_GetExponent(field_A8_xpos - FP_FromInteger(camLoc.field_0_x))));

    if (field_F8_pLiftPoint)
    {
        // Move lift point into the new path
        const FP rect_left = field_A8_xpos - oldx;
        const FP rect_right = field_AC_ypos - oldy;

        field_F8_pLiftPoint->field_A8_xpos += rect_left;
        field_F8_pLiftPoint->field_AC_ypos += rect_right;

        field_F4_pLine->field_0_rect.x += FP_GetExponent(rect_left);
        field_F4_pLine->field_0_rect.w += FP_GetExponent(rect_left);
        field_F4_pLine->field_0_rect.y += FP_GetExponent(rect_right);
        field_F4_pLine->field_0_rect.h += FP_GetExponent(rect_right);
    }
    else
    {
        // OG FIX: After a path trans the collision items are free so we can't possibly have a line
        // so try to find a new one under our feet.
        if (!field_F4_pLine)
        {
            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos,
                    field_AC_ypos - FP_FromInteger(40),
                    field_A8_xpos,
                    field_AC_ypos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                field_F4_pLine = pLine;
                field_AC_ypos = hitY;
            }
            else
            {
                field_F4_pLine = nullptr;
            }
        }

        // If we still didn't get a line then look for a start controller
        if (!field_F4_pLine)
        {
            if (field_F0_pTlv->field_4_type == TlvTypes::StartController_28)
            {
                field_E8_LastLineYPos += field_AC_ypos - oldy;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos,
                    field_E8_LastLineYPos - FP_FromInteger(40),
                    field_A8_xpos,
                    field_E8_LastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                field_AC_ypos += hitY - field_E8_LastLineYPos;
            }
        }
    }

    if (field_BC_sprite_scale == FP_FromInteger(1) && field_10_anim.field_14_scale == FP_FromDouble(0.5))
    {
        field_B4_velx = (field_B4_velx * FP_FromInteger(2));
        return;
    }

    if (field_BC_sprite_scale == FP_FromDouble(0.5) && field_10_anim.field_14_scale == FP_FromInteger(1))
    {
        field_B4_velx = (field_B4_velx * FP_FromDouble(0.5));
        return;
    }
}


s16 BaseAliveGameObject::MapFollowMe_401D30(s16 snapToGrid)
{
    PSX_Point camCoords = {};
    gMap_507BA8.GetCurrentCamCoords_444890(&camCoords);

    // Are we "in" the current camera X bounds?
    if (field_B2_lvl_number == gMap_507BA8.field_0_current_level && field_B0_path_number == gMap_507BA8.field_2_current_path && field_A8_xpos > FP_FromInteger(camCoords.field_0_x) && field_A8_xpos < FP_FromInteger(camCoords.field_0_x + 1024))
    {
        // Snapped XPos in camera space
        const s32 snappedXLocalCoords = SnapToXGrid_41FAA0(field_BC_sprite_scale, FP_GetExponent(field_A8_xpos - FP_FromInteger(camCoords.field_0_x)));

        // In the left camera void and moving left?
        if (snappedXLocalCoords < 256 && field_B4_velx < FP_FromInteger(0))
        {
            if (sControlledCharacter_50767C == this && gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapLeft_0, this, -1))
            {
                field_B0_path_number = gMap_507BA8.field_2_current_path;
                field_B2_lvl_number = gMap_507BA8.field_0_current_level;
                return 1;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(field_A8_xpos));
                const s32 camXIndex = x_i % 1024;
                if (x_i > 1024)
                {
                    UsePathTransScale_4020D0();

                    // Put at the right side of the camera to the left
                    const s32 cam1GridBeforeRight = XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, (MaxGridBlocks_41FA10(field_BC_sprite_scale) - 1));
                    const s32 camRightEdge = x_i - camXIndex - 1024;
                    field_A8_xpos = FP_FromInteger(camRightEdge) + FP_FromInteger(cam1GridBeforeRight) + ScaleToGridSize_41FA30(field_BC_sprite_scale);

                    VCheckCollisionLineStillValid(40);
                }
            }
        }
        // In the right camera void and moving right?
        else if (snappedXLocalCoords > 624 && field_B4_velx > FP_FromInteger(0))
        {
            // Go to the right camera in under player control
            if (sControlledCharacter_50767C == this && gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapRight_1, this, -1))
            {
                field_B0_path_number = gMap_507BA8.field_2_current_path;
                field_B2_lvl_number = gMap_507BA8.field_0_current_level;
                return 1;
            }
            else
            {
                const s32 x_i = abs(FP_GetExponent(field_A8_xpos));
                const s32 camXIndex = x_i % 1024;

                gMap_507BA8.Get_map_size_444870(&camCoords);
                if (x_i < (camCoords.field_0_x - 1024))
                {
                    UsePathTransScale_4020D0();

                    // Put at the left side of the camera to the right
                    const s32 cam1GridAfterLeft = XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, 1);
                    const s32 camLeftEdge = x_i - camXIndex + 1024;
                    field_A8_xpos = FP_FromInteger(camLeftEdge) + FP_FromInteger(cam1GridAfterLeft) - ScaleToGridSize_41FA30(field_BC_sprite_scale);

                    VCheckCollisionLineStillValid(40);
                }
            }
        }
        else if (snapToGrid)
        {
            // Not in the voids of the camera, just snap to the x grid
            field_A8_xpos = FP_FromInteger(snappedXLocalCoords + camCoords.field_0_x);
        }
        return 0;
    }
    else
    {
        const s32 x_i = abs(FP_GetExponent(field_A8_xpos));
        const s32 camXIndex = x_i % 1024;

        // In the left camera void and moving left?
        if (camXIndex < 256 && field_B4_velx < FP_FromInteger(0))
        {
            if (x_i > 1024)
            {
                UsePathTransScale_4020D0();

                const s32 camRightGrid = XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, MaxGridBlocks_41FA10(field_BC_sprite_scale));
                const s32 camRightEdge = x_i - camXIndex - 1024;
                field_A8_xpos = FP_FromInteger(camRightEdge) + FP_FromInteger(camRightGrid) + ScaleToGridSize_41FA30(field_BC_sprite_scale);

                VCheckCollisionLineStillValid(40);
            }
        }
        // In the right camera void and moving right?
        else if (camXIndex > 624 && field_B4_velx > FP_FromInteger(0)) // Never hit as velx is < 0
        {
            gMap_507BA8.Get_map_size_444870(&camCoords);
            if (x_i < (camCoords.field_0_x - 1024))
            {
                UsePathTransScale_4020D0();

                const s32 camLeftGrid = XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, 0);
                const s32 camLeftEdge = x_i - camXIndex + 1024;
                field_A8_xpos = FP_FromInteger(camLeftEdge) + FP_FromInteger(camLeftGrid) - ScaleToGridSize_41FA30(field_BC_sprite_scale);

                VCheckCollisionLineStillValid(40);
            }
        }
        return 0;
    }
}

void BaseAliveGameObject::SetActiveCameraDelayedFromDir_401C90()
{
    if (sControlledCharacter_50767C == this)
    {
        switch (Is_In_Current_Camera_417CC0())
        {
            case CameraPos::eCamTop_1:
                if (field_B8_vely < FP_FromInteger(0))
                {
                    gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapTop_2, this, -1);
                }
                break;

            case CameraPos::eCamBottom_2:
                if (field_B8_vely > FP_FromInteger(0))
                {
                    gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapBottom_3, this, -1);
                }
                break;

            case CameraPos::eCamLeft_3:
                if (field_B4_velx < FP_FromInteger(0))
                {
                    gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapLeft_0, this, -1);
                }
                break;

            case CameraPos::eCamRight_4:
                if (field_B4_velx > FP_FromInteger(0))
                {
                    gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapRight_1, this, -1);
                }
                break;

            default:
                return;
        }
    }
}

s16 BaseAliveGameObject::OnTrapDoorIntersection_401C10(PlatformBase* pPlatform)
{
    PSX_RECT rect = {};

    pPlatform->VGetBoundingRect(&rect, 1);
    if (FP_GetExponent(field_A8_xpos) < rect.x || FP_GetExponent(field_A8_xpos) > rect.w || FP_GetExponent(field_AC_ypos) > rect.h)
    {
        return 1;
    }

    field_F8_pLiftPoint = pPlatform;
    field_F8_pLiftPoint->VAdd(this);
    field_F8_pLiftPoint->field_C_refCount++;

    return 1;
}

s16 BaseAliveGameObject::WallHit_401930(FP offY, FP offX)
{
    PathLine* pLine = nullptr;
    return sCollisions_DArray_504C6C->RayCast_40C410(
               field_A8_xpos,
               field_AC_ypos - offY,
               field_A8_xpos + offX,
               field_AC_ypos - offY,
               &pLine,
               &offX,
               &offY,
               field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60)
        != 0;
}

s16 BaseAliveGameObject::InAirCollision_4019C0(PathLine** ppLine, FP* hitX, FP* hitY, FP vely)
{
    field_B8_vely += (field_BC_sprite_scale * vely);

    if (field_B8_vely > (field_BC_sprite_scale * FP_FromInteger(20)))
    {
        field_B8_vely = (field_BC_sprite_scale * FP_FromInteger(20));
    }

    const FP old_xpos = field_A8_xpos;
    const FP old_ypos = field_AC_ypos;

    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    return sCollisions_DArray_504C6C->RayCast_40C410(
        old_xpos,
        old_ypos,
        field_A8_xpos,
        field_AC_ypos,
        ppLine,
        hitX,
        hitY,
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70);
}

void CC BaseAliveGameObject::OnResourceLoaded_4019A0(BaseAliveGameObject* ppRes)
{
    ppRes->field_104_pending_resource_count--;
}

void BaseAliveGameObject::VSetXSpawn_401150(s16 camWorldX, s32 screenXPos)
{
    const FP old_x = field_A8_xpos;
    const FP old_y = field_AC_ypos;

    field_A8_xpos = FP_FromInteger(camWorldX + XGrid_Index_To_XPos_41FA60(field_BC_sprite_scale, screenXPos));

    field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(0, field_A8_xpos, old_y, field_A8_xpos, old_y);

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->field_A8_xpos += (field_A8_xpos - old_x);

        field_F4_pLine->field_0_rect.x += FP_GetExponent(field_A8_xpos - old_x);
        field_F4_pLine->field_0_rect.w += FP_GetExponent(field_A8_xpos - old_x);
        field_F4_pLine->field_0_rect.y = field_F4_pLine->field_0_rect.y;
        field_F4_pLine->field_0_rect.h = field_F4_pLine->field_0_rect.h;
    }
    else
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (field_F4_pLine)
        {
            if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos,
                    old_y - FP_FromInteger(40),
                    field_A8_xpos,
                    old_y + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    1 << field_F4_pLine->field_8_type))
            {
                field_F4_pLine = pLine;
                field_AC_ypos = hitY;
            }
            else
            {
                field_F0_pTlv = gMap_507BA8.TLV_First_Of_Type_In_Camera_4464A0(TlvTypes::StartController_28, 0);
                if (field_F0_pTlv
                    && sCollisions_DArray_504C6C->RayCast_40C410(
                        field_A8_xpos,
                        FP_FromInteger(field_F0_pTlv->field_10_top_left.field_2_y),
                        field_A8_xpos,
                        FP_FromInteger(field_F0_pTlv->field_14_bottom_right.field_2_y),
                        &pLine,
                        &hitX,
                        &hitY,
                        1 << field_F4_pLine->field_8_type))
                {
                    field_F4_pLine = pLine;
                    field_AC_ypos = hitY;
                }
                else
                {
                    field_F4_pLine = nullptr;
                }
            }
        }
        else
        {
            if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos,
                    field_E8_LastLineYPos - FP_FromInteger(40),
                    field_A8_xpos,
                    field_E8_LastLineYPos + FP_FromInteger(40),
                    &pLine,
                    &hitX,
                    &hitY,
                    field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
            {
                field_AC_ypos += hitY - field_E8_LastLineYPos;
            }
        }
    }
}

void BaseAliveGameObject::VSetYSpawn_401380(s32 camWorldY, s16 bLeft)
{
    const FP oldx = field_A8_xpos;
    const FP oldy = field_AC_ypos;

    auto pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_10_anim.field_20_ppBlock)[field_10_anim.Get_FrameHeader_403A00(-1)->field_0_frame_header_offset]);

    if (bLeft == 1)
    {
        field_AC_ypos = FP_FromInteger(pFrameHeader->field_5_height + camWorldY + 356);
    }
    else
    {
        field_AC_ypos = FP_FromInteger(camWorldY + 124);
    }

    field_F0_pTlv = gMap_507BA8.TLV_Get_At_446060(
        nullptr,
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos,
        field_AC_ypos);

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->field_A8_xpos += field_A8_xpos - oldx;
        field_F8_pLiftPoint->field_AC_ypos += field_AC_ypos - oldy;

        field_F4_pLine->field_0_rect.x += FP_GetExponent(field_A8_xpos - oldx);
        field_F4_pLine->field_0_rect.w += FP_GetExponent(field_A8_xpos - oldx);
        field_F4_pLine->field_0_rect.y += FP_GetExponent(field_AC_ypos - oldy);
        field_F4_pLine->field_0_rect.h += FP_GetExponent(field_AC_ypos - oldy);
    }
}

s16 BaseAliveGameObject::IsBeeSwarmChasingMe_4022B0()
{
    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eBeeSwarm_95)
        {
            if (static_cast<BeeSwarm*>(pObj)->field_D98_pChaseTarget == this)
            {
                return 1;
            }
        }
    }
    return 0;
}

void BaseAliveGameObject::VSetMotion_402520(s16 state)
{
    field_108_bMotionChanged = TRUE;
    field_FC_current_motion = state;
}

void BaseAliveGameObject::UsePathTransScale_4020D0()
{
    auto pPathTrans = static_cast<Path_Change*>(gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        TlvTypes::PathTransition_1));

    if (pPathTrans)
    {
        if (pPathTrans->field_22_next_path_scale == Scale_short::eHalf_1)
        {
            if (field_BC_sprite_scale != FP_FromDouble(0.5))
            {
                field_BC_sprite_scale = FP_FromDouble(0.5);
                field_C6_scale = 0;
                field_B4_velx = (field_B4_velx * FP_FromDouble(0.5));
            }
        }
        else if (pPathTrans->field_22_next_path_scale == Scale_short::eFull_0)
        {
            if (field_BC_sprite_scale != FP_FromInteger(1))
            {
                field_BC_sprite_scale = FP_FromInteger(1);
                field_C6_scale = 1;
                field_B4_velx = (field_B4_velx * FP_FromInteger(2));
            }
        }
    }
}

BaseGameObject* CC BaseAliveGameObject::FindObjectOfType_418280(Types typeToFind, FP xpos, FP ypos)
{
    const s32 xpos_int = FP_GetExponent(xpos);
    const s32 ypos_int = FP_GetExponent(ypos);

    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == typeToFind)
        {
            auto pObj2 = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

            PSX_RECT bRect = {};
            pObj2->VGetBoundingRect(&bRect, 1);

            if (xpos_int >= bRect.x && xpos_int <= bRect.w && ypos_int >= bRect.y && ypos_int <= bRect.h)
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

} // namespace AO
