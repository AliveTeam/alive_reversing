#include "stdafx.h"
#include "Scrab.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "MusicController.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"
#include "PlatformBase.hpp"

TintEntry stru_560260[15] =
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
    { -1, 127u, 127u, 127u }
};


Scrab* Scrab::ctor_4A3C40(Path_Scrab* pTlv, int tlvInfo, __int16 spawnedScale)
{
    ctor_408240(14);
    field_190 = 0;
    field_17C_last_event = -1;
    field_192 = -1;

    SetVTable(this, 0x546DD0);

    field_4_typeId = Types::eScrab_112;

    if (tlvInfo != 0xFFFF)
    {
        field_C_objectId = tlvInfo;
    }

    field_124_fight_target_obj_id = -1;
    field_120_obj_id = -1;

    field_10_resources_array.SetAt(0,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 700, 1, 0));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 711, 1, 0));
    field_10_resources_array.SetAt(6,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 706, 1, 0));
    field_10_resources_array.SetAt(8,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 708, 1, 0));
    field_10_resources_array.SetAt(1,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 701, 1, 0));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 710, 1, 0));
    field_10_resources_array.SetAt(5,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 705, 1, 0));
    field_10_resources_array.SetAt(2,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 702, 1, 0));
    field_10_resources_array.SetAt(9,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 709, 1, 0));
    field_10_resources_array.SetAt(3,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 703, 1, 0));
    field_10_resources_array.SetAt(4,  ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 704, 1, 0));
    field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 713, 1, 0));
    
    Animation_Init_424E10(224764, 156, 69, field_10_resources_array.ItemAt(0), 1, 1);

    field_140 = 0;
    field_12C = 0;
    field_108_next_motion = 0;
    field_110_id = -1;
    field_106_current_motion = 0;
    field_11E = 0;

    field_1AA_flags.Clear(Flags_1AA::eBit1);
    field_1AA_flags.Clear(Flags_1AA::eBit4);

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_114_flags.Clear(Flags_114::e114_Bit9);

    field_164 = 0;
    field_16C = 0;
    field_170 = sGnFrame_5C1B84;
    field_1A2 = 0;
    field_1A4 = 0;
    field_1A6 = 25;
    field_178 = 0;
    field_154 = 0;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    if (spawnedScale == 1)
    {
        field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else if (spawnedScale == 2)
    {
        field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    field_128_attack_delay = pTlv->field_12_attack_delay;
    field_12A_patrol_type = pTlv->field_14_patrol_type;
    field_158_left_min_delay = pTlv->field_16_left_min_delay;
    field_15A_left_max_delay = pTlv->field_18_left_max_delay;
    field_15C = pTlv->field_1A_right_min_delay;
    field_15E_right_max_delay = pTlv->field_1C_right_max_delay;
    field_148_attack_duration = pTlv->field_1E_attack_duration;
    field_174_whirl_attack_duration = pTlv->field_26_whirl_attack_duration;
    field_176_whirl_attack_recharge = pTlv->field_28_whirl_attack_recharge;
    field_1A8_kill_close_fleech = pTlv->field_2A_kill_close_fleech;

    field_1A0 = 3;

    field_1AA_flags.Set(Flags_1AA::eBit5_roar_randomly, pTlv->field_22_roar_randomly & 1);
    field_1AA_flags.Set(Flags_1AA::eBit6_persistant, pTlv->field_24_persistant & 1);
    field_1AA_flags.Clear(Flags_1AA::eBit3);

    if (!sub_4A41E0())
    {
        field_106_current_motion = 8;
        vUpdateAnim_4A34F0();
        field_1C_update_delay = 30;
    }

    SetTint_425600(&stru_560260[0], gMap_5C3030.sCurrentLevelId_5C3030);
    
    field_DC_bApplyShadows |= 2u;

    field_14C = 0;
    field_150 = 0;
    field_144_tlvInfo = tlvInfo;
    field_11C_sub_state = 0;

    sub_4AA600();

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* Scrab::VDestructor(signed int flags)
{
    return vdtor_4A41B0(flags);
}

Scrab* Scrab::vdtor_4A41B0(signed int flags)
{
    dtor_4A42B0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Scrab::dtor_4A42B0()
{
    SetVTable(this, 0x546DD0);

    field_120_obj_id = -1;
    field_124_fight_target_obj_id = -1;

    vOnTrapDoorOpen_4A7ED0();

    if (field_10C_health <= FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_144_tlvInfo, -1, 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_144_tlvInfo, -1, 0, 0);
    }

    MusicController::sub_47FD60(0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        if (gMap_5C3030.field_A_5C303A_levelId != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_166_level,
                field_168_path,
                field_16A_camera,
                CameraSwapEffects::eEffect0_InstantChange,
                0,
                0);
        }
    }
    dtor_4080B0();
}

void Scrab::vOnTrapDoorOpen_4A7ED0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_1AA_flags.Set(Flags_1AA::eBit4);
        field_110_id = -1;
        field_106_current_motion = 15;
    }
}

void Scrab::vUpdateAnim_4A34F0()
{
    NOT_IMPLEMENTED();
}

__int16 Scrab::sub_4A41E0()
{
    NOT_IMPLEMENTED();
    return 0;
}

void Scrab::sub_4AA600()
{
    NOT_IMPLEMENTED();
}
