#include "stdafx.h"
#include "Meat.hpp"
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
#include "Particle.hpp"

Meat* Meat::ctor_4694A0(FP xpos, FP ypos, __int16 count)
{
    ctor_408240(0);
    SetVTable(this, 0x546040);

    field_11A_bDead = 0;
    field_4_typeId = Types::eMeat_84;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kMeatResID, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMeatResID);
    Animation_Init_424E10(488, 17, 9, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_128_timer = 0;
    field_6_flags.Clear(BaseGameObject::eInteractive_Bit8);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_12C_deadtimer = sGnFrame_5C1B84 + 600;
    field_130_pLine = 0;
    field_118_count = count;
    field_11C_state = MeatStates::State_0;

    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

BaseGameObject* Meat::VDestructor(signed int flags)
{
    return vdtor_4696C0(flags);
}

void Meat::VUpdate()
{
    vUpdate_469BA0();
}

void Meat::VScreenChanged()
{
    vScreenChanged_46A130();
}

void Meat::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_46A2E0();
}

void Meat::VThrow_49E460(FP velX, FP velY)
{
    vThrow_469790(velX, velY);
}

BOOL Meat::VCanThrow_49E350()
{
    return vCanThrow_469680();
}

BOOL Meat::VIsFalling_49E330()
{
    return vIsFalling_469660();
}

void Meat::VTimeToExplodeRandom_411490()
{
    // TODO
}

__int16 Meat::VGetCount_448080()
{
    return vGetCount_46A350();
}

BOOL Meat::VCanEatMe_4696A0()
{
    return vCanEatMe_4696A0();
}

void Meat::vScreenChanged_46A130()
{
    if (gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path ||
        gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Meat::AddToPlatform_46A170()
{
    // TODO: OG bug - why doesn't meat check for trap doors ??
    BaseAddToPlatform([](Types type) { return type == Types::eLiftPoint_78; });
}

void Meat::vOnTrapDoorOpen_46A2E0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        if (field_11C_state == MeatStates::State_3_BecomeAPickUp || field_11C_state == MeatStates::State_4_WaitForPickUp)
        {
            field_11C_state = MeatStates::State_1_Idle;
        }
    }
}

BOOL Meat::vIsFalling_469660()
{
    return field_11C_state == MeatStates::State_5_Fall;
}

BOOL Meat::vCanThrow_469680()
{
    return field_11C_state == MeatStates::State_2_BeingThrown;
}

void Meat::dtor_4696F0()
{
    SetVTable(this, 0x546040);
    if (!field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove_49AA00(field_118_count >= 1 ? field_118_count : 1);
        }
    }
    dtor_4080B0();
}

Meat* Meat::vdtor_4696C0(signed int flags)
{
    dtor_4696F0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Meat::vThrow_469790(FP velX, FP velY)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_C4_velx = velX;
    field_C8_vely = velY;

    if (field_118_count == 0)
    {
        field_11C_state = MeatStates::State_2_BeingThrown;
    }
    else
    {
        field_11C_state = MeatStates::State_1_Idle;
    }
}

__int16 Meat::vGetCount_46A350()
{
    if (field_11C_state == MeatStates::State_4_WaitForPickUp && field_118_count == 0)
    {
        return 1;
    }

    return field_118_count;
}

void Meat::InTheAir_4697E0()
{
    field_120_xpos = field_B8_xpos;
    field_124_ypos = field_BC_ypos;

    if (field_C8_vely < FP_FromInteger(18))
    {
        field_C8_vely += FP_FromInteger(1);
    }

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(field_120_xpos, field_124_ypos, field_B8_xpos, field_BC_ypos, &field_130_pLine, &hitX, &hitY, field_D6_scale == 0 ? 0xF0 : 0xF) == 1)
    {
        switch (field_130_pLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            if (field_C8_vely > FP_FromInteger(0))
            {
                field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(hitX)));
                field_BC_ypos = hitY;
                field_11C_state = MeatStates::State_3_BecomeAPickUp;
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                SFX_Play_46FBA0(SoundEffect::MeatBounce_36, 0, -650);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                AddToPlatform_46A170();
            }
            break;

        case 1u:
        case 5u:
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                field_C4_velx = (-field_C4_velx / FP_FromInteger(4));
                SFX_Play_46FBA0(SoundEffect::MeatBounce_36, 0, -650);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                if (field_C8_vely < FP_FromInteger(0))
                {
                    field_C8_vely = FP_FromInteger(0);
                }
            }
            field_130_pLine = nullptr;
            break;

        case 2u:
        case 6u:
            if (field_C4_velx > FP_FromInteger(0))
            {
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                field_C4_velx = (-field_C4_velx / FP_FromInteger(4));
                SFX_Play_46FBA0(SoundEffect::MeatBounce_36, 0, -650);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                if (field_C8_vely < FP_FromInteger(0))
                {
                    field_C8_vely = FP_FromInteger(0);
                }
            }
            field_130_pLine = nullptr;
            break;

        case 3u:
        case 7u:
            if (field_C8_vely < FP_FromInteger(0))
            {
                field_B8_xpos = hitX;
                field_BC_ypos = hitY + FP_FromInteger(1);
                field_C8_vely = FP_FromInteger(0);
                SFX_Play_46FBA0(SoundEffect::MeatBounce_36, 0, -650);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
            break;
        }
    }
}

__int16 Meat::OnCollision_469FF0(BaseGameObject* pHit)
{
    // TODO: Check if pHit type is correct for all throwables

    if (!pHit->field_6_flags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    if (pHit->field_4_typeId == Types::eMine_88 || pHit->field_4_typeId == Types::eUXB_143 || pHit->field_4_typeId == Types::eTimedMine_or_MovingBomb_10)
    {
        return 1;
    }
    
    PSX_RECT bRect = {};
    static_cast<BaseAliveGameObject*>(pHit)->vGetBoundingRect_424FD0(&bRect, 1);

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
    {
        field_B8_xpos -= field_C4_velx;
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    }
    else
    {
        field_BC_ypos -= field_C8_vely;
        field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
    }
    
    static_cast<BaseAliveGameObject*>(pHit)->VOnThrowableHit(this);

    SFX_Play_46FBA0(SoundEffect::MeatBounce_36, 0, -650);

    return 0;
}

void Meat::vUpdate_469BA0()
{
    auto v2 = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    if (sNum_CamSwappers_5C1B66 == 0)
    {
        if (Event_Get_422C00(kEventDeathReset))
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        switch (field_11C_state)
        {
        case MeatStates::State_1_Idle:
            InTheAir_4697E0();
            break;

        case MeatStates::State_2_BeingThrown:
        {
            InTheAir_4697E0();
            PSX_RECT bRect = {};
            vGetBoundingRect_424FD0(&bRect, 1);
            const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
            const PSX_Point wh = { bRect.w, static_cast<short>(bRect.h + 5) };
            vOnCollisionWith_424EE0(
                xy,
                wh,
                gBaseGameObject_list_BB47C4,
                1,
                (TCollisionCallBack)&Meat::OnCollision_469FF0);

            // TODO: OG bug - why only checking for out of the bottom of the map?? Nades check for death object - probably should check both
            if (field_BC_ypos > FP_FromInteger(gMap_5C3030.field_D4_ptr->field_6_bBottom))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
            break;

        case MeatStates::State_3_BecomeAPickUp:
            if (FP_Abs(field_C4_velx) < FP_FromInteger(1))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit8_Loop);
            }

            if (FP_Abs(field_C4_velx) >= FP_FromDouble(0.5))
            {
                if (field_C4_velx <= FP_FromInteger(0))
                {
                    field_C4_velx += FP_FromDouble(0.01);
                }
                else
                {
                    field_C4_velx -= FP_FromDouble(0.01);
                }

                field_130_pLine = field_130_pLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
                if (!field_130_pLine)
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
                    field_11C_state = MeatStates::State_2_BeingThrown;
                }
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
                
                field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
                field_E4_collection_rect.w = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2)) + field_B8_xpos;
                field_E4_collection_rect.h = field_BC_ypos;

                field_6_flags.Set(BaseGameObject::eInteractive_Bit8);
                field_11C_state = MeatStates::State_4_WaitForPickUp;
            }
            break;

        case MeatStates::State_4_WaitForPickUp:
            if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
            {
                field_12C_deadtimer = sGnFrame_5C1B84 + 600;
            }

            if (static_cast<int>(sGnFrame_5C1B84) > field_128_timer && !v2)
            {
                // That strange "shimmer" the meat gives off
                New_TintShiny_Particle_426C30(
                    (field_CC_sprite_scale * FP_FromInteger(1)) + field_B8_xpos,
                    field_BC_ypos + (field_CC_sprite_scale * FP_FromInteger(-7)), 
                    FP_FromDouble(0.3), 36);
                field_128_timer = Math_NextRandom() % 16 + sGnFrame_5C1B84 + 60;
            }
            if (field_12C_deadtimer < (signed int)sGnFrame_5C1B84)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            break;

        case MeatStates::State_5_Fall:
            field_C8_vely += FP_FromInteger(1);
            field_B8_xpos += field_C4_velx;
            field_BC_ypos = field_C8_vely + field_BC_ypos;
            if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            break;

        default:
            return;
        }
    }
}

// =================================================================================



TintEntry stru_55C254[] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 137u, 137u, 137u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
};


MeatSack* MeatSack::ctor_46A410(Path_MeatSack* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x5460D4);

    field_4_typeId = Types::eMeatSack_85;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kD2elumResID);
    Animation_Init_424E10(15848, 93, 86, ppRes, 1, 1);
    SetTint_425600(&stru_55C254[0], gMap_5C3030.field_0_current_level);

    field_DC_bApplyShadows &= ~1u;
    field_118_tlvInfo = tlvInfo;

    field_11C = 0;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_124_velX = FP_FromRaw(pTlv->field_12_xVel << 8);

    // Throw the meat up into the air as it falls from the sack
    field_128_velY = -FP_FromRaw(pTlv->field_14_yVel << 8);

    if (!pTlv->field_10_side)
    {
        field_124_velX = -field_124_velX;
    }

    if (pTlv->field_16_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    field_11E_num_items = pTlv->field_18_num_items;

    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* MeatSack::VDestructor(signed int flags)
{
    return vdtor_46A5E0(flags);
}

void MeatSack::VScreenChanged()
{
    vScreenChanged_46A9C0();
}

void MeatSack::VUpdate()
{
    vUpdate_46A6A0();
}

int Meat::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_46AC40(reinterpret_cast<Meat_SaveState*>(pSaveBuffer));
}

int CC Meat::CreateFromSaveState_46A9E0(const BYTE* pBuffer)
{
    const auto pState = reinterpret_cast<const Meat_SaveState*>(pBuffer);

    auto pMeat = ae_new<Meat>();
    pMeat->ctor_4694A0(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pMeat->field_C_objectId = pState->field_4_obj_id;

    pMeat->field_B8_xpos = pState->field_8_xpos;
    pMeat->field_BC_ypos = pState->field_C_ypos;

    pMeat->field_E4_collection_rect.x = pMeat->field_B8_xpos - (ScaleToGridSize_4498B0(pMeat->field_CC_sprite_scale) / FP_FromInteger(2));
    pMeat->field_E4_collection_rect.y = pMeat->field_BC_ypos - ScaleToGridSize_4498B0(pMeat->field_CC_sprite_scale);
    pMeat->field_E4_collection_rect.w = (ScaleToGridSize_4498B0(pMeat->field_CC_sprite_scale) / FP_FromInteger(2)) + pMeat->field_B8_xpos;
    pMeat->field_E4_collection_rect.h = pMeat->field_BC_ypos;

    pMeat->field_C4_velx = pState->field_10_velx;
    pMeat->field_C8_vely = pState->field_14_vely;

    pMeat->field_C0_path_number = pState->field_1C_path_number;
    pMeat->field_C2_lvl_number = pState->field_1E_lvl_number;

    pMeat->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pMeat->field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Meat_SaveState::eBit3_bLoop));
    pMeat->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Meat_SaveState::eBit1_bRender));

    pMeat->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Meat_SaveState::eBit2_bDrawable));
    pMeat->field_6_flags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Meat_SaveState::eBit4_bInteractive));

    pMeat->field_114_flags.Set(Flags_114::e114_Bit9);

    pMeat->field_128_timer = sGnFrame_5C1B84;
    pMeat->field_104_collision_line_type = pState->field_28_line_type;

    pMeat->field_118_count = pState->field_2A_count;
    pMeat->field_11C_state = pState->field_2C_state;

    pMeat->field_120_xpos = pState->field_30_xpos;
    pMeat->field_124_ypos = pState->field_34_ypos;

    pMeat->field_12C_deadtimer = pState->field_38_savedfield12C;
    return sizeof(Meat_SaveState);
}

MeatSack* MeatSack::vdtor_46A5E0(signed int flags)
{
    dtor_46A610();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void MeatSack::dtor_46A610()
{
    SetVTable(this, 0x5460D4);
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

void MeatSack::vScreenChanged_46A9C0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void MeatSack::vUpdate_46A6A0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_120)
        {
            if (Math_NextRandom() < 40u || field_122)
            {
                field_120 = 0;
                field_122 = 0;
                SFX_Play_46FBA0(SoundEffect::SackWobble_29, 24, Math_RandomRange_496AB0(-2400, -2200));
            }
        }
    }
    else
    {
        field_120 = 1;
    }

    if (field_11C)
    {
        if (field_11C == 1 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.Set_Animation_Data_409C80(15848, 0);
            field_11C = 0;
        }
    }
    else
    {
        PSX_RECT bPlayerRect = {};
        sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bPlayerRect, 1);

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);

        if (bRect.x <= bPlayerRect.w &&
            bRect.w >= bPlayerRect.x &&
            bRect.h >= bPlayerRect.y &&
            bRect.y <= bPlayerRect.h &&
            field_CC_sprite_scale == sActiveHero_5C1B68->field_CC_sprite_scale)
        {
            if (gpThrowableArray_5D1E2C)
            {
                if (gpThrowableArray_5D1E2C->field_20_count)
                {
                    field_20_animation.Set_Animation_Data_409C80(15888, 0);
                    field_11C = 1;
                    return;
                }
            }
            else
            {
                gpThrowableArray_5D1E2C = ae_new<ThrowableArray>();
                gpThrowableArray_5D1E2C->ctor_49A630();
            }

            gpThrowableArray_5D1E2C->Add_49A7A0(field_11E_num_items);

            auto pMeat = ae_new<Meat>();
            if (pMeat)
            {
                pMeat->ctor_4694A0(field_B8_xpos, field_BC_ypos - FP_FromInteger(30), field_11E_num_items);
            }

            pMeat->VThrow_49E460(field_124_velX, field_128_velY);
            pMeat->field_CC_sprite_scale = field_CC_sprite_scale;

            SFX_Play_46FA90(SoundEffect::SackHit_25, 0);
            Environment_SFX_457A40(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);

            field_20_animation.Set_Animation_Data_409C80(15888, 0);
            field_11C = 1;
        }
    }
}

int Meat::vGetSaveState_46AC40(Meat_SaveState* pState)
{
    pState->field_0_type = Types::eMeat_84;
    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;

    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_1C_path_number = field_C0_path_number;
    pState->field_1E_lvl_number = field_C2_lvl_number;

    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_20_flags.Set(Meat_SaveState::eBit3_bLoop, field_20_animation.field_4_flags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Meat_SaveState::eBit1_bRender, field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render));

    pState->field_20_flags.Set(Meat_SaveState::eBit2_bDrawable, field_6_flags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Meat_SaveState::eBit4_bInteractive, field_6_flags.Get(BaseGameObject::eInteractive_Bit8));

    if (field_130_pLine)
    {
        pState->field_28_line_type = field_130_pLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = field_110_id;
    pState->field_2A_count = field_118_count;
    pState->field_2C_state = field_11C_state;

    pState->field_30_xpos = field_120_xpos;
    pState->field_34_ypos = field_124_ypos;

    pState->field_38_savedfield12C = field_12C_deadtimer;

    return sizeof(Meat_SaveState);
}

BOOL Meat::vCanEatMe_4696A0()
{
    return field_11C_state != MeatStates::State_0;
}
