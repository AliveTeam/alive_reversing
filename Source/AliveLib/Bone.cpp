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
    field_120 = xpos;
    field_124 = ypos;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_6_flags.Clear(BaseGameObject::eInteractive);
    field_130 &= ~1u;

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_12C = sGnFrame_5C1B84 + 300;
    field_118_count = countId;
    field_11C_state = 0;
    field_11E = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
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
    pBone->field_120 = pState->field_30;
    pBone->field_124 = pState->field_34;

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
        field_11C_state = 4;
    }
    else
    {
        field_11C_state = 1;
    }
}

void Bone::vOnTrapDoorOpen_412490()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        if (field_11C_state == 2 || field_11C_state == 3)
        {
            field_11C_state = 1;
        }
    }
}

BOOL Bone::vIsFalling_411510()
{
    return field_11C_state == 5;
}

BOOL Bone::vCanThrow_411530()
{
    return field_11C_state != 0 && field_11C_state != 1;
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

    if (field_120 < FP_FromInteger(bRect.x) || field_120 > FP_FromInteger(bRect.w))
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
    return field_11C_state == 4;
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
    pState->field_30 = field_120;

    pState->field_34 = field_124;
    pState->field_38 = field_12C;

    return sizeof(Bone_SaveState);
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
    field_DC_bApplyShadows &= ~1u;
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_124 = pTlv->field_12_x_vel << 8;
    field_128 = -256 * pTlv->field_14_y_vel; // TODO: << 8 negated ??

    if (!pTlv->field_10_side)
    {
        field_124 = -field_124;
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
