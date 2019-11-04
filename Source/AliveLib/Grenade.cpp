#include "stdafx.h"
#include "Grenade.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"

Grenade* Grenade::ctor_447F70(FP xpos, FP ypos, __int16 numGrenades, __int16 a5, __int16 a6, BaseGameObject* pOwner)
{
    ctor_408240(0);
    SetVTable(this, 0x5456E0);

    field_11A_bDead = 0;

    Init_448110(xpos, ypos);

    field_118_count = numGrenades;
    field_132 = a5;

    if (a5)
    {
        field_120_state = GrenadeStates::eState_8;
        field_11A_bDead = 1;
    }
    else if (numGrenades)
    {
        field_120_state = GrenadeStates::eState_0_FallingToBeCollected;
    }
    else
    {
        field_120_state = GrenadeStates::eState_3_CountingDown;
        field_122_explode_timer = 90;
    }

    field_138_pOwner = pOwner;
    field_130 = a6;

    return this;
}

BaseGameObject* Grenade::VDestructor(signed int flags)
{
    return vdtor_4480E0(flags);
}

void Grenade::VScreenChanged()
{
    vScreenChanged_449140();
}

int Grenade::VGetSaveState(BYTE* pSaveBuffer)
{
    return GetSaveState_4496B0(reinterpret_cast<Grenade_SaveState*>(pSaveBuffer));
}

void Grenade::VUpdate()
{
    vUpdate_4489C0();
}

void Grenade::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_449390();
}

void Grenade::VThrow_49E460(FP velX, FP velY)
{
    vThrow_4482E0(velX, velY);
}

BOOL Grenade::VCanThrow_49E350()
{
    return vCanThrow_49A5F0();
}

BOOL Grenade::VIsFalling_49E330()
{
    return vIsFalling_49A610();
}

void Grenade::VTimeToExplodeRandom_411490()
{
    vTimeToExplodeRandom_4480A0();
}

int CC Grenade::CreateFromSaveState_449410(const BYTE* pBuffer)
{
    auto pState = reinterpret_cast<const Grenade_SaveState*>(pBuffer);
    auto pGrenade = alive_new<Grenade>();

    pGrenade->ctor_447F70(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A, 0, 0, 0);
    pGrenade->field_C_objectId = pState->field_4_obj_id;

    pGrenade->field_B8_xpos = pState->field_8_xpos;
    pGrenade->field_BC_ypos = pState->field_C_ypos;

    pGrenade->field_E4_collection_rect.x = pGrenade->field_B8_xpos - (ScaleToGridSize_4498B0(pGrenade->field_CC_sprite_scale) / FP_FromInteger(2));
    pGrenade->field_E4_collection_rect.w = pGrenade->field_B8_xpos + (ScaleToGridSize_4498B0(pGrenade->field_CC_sprite_scale) / FP_FromInteger(2));
    pGrenade->field_E4_collection_rect.h = pGrenade->field_BC_ypos;
    pGrenade->field_E4_collection_rect.y = pGrenade->field_BC_ypos - ScaleToGridSize_4498B0(pGrenade->field_CC_sprite_scale);

    pGrenade->field_C4_velx = pState->field_10_velx;
    pGrenade->field_C8_vely = pState->field_14_vely;
    pGrenade->field_C0_path_number = pState->field_1C_path_number;
    pGrenade->field_C2_lvl_number = pState->field_1E_lvl_number;
    pGrenade->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pGrenade->field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Grenade_SaveState::eBit3_bLoop));
    pGrenade->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Grenade_SaveState::eBit1_bRender));

    pGrenade->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Grenade_SaveState::eBit2_bDrawable));
    pGrenade->field_6_flags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Grenade_SaveState::eBit4_bInteractive));

    pGrenade->field_114_flags.Set(Flags_114::e114_Bit9);
    pGrenade->field_104_collision_line_type = pState->field_28_line_type;
    pGrenade->field_118_count = pState->field_2A;
    pGrenade->field_120_state = pState->field_2C_state;
    pGrenade->field_124 = pState->field_2E;
    pGrenade->field_128_xpos = pState->field_34_xpos;
    pGrenade->field_12C_ypos = pState->field_38_ypos;

    pGrenade->field_130 = pState->field_20_flags.Get(Grenade_SaveState::eBit5);
    pGrenade->field_134_bExplodeNow = pState->field_20_flags.Get(Grenade_SaveState::eBit6_bExplodeNow);
    pGrenade->field_132 = pState->field_20_flags.Get(Grenade_SaveState::eBit7);

    pGrenade->field_122_explode_timer = pState->field_30_explode_timer;

    return sizeof(Grenade_SaveState);
}

int Grenade::GetSaveState_4496B0(Grenade_SaveState* pState)
{
    pState->field_0_type = Types::eGrenade_65;
    
    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;
    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_1C_path_number = field_C0_path_number;
    pState->field_1E_lvl_number = field_C2_lvl_number;
    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_20_flags.Set(Grenade_SaveState::eBit3_bLoop, field_20_animation.field_4_flags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Grenade_SaveState::eBit2_bDrawable, field_6_flags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Grenade_SaveState::eBit1_bRender, field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render));
    pState->field_20_flags.Set(Grenade_SaveState::eBit4_bInteractive, field_6_flags.Get(BaseGameObject::eInteractive_Bit8));

    if (field_100_pCollisionLine)
    {
        pState->field_28_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = field_110_id;
    pState->field_2A = field_118_count;
    pState->field_2C_state = field_120_state;
    pState->field_2E = field_124;
    pState->field_34_xpos = field_128_xpos;
    pState->field_38_ypos = field_12C_ypos;

    pState->field_20_flags.Set(Grenade_SaveState::eBit5, field_130 & 1);
    pState->field_20_flags.Set(Grenade_SaveState::eBit6_bExplodeNow, field_134_bExplodeNow & 1);
    pState->field_20_flags.Set(Grenade_SaveState::eBit7, field_132 & 1);

    pState->field_30_explode_timer = field_122_explode_timer;

    return sizeof(Grenade_SaveState);
}

void Grenade::vScreenChanged_449140()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    field_11C_explosion_id = -1;
}

void Grenade::Init_448110(FP xpos, FP ypos)
{
    field_4_typeId = Types::eGrenade_65;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 6005, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kGrenadeResID);
    Animation_Init_424E10(1252, 17, 11, ppRes, 1, 1);
    field_6_flags.Clear(BaseGameObject::eInteractive_Bit8);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_20_animation.field_B_render_mode = 0;

    field_11C_explosion_id = -1;
    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_128_xpos = xpos;
    field_12C_ypos = ypos;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_118_count = 0;
    field_124 = 0;
    field_134_bExplodeNow = FALSE;
}

void Grenade::vOnTrapDoorOpen_449390()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        if (field_120_state == GrenadeStates::eState_1_WaitToBeCollected || field_120_state == GrenadeStates::eState_2)
        {
            field_120_state = GrenadeStates::eState_0_FallingToBeCollected;
        }
        else if (field_120_state != GrenadeStates::eState_6_WaitForExplodeEnd)
        {
            field_120_state = GrenadeStates::eState_4_Falling;
        }
    }
}

void Grenade::vThrow_4482E0(FP velX, FP velY)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_C4_velx = velX;
    field_C8_vely = velY;

    if (field_132 == 0)
    {
        if (field_118_count == 0)
        {
            field_120_state = GrenadeStates::eState_4_Falling;
        }
    }
    else
    {
        field_120_state = GrenadeStates::eState_9_FallingBlowUpOnGround;
    }
}

BOOL Grenade::vCanThrow_49A5F0()
{
    return FALSE;
}

BOOL Grenade::vIsFalling_49A610()
{
    return FALSE;
}

void Grenade::vTimeToExplodeRandom_4480A0()
{
    field_122_explode_timer -= Math_NextRandom() % 16;
}

void Grenade::BlowUp_4483C0(__int16 bSmallExplosion)
{
    auto pExplosion = alive_new<Explosion>();
    if (pExplosion)
    {
        pExplosion->ctor_4A1200(
            field_B8_xpos,
            field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
            field_CC_sprite_scale,
            bSmallExplosion);
        field_11C_explosion_id = pExplosion->field_8_object_id;
    }

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_120_state = GrenadeStates::eState_6_WaitForExplodeEnd;

    auto pGibs = alive_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_40FB40(5, field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, bSmallExplosion);
    }
}

void Grenade::dtor_448220()
{
    SetVTable(this, 0x5456E0);

    if (!gInfiniteGrenades_5C1BDE && !field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove_49AA00(field_118_count >= 1 ? field_118_count : 1);
        }
    }

    dtor_4080B0();
}

Grenade* Grenade::vdtor_4480E0(signed int flags)
{
    dtor_448220();

    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

signed __int16 Grenade::TimeToBlowUp_448350()
{
    if (!(--field_122_explode_timer % 16))
    {
        SFX_Play_46FA90(2, 0);
    }

    if (field_122_explode_timer != 0)
    {
        return 0;
    }

    BlowUp_4483C0(0);
    return 1;
}

void Grenade::vUpdate_4489C0()
{
    auto pExplosion = sObjectIds_5C1B70.Find_449CF0(field_11C_explosion_id);


    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_114_flags.Get(Flags_114::e114_Bit9))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(10),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(10),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
        }
        field_104_collision_line_type = 0;
    }

    switch (field_120_state)
    {
    case GrenadeStates::eState_0_FallingToBeCollected:
        if (!InTheAir_4484F0(FALSE))
        {
            field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_E4_collection_rect.w = field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_E4_collection_rect.h = field_BC_ypos;

            field_6_flags.Set(BaseGameObject::eInteractive_Bit8);
            field_120_state = GrenadeStates::eState_1_WaitToBeCollected;
        }
        break;

    case GrenadeStates::eState_1_WaitToBeCollected:
        if (FP_Abs(field_C4_velx) >= FP_FromInteger(1))
        {
            if (field_C4_velx <= FP_FromInteger(0))
            {
                field_C4_velx = field_C4_velx + FP_FromDouble(0.01);
            }
            else
            {
                field_C4_velx = field_C4_velx - FP_FromDouble(0.01);
            }

            auto oldLine = field_100_pCollisionLine;
            field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
            if (field_110_id != -1 && field_100_pCollisionLine != oldLine)
            {
                VOnTrapDoorOpen();
            }

            if (!field_100_pCollisionLine)
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
                field_120_state = GrenadeStates::eState_0_FallingToBeCollected;
            }
        }
        else if (abs(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)) - FP_GetExponent(field_B8_xpos)) > 1)
        {
            auto oldLine = field_100_pCollisionLine;
            field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
            if (field_110_id != -1 && field_100_pCollisionLine != oldLine)
            {
                VOnTrapDoorOpen();
            }

            if (!field_100_pCollisionLine)
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
                field_120_state = GrenadeStates::eState_4_Falling;
            }
        }
        else
        {
            field_C4_velx = FP_FromInteger(0);

            field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_E4_collection_rect.w = field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_E4_collection_rect.h = field_BC_ypos;

            field_6_flags.Set(BaseGameObject::eInteractive_Bit8);
            field_120_state = GrenadeStates::eState_2;
        }
        break;

    case GrenadeStates::eState_3_CountingDown:
        TimeToBlowUp_448350();
        break;

    case GrenadeStates::eState_4_Falling:
        if (InTheAir_4484F0(FALSE))
        {
            if (!TimeToBlowUp_448350())
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);

                const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
                const PSX_Point wh = { bRect.w, static_cast<short>(bRect.h + 5) };
                vOnCollisionWith_424EE0(xy, wh, gBaseGameObject_list_BB47C4, 1, (TCollisionCallBack)&Grenade::OnCollision_BounceOff_448F90);
            }
        }
        else
        {
            field_120_state = GrenadeStates::eState_5_HitGround;
        }
        break;

    case GrenadeStates::eState_5_HitGround:
        field_C4_velx = FP_FromRaw(field_C4_velx.fpValue / 2);

        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (!field_100_pCollisionLine)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
            field_120_state = GrenadeStates::eState_4_Falling;
        }

        TimeToBlowUp_448350();
        break;

    case GrenadeStates::eState_6_WaitForExplodeEnd:
        if (!pExplosion || pExplosion->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_120_state = GrenadeStates::eState_7_Exploded;
            field_11C_explosion_id = -1;
        }
        break;

    case GrenadeStates::eState_7_Exploded:
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        break;

    case GrenadeStates::eState_9_FallingBlowUpOnGround:
    {
        InTheAir_4484F0(TRUE);

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);

        const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
        const PSX_Point wh = { bRect.w, static_cast<short>(bRect.h + 5) };
        vOnCollisionWith_424EE0(xy, wh, gBaseGameObject_list_BB47C4, 1, (TCollisionCallBack)&Grenade::OnCollision_InstantExplode_4490D0);

        if (field_134_bExplodeNow)
        {
            BlowUp_4483C0(TRUE);
        }
    }
    break;
    }
}

__int16 Grenade::InTheAir_4484F0(__int16 blowUpOnFloorTouch)
{
    sObjectIds_5C1B70.Find_449CF0(field_110_id);

    field_128_xpos = field_B8_xpos;
    field_12C_ypos = field_BC_ypos;

    field_C8_vely += FP_FromInteger(1);
    field_B8_xpos += field_C4_velx;
    field_BC_ypos = field_C8_vely + field_BC_ypos;

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
        nullptr,
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos);

    while (field_FC_pPathTLV)
    {
        if (field_FC_pPathTLV->field_4_type == TlvTypes::DeathDrop_4)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            return 1;
        }

        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            field_FC_pPathTLV,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_128_xpos,
        field_12C_ypos,
        field_B8_xpos,
        field_BC_ypos,
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale == 0 ? 0x10 : 0x01) == 1)
    {
        if (field_C8_vely <= FP_FromInteger(0))
        {
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
            short vol = 75 - 20 * field_124;
            if (vol < 40)
            {
                vol = 40;
            }
            SFX_Play_46FA90(68u, vol);
            Event_Broadcast_422BC0(kEventNoise, this);
            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            Event_Broadcast_422BC0(kEventSpeaking, this);
        }
        else
        {
            if (field_C8_vely < FP_FromInteger(1))
            {
                if (field_110_id == -1)
                {
                    AddToPlatform_449210();
                }
                return 0;
            }

            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
            field_C4_velx = (field_C4_velx / FP_FromInteger(2));

            if (blowUpOnFloorTouch)
            {
                field_134_bExplodeNow = TRUE;
                return 1;
            }

            if (field_124 <= 4)
            {
                short vol = 75 - 20 * field_124;
                if (vol < 40)
                {
                    vol = 40;
                }

                SFX_Play_46FA90(68u, vol);
                ++field_124;

                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }
        }
    }

    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_128_xpos,
        field_12C_ypos,
        field_B8_xpos,
        field_BC_ypos,
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale == 0 ? 0x60 : 0x06) == 1)
    {
        switch (field_100_pCollisionLine->field_8_type)
        {
        case 1u:
        case 5u:
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_BC_ypos = hitY;
                field_B8_xpos = hitX;
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                short vol = 75 - 20 * field_124;
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(68u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }
            break;

        case 2u:
        case 6u:
            if (field_C4_velx > FP_FromInteger(0))
            {
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                short vol = 75 - 20 * field_124;
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(68u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }
            break;

        default:
            return 1;
        }
    }

    return 1;
}

void Grenade::AddToPlatform_449210()
{
    BaseAddToPlatform([](Types type) { return type == Types::eLiftPoint_78 || type == Types::eTrapDoor_142; });
}

__int16 Grenade::OnCollision_BounceOff_448F90(BaseGameObject* pHit)
{
    if (!pHit->field_6_flags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);
    if (pHit2->field_CC_sprite_scale != field_CC_sprite_scale)
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pHit2->vGetBoundingRect_424FD0(&bRect, 1);

    if (field_128_xpos < FP_FromInteger(bRect.x + 12) || field_128_xpos > FP_FromInteger(bRect.w - 12))
    {
        field_B8_xpos = field_128_xpos;
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    }
    else
    {
        field_BC_ypos = field_12C_ypos;
        field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
    }

    pHit2->VOnThrowableHit(this);

    SFX_Play_46FA90(24u, 0);
    return 0;
}

__int16 Grenade::OnCollision_InstantExplode_4490D0(BaseGameObject* pHit)
{
    if (pHit == field_138_pOwner)
    {
        // Don't do anything if hit the person who threw it
        return 1;
    }

    if (pHit->field_6_flags.Get(BaseGameObject::eCanExplode_Bit7) && static_cast<BaseAliveGameObject*>(pHit)->field_CC_sprite_scale == field_CC_sprite_scale)
    {
        field_134_bExplodeNow = 1;
        return 0;
    }

    return 1;
}
