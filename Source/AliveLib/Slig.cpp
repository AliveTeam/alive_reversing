#include "stdafx.h"
#include "Slig.hpp"
#include "Function.hpp"
#include "Collisions.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "PathData.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"

TintEntry stru_560570[15] =
{
    { 1u, 102u, 127u, 118u },
    { 2u, 102u, 127u, 118u },
    { 3u, 102u, 127u, 118u },
    { 4u, 102u, 127u, 118u },
    { 5u, 102u, 127u, 118u },
    { 6u, 102u, 127u, 118u },
    { 7u, 102u, 127u, 118u },
    { 8u, 102u, 127u, 118u },
    { 9u, 102u, 127u, 118u },
    { 10u, 102u, 127u, 118u },
    { 11u, 102u, 127u, 118u },
    { 12u, 102u, 127u, 118u },
    { 13u, 102u, 127u, 118u },
    { 14u, 102u, 127u, 118u },
    { -1, 102u, 127u, 118u }
};

ALIVE_VAR(1, 0xBAF7E4, int, dword_BAF7E4, 0);

Slig* Slig::ctor_4B1370(Path_Slig* pTlv, int tlvInfo)
{
    ctor_408240(17);

    field_160 = -1;
    field_176 = -1;
    field_174 = 0;
    
    SetVTable(this, 0x547460);

    if (tlvInfo != 0xFFFF)
    {
        field_C_objectId = tlvInfo;
    }

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 412, 1, 0));
    Animation_Init_424E10(135512, 160, 68, field_10_resources_array.ItemAt(0), 1, 1);

    field_4_typeId = Types::eSlig_125;

    field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
    field_114_flags.Clear(Flags_114::e114_Bit9);
    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_292 &= ~1u;

    dword_BAF7E4 = 0;
    field_216 &= ~0xDu;


    field_120_timer = 0;
    field_11E = 45 * (Math_NextRandom() % 5 - 2);
    field_108_next_motion = 0;
    field_124 = 0;
    field_150 = 0;
    field_14C = 0;
    field_FC_pPathTLV = pTlv;
    
    field_218_tlv_data = *pTlv;

    field_110_id = -1;
    
    field_106_current_motion = 7;

    field_124 = 0;
    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_130 = 0;
    field_118_tlvInfo = tlvInfo;
    field_128_input = 0;
    field_158 = 0;
    field_15C = 0;
    field_136 = -1;
    field_208 = 0;
    field_20C = -1;
    field_20E = 0;
    field_210 = 0;
    field_134 = 0;

    field_20_animation.field_1C_fn_ptr_array = kSlig_Anim_Frame_Fns_55EFAC;

    if (pTlv->field_10_scale)
    {
        if (pTlv->field_10_scale == 1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = 14;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 33;
        field_D6_scale = 1;
    }

    SetBaseAnimPaletteTint_425690(&stru_560570[0], gMap_5C3030.sCurrentLevelId_5C3030, 412);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        field_D6_scale != 0 ? 1 : 16) == 1)
    {
        field_106_current_motion = 0;
        field_BC_ypos = hitY;
    }

    MapFollowMe_408D10(TRUE);
    
    Init_4BB0D0();
    
    vStackOnObjectsOfType_425840(Types::eSlig_125);
    
    if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eBonewerkz_8 && gMap_5C3030.sCurrentPathId_5C3032 == 2 && gMap_5C3030.sCurrentCamId_5C3034 == 5)
    {
        field_DA_xOffset = 0;
    }

    field_DC_bApplyShadows |= 2u;
    field_142 = 0;
    field_140 = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* Slig::VDestructor(signed int flags)
{
    return vdtor_4B1790(flags);
}

void Slig::Init_4BB0D0()
{
    NOT_IMPLEMENTED();
}

void Slig::dtor_4B1CF0()
{
    SetVTable(this, 0x547460);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        
        MusicController::sub_47FD60(0, this, 0, 0);

        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_146_level,
                field_148_path,
                field_14A_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_118_tlvInfo);
    if (pTlv)
    {
        if (pTlv->field_4_type != SligGetPants_104 && pTlv->field_4_type != SligSpawner_37)
        {
            if (field_10C_health <= FP_FromInteger(0))
            {
                pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                pTlv->field_0_flags.Set(TLV_Flags::eBit2_Unknown);
                pTlv->field_1_unknown = 0;
            }
            else
            {
                pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Unknown);
                pTlv->field_1_unknown = 0;
            }
        }
    }

    dtor_4080B0();
}

Slig* Slig::vdtor_4B1790(signed int flags)
{
    dtor_4B1CF0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}
