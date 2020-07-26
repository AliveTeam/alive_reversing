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

START_NS_AO

void BaseAliveGameObject_ForceLink() {}

FP CC ScaleToGridSize_41FA30(FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return {};
}

ALIVE_VAR(1, 0x4FC8A0, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0, nullptr);

BaseAliveGameObject *BaseAliveGameObject::ctor_401090()
{
    ctor_417C10();
    
    SetVTable(this, 0x4BA000);

    field_10A_flags.Clear(Flags_10A::e10A_Bit1_Can_Be_Possessed);
    field_10A_flags.Clear(Flags_10A::e10A_Bit2);
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
    field_EC_oldY = 0;
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

void BaseAliveGameObject::VCheckCollisionLineStillValid_401A90(int /*distance*/)
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

END_NS_AO
