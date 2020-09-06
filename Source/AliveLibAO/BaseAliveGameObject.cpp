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

START_NS_AO

void BaseAliveGameObject_ForceLink() {}

ALIVE_VAR(1, 0x4FC8A0, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0, nullptr);

EXPORT int CC Grid_SnapX_41FAA0(FP /*scale*/, int /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}


EXPORT FP CC CamX_VoidSkipper_418590(FP xpos, FP xvel, __int16 xMargin, WORD* pResult)
{
    // Disabled until #535 is fixed
    NOT_IMPLEMENTED();

    const FP v1 = xpos - FP_FromInteger(256);

    const FP xDiv = FP_FromInteger(FP_GetExponent(v1) / 512);
    const FP xMod = FP_FromInteger(FP_GetExponent(v1) % 512);

    const int xDivEven = FP_GetExponent(xDiv) % 2;

    FP result = {};
    if ((!xDivEven || xMod >= FP_FromInteger(xMargin - 512)) &&
        (xDivEven || xMod <= FP_FromInteger(xMargin + 368)))
    {
        *pResult = 0;
        result = xpos;
    }
    else if (xvel <= FP_FromInteger(0))
    {
        *pResult = 1;
        result = FP_FromInteger((xDiv.fpValue << 9) + xMargin + 112);
    }
    else
    {
        if (xDivEven)
        {
            *pResult = 2;
            result = FP_FromInteger((xDiv.fpValue << 9) - xMargin + 768);
        }
        else
        {
            *pResult = 2;
            result = FP_FromInteger((xDiv.fpValue << 9) - xMargin + 1280);
        }
    }
    return result;
}

EXPORT FP CC CamY_VoidSkipper_418690(FP /*ypos*/, FP /*yvel*/, __int16 /*yMargin*/, WORD* /*pResult*/)
{
    NOT_IMPLEMENTED();
    return {};
}

BaseAliveGameObject *BaseAliveGameObject::ctor_401090()
{
    ctor_417C10();
    
    SetVTable(this, 0x4BA000);

    field_10A_flags.Clear(Flags_10A::e10A_Bit1_Can_Be_Possessed);
    field_10A_flags.Clear(Flags_10A::e10A_Bit2_bPossesed);
    field_10A_flags.Clear(Flags_10A::e10A_Bit3);
    field_10A_flags.Clear(Flags_10A::e10A_Bit4_SetOffExplosives);
    field_10A_flags.Clear(Flags_10A::e10A_Bit5);
    field_10A_flags.Clear(Flags_10A::e10A_Bit6);

    field_F0_pTlv = nullptr;
    field_F8_pLiftPoint = nullptr;
    field_F4_pLine = nullptr;
    field_100_health = FP_FromInteger(1);
    field_106_shot = 0;
    field_108_bMotionChanged = 0;
    field_EC = 0;
    field_FC_current_motion = 0;
    field_FE_next_state = 0;
    field_E4 = 0;
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
        ResourceManager::CancelPendingResourcesFor_41EA60(this);
    }

    return dtor_417D10();
}

BaseGameObject* BaseAliveGameObject::VDestructor(signed int flags)
{
    return Vdtor_402540(flags);
}

BaseGameObject* BaseAliveGameObject::Vdtor_402540(signed int flags)
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

void BaseAliveGameObject::VSetMotion(__int16 state)
{
    VSetMotion_402520(state);
}

void BaseAliveGameObject::VSetXSpawn(__int16 camWorldX, int screenXPos)
{
    VSetXSpawn_401150(camWorldX, screenXPos);
}

void BaseAliveGameObject::VSetYSpawn(int camWorldY, __int16 bLeft)
{
    VSetYSpawn_401380(camWorldY, bLeft);
}

void BaseAliveGameObject::VOnPathTransition(__int16 camWorldX, int camWorldY, CameraPos direction)
{
    VOnPathTransition_401470(camWorldX, camWorldY, direction);
}

__int16 BaseAliveGameObject::VTakeDamage(BaseGameObject* /*pFrom*/)
{
    // Empty
    return 0;
}

void BaseAliveGameObject::VOn_TLV_Collision(Path_TLV* /*pTlv*/)
{
    // Empty
}

void BaseAliveGameObject::VCheckCollisionLineStillValid(int distance)
{
    VCheckCollisionLineStillValid_401A90(distance);
}

void BaseAliveGameObject::VOnTrapDoorOpen()
{
    // Empty
}

void BaseAliveGameObject::VCheckCollisionLineStillValid_401A90(int distance)
{
    if (field_F4_pLine)
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};

        const int mask = field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70;
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
                if (pLine->field_8_type == 32 || pLine->field_8_type == 36)
                {
                    field_F8_pLiftPoint->field_C_refCount--;
                    field_F8_pLiftPoint = nullptr;

                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);
                    bRect.y += 5;
                    bRect.h += 5;

                    VOnCollisionWith(
                        { bRect.x, bRect.y },
                        { bRect.w, bRect.h },
                        ObjListPlatforms_50766C,
                        1,
                        (TCollisionCallBack)&BaseAliveGameObject::OnTrapDoorIntersection_401C10);
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

BirdPortal* BaseAliveGameObject::IntoBirdPortal_402350(__int16 distance)
{
    for (int i=0; i<gBaseGameObject_list_9F2DF0->Size(); i++)
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
                                if (pPortal->Vsub_4533E0(1))
                                {
                                    field_10_anim.field_C_layer = field_BC_sprite_scale != FP_FromInteger(1) ? 11 : 30;
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
                                if (pPortal->Vsub_4533E0(1))
                                {
                                    field_10_anim.field_C_layer = field_BC_sprite_scale != FP_FromInteger(1) ? 11 : 30;
                                    return pPortal;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void BaseAliveGameObject::VOnPathTransition_401470(__int16 /*camWorldX*/, int /*camWorldY*/, CameraPos /*direction*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAliveGameObject::MapFollowMe_401D30(__int16 /*snapToGrid*/)
{
    NOT_IMPLEMENTED();
    return 0;
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

signed __int16 BaseAliveGameObject::OnTrapDoorIntersection_401C10(PlatformBase* pPlatform)
{
    PSX_RECT rect = {};

    pPlatform->VGetBoundingRect(&rect, 1);
    if ( FP_GetExponent(field_A8_xpos) < rect.x ||  FP_GetExponent(field_A8_xpos) > rect.w || FP_GetExponent(field_AC_ypos) > rect.h)
    {
        return 1;
    }

    field_F8_pLiftPoint = pPlatform;
    field_F8_pLiftPoint->VAdd(this);
    field_F8_pLiftPoint->field_C_refCount++;

    return 1;
}

__int16 BaseAliveGameObject::WallHit_401930(FP offY, FP offX)
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
        field_BC_sprite_scale != FP_FromDouble(0.5) ? 6 : 0x60) != 0;
}

__int16 BaseAliveGameObject::InAirCollision_4019C0(PathLine** ppLine, FP* hitX, FP* hitY, FP vely)
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

void BaseAliveGameObject::VSetXSpawn_401150(__int16 camWorldX, int screenXPos)
{
    const FP old_x = field_A8_xpos;
    const FP old_y = field_AC_ypos;

    field_A8_xpos = FP_FromInteger(camWorldX + GridXMidPos_41FA60(field_BC_sprite_scale, screenXPos));

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

void BaseAliveGameObject::VSetYSpawn_401380(int camWorldY, __int16 bLeft)
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

__int16 BaseAliveGameObject::IsBeeSwarmChasingMe_4022B0()
{
    for (int i=0; i<gBaseGameObject_list_9F2DF0->Size(); i++)
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

void BaseAliveGameObject::VSetMotion_402520(__int16 state)
{
    field_108_bMotionChanged = TRUE;
    field_FC_current_motion = state;
}

void BaseAliveGameObject::sub_4020D0()
{
    auto pPathTrans = static_cast<Path_ChangeTLV*>(gMap_507BA8.TLV_Get_At_446260(
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        FP_GetExponent(field_A8_xpos),
        FP_GetExponent(field_AC_ypos),
        TlvTypes::PathTransition_1));

    if (pPathTrans)
    {
        if (pPathTrans->field_22_scale == 1)
        {
            if (field_BC_sprite_scale != FP_FromDouble(0.5))
            {
                field_BC_sprite_scale = FP_FromDouble(0.5);
                field_C6_scale = 0;
                field_B4_velx = (field_B4_velx * FP_FromDouble(0.5));
            }
        }
        else if (pPathTrans->field_22_scale == 0)
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
    const int xpos_int = FP_GetExponent(xpos);
    const int ypos_int = FP_GetExponent(ypos);

    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
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

            if (xpos_int >= bRect.x &&
                xpos_int <= bRect.w &&
                ypos_int >= bRect.y &&
                ypos_int <= bRect.h)
            {
                return pObj;
            }
        }
    }
    return nullptr;
}

END_NS_AO
