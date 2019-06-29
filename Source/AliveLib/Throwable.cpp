#include "stdafx.h"
#include "Throwable.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "Abe.hpp"

ALIVE_VAR(1, 0x5c1bde, WORD, gInfiniteGrenades_5C1BDE, 0);

void BaseThrowable::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_4114D0();
}

__int16 BaseThrowable::VGetCount_448080()
{
    return vGetCount_448080();
}

void BaseThrowable::VToDead_4114B0()
{
    vToDead_4114B0();
}

void BaseThrowable::vToDead_4114B0()
{
    field_6_flags.Set(BaseGameObject::eDead);
    field_11A_bDead = 1;
}

void BaseThrowable::vOnPickUpOrSlapped_4114D0()
{
    VToDead_4114B0();
}

__int16 BaseThrowable::vGetCount_448080()
{
    return field_118_count;
}

Rock* Rock::ctor_49E150(FP xpos, FP ypos, __int16 count)
{
    NOT_IMPLEMENTED();

    ctor_408240(0);
    SetVTable(this, 0x546AF8);
    field_4_typeId = Types::eRock_105;

    field_11A_bDead = 0;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 350, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 350);
    Animation_Init_424E10(488, 17, 9, ppRes, 1, 1);
    field_6_flags.Clear(BaseGameObject::eInteractive);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_118_count = count;
    field_11C_state = 0;
    
    BYTE** ppPal = ResourceManager::GetLoadedResource_49C2A0('tlaP', 350, 0, 0);
    if (ppPal)
    {
        field_20_animation.Load_Pal_40A530(ppPal, 0);
    }
    else
    {
        // Pal offset is... WTF ??
        //field_20_animation.Load_Pal_40A530(field_20_animation.field_20_ppBlock, *(_DWORD *)&(*field_20_animation.field_20_ppBlock)[*(_DWORD *)&(*field_20_animation.field_20_ppBlock)[*((_DWORD *)*field_20_animation.field_20_ppBlock + 124)]]);
    }

    field_11E = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

// ====================================================================

void Rock::VScreenChanged()
{
    vScreenChanged_49F030();
}

int Rock::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_49F9A0(pSaveBuffer);
}

EXPORT void Rock::vScreenChanged_49F030()
{
    NOT_IMPLEMENTED();
}

EXPORT int Rock::vGetSaveState_49F9A0(BYTE* /*pSaveBuffer*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

// Overlay to throwable type table ??
const Types word_55FAFC[252] =
{
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eRock_105,   Types::eRock_105,  Types::eRock_105,   Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eRock_105,   Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eRock_105,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eRock_105,   Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eRock_105,     Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eRock_105,   Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eMeat_84,      Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eMeat_84,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eMeat_84,      Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eRock_105,   Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eBone_11,    Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,    Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eBone_11,    Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eBone_11,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eGrenade_65, Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eBone_11,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eBone_11,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,    Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eBone_11,    Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eGrenade_65, Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eBone_11,      Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eGrenade_65, Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eBone_11,    Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eGrenade_65, Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eGrenade_65,   Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eGrenade_65, Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,       Types::eNone_0,    Types::eNone_0,     Types::eNone_0,    Types::eNone_0,     Types::eNone_0,   Types::eNone_0,     Types::eNone_0,
    Types::eNone_0,       Types::eNone_0 };


EXPORT BaseThrowable* CCSTD Make_Throwable_49AF30(FP xpos, FP ypos, short count)
{
    switch (word_55FAFC[gMap_5C3030.field_22])
    {
    case Types::eBone_11:
        return alive_new<Bone>()->ctor_4112C0(xpos, ypos, count);
    case Types::eType_24:
        return alive_new<Grenade>()->ctor_447F70(xpos, ypos, count, 0, 1, 0);
    case Types::eGrenade_65:
        return alive_new<Grenade>()->ctor_447F70(xpos, ypos, count, 0, 0, 0);
    case Types::eMeat_84:
        return alive_new<Meat>()->ctor_4694A0(xpos, ypos, count);
    case Types::eRock_105:
        return alive_new<Rock>()->ctor_49E150(xpos, ypos, count);
    default:
        return nullptr;
    }
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
    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x)  / 2);
    field_DC_bApplyShadows &= ~1u;
    field_BC_ypos = FP_FromInteger(pTlv->field_C_bottom_right.field_2_y);
    field_124 =  pTlv->field_12_x_vel << 8;
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
        field_120_state = States::eState_8;
        field_11A_bDead = 1;
    }
    else if (numGrenades)
    {
        field_120_state = States::eState_0_FallingToBeCollected;
    }
    else
    {
        field_120_state = States::eState_3_CountingDown;
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

void Grenade::vScreenChanged_449140()
{
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
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
    field_6_flags.Clear(BaseGameObject::eInteractive);

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
        if (field_120_state == States::eState_1_WaitToBeCollected || field_120_state == States::eState_2)
        {
            field_120_state = States::eState_0_FallingToBeCollected;
        }
        else if (field_120_state != States::eState_6_WaitForExplodeEnd)
        {
            field_120_state = States::eState_4_Falling;
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
            field_120_state = States::eState_4_Falling;
        }
    }
    else
    {
        field_120_state = States::eState_9_FallingBlowUpOnGround;
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

    field_120_state = States::eState_6_WaitForExplodeEnd;
    
    auto pGibs = alive_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_40FB40(5, field_B8_xpos, field_BC_ypos, FP_FromInteger(0) , FP_FromInteger(5), field_CC_sprite_scale,  bSmallExplosion);
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
        field_6_flags.Set(BaseGameObject::eDead);
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
    case States::eState_0_FallingToBeCollected:
        if (!InTheAir_4484F0(FALSE))
        {
            field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_E4_collection_rect.w = field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_E4_collection_rect.h = field_BC_ypos;

            field_6_flags.Set(BaseGameObject::eInteractive);
            field_120_state = States::eState_1_WaitToBeCollected;
        }
        break;

    case States::eState_1_WaitToBeCollected:
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
                field_120_state = States::eState_0_FallingToBeCollected;
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
                field_120_state = States::eState_4_Falling;
            }
        }
        else
        {
            field_C4_velx = FP_FromInteger(0);

            field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale);
            field_E4_collection_rect.w = field_B8_xpos + (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2));
            field_E4_collection_rect.h = field_BC_ypos;

            field_6_flags.Set(BaseGameObject::eInteractive);
            field_120_state = States::eState_2;
        }
        break;

    case States::eState_3_CountingDown:
        TimeToBlowUp_448350();
        break;

    case States::eState_4_Falling:
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
            field_120_state = States::eState_5_HitGround;
        }
        break;

    case States::eState_5_HitGround:
        field_C4_velx = FP_FromRaw(field_C4_velx.fpValue / 2);

        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (!field_100_pCollisionLine)
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
            field_120_state = States::eState_4_Falling;
        }

        TimeToBlowUp_448350();
        break;

    case States::eState_6_WaitForExplodeEnd:
        if ( !pExplosion || pExplosion->field_6_flags.Get(BaseGameObject::eDead))
        {
            field_120_state = States::eState_7_Exploded;
            field_11C_explosion_id = -1;
        }
        break;

    case States::eState_7_Exploded:
        field_6_flags.Set(BaseGameObject::eDead);
        break;

    case States::eState_9_FallingBlowUpOnGround:
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
            field_6_flags.Set(BaseGameObject::eDead);
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
        field_D6_scale != 0 ? 1 : 16) == 1)
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
        field_D6_scale == 0 ?  0x60 : 0x106) == 1)
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
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos - FP_FromInteger(20),
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(20),
        &pLine,
        &hitX,
        &hitY,
        field_D6_scale == 0 ? 0xF0 : 0x10F))
    {
        if (pLine->field_8_type == 32 || pLine->field_8_type == 36)
        {
            if (ObjList_5C1B78)
            {
                for (int idx = 0; idx < ObjList_5C1B78->Size(); idx++)
                {
                    BaseGameObject* pObj = ObjList_5C1B78->ItemAt(idx);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->field_4_typeId == Types::eLiftPoint_78 || pObj->field_4_typeId == Types::eTrapDoor_142)
                    {
                        auto pPlatform = static_cast<PlatformBase*>(pObj);

                        PSX_RECT bRect = {};
                        pPlatform->vGetBoundingRect_424FD0(&bRect, 1);

                        if (FP_GetExponent(field_B8_xpos) > bRect.x  && FP_GetExponent(field_B8_xpos) < bRect.w  && FP_GetExponent(field_BC_ypos) < bRect.h)
                        {
                            pPlatform->VAdd(this);
                            field_110_id = pPlatform->field_8_object_id;
                            return;
                        }
                    }
                }
            }
        }
    }
}

__int16 Grenade::OnCollision_BounceOff_448F90(BaseGameObject* pHit)
{
    if (!pHit->field_6_flags.Get(BaseGameObject::eCanExplode))
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

    pHit2->vnull_4081A0(this);

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

     if (pHit->field_6_flags.Get(BaseGameObject::eCanExplode) && static_cast<BaseAliveGameObject*>(pHit)->field_CC_sprite_scale == field_CC_sprite_scale)
     {
         field_134_bExplodeNow = 1;
         return 0;
     }

     return 1;
}

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

Meat* Meat::ctor_4694A0(FP xpos, FP ypos, __int16 a4)
{
    ctor_408240(0);
    SetVTable(this, 0x546040);

    field_11A_bDead = 0;
    field_4_typeId = Types::eMeat_84;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 2023, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 2023);
    Animation_Init_424E10(488, 17, 9, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_128 = 0;
    field_6_flags.Clear(BaseGameObject::eInteractive);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_12C = sGnFrame_5C1B84 + 600;
    field_130_pLine = 0;
    field_118_count = a4;
    field_11C = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

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

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 4002);
    Animation_Init_424E10(15848, 93, 86, ppRes, 1, 1);
    SetTint_425600(&stru_55C254[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_DC_bApplyShadows &= ~1u;
    field_118_tlvInfo = tlvInfo;
    
    field_11C = 0;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_124_velX = FP_FromRaw(pTlv->field_12_xVel << 8);
    field_128_velY = -FP_FromRaw(256 * pTlv->field_14_yVel);

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

    field_E0_pShadow = alive_new<Shadow>();
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

MeatSack* MeatSack::vdtor_46A5E0(signed int flags)
{
    dtor_46A610();
    if (flags & 1)
    {
        Mem_Free_495540(this);
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
    field_6_flags.Set(BaseGameObject::eDead);
}

void MeatSack::vUpdate_46A6A0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_120)
        {
            if (Math_NextRandom() < 40u || field_122)
            {
                field_120 = 0;
                field_122 = 0;
                SFX_Play_46FBA0(29, 24, Math_RandomRange_496AB0(-2400, -2200));
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
                gpThrowableArray_5D1E2C = alive_new<ThrowableArray>();
                gpThrowableArray_5D1E2C->ctor_49A630();
            }

            gpThrowableArray_5D1E2C->Add_49A7A0(field_11E_num_items);

            auto pMeat = alive_new<Meat>();
            if (pMeat)
            {
                pMeat->ctor_4694A0(field_B8_xpos, field_BC_ypos - FP_FromInteger(30), field_11E_num_items);
            }

            pMeat->VThrow_49E460(field_124_velX, field_128_velY);
            pMeat->field_CC_sprite_scale = field_CC_sprite_scale;

            SFX_Play_46FA90(25, 0);
            Abe_SFX_2_457A40(7, 0, 0x7FFF, 0);

            field_20_animation.Set_Animation_Data_409C80(15888, 0);
            field_11C = 1;
        }
    }
}
