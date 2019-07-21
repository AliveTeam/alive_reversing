#include "stdafx.h"
#include "Bone.hpp"
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

Bone* Bone::ctor_4112C0(FP xpos, FP ypos, __int16 countId)
{
    ctor_408240(0);
    field_11A_bDead = 0;
    SetVTable(this, 0x54431C);
    field_4_typeId = Types::eBone_11;
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 591, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 591);
    Animation_Init_424E10(456, 15, 9, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_120_xpos = xpos;
    field_124_ypos = ypos;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_6_flags.Clear(BaseGameObject::eInteractive);
    field_130 &= ~1u;

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_12C = sGnFrame_5C1B84 + 300;
    field_118_count = countId;
    field_11C_state = BoneStates::eState_0;
    field_11E = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

BaseGameObject* Bone::VDestructor(signed int flags)
{
    return vdtor_411580(flags);
}

void Bone::VUpdate()
{
    vUpdate_411BC0();
}

void Bone::VScreenChanged()
{
    vScreenChanged_4122D0();
}

void Bone::VThrow_49E460(FP velX, FP velY)
{
    vThrow_411670(velX, velY);
}

void Bone::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_412490();
}

BOOL Bone::VCanThrow_49E350()
{
    return vCanThrow_411530();
}

BOOL Bone::VIsFalling_49E330()
{
    return vIsFalling_411510();
}

void Bone::VTimeToExplodeRandom_411490()
{
    // Empty
}

BOOL Bone::VCanBeEaten_411560()
{
    return vCanBeEaten_411560();
}

int Bone::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_412ED0(reinterpret_cast<Bone_SaveState*>(pSaveBuffer));
}

__int16 Bone::VGetCount_448080()
{
    return vGetCount_412500();
}

int CC Bone::CreateFromSaveState_412C10(const BYTE* pData)
{
    auto pState = reinterpret_cast<const Bone_SaveState*>(pData);

    auto pBone = alive_new<Bone>();
    pBone->ctor_4112C0(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_count);

    pBone->field_C_objectId = pState->field_4_obj_id;

    pBone->field_B8_xpos = pState->field_8_xpos;
    pBone->field_BC_ypos = pState->field_C_ypos;

    pBone->field_E4_collection_rect.x = pBone->field_B8_xpos - (ScaleToGridSize_4498B0(pBone->field_CC_sprite_scale) / FP_FromInteger(2));
    pBone->field_E4_collection_rect.y = pBone->field_BC_ypos - ScaleToGridSize_4498B0(pBone->field_CC_sprite_scale);
    pBone->field_E4_collection_rect.w = (ScaleToGridSize_4498B0(pBone->field_CC_sprite_scale) / FP_FromInteger(2)) + pBone->field_B8_xpos;
    pBone->field_E4_collection_rect.h = pBone->field_BC_ypos;

    pBone->field_C4_velx = pState->field_10_velx;
    pBone->field_C8_vely = pState->field_14_vely;

    pBone->field_C0_path_number = pState->field_1C_path_number;
    pBone->field_C2_lvl_number = pState->field_1E_lvl_number;
    pBone->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pBone->field_D6_scale = pState->field_18_sprite_scale > FP_FromDouble(0.75);

    pBone->field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Bone_SaveState::eBit3_bLoop));
    pBone->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Bone_SaveState::eBit1_bRender));

    pBone->field_6_flags.Set(BaseGameObject::eDrawable, pState->field_20_flags.Get(Bone_SaveState::eBit2_bDrawable));
    pBone->field_6_flags.Set(BaseGameObject::eInteractive, pState->field_20_flags.Get(Bone_SaveState::eBit4_bInteractive));

    pBone->field_114_flags.Set(Flags_114::e114_Bit9);

    pBone->field_128 = sGnFrame_5C1B84;
    
    pBone->field_104_collision_line_type = pState->field_28_line_type;
    pBone->field_118_count = pState->field_2A_count;
    pBone->field_11C_state = pState->field_2C_state;
    
    pBone->field_11E = pState->field_2E;
    pBone->field_120_xpos = pState->field_30;
    pBone->field_124_ypos = pState->field_34;

    pBone->field_12C = pState->field_38;

    pBone->field_130 = 0;
    if (pState->field_20_flags.Get(Bone_SaveState::eBit5_Unknown))
    {
        pBone->field_130 |= 1;
    }

    return sizeof(Bone_SaveState);
}

Bone* Bone::vdtor_411580(signed int flags)
{
    dtor_4115B0();

    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Bone::dtor_4115B0()
{
    SetVTable(this, 0x54431C);

    if (!gInfiniteGrenades_5C1BDE && !field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove_49AA00(field_118_count >= 1 ? field_118_count : 1);
        }
    }

    dtor_4080B0();
}

void Bone::AddToPlatform_412310()
{
    BaseAddToPlatform([](Types type) { return type == Types::eLiftPoint_78 || type == Types::eTrapDoor_142; });
}

void Bone::vThrow_411670(FP velX, FP velY)
{
    field_C4_velx = velX;
    field_C8_vely = velY;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    if (field_118_count == 0)
    {
        field_11C_state = BoneStates::eState_4;
    }
    else
    {
        field_11C_state = BoneStates::eState_1;
    }
}

void Bone::vOnTrapDoorOpen_412490()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        if (field_11C_state == BoneStates::eState_2 || field_11C_state == BoneStates::eState_3)
        {
            field_11C_state = BoneStates::eState_1;
        }
    }
}

BOOL Bone::vIsFalling_411510()
{
    return field_11C_state == BoneStates::eState_5;
}

BOOL Bone::vCanThrow_411530()
{
    return field_11C_state != BoneStates::eState_0 && field_11C_state != BoneStates::eState_1;
}

__int16 Bone::OnCollision_412140(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->field_6_flags.Get(BaseGameObject::eCanExplode))
    {
        return 1;
    }

    if (pObj->field_4_typeId != Types::eMine_88 && pObj->field_4_typeId != Types::eUXB_143 && (field_130 & 1))
    {
        return 1;
    }

    if (pObj->field_4_typeId == Types::eAntiChant_83 && sControlledCharacter_5C1B8C->field_D6_scale != pObj->field_D6_scale)
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pObj->vGetBoundingRect_424FD0(&bRect, 1);

    if (field_120_xpos < FP_FromInteger(bRect.x) || field_120_xpos > FP_FromInteger(bRect.w))
    {
        field_B8_xpos -= field_C4_velx;
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    }
    else
    {
        if (field_C8_vely > FP_FromInteger(0))
        {
            const FP slowerVelY = (-field_C8_vely / FP_FromInteger(2));
            field_C8_vely = slowerVelY;
            field_BC_ypos += slowerVelY;
        }
    }
    
    pObj->vnull_4081A0(this);

    field_130 |= 1u;
    SFX_Play_46FA90(24u, 80);

    if (pObj->field_4_typeId == Types::eMine_88 || pObj->field_4_typeId == Types::eUXB_143)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    return 0;
}

void Bone::vScreenChanged_4122D0()
{
    if (gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

BOOL Bone::vCanBeEaten_411560()
{
    return field_11C_state == BoneStates::eState_4;
}

int Bone::vGetSaveState_412ED0(Bone_SaveState* pState)
{
    pState->field_0_type = Types::eBone_11;
    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;

    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_1C_path_number = field_C0_path_number;
    pState->field_1E_lvl_number = field_C2_lvl_number;

    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_20_flags.Set(Bone_SaveState::eBit3_bLoop, field_20_animation.field_4_flags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Bone_SaveState::eBit1_bRender, field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render));

    pState->field_20_flags.Set(Bone_SaveState::eBit2_bDrawable, field_6_flags.Get(BaseGameObject::eDrawable));
    pState->field_20_flags.Set(Bone_SaveState::eBit4_bInteractive, field_6_flags.Get(BaseGameObject::eInteractive));

    pState->field_20_flags.Set(Bone_SaveState::eBit5_Unknown, field_130 & 1);

    if (field_100_pCollisionLine)
    {
        pState->field_28_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = field_110_id;
    pState->field_2A_count = field_118_count;
    pState->field_2C_state = field_11C_state;

    pState->field_2E = field_11E;
    pState->field_30 = field_120_xpos;

    pState->field_34 = field_124_ypos;
    pState->field_38 = field_12C;

    return sizeof(Bone_SaveState);
}

void Bone::InTheAir_4116C0()
{
    field_120_xpos = field_B8_xpos;
    field_124_ypos = field_BC_ypos;

    if (field_C8_vely > FP_FromInteger(30))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    field_C8_vely += FP_FromInteger(1);
    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_120_xpos,
        field_124_ypos,
        field_B8_xpos,
        field_BC_ypos,
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale == 1 ? 0x09 : 0x90) == 1)
    {
        switch (field_100_pCollisionLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            if (field_C8_vely <= FP_FromInteger(0))
            {
                break;
            }
            
            field_BC_ypos = hitY;

            if (field_C8_vely < FP_FromInteger(1))
            {
                field_11C_state = BoneStates::eState_2;

                field_BC_ypos = FP_FromInteger(field_100_pCollisionLine->field_0_rect.y);
                field_C8_vely = FP_FromInteger(0);
                if (field_C4_velx >= FP_FromInteger(0) && field_C4_velx < FP_FromInteger(1))
                {
                    field_C4_velx = FP_FromInteger(1);
                }

                if (field_C4_velx < FP_FromInteger(0) && field_C4_velx > FP_FromInteger(-1))
                {
                    field_C4_velx = FP_FromInteger(-1);
                }
                return;
            }
            else
            {
                field_BC_ypos -= FP_FromDouble(0.1);
                field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
                field_C4_velx = (field_C4_velx / FP_FromInteger(2));
                short vol = 20 * (4 - field_11E);
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(26u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                field_11E++;
            }
            break;

        case 3u:
        case 7u:
            if (field_C8_vely < FP_FromInteger(0))
            {
                field_BC_ypos = hitY;
                field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
                short vol = 20 * (4 - field_11E);
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(26u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
            break;
        }
    }

    if (sCollisions_DArray_5C1128->Raycast_417A60(field_120_xpos, field_124_ypos, field_B8_xpos, field_BC_ypos, &field_100_pCollisionLine, &hitX, &hitY, field_D6_scale == 1 ? 0x06 : 0x60) == 1)
    {
        switch (field_100_pCollisionLine->field_8_type)
        {
        case 1u:
        case 5u:
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                short vol = 20 * (4 - field_11E);
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(26u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
            field_100_pCollisionLine = nullptr;
            break;

        case 2u:
        case 6u:
            if (field_C4_velx > FP_FromInteger(0))
            {
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                short vol = 20 * (4 - field_11E);
                if (vol < 40)
                {
                    vol = 40;
                }
                SFX_Play_46FA90(26u, vol);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
            field_100_pCollisionLine = nullptr;
            break;
        }
    }
}

void Bone::vUpdate_411BC0()
{
    auto pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    switch (field_11C_state)
    {
    case BoneStates::eState_0:
        break;

    case BoneStates::eState_1:
        InTheAir_4116C0();
        return;

    case BoneStates::eState_2:
    {
        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);
        const short offset = field_D6_scale != 0 ? 5 : 0;
        const PSX_Point xy{ bRect.x, static_cast<short>(bRect.y + offset) };
        const PSX_Point wh{ bRect.w, static_cast<short>(bRect.h + offset) };
        vOnCollisionWith_424EE0(xy, wh, gBaseGameObject_list_BB47C4, 1, (TCollisionCallBack)&Bone::OnCollision_412140);

        if (WallHit_408750(FP_FromInteger(5), field_C4_velx))
        {
            field_C4_velx = -field_C4_velx;
        }

        if (FP_Abs(field_C4_velx) >= FP_FromInteger(1))
        {
            if (field_C4_velx <= FP_FromInteger(0))
            {
                field_C4_velx = (FP_FromDouble(0.01) / field_CC_sprite_scale) + field_C4_velx;
            }
            else
            {
                field_C4_velx = field_C4_velx - (FP_FromDouble(0.01) / field_CC_sprite_scale);
            }
            field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        }
        else
        {
            if (abs(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)) - FP_GetExponent(field_B8_xpos)) <= 1)
            {
                field_C4_velx = FP_FromInteger(0);
                field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
                field_E4_collection_rect.w = field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
                field_E4_collection_rect.h = field_BC_ypos;

                field_11C_state = BoneStates::eState_3;
                field_6_flags.Set(BaseGameObject::eInteractive);
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit8_Loop);
                field_128 = sGnFrame_5C1B84;
                AddToPlatform_412310();
                return;
            }
            field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        }

        if (field_100_pCollisionLine)
        {
            return;
        }

        field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
        field_11C_state = BoneStates::eState_4;
    }
        return;

    case BoneStates::eState_3:
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            field_12C = sGnFrame_5C1B84 + 300;
        }

        if (static_cast<int>(sGnFrame_5C1B84) > field_128 && !pObj)
        {
            New_Particle_426C30(
                    field_B8_xpos + (field_CC_sprite_scale * FP_FromInteger(1)),
                    (field_CC_sprite_scale * FP_FromInteger(-7)) + field_BC_ypos,
                    FP_FromDouble(0.3),
                    36);

            field_128 = (Math_NextRandom() % 16) + sGnFrame_5C1B84 + 60;
        }

        if (field_12C < static_cast<int>(sGnFrame_5C1B84))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        return;

    case BoneStates::eState_4:
    {
        InTheAir_4116C0();
        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);
        const short offset = field_D6_scale != 0 ? 5 : 0;
        const PSX_Point xy{ bRect.x, static_cast<short>(bRect.y + offset) };
        const PSX_Point wh{ bRect.w, static_cast<short>(bRect.h + offset) };
        vOnCollisionWith_424EE0(xy, wh, gBaseGameObject_list_BB47C4, 1, (TCollisionCallBack)&Bone::OnCollision_412140);

        if (field_BC_ypos > FP_FromInteger(gMap_5C3030.field_D4_ptr->field_6_bBottom))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }
    return;

    case BoneStates::eState_5:
        field_C8_vely += FP_FromInteger(1);
        field_B8_xpos += field_C4_velx;
        field_BC_ypos = field_C8_vely + field_BC_ypos;
        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        return;
    }
}

__int16 Bone::vGetCount_412500()
{
    if (field_11C_state == BoneStates::eState_3 && field_118_count == 0)
    {
        return 1;
    }
    return field_118_count;
}

TintEntry stru_550EC0[18] =
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
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


BoneBag* BoneBag::ctor_4125C0(Path_BoneBag* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x5443B0);
    field_4_typeId = Types::eBoneBag_12;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 590);
    // TODO: Super super OWI hack, WTF?? Figure out exactly what this is patching in the animation
    *((WORD *)*ppRes + 4374) = 0;

    Animation_Init_424E10(8748, 64, 60, ppRes, 1, 1);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    SetTint_425600(&stru_550EC0[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_11C = 0;
    field_118_tlvInfo = tlvInfo;

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);

    field_DC_bApplyShadows &= ~1u;

    field_124_velX = FP_FromRaw(pTlv->field_12_x_vel << 8);
    field_128_velY = FP_FromRaw(-256 * pTlv->field_14_y_vel); // TODO: << 8 negated ??

    if (!pTlv->field_10_side)
    {
        field_124_velX = -field_124_velX;
    }

    if (pTlv->field_16_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }

    field_11E_count = pTlv->field_18_num_bones;
    field_120 = 1;
    field_122 = 1;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

BaseGameObject* BoneBag::VDestructor(signed int flags)
{
    return vdtor_4127C0(flags);
}

void BoneBag::VUpdate()
{
    vUpdate_412880();
}

void BoneBag::VScreenChanged()
{
    vScreenChanged_412BF0();
}

void BoneBag::vScreenChanged_412BF0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

BoneBag* BoneBag::vdtor_4127C0(signed int flags)
{
    dtor_4127F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void BoneBag::dtor_4127F0()
{
    SetVTable(this, 0x5443B0);
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

void BoneBag::vUpdate_412880()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_120)
        {
            if (Math_NextRandom() < 40 || field_122)
            {
                field_120 = 0;
                field_122 = 0;
                SFX_Play_46FBA0(29u, 24, Math_RandomRange_496AB0(-2400, -2200));
            }
        }
    }
    else
    {
        field_120 = 1;
    }

    if (field_11C)
    {
        if (field_11C != 1)
        {
            return;
        }

        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            return;
        }

        field_20_animation.Set_Animation_Data_409C80(8748, nullptr);
        field_11C = 0;
        return;
    }

    if (field_20_animation.field_E_frame_change_counter == 0)
    {
        field_20_animation.field_E_frame_change_counter = Math_RandomRange_496AB0(2, 10);
    }

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
                if (sActiveHero_5C1B68->field_106_current_motion == 31)
                {
                    field_20_animation.Set_Animation_Data_409C80(8708, nullptr);
                }
                else
                {
                    field_20_animation.Set_Animation_Data_409C80(8788, nullptr);
                }
                field_11C = 1;
                return;
            }
        }
        else
        {
            gpThrowableArray_5D1E2C = alive_new<ThrowableArray>();
            gpThrowableArray_5D1E2C->ctor_49A630();
        }

        gpThrowableArray_5D1E2C->Add_49A7A0(field_11E_count);
        
        auto pBone = alive_new<Bone>();
        pBone->ctor_4112C0(field_B8_xpos, field_BC_ypos - FP_FromInteger(30), field_11E_count);
        
        pBone->field_CC_sprite_scale = field_CC_sprite_scale;
        pBone->field_D6_scale = field_D6_scale;
        
        pBone->VThrow_49E460(field_124_velX, field_128_velY);
       
        SFX_Play_46FA90(25u, 0);
        Abe_SFX_2_457A40(7, 0, 0x7FFF, 0);

        if (sActiveHero_5C1B68->field_106_current_motion == 31)
        {
            field_20_animation.Set_Animation_Data_409C80(8708, nullptr);
        }
        else
        {
            field_20_animation.Set_Animation_Data_409C80(8788, nullptr);
        }

        field_11C = 1;
    }
}
