#include "stdafx.h"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "ObjectIds.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "PathData.hpp"
#include "PlatformBase.hpp"
#include "Game.hpp"
#include "BirdPortal.hpp"
#include "Events.hpp"

ALIVE_VAR(1, 0x5C1B7C, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_5C1B7C, nullptr);

EXPORT int CC SnapToXGrid_449930(FP scale, int x)
{
    if (scale == FP_FromDouble(0.5))
    {
        int v4 = (x % 375 - 6) % 13;
        if (v4 >= 7)
        {
            return x - v4 + 13;
        }
        else
        {
            return x - v4;
        }
    }
    else if (scale == FP_FromInteger(1))
    {
        int v3 = (x - 12) % 25;
        if (v3 >= 13)
        {
            return x - v3 + 25;
        }
        else
        {
            return x - v3;
        }
    }
    else
    {
        return x;
    }
}

EXPORT BaseAliveGameObject* BaseAliveGameObject::ctor_408240(short resourceArraySize)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(resourceArraySize);
    SetVTable(this, 0x544000);

    field_114_flags.Clear(Flags_114::e114_Bit1_bShot);
    field_114_flags.Clear(Flags_114::e114_MotionChanged_Bit2);
    field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
    field_114_flags.Clear(Flags_114::e114_Bit5);
    field_114_flags.Clear(Flags_114::e114_Bit6_SetOffExplosives);
    field_114_flags.Clear(Flags_114::e114_Bit7_Electrocuted);
    field_114_flags.Clear(Flags_114::e114_Bit8_bInvisible);
    field_114_flags.Clear(Flags_114::e114_Bit9);
    field_114_flags.Clear(Flags_114::e114_Bit10);
    field_114_flags.Clear(Flags_114::e114_Bit11);

    field_FC_pPathTLV = nullptr;
    field_100_pCollisionLine = nullptr;
    field_10C_health = FP_FromDouble(1.0);
    field_110_id = -1;
    field_106_current_motion = 0;
    field_108_next_motion = 0;
    field_F4 = 0;
    field_F6_anim_frame = 0;
    field_F8_LastLineYPos = FP_FromInteger(0);
    field_10A = 0;

    gBaseAliveGameObjects_5C1B7C->Push_Back(this);

    field_6_flags.Set(BaseGameObject::eIsBaseAliveGameObject_Bit6);

    return this;
}

EXPORT void BaseAliveGameObject::dtor_4080B0()
{
    SetVTable(this, 0x544000);

    BaseAliveGameObject* pField_110 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    gBaseAliveGameObjects_5C1B7C->Remove_Item(this);

    if (pField_110)
    {
        pField_110->VOnTrapDoorOpen();
        field_110_id = -1;
    }

    if (field_10A)
    {
        pResourceManager_5C1BB0->Shutdown_465610();
    }

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}


BaseGameObject* BaseAliveGameObject::vdtor_408210(signed int flags)
{
    dtor_4080B0();
    if (flags & 1)
    {
        alive_delete_free(this);
    }
    return this;
}

void BaseAliveGameObject::VRender(int** pOrderingTable)
{
    Render_424B90(pOrderingTable);
}

BaseGameObject* BaseAliveGameObject::VDestructor(signed int flags)
{
    return vdtor_408210(flags);
}

void BaseAliveGameObject::VUnPosses_408F90()
{
    vUnPosses_408F90();
}

void BaseAliveGameObject::VPossessed_408F70()
{
    vPossessed_408F70();
}

void BaseAliveGameObject::VSetMotion_4081C0(__int16 state)
{
    vSetMotion_4081C0(state);
}

void BaseAliveGameObject::VOnPathTransition_408320(__int16 cameraWorldXPos, __int16 cameraWorldYPos, CameraPos direction)
{
    vOnPathTransition_408320(cameraWorldXPos, cameraWorldYPos, direction);
}

__int16 BaseAliveGameObject::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_408730(pFrom);
}

void BaseAliveGameObject::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    vOn_TLV_Collision_4087F0(pTlv);
}

void BaseAliveGameObject::VCheckCollisionLineStillValid_408A40(__int16 distance)
{
    vCheckCollisionLineStillValid_408A40(distance);
}

BirdPortal* BaseAliveGameObject::VIntoBirdPortal_408FD0(__int16 gridBlocks)
{
    return vIntoBirdPortal_408FD0(gridBlocks);
}

void BaseAliveGameObject::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_4081F0();
}

__int16 CCSTD BaseAliveGameObject::IsInInvisibleZone_425710(BaseAliveGameObject* pObj)
{
    /* OG unused feature to always appear as if you are in an invisible zone
    if (word_5C1BE4)
    {
        return TRUE;
    }*/

    if (Event_Get_422C00(kEventAbeOhm))
    {
        return FALSE;
    }

    PSX_RECT bRect = {};
    pObj->vGetBoundingRect_424FD0(&bRect, 1);

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        bRect.x,
        bRect.y,
        bRect.w,
        bRect.h,
        TlvTypes::InvisibleZone_33);

    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::InvisibleZone_33)
        {
            if (bRect.x >= pTlv->field_8_top_left.field_0_x &&
                bRect.x <= pTlv->field_C_bottom_right.field_0_x &&
                bRect.y >= pTlv->field_8_top_left.field_2_y)
            {
                if (bRect.y <= pTlv->field_C_bottom_right.field_2_y &&
                    bRect.w >= pTlv->field_8_top_left.field_0_x &&
                    bRect.w <= pTlv->field_C_bottom_right.field_0_x &&
                    bRect.h >= pTlv->field_8_top_left.field_2_y &&
                    bRect.h <= pTlv->field_C_bottom_right.field_2_y)
                {
                    return TRUE;
                }
            }
        }

        // Check for stacked/overlaping TLV's
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv,
            FP_FromInteger(bRect.x),
            FP_FromInteger(bRect.y),
            FP_FromInteger(bRect.w),
            FP_FromInteger(bRect.h));
    }
    return FALSE;
}

// =======

void BaseAliveGameObject::vUnPosses_408F90()
{
    // Empty
}

void BaseAliveGameObject::vPossessed_408F70()
{
    // Empty
}

void BaseAliveGameObject::vSetMotion_4081C0(__int16 state)
{
    field_114_flags.Set(Flags_114::e114_MotionChanged_Bit2);
    field_106_current_motion = state;
}

EXPORT int CC sub_449880(FP scale)
{
    if (scale == FP_FromDouble(0.5))
    {
        return 30;
    }

    if (scale == FP_FromInteger(1))
    {
        return 16;
    }

    return 0;
}

void BaseAliveGameObject::vOnPathTransition_408320(__int16 cameraWorldXPos, __int16 cameraWorldYPos, CameraPos direction)
{
    const FP oldY = field_BC_ypos;
    switch (direction)
    {
    case CameraPos::eCamTop_1:
        field_B8_xpos = FP_FromInteger(cameraWorldXPos + FP_GetExponent(field_B8_xpos) % 375);
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit22_DeadMode))
        {
            ALIVE_FATAL("Impossible case BaseAliveGameObject::vsub_408320 AnimFlags::eBit22_DeadMode");
        }
        else
        {
            // TODO: This is actualy wrong !!
            const DWORD off = field_20_animation.Get_FrameHeader_40B730(-1)->field_0_frame_header_offset;
            field_BC_ypos = FP_FromInteger((*field_20_animation.field_20_ppBlock)[off + 5] + cameraWorldYPos + 236);
        }
        break;

    case CameraPos::eCamBottom_2:
        field_B8_xpos = FP_FromInteger(cameraWorldXPos + FP_GetExponent(field_B8_xpos) % 375);
        field_BC_ypos = FP_FromInteger(cameraWorldYPos + 4);
        break;

    case CameraPos::eCamLeft_3:
        field_B8_xpos = FP_FromInteger(cameraWorldXPos + (GridXMidPos_4498F0(field_CC_sprite_scale, sub_449880(field_CC_sprite_scale) - 1)));
        field_BC_ypos = FP_FromInteger(cameraWorldYPos + FP_GetExponent(field_BC_ypos) % 260);
        break;

    case CameraPos::eCamRight_4:
        field_B8_xpos = FP_FromInteger(cameraWorldXPos + GridXMidPos_4498F0(field_CC_sprite_scale, 1));
        field_BC_ypos = FP_FromInteger(cameraWorldYPos + FP_GetExponent(field_BC_ypos) % 260);
        break;

    default:
        break;
    }

    field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));

    if (sActiveHero_5C1B68 == this && 
        gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eNecrum_2 &&
        gMap_5C3030.sCurrentPathId_5C3032 == 2 &&
        (field_106_current_motion == eAbeStates::State_23_RollLoop_453A90 || field_106_current_motion == eAbeStates::State_17_CrouchIdle_456BC0))
    {
        // Yummy OWI hack - hard code Abe's location when path change to Necrum's first path after the Mines :)
        field_100_pCollisionLine = nullptr;
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        field_B8_xpos = FP_FromInteger(1011);
        field_BC_ypos = FP_FromInteger(784);
    }
    else
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        if (field_100_pCollisionLine)
        {
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos, field_BC_ypos - FP_FromInteger(40),
                field_B8_xpos, field_BC_ypos + FP_FromInteger(40),
                &pLine, &hitX, &hitY, field_D6_scale != 0 ? 1 : 16))
            {
                field_BC_ypos = hitY;
                field_100_pCollisionLine = pLine;
            }
            else
            {
                field_100_pCollisionLine = nullptr;
            }
        }
        else
        {
            field_F8_LastLineYPos = field_BC_ypos - oldY + field_F8_LastLineYPos;
            if (sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos, field_F8_LastLineYPos - FP_FromInteger(40),
                field_B8_xpos, field_F8_LastLineYPos + FP_FromInteger(40),
                &pLine, &hitX, &hitY, field_D6_scale != 0 ? 1 : 16))
            {
                field_BC_ypos = hitY - field_F8_LastLineYPos + field_BC_ypos;
            }
            else
            {
                // Not set to nullptr in this case ??
            }
        }
    }

    if (field_CC_sprite_scale == FP_FromInteger(1) && field_20_animation.field_14_scale == FP_FromDouble(0.5))
    {
        // From 0.5 to 1 scale, double velx
        field_C4_velx = field_C4_velx * FP_FromInteger(2);
    }

    if (field_CC_sprite_scale == FP_FromDouble(0.5) && field_20_animation.field_14_scale == FP_FromInteger(1))
    {
        // From 1 to 0.5 scale, halve velx
        field_C4_velx = field_C4_velx * FP_FromDouble(0.5);
    }
}

__int16 BaseAliveGameObject::vTakeDamage_408730(BaseGameObject* /*pFrom*/)
{
    // Defaults to no damage.
    return 0;
}

void BaseAliveGameObject::vOn_TLV_Collision_4087F0(Path_TLV* /*pTlv*/)
{
    // Empty
}

void BaseAliveGameObject::vCheckCollisionLineStillValid_408A40(__int16 distance)
{
    PlatformBase* pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (!field_100_pCollisionLine)
    {
        return;
    }

    if (pPlatform)
    {
        field_110_id = -1;
        pPlatform->VRemove(this);
    }

    const FP distanceFp = FP_FromInteger(distance);
    const FP yTop = field_BC_ypos + distanceFp;
    const FP yBottom = field_BC_ypos - distanceFp;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        yBottom,
        field_B8_xpos,
        yTop,
        &pLine,
        &hitX,
        &hitY,
        field_D6_scale == 1 ? 0x0F : 0xF0))
    {
        field_100_pCollisionLine = pLine;
        field_BC_ypos = hitY;

        if (pLine->field_8_type == 32 || pLine->field_8_type == 36)
        {
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);

            PSX_Point xy = { bRect.x, bRect.y };
            PSX_Point wh = { bRect.w, bRect.h };
            xy.field_2_y += 5;
            wh.field_2_y += 5;

            vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
        }
    }
    else
    {
        field_100_pCollisionLine = nullptr;
    }
    
}

BirdPortal* BaseAliveGameObject::vIntoBirdPortal_408FD0(__int16 numGridBlocks)
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eBirdPortal_99)
        {
            auto pBirdPortal = static_cast<BirdPortal*>(pObj);
            if (pBirdPortal->field_2C_xpos >= field_B8_xpos)
            {
                if (pBirdPortal->field_26_side == PortalSide::eLeft_1)
                {
                    if (pBirdPortal->field_2C_xpos - field_B8_xpos <= (ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(numGridBlocks)) &&
                        !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
                    {
                        if (FP_Abs(field_BC_ypos - pBirdPortal->field_3C_YPos) < field_CC_sprite_scale * FP_FromInteger(10) && pBirdPortal->Vsub_499430(1))
                        {
                            field_20_animation.field_C_render_layer = field_CC_sprite_scale != FP_FromInteger(1) ? 11 : 30;
                            return pBirdPortal;
                        }
                    }
                }
            }
            else if (pBirdPortal->field_26_side == PortalSide::eRight_0)
            {
                if (field_B8_xpos - pBirdPortal->field_2C_xpos <= ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(numGridBlocks))
                {
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        if (FP_Abs(field_BC_ypos - pBirdPortal->field_3C_YPos) < field_CC_sprite_scale * FP_FromInteger(10) && pBirdPortal->Vsub_499430(1))
                        {
                            field_20_animation.field_C_render_layer = field_CC_sprite_scale != FP_FromInteger(1) ? 11 : 30;
                            return pBirdPortal;
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

void BaseAliveGameObject::vOnTrapDoorOpen_4081F0()
{
    // Empty
}

signed __int16 BaseAliveGameObject::SetBaseAnimPaletteTint_425690(TintEntry * pTintArray, LevelIds level_id, int resourceID)
{
    SetTint_425600(pTintArray, level_id);

    BYTE ** pPalResource = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Palt, resourceID, 1u, 0);

    if (!pPalResource)
    {
        return 0;
    }

    field_20_animation.Load_Pal_40A530(pPalResource, 0);
    ResourceManager::FreeResource_49C330(pPalResource);
    return 1;
}

BOOL BaseAliveGameObject::Check_IsOnEndOfLine_408E90(__int16 direction, __int16 distance)
{
    // Check if distance grid blocks from current snapped X is still on the line or not, if not then we are
    // about to head off an edge.

    const FP gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    FP xLoc = {};
    if (direction == 1)
    {
        xLoc = -(gridSize * FP_FromInteger(distance));
    }
    else
    {
        xLoc = gridSize * FP_FromInteger(distance);;
    }

    const FP xPosSnapped = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_5C1128->Raycast_417A60(
        xLoc + xPosSnapped,
        field_BC_ypos - FP_FromInteger(4),
        xLoc + xPosSnapped,
        field_BC_ypos + FP_FromInteger(4),
        &pLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 0xF : 0xF0) == 0;
}

BaseAliveGameObject* BaseAliveGameObject::GetStackedSlapTarget_425290(int idToFind, Types typeToFind, FP xpos, FP ypos)
{
    const short xposD = FP_GetExponent(xpos);
    const short yposD = FP_GetExponent(ypos);

    BOOL bFound = FALSE;
    for (int idx = 0; idx < gBaseGameObject_list_BB47C4->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == typeToFind && pObj != this)
        {
            if (pObj->field_8_object_id == idToFind)
            {
                // So that we pick the one AFTER this
                bFound = TRUE;
            }
            else if (bFound)
            {
                PSX_RECT bRect = {};
                BaseAliveGameObject* pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
                pAliveObj->vGetBoundingRect_424FD0(&bRect, 1);
                // TODO: Similar to PSX_Rects_overlap_no_adjustment
                if (xposD >= bRect.x &&
                    xposD <= bRect.w &&
                    yposD >= bRect.y &&
                    yposD <= bRect.h)
                {
                    return pAliveObj;
                }
            }
        }
    }
    return nullptr;
}

EXPORT void BaseAliveGameObject::SetActiveCameraDelayedFromDir_408C40()
{
     if (sControlledCharacter_5C1B8C == this)
     {
         switch (Is_In_Current_Camera_424A70())
         {
         case CameraPos::eCamTop_1:
             if (field_C8_vely < FP_FromInteger(0))
             {
                 gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapTop_2, this, -1);
             }
             break;

         case CameraPos::eCamBottom_2:
             if (field_C8_vely > FP_FromInteger(0))
             {
                 gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapBottom_3, this, -1);
             }
             break;

         case CameraPos::eCamLeft_3:
             if (field_C4_velx < FP_FromInteger(0))
             {
                 gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapLeft_0, this, -1);
             }
             break;

         case CameraPos::eCamRight_4:
             if (field_C4_velx > FP_FromInteger(0))
             {
                 gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapRight_1, this, -1);
             }
             break;

         case CameraPos::eCamCurrent_0:
         case CameraPos::eCamNone_5:
             return;

         default: return;
         }
     }
}

__int16 BaseAliveGameObject::MapFollowMe_408D10(__int16 snapToGrid)
{
    const int xposSnapped = SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos));
    if (snapToGrid)
    {
        field_B8_xpos = FP_FromInteger(xposSnapped);
    }

    PSX_Point currentCamXY = {};
    gMap_5C3030.GetCurrentCamCoords_480680(&currentCamXY);

    // Gone off the left edge of the current screen
    if (xposSnapped < currentCamXY.field_0_x && (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) || field_C4_velx < FP_FromInteger(0)))
    {
        if (sControlledCharacter_5C1B8C == this && gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapLeft_0, this, -1))
        {
            field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
            field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
            return 1;
        }
    }
    // Gone off the right edge of the current screen
    else if (xposSnapped > currentCamXY.field_0_x + 368 && (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) || field_C4_velx > FP_FromInteger(0)))
    {
        if (sControlledCharacter_5C1B8C == this && gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapRight_1, this, -1))
        {
            field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
            field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
            return 1;
        }
    }
    return 0;
}

EXPORT BOOL BaseAliveGameObject::WallHit_408750(FP offY, FP offX)
{
    PathLine* pLine = nullptr;
    return sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos - offY,
        field_B8_xpos + offX,
        field_BC_ypos - offY,
        &pLine,
        &offX,
        &offY,
        field_D6_scale != 0 ? 6 : 96) != 0; // TODO: Enum for line types
}

BOOL BaseAliveGameObject::InAirCollision_408810(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY)
{
    field_C8_vely += field_CC_sprite_scale * velY;
    if (field_C8_vely > (field_CC_sprite_scale * FP_FromInteger(20)))
    {
        field_C8_vely = field_CC_sprite_scale * FP_FromInteger(20);
    }

    const FP oldYPos = field_BC_ypos;
    const FP oldXPos = field_B8_xpos;

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    auto bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        oldXPos,
        oldYPos,
        field_B8_xpos,
        field_BC_ypos,
        ppPathLine,
        hitX,
        hitY,
        (field_D6_scale == 1) ? 15 : 240);

    if (bCollision)
    {
        return bCollision;
    }
    
    FP velYClamped = field_C8_vely;
    if (field_4_typeId == Types::eMudokon_110 && velYClamped >= FP_FromInteger(0) && velYClamped < FP_FromInteger(4))
    {
        velYClamped = FP_FromInteger(4);
    }

    bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos + field_C4_velx,
        velYClamped + field_BC_ypos,
        ppPathLine,
        hitX,
        hitY,
        field_D6_scale != 0 ? 1 : 16);

    if (bCollision)
    {
        // TODO: Enum type for unknowns, trap doors ??
        if ((*ppPathLine)->field_8_type == 32 || (*ppPathLine)->field_8_type == 36)
        {
            return bCollision;
        }

        bCollision = FALSE;
        *ppPathLine = nullptr;
    }

    if (this != sActiveHero_5C1B68)
    {
        return bCollision;
    }

    const FP k10Scaled = field_CC_sprite_scale * FP_FromInteger(10);
    return sCollisions_DArray_5C1128->Raycast_417A60(
        oldXPos,
        oldYPos - k10Scaled,
        field_B8_xpos,
        field_BC_ypos - k10Scaled,
        ppPathLine,
        hitX,
        hitY,
        field_D6_scale != 0 ? 6 : 96);
}

BaseGameObject* BaseAliveGameObject::FindObjectOfType_425180(Types typeToFind, FP xpos, FP ypos)
{
    const int xposI = FP_GetExponent(xpos);
    const int yposI = FP_GetExponent(ypos);

    for (int i=0; i<gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == typeToFind && pObj != this)
        {
            auto pCasted = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);
            if (pCasted->field_D6_scale == field_D6_scale)
            {
                PSX_RECT bRect = {};
                pCasted->vGetBoundingRect_424FD0(&bRect, 1);

                if (xposI >= bRect.x && xposI <= bRect.w &&
                    yposI >= bRect.y && yposI <= bRect.h)
                {
                    return pObj;
                }
            }
        }
    }
    return nullptr;
}

__int16 BaseAliveGameObject::OnTrapDoorIntersection_408BA0(PlatformBase* pPlatform)
{
    PSX_RECT bRect = {};
    pPlatform->vGetBoundingRect_424FD0(&bRect, 1);

    if (FP_GetExponent(field_B8_xpos) < bRect.x ||
        FP_GetExponent(field_B8_xpos) > bRect.w ||
        FP_GetExponent(field_BC_ypos) > bRect.h)
    {
        return 1;
    }

    pPlatform->VAdd(this);

    field_110_id = pPlatform->field_8_object_id;
    return 1;
}
