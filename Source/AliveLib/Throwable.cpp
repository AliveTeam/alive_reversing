#include "stdafx.h"
#include "Throwable.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"

ALIVE_VAR(1, 0x5c1bde, WORD, gInfiniteGrenades_5C1BDE, 0);

void BaseThrowable::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_4114D0();
}

void BaseThrowable::VToDead_4114B0()
{
    vToDead_4114B0();
}

EXPORT void BaseThrowable::vToDead_4114B0()
{
    NOT_IMPLEMENTED();
}

EXPORT void BaseThrowable::vOnPickUpOrSlapped_4114D0()
{
    NOT_IMPLEMENTED();
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

EXPORT BaseThrowable* CCSTD Make_Throwable_49AF30(FP /*xpos*/, FP /*ypos*/, FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
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

Grenade* Grenade::ctor_447F70(FP xpos, FP ypos, __int16 numGrenades, __int16 a5, __int16 a6, int a7)
{
    ctor_408240(0);
    SetVTable(this, 0x5456E0);

    field_11A = 0;

    Init_448110(xpos, ypos);

    field_118 = numGrenades;
    field_132 = a5;

    if (a5)
    {
        field_120 = 8;
        field_11A = 1;
    }
    else if (numGrenades)
    {
        field_120 = a5;
    }
    else
    {
        field_120 = 3;
        field_122 = 90;
    }

    field_138 = a7;
    field_130 = a6;

    return this;
}

void Grenade::VThrow_49E460(FP velX, FP velY)
{
    vThrow_449390(velX, velY);
}

BOOL Grenade::VCanThrow_49E350()
{
    return vCanThrow_4482E0();
}

BOOL Grenade::VIsFalling_49E330()
{
    return vIsFalling_49A5F0();
}

__int16 Grenade::VTimeToExplodeRandom_411490()
{
    return vTimeToExplodeRandom_49A610();
}

__int16 Grenade::VGetCount_448080()
{
    return vGetCount_4480A0();
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

    field_11C = -1;
    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_128_xpos = xpos;
    field_12C_ypos = ypos;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_118 = 0;
    field_124 = 0;
    field_134 = 0;
}

void Grenade::vThrow_449390(FP /*velX*/, FP /*velY*/)
{
    NOT_IMPLEMENTED();
}

BOOL Grenade::vCanThrow_4482E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

BOOL Grenade::vIsFalling_49A5F0()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Grenade::vTimeToExplodeRandom_49A610()
{
    NOT_IMPLEMENTED();
    return 0;
}

__int16 Grenade::vGetCount_4480A0()
{
    NOT_IMPLEMENTED();
    return 0;
}

Bone* Bone::ctor_4112C0(FP xpos, FP ypos, __int16 countId)
{
    ctor_408240(0);
    field_11A = 0;
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
    field_118_count_id = countId;
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

    if (gInfiniteGrenades_5C1BDE || field_11A)
    {
        return;
    }

    if (gpThrowableArray_5D1E2C)
    {
        gpThrowableArray_5D1E2C->Remove_49AA00(field_118_count_id >= 1 ? field_118_count_id : 1);
    }
   
    dtor_4080B0();
}

Meat* Meat::ctor_4694A0(FP xpos, FP ypos, __int16 a4)
{
    ctor_408240(0);
    SetVTable(this, 0x546040);

    field_11A = 0;
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
    field_118 = a4;
    field_11C = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}
