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

    field_114_flags.Clear(Flags_114::e114_Bit1);
    field_114_flags.Clear(Flags_114::e114_Bit2);
    field_114_flags.Clear(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
    field_114_flags.Clear(Flags_114::e114_Bit5);
    field_114_flags.Clear(Flags_114::e114_Bit6_SetOffExplosives);
    field_114_flags.Clear(Flags_114::e114_Bit7);
    field_114_flags.Clear(Flags_114::e114_Bit8);
    field_114_flags.Clear(Flags_114::e114_Bit9);
    field_114_flags.Clear(Flags_114::e114_Bit10);
    field_114_flags.Clear(Flags_114::e114_Bit11);

    field_FC_pPathTLV = nullptr;
    field_100_pCollisionLine = nullptr;
    field_10C_health = FP_FromDouble(1.0);
    field_110_id = -1;
    field_106_current_state = 0;
    field_108_delayed_state = 0;
    field_F4 = 0;
    field_F6_anim_frame = 0;
    field_F8 = FP_FromInteger(0);
    field_10A = 0;

    gBaseAliveGameObjects_5C1B7C->Push_Back(this);

    field_6_flags.Set(BaseGameObject::eIsBaseAliveGameObject);

    return this;
}

EXPORT void BaseAliveGameObject::dtor_4080B0()
{
    SetVTable(this, 0x544000);

    BaseAliveGameObject* pField_110 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    gBaseAliveGameObjects_5C1B7C->Remove_Item(this);

    if (pField_110)
    {
        pField_110->Vnull_4081F0(); // TODO: Passes this ??
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
        Mem_Free_495540(this);
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

int BaseAliveGameObject::VUnPosses_408F90()
{
    return vnull_408F90();
}

unsigned __int16 BaseAliveGameObject::Vnull_408F70()
{
    return vnull_408F70();
}

void BaseAliveGameObject::VUpdateState_4081C0(__int16 state)
{
    vUpdateState_4081C0(state);
}

int BaseAliveGameObject::Vsub_408320(__int16 a2, __int16 a3, int a4)
{
    return vsub_408320(a2, a3, a4);
}

__int16 BaseAliveGameObject::VTakeDamage_408730(BaseAnimatedWithPhysicsGameObject* pFrom)
{
    return vTakeDamage_408730(pFrom);
}

signed __int16 BaseAliveGameObject::VOn_TLV_Collision_4087F0(Path_TLV* pTlv)
{
    return vOn_TLV_Collision_4087F0(pTlv);
}

char BaseAliveGameObject::Vsub_408A40(__int16 a2)
{
    return vsub_408A40(a2);
}

BaseGameObject* BaseAliveGameObject::Vsub_408FD0(__int16 a2)
{
    return vsub_408FD0(a2);
}

BaseGameObject* BaseAliveGameObject::Vnull_4081F0()
{
    return vnull_4081F0();
}

// =======

int BaseAliveGameObject::vnull_408F90()
{
    // Empty
    return 0;
}

unsigned __int16 BaseAliveGameObject::vnull_408F70()
{
    // Empty
    return 0;
}

void BaseAliveGameObject::vUpdateState_4081C0(__int16 state)
{
    field_114_flags.Set(Flags_114::e114_Bit2);
    field_106_current_state = state;
}

int BaseAliveGameObject::vsub_408320(__int16 /*a2*/, __int16 /*a3*/, int /*a4*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 BaseAliveGameObject::vTakeDamage_408730(BaseAnimatedWithPhysicsGameObject* /*pFrom*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

signed __int16 BaseAliveGameObject::vOn_TLV_Collision_4087F0(Path_TLV* /*pTlv*/)
{
    // Empty
    return 0;
}

char BaseAliveGameObject::vsub_408A40(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

BaseGameObject* BaseAliveGameObject::vsub_408FD0(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

BaseGameObject* BaseAliveGameObject::vnull_4081F0()
{
    // Empty
    return nullptr;
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

EXPORT void BaseAliveGameObject::sub_408C40()
{
     if (sControlledCharacter_5C1B8C == this)
     {
         switch (Is_In_Current_Camera_424A70())
         {
         case Map::CameraPos::eCamTop:
             if (field_C8_vely < FP_FromInteger(0))
             {
                 gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapTop, this, -1);
             }
             break;
         case Map::CameraPos::eCamBottom:
             if (field_C8_vely > FP_FromInteger(0))
             {
                 gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapBottom, this, -1);
             }
             break;
         case Map::CameraPos::eCamLeft:
             if (field_C4_velx < FP_FromInteger(0))
             {
                 gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapLeft, this, -1);
             }
             break;
         case Map::CameraPos::eCamRight:
             if (field_C4_velx > FP_FromInteger(0))
             {
                 gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapRight, this, -1);
             }
             break;
         case Map::CameraPos::eCamCurrent:
         case Map::CameraPos::eCamNone:
             return;
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
        if (sControlledCharacter_5C1B8C == this && gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapLeft, this, -1))
        {
            field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
            field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
            return 1;
        }
    }
    // Gone off the right edge of the current screen
    else if (xposSnapped > currentCamXY.field_0_x + 368 && (!(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) || field_C4_velx > FP_FromInteger(0)))
    {
        if (sControlledCharacter_5C1B8C == this && gMap_5C3030.SetActiveCameraDelayed_4814A0(Map::MapDirections::eMapRight, this, -1))
        {
            field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
            field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;
            return 1;
        }
    }
    return 0;
}

EXPORT int BaseAliveGameObject::Raycast_408750(FP offY, FP offX)
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

__int16 BaseAliveGameObject::InAirCollision_408810(PathLine** ppPathLine, FP* hitX, FP* hitY, FP velY)
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

    __int16 bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
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

__int16 BaseAliveGameObject::OnTrapDoorIntersection_408BA0(BaseAliveGameObject* /*pOther*/)
{
    NOT_IMPLEMENTED();
    return 0;
}
