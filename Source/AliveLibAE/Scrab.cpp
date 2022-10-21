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
#include "Events.hpp"
#include "DDCheat.hpp"
#include "Particle.hpp"
#include "Sfx.hpp"
#include "Sound/Midi.hpp"
#include "GameSpeak.hpp"
#include "SwitchStates.hpp"
#include "SlamDoor.hpp"
#include "LiftPoint.hpp"
#include "Slurg.hpp"
#include "Grid.hpp"

static const TintEntry sScrabTints_560260[15] = {
    {LevelIds_s8::eMines_1, 127u, 127u, 127u},
    {LevelIds_s8::eNecrum_2, 137u, 137u, 137u},
    {LevelIds_s8::eMudomoVault_3, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_4, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_5, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_6, 127u, 127u, 127u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_8, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_9, 127u, 127u, 127u},
    {LevelIds_s8::eBrewery_Ender_10, 127u, 127u, 127u},
    {LevelIds_s8::eMudomoVault_Ender_11, 127u, 127u, 127u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 127u, 127u, 127u},
    {LevelIds_s8::eBarracks_Ender_13, 127u, 127u, 127u},
    {LevelIds_s8::eBonewerkz_Ender_14, 127u, 127u, 127u},
    {LevelIds_s8::eNone, 127u, 127u, 127u}};

// TODO: repetition with `MainMenu.cpp`
static const SfxDefinition scrab_sScrabSfx_560330[9] = {
    {0u, 4u, 60u, 55u, 0, 0},
    {0u, 4u, 61u, 70u, 0, 0},
    {0u, 4u, 62u, 80u, 0, 0},
    {0u, 4u, 63u, 80u, 0, 0},
    {0u, 4u, 64u, 60u, -127, 127},
    {0u, 4u, 66u, 90u, 0, 0},
    {0u, 4u, 67u, 50u, -511, 0},
    {0u, 4u, 67u, 50u, 0, 511},
    {0u, 4u, 68u, 110u, -1791, -1791},
};


static const TScrabMotionFn sScrab_motion_table_560120[40] = {
#define MAKE_FN(VAR) &Scrab::VAR,
    SCRAB_MOTIONS_ENUM(MAKE_FN)
#undef MAKE_FN
};

static const TScrabBrainFn sScrab_brain_table_56029C[6] = {
    &Scrab::Brain_0_Patrol_4AA630,
    &Scrab::Brain_1_ChasingEnemy_4A6470,
    &Scrab::Brain_2_Fighting_4A5840,
    &Scrab::Brain_3_Death_4A62B0,
    &Scrab::Brain_4_ShrinkDeath_4A6420,
    &Scrab::Brain_5_Possessed_4A6180};

static const BrainFunctionData<TScrabBrainFn> sScrabBrainTable[6] = {
    {
        &Scrab::Brain_0_Patrol_4AA630,
        0x404070,
        "Brain_0_Patrol",
    },
    {
        &Scrab::Brain_1_ChasingEnemy_4A6470,
        0x403B98,
        "Brain_1_ChasingEnemy",
    },
    {
        &Scrab::Brain_2_Fighting_4A5840,
        0x403E4F,
        "Brain_2_Fighting",
    },
    {
        &Scrab::Brain_3_Death_4A62B0,
        0x4039C2,
        "Brain_3_Death",
    },
    {
        &Scrab::Brain_4_ShrinkDeath_4A6420,
        0x4036B1,
        "Brain_4_ShrinkDeath",
    },
    {
        &Scrab::Brain_5_Possessed_4A6180,
        0x4021F3,
        "Brain_5_Possessed",
    },
};

void Scrab::SetBrain(TScrabBrainFn fn)
{
    ::SetBrain(fn, field_118_brain_state, sScrabBrainTable);
}

bool Scrab::BrainIs(TScrabBrainFn fn)
{
    return ::BrainIs(fn, field_118_brain_state, sScrabBrainTable);
}

Scrab* Scrab::ctor_4A3C40(Path_Scrab* pTlv, s32 tlvInfo, ScrabSpawnDirection spawnDirection)
{
    ctor_408240(14);
    field_190_unused = 0;
    field_17C_last_event = -1;
    field_192_unused = -1;

    SetVTable(this, 0x546DD0);

    SetType(AETypes::eScrab_112);

    if (tlvInfo != 0xFFFF)
    {
        field_C_objectId = tlvInfo;
    }

    field_124_fight_target_obj_id = -1;
    field_120_obj_id = -1;

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArsbasicResID, 1, 0));
    field_10_resources_array.SetAt(11, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArschewResID, 1, 0));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArsdanceResID, 1, 0));
    field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArsdeadResID, 1, 0));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArseatResID, 1, 0));
    field_10_resources_array.SetAt(10, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArsgrwlResID, 1, 0));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArshowlResID, 1, 0));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArsprceResID, 1, 0));
    field_10_resources_array.SetAt(9, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArsroarResID, 1, 0));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArsskwrResID, 1, 0));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArswhirlResID, 1, 0));
    field_10_resources_array.SetAt(13, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArscrshResID, 1, 0));

    const AnimRecord& rec = AnimRec(AnimId::Scrab_Idle);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_140_motion_resource_block_index = 0;
    field_12C_timer = 0;
    field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
    field_110_id = -1;
    field_106_current_motion = eScrabMotions::M_Stand_0_4A8220;
    field_11E_return_to_previous_motion = 0;

    field_1AA_flags.Clear(Flags_1AA::eBit1_attacking);
    field_1AA_flags.Clear(Flags_1AA::eBit4_force_update_animation);

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);
    field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);

    field_164_prevent_depossession = 0;
    field_16C_input = 0;
    field_170_unused = sGnFrame_5C1B84;
    field_1A2_speak_counter = 0;
    field_1A4_unused = 0;
    field_1A6_unused = 25;
    field_178_shred_power_active = 0;
    field_154_movement_timer = 0;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x + 12);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_scale == Scale_short::eFull_0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_27;
        field_D6_scale = 1;
    }

    if (spawnDirection == ScrabSpawnDirection::eLeft_1)
    {
        field_B8_xpos -= ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else if (spawnDirection == ScrabSpawnDirection::eRight_2)
    {
        field_B8_xpos += ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    field_128_attack_delay = pTlv->field_12_attack_delay;
    field_12A_patrol_type_run_or_walk_chance = pTlv->field_14_patrol_type_run_or_walk_chance;
    field_158_left_min_delay = pTlv->field_16_left_min_delay;
    field_15A_left_max_delay = pTlv->field_18_left_max_delay;
    field_15C_right_min_delay = pTlv->field_1A_right_min_delay;
    field_15E_right_max_delay = pTlv->field_1C_right_max_delay;
    field_148_pause_after_chase_delay = pTlv->field_1E_pause_after_chase_delay;
    field_174_possessed_max_whirl_attack_duration = pTlv->field_26_possessed_max_whirl_attack_duration;
    field_176_unused = pTlv->field_28_unused;
    field_1A8_bKill_enemy = pTlv->field_2A_bKill_enemy;

    field_1A0_speak_max = 3;

    field_1AA_flags.Set(Flags_1AA::eBit5_roar_randomly, pTlv->field_22_roar_randomly == Choice_short::eYes_1);
    field_1AA_flags.Set(Flags_1AA::eBit6_persistant, pTlv->field_24_persistant == Choice_short::eYes_1);
    field_1AA_flags.Clear(Flags_1AA::eBit3_unused);

    if (!OnFloor_4A41E0())
    {
        field_106_current_motion = eScrabMotions::M_JumpToFall_8_4A9220;
        vUpdateAnim_4A34F0();
        SetUpdateDelay(30);
    }

    SetTint_425600(&sScrabTints_560260[0], gMap_5C3030.field_0_current_level);

    field_DC_bApplyShadows |= 2u;

    field_14C_pause_after_chase_timer = 0;
    field_150_attack_delay_timer = 0;
    field_144_tlvInfo = tlvInfo;
    field_11C_brain_sub_state = 0;

    ToPatrol_4AA600();

    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* Scrab::VDestructor(s32 flags)
{
    return vdtor_4A41B0(flags);
}

void Scrab::VUpdate()
{
    vUpdate_4A3530();
}

void Scrab::vOn_TLV_Collision_4A4B80(Path_TLV* pTlv)
{
    while (pTlv != nullptr)
    {
        if (pTlv->field_4_type == TlvTypes::DeathDrop_4)
        {
            Scrab_SFX_4AADB0(ScrabSounds::eYell_8, 127, -1000, 0);
            field_6_flags.Set(Options::eDead_Bit3);
            field_10C_health = FP_FromInteger(0);
        }
        else if (pTlv->field_4_type == TlvTypes::EnemyStopper_47)
        {
            const auto enemyStopperPath = static_cast<Path_EnemyStopper*>(field_FC_pPathTLV); //TODO it should probably be pTlv, instead - OG bug?
            const Path_EnemyStopper::StopDirection stopDirection = enemyStopperPath->field_10_stop_direction;
            if ((stopDirection == Path_EnemyStopper::StopDirection::Left_0 && field_B8_xpos < field_198_max_xpos) || (stopDirection == Path_EnemyStopper::StopDirection::Right_1 && field_B8_xpos > field_198_max_xpos) || stopDirection == Path_EnemyStopper::StopDirection::Both_2)
            {
                if (SwitchStates_Get_466020(enemyStopperPath->field_12_switch_id))
                {
                    if (sControlledCharacter_5C1B8C != this)
                    {
                        field_B8_xpos = field_198_max_xpos;
                    }
                }
            }
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            pTlv,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
    }
}

const AnimId sScrabFrameTableOffsets_5601C0[40] = {
    AnimId::Scrab_Idle,
    AnimId::Scrab_Walk,
    AnimId::Scrab_Run,
    AnimId::Scrab_Turn,
    AnimId::Scrab_RunToStand,
    AnimId::Scrab_HopBegin,
    AnimId::Scrab_Jump,
    AnimId::Scrab_Landing,
    AnimId::Scrab_JumpAndRunToFall,
    AnimId::Scrab_StandToWalk,
    AnimId::Scrab_StandToRun,
    AnimId::Scrab_WalkToStand,
    AnimId::Scrab_Jump,
    AnimId::Scrab_Landing,
    AnimId::Scrab_WalkToFall,
    AnimId::Scrab_JumpAndRunToFall,
    AnimId::Scrab_WalkToRun,
    AnimId::Scrab_RunToWalk,
    AnimId::Scrab_Knockback,
    AnimId::Scrab_GetEaten,
    AnimId::Scrab_Landing,
    AnimId::Scrab_Stamp,
    AnimId::Scrab_Stamp,
    AnimId::Scrab_DeathEnd,
    AnimId::Scrab_DeathEnd,
    AnimId::Scrab_Empty,
    AnimId::Scrab_HowlBegin,
    AnimId::Scrab_HowlEnd,
    AnimId::Scrab_HowlBegin,
    AnimId::Scrab_HowlEnd,
    AnimId::Scrab_Shriek,
    AnimId::Scrab_ScrabBattleAnim,
    AnimId::Scrab_AttackSpin,
    AnimId::Scrab_FeedToGulp,
    AnimId::Scrab_GulpToStand,
    AnimId::Scrab_StandToFeed,
    AnimId::Scrab_Feed,
    AnimId::Scrab_AttackLunge,
    AnimId::Scrab_LegKick,
    AnimId::Scrab_DeathBegin};

s32 CC Scrab::CreateFromSaveState_4A70A0(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Scrab_State*>(pBuffer);

    auto pTlv = static_cast<Path_Scrab*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pState->field_44_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kArsbasicResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SCRAB.BND", nullptr);
    }

    auto pScrab = ae_new<Scrab>();
    pScrab->ctor_4A3C40(pTlv, pState->field_44_tlvInfo, ScrabSpawnDirection::eNone_0);
    pScrab->field_C_objectId = pState->field_4_obj_id;

    if (pState->field_40_bIsControlled)
    {
        sControlledCharacter_5C1B8C = pScrab;
    }

    pScrab->field_FC_pPathTLV = nullptr;
    pScrab->field_100_pCollisionLine = nullptr;

    pScrab->field_B8_xpos = pState->field_8_xpos;
    pScrab->field_BC_ypos = pState->field_C_ypos;
    pScrab->field_C4_velx = pState->field_10_velx;
    pScrab->field_C8_vely = pState->field_14_vely;

    pScrab->field_134_falling_velx_scale_factor = pState->field_64_falling_velx_scale_factor;
    pScrab->field_C0_path_number = pState->field_18_path_number;
    pScrab->field_C2_lvl_number = pState->field_1A_lvl_number;
    pScrab->field_CC_sprite_scale = pState->field_1C_sprite_scale;
    pScrab->field_D0_r = pState->field_20_r;
    pScrab->field_D2_g = pState->field_22_g;
    pScrab->field_D4_b = pState->field_24_b;
    pScrab->field_106_current_motion = pState->field_28_current_motion;

    const AnimRecord& animRec = AnimRec(sScrabFrameTableOffsets_5601C0[pState->field_28_current_motion]);
    u8** ppRes = pScrab->ResBlockForMotion_4A43E0(pState->field_28_current_motion);
    pScrab->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);

    pScrab->field_20_animation.field_92_current_frame = pState->field_2A_current_frame;
    pScrab->field_20_animation.field_E_frame_change_counter = pState->field_2C_frame_change_counter;

    pScrab->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pState->field_2F_bDrawable & 1);

    pScrab->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pState->field_26_bAnimFlipX & 1);
    pScrab->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_2E_bAnimRender & 1);

    if (IsLastFrame(&pScrab->field_20_animation))
    {
        pScrab->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pScrab->field_10C_health = pState->field_30_health;
    pScrab->field_106_current_motion = pState->field_34_current_motion;
    pScrab->field_108_next_motion = pState->field_36_next_motion;
    pScrab->field_F8_LastLineYPos = FP_FromInteger(pState->field_38_last_line_ypos);
    pScrab->field_130_depossession_timer = pState->field_60_depossession_timer;
    pScrab->field_114_flags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
    pScrab->field_12C_timer = pState->field_5C_timer;
    pScrab->field_104_collision_line_type = pState->field_3A_line_type;
    pScrab->field_144_tlvInfo = pState->field_44_tlvInfo;

    pScrab->SetBrain(sScrab_brain_table_56029C[pState->field_48_brain_idx]);

    pScrab->field_11C_brain_sub_state = pState->field_50_sub_state;
    pScrab->field_120_obj_id = pState->field_54_obj_id;
    pScrab->field_124_fight_target_obj_id = pState->field_58_target_obj_id;

    pScrab->field_140_motion_resource_block_index = pState->field_68_motion_resource_block_index;
    pScrab->field_14C_pause_after_chase_timer = pState->field_6C_spotting_abe_timer;
    pScrab->field_150_attack_delay_timer = pState->field_70_attack_delay_timer;
    pScrab->field_154_movement_timer = pState->field_74_movement_timer;
    pScrab->field_160_sfx_bitmask = pState->field_78_sfx_bitmask;
    pScrab->field_164_prevent_depossession = pState->field_7C_prevent_depossession;

    pScrab->field_166_level = pState->field_7E_level;
    pScrab->field_168_path = pState->field_80_path;
    pScrab->field_16A_camera = pState->field_82_camera;
    pScrab->field_16C_input = InputObject::PsxButtonsToKeyboardInput_45EE40(pState->field_84_input);
    pScrab->field_170_unused = pState->field_88_unused;
    pScrab->field_178_shred_power_active = pState->field_8C_shred_power_active;
    pScrab->field_194_speak = pState->field_8E_speak;
    pScrab->field_198_max_xpos = pState->field_90_max_xpos;
    pScrab->field_19C_max_ypos = pState->field_94_max_ypos;
    pScrab->field_1A2_speak_counter = pState->field_98_speak_counter;
    pScrab->field_1A4_unused = pState->field_9A_unused;
    pScrab->field_1A6_unused = pState->field_9C_unused;

    pScrab->field_1AA_flags.Set(Flags_1AA::eBit1_attacking, pState->field_9E_flags.Get(Scrab_State::eBit1_attacking));
    pScrab->field_1AA_flags.Set(Flags_1AA::eBit2_unused, pState->field_9E_flags.Get(Scrab_State::eBit2_unused));
    pScrab->field_1AA_flags.Set(Flags_1AA::eBit3_unused, pState->field_9E_flags.Get(Scrab_State::eBit3_unused));
    pScrab->field_1AA_flags.Set(Flags_1AA::eBit4_force_update_animation, pState->field_9E_flags.Get(Scrab_State::eBit4_force_update_animation));
    pScrab->field_1AA_flags.Set(Flags_1AA::eBit5_roar_randomly, pState->field_9E_flags.Get(Scrab_State::eBit5_roar_randomly));
    pScrab->field_1AA_flags.Set(Flags_1AA::eBit6_persistant, pState->field_9E_flags.Get(Scrab_State::eBit6_persistant));

    return sizeof(Scrab_State);
}

s32 Scrab::vGetSaveState_4AB020(Scrab_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = AETypes::eScrab_112;
    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;
    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_64_falling_velx_scale_factor = field_134_falling_velx_scale_factor;

    pState->field_18_path_number = field_C0_path_number;
    pState->field_1A_lvl_number = field_C2_lvl_number;
    pState->field_1C_sprite_scale = field_CC_sprite_scale;

    pState->field_20_r = field_D0_r;
    pState->field_22_g = field_D2_g;
    pState->field_24_b = field_D4_b;

    pState->field_26_bAnimFlipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_28_current_motion = field_106_current_motion;
    pState->field_2A_current_frame = field_20_animation.field_92_current_frame;
    pState->field_2C_frame_change_counter = field_20_animation.field_E_frame_change_counter;
    pState->field_2F_bDrawable = field_6_flags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2E_bAnimRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_30_health = field_10C_health;
    pState->field_34_current_motion = field_106_current_motion;
    pState->field_36_next_motion = field_108_next_motion;
    pState->field_3A_line_type = -1;

    // TODO: Check correctness
    pState->field_38_last_line_ypos = FP_GetExponent(field_F8_LastLineYPos);
    if (field_100_pCollisionLine)
    {
        pState->field_3A_line_type = field_100_pCollisionLine->field_8_type;
    }

    pState->field_40_bIsControlled = (this == sControlledCharacter_5C1B8C);
    pState->field_60_depossession_timer = field_130_depossession_timer;
    pState->field_5C_timer = field_12C_timer;
    pState->field_44_tlvInfo = field_144_tlvInfo;
    pState->field_48_brain_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sScrab_brain_table_56029C)
    {
        if (BrainIs(fn))
        {
            pState->field_48_brain_idx = idx;
        }
        idx++;
    }

    pState->field_54_obj_id = -1;
    pState->field_50_sub_state = field_11C_brain_sub_state;

    if (field_120_obj_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_120_obj_id);
        if (pObj)
        {
            pState->field_54_obj_id = pObj->field_C_objectId;
        }
    }

    pState->field_58_target_obj_id = -1;
    if (field_124_fight_target_obj_id != -1)
    {
        BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_124_fight_target_obj_id);
        if (pObj)
        {
            pState->field_58_target_obj_id = pObj->field_C_objectId;
        }
    }

    pState->field_68_motion_resource_block_index = field_140_motion_resource_block_index;
    pState->field_6C_spotting_abe_timer = field_14C_pause_after_chase_timer;
    pState->field_70_attack_delay_timer = field_150_attack_delay_timer;
    pState->field_74_movement_timer = field_154_movement_timer;
    pState->field_78_sfx_bitmask = field_160_sfx_bitmask;
    pState->field_7C_prevent_depossession = field_164_prevent_depossession;
    pState->field_7E_level = field_166_level;
    pState->field_80_path = field_168_path;
    pState->field_82_camera = field_16A_camera;
    pState->field_84_input = InputObject::KeyboardInputToPsxButtons_45EF70(field_16C_input);
    pState->field_88_unused = field_170_unused;
    pState->field_8C_shred_power_active = field_178_shred_power_active;
    pState->field_8E_speak = field_194_speak;
    pState->field_90_max_xpos = field_198_max_xpos;
    pState->field_94_max_ypos = field_19C_max_ypos;
    pState->field_98_speak_counter = field_1A2_speak_counter;
    pState->field_9A_unused = field_1A4_unused;
    pState->field_9C_unused = field_1A6_unused;

    pState->field_9E_flags.Set(Scrab_State::eBit1_attacking, field_1AA_flags.Get(Flags_1AA::eBit1_attacking));
    pState->field_9E_flags.Set(Scrab_State::eBit2_unused, field_1AA_flags.Get(Flags_1AA::eBit2_unused));
    pState->field_9E_flags.Set(Scrab_State::eBit3_unused, field_1AA_flags.Get(Flags_1AA::eBit3_unused));
    pState->field_9E_flags.Set(Scrab_State::eBit4_force_update_animation, field_1AA_flags.Get(Flags_1AA::eBit4_force_update_animation));
    pState->field_9E_flags.Set(Scrab_State::eBit5_roar_randomly, field_1AA_flags.Get(Flags_1AA::eBit5_roar_randomly));
    pState->field_9E_flags.Set(Scrab_State::eBit6_persistant, field_1AA_flags.Get(Flags_1AA::eBit6_persistant));

    return sizeof(Scrab_State);
}

Scrab* Scrab::vdtor_4A41B0(s32 flags)
{
    dtor_4A42B0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
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

    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        if (gMap_5C3030.field_A_level != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_166_level,
                field_168_path,
                field_16A_camera,
                CameraSwapEffects::eInstantChange_0,
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
        field_1AA_flags.Set(Flags_1AA::eBit4_force_update_animation);
        field_110_id = -1;
        field_106_current_motion = eScrabMotions::M_RunToFall_15_4A9430;
    }
}

void Scrab::vUpdateAnim_4A34F0()
{
    const AnimRecord& animRec = AnimRec(sScrabFrameTableOffsets_5601C0[field_106_current_motion]);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ResBlockForMotion_4A43E0(field_106_current_motion));
}

s16 Scrab::OnFloor_4A41E0()
{
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos + FP_FromInteger(30),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16))
    {
        field_BC_ypos = hitY;
        ToStand_4A75A0();
        if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32 ||
            field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_36)
        {
            PlatformCollide_4A7E50();
        }
        return TRUE;
    }
    return FALSE;
}

const FP velx_input_entries_546D84[8] = {
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4)};

const FP vely_input_entries_546DA4[11] = {
    FP_FromInteger(0),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(-4),
    FP_FromInteger(0),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(4),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)};


void Scrab::vUpdate_4A3530()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(20),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(20),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
        }
        field_104_collision_line_type = 0;
        field_120_obj_id = BaseGameObject::Find_Flags_4DC170(field_120_obj_id);
        field_124_fight_target_obj_id = BaseGameObject::Find_Flags_4DC170(field_124_fight_target_obj_id);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    const FP xDelta = FP_Abs(field_B8_xpos - sControlledCharacter_5C1B8C->field_B8_xpos);
    if (xDelta <= FP_FromInteger(750))
    {
        const FP yDelta = FP_Abs(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos);
        if (yDelta <= FP_FromInteger(520))
        {
            if (field_10C_health > FP_FromInteger(0))
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }

            if (!Input_IsChanting_45F260())
            {
                field_164_prevent_depossession = 0;
            }

            if (sDDCheat_FlyingEnabled_5C2C08 && sControlledCharacter_5C1B8C == this)
            {
                // Handle DDCheat mode
                field_100_pCollisionLine = nullptr;

                if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eUp | InputCommands::Enum::eDown | InputCommands::Enum::eLeft | InputCommands::Enum::eRight))
                {
                    // TODO: InputCommand constants
                    field_C4_velx = velx_input_entries_546D84[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                    field_C8_vely = vely_input_entries_546DA4[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];

                    if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRun))
                    {
                        field_C4_velx += velx_input_entries_546D84[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                        field_C4_velx += velx_input_entries_546D84[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                        field_C8_vely += vely_input_entries_546DA4[sInputObject_5BD4E0.field_0_pads[sCurrentControllerIndex_5C1BBE].field_4_dir >> 5];
                    }

                    field_B8_xpos += field_C4_velx;
                    field_BC_ypos += field_C8_vely;

                    // Keep in map bounds
                    PSX_Point point = {};
                    gMap_5C3030.Get_map_size_480640(&point);
                    if (field_B8_xpos < FP_FromInteger(0))
                    {
                        field_B8_xpos = FP_FromInteger(0);
                    }

                    if (field_B8_xpos >= FP_FromInteger(point.field_0_x))
                    {
                        field_B8_xpos = FP_FromInteger(point.field_0_x) - FP_FromInteger(1);
                    }

                    if (field_BC_ypos < FP_FromInteger(0))
                    {
                        field_BC_ypos = FP_FromInteger(0);
                    }

                    if (field_BC_ypos >= FP_FromInteger(point.field_2_y))
                    {
                        field_BC_ypos = FP_FromInteger(point.field_2_y) - FP_FromInteger(1);
                    }
                }
                else
                {
                    field_C4_velx = FP_FromInteger(0);
                    field_C8_vely = FP_FromInteger(0);
                }

                SetActiveCameraDelayedFromDir_408C40();
                field_F8_LastLineYPos = field_BC_ypos;
                return;
            }

            if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                    field_C2_lvl_number,
                    field_C0_path_number,
                    field_B8_xpos,
                    field_BC_ypos,
                    1))
            {
                field_1A2_speak_counter = 0;
            }

            const auto oldMotion = field_106_current_motion;

            field_11C_brain_sub_state = (this->*field_118_brain_state)();

            if (sDDCheat_ShowAI_Info_5C1BD8)
            {
                DDCheat::DebugStr_4F5560(
                    "Scrab %d %d %d %d\n",
                    field_11C_brain_sub_state,
                    field_12C_timer,
                    field_106_current_motion,
                    field_108_next_motion);
            }

            field_19C_max_ypos = field_BC_ypos;
            field_198_max_xpos = field_B8_xpos;

            (this->*sScrab_motion_table_560120[field_106_current_motion])();

            if (field_198_max_xpos != field_B8_xpos || field_19C_max_ypos != field_BC_ypos)
            {
                field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
                    nullptr,
                    field_B8_xpos,
                    field_BC_ypos,
                    field_B8_xpos,
                    field_BC_ypos);
                VOn_TLV_Collision_4087F0(field_FC_pPathTLV);
            }

            if (oldMotion != field_106_current_motion || field_1AA_flags.Get(Flags_1AA::eBit4_force_update_animation))
            {
                field_1AA_flags.Clear(Flags_1AA::eBit4_force_update_animation);
                vUpdateAnim_4A34F0();
            }
            else if (field_11E_return_to_previous_motion)
            {
                // NOTE: This never gets set to true, so this branch will never be executed.
                field_106_current_motion = field_F4_previous_motion;
                vUpdateAnim_4A34F0();
                field_20_animation.SetFrame_409D50(field_F6_anim_frame);
                field_11E_return_to_previous_motion = 0;
            }

            Update_Slurg_Step_Watch_Points_4A5780();

            if (field_178_shred_power_active)
            {
                if (sControlledCharacter_5C1B8C != this)
                {
                    field_13C_last_ypos = field_BC_ypos;
                    return;
                }

                if (field_10C_health > FP_FromInteger(0) && !(static_cast<s32>(sGnFrame_5C1B84) % 4))
                {
                    // Draw green particle
                    s16 particleXRand = Math_RandomRange_496AB0(40, 50);
                    const s16 particleYRand = Math_RandomRange_496AB0(45, 55);
                    FP particleVelX = (FP_FromInteger(-7) * field_CC_sprite_scale);
                    FP particleVelY = (FP_FromDouble(0.3) * field_CC_sprite_scale);

                    if (Math_NextRandom() & 1)
                    {
                        particleVelX = -particleVelX;
                        particleXRand = -particleXRand;
                    }

                    if (Math_NextRandom() & 1)
                    {
                        particleVelY = -particleVelY;
                    }

                    const FP particleYPos = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(particleYRand));
                    const FP particleXPos = field_B8_xpos + (field_CC_sprite_scale * FP_FromInteger(particleXRand));

                    New_Orb_Particle_426AA0(
                        particleXPos,
                        particleYPos,
                        particleVelX,
                        particleVelY,
                        field_CC_sprite_scale,
                        Layer::eLayer_0,
                        255,
                        0,
                        0);
                }
            }

            if (sControlledCharacter_5C1B8C == this && field_110_id != -1)
            {
                field_C8_vely = field_BC_ypos - field_13C_last_ypos;
                SetActiveCameraDelayedFromDir_408C40();
            }
            field_13C_last_ypos = field_BC_ypos;
            return;
        }
    }

    if (field_1AA_flags.Get(Flags_1AA::eBit6_persistant))
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Scrab::Update_Slurg_Step_Watch_Points_4A5780()
{
    if (field_106_current_motion == eScrabMotions::M_Walk_1_4A84D0 || field_106_current_motion == eScrabMotions::M_Run_2_4A89C0 || field_106_current_motion == eScrabMotions::M_Stamp_21_4A9CC0 || field_106_current_motion == eScrabMotions::M_AttackSpin_32_4A8DC0)
    {
        if (sGnFrame_5C1B84 & 1)
        {
            const s8 count = sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08];
            if (count < 5)
            {
                Slurg_Step_Watch_Points* pPoints = &sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08];
                pPoints->field_0_points[count].field_0_xPos = FP_GetExponent(field_B8_xpos);
                pPoints->field_0_points[count].field_2_yPos = field_100_pCollisionLine->field_0_rect.y - 5;
                sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = count + 1;
            }
        }
    }
}

enum Brain_0_Patrol
{
    eBrain0_ToMoving_0 = 0,
    eBrain0_Moving_1 = 1,
    eBrain0_Turning_2 = 2,
    eBrain0_Idle_3 = 3,
    eBrain0_Howling_4 = 4,
    eBrain0_Shriek_5 = 5,
    eBrain0_OnLift_6 = 6,
    eBrain0_KickFleech_7 = 7,
    eBrain0_ToSpeak_8 = 8,
    eBrain0_UsingInvisibility_9 = 9,
    eBrain0_PostInvisibilitySmash_10 = 10,
    eBrain0_BeingSpawned_11 = 11,
};

enum Brain_1_ChasingEnemy
{
    eBrain1_Inactive_0 = 0,
    eBrain1_Idle_1 = 1,
    eBrain1_Running_2 = 2,
    eBrain1_Turning_3 = 3,
    eBrain1_Panic_4 = 4,
    eBrain1_Falling_5 = 5,
    eBrain1_ToIdle_6 = 6,
    eBrain1_Jumping_7 = 7,
    eBrain1_Attacking_8 = 8,
    eBrain1_Walking_9 = 9,
    eBrain1_EnemyDead_10 = 10,
    eBrain1_SmashingEnemy_11 = 11,
    eBrain1_Eating_12 = 12,
    eBrain1_Howl_13 = 13,
    eBrain1_Shriek_14 = 14,
    eBrain1_PreparingToHowlOrShriek_15 = 15,
    eBrain1_KilledPossessedScrab_16 = 16
};

enum Brain_2_Fighting
{
    eBrain2_LookingForOpponent_0 = 0,
    eBrain2_SpottedOpponent_1 = 1,
    eBrain2_Turning_2 = 2,
    eBrain2_Walking_3 = 3,
    eBrain2_SetInPosition_4 = 4,
    eBrain2_Idle_5 = 5,
    eBrain2_Yelling_6 = 6,
    eBrain2_Shriek_7 = 7,
    eBrain2_ToRun_8 = 8,
    eBrain2_Running_9 = 9,
    eBrain2_Battling_10 = 10,
    eBrain2_Victorious_11 = 11,
    eBrain2_SmashingOpponent_12 = 12,
    eBrain2_VictoryYell_13 = 13,
    eBrain2_InterruptVictoryStates_14 = 14,
    eBrain2_WaitingForBattle_15 = 15
};

static const s32 kPatrolTypeRunOrWalkChance_546FD8[6] = {256, 192, 128, 64, 0, 0};

s16 Scrab::Brain_0_Patrol_4AA630()
{
    Scrab* pFighter = FindScrabToFight_4A4E20();
    if (pFighter)
    {
        SetBrain(&Scrab::Brain_2_Fighting_4A5840);
        field_124_fight_target_obj_id = pFighter->field_8_object_id;
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        return Brain_2_Fighting::eBrain2_LookingForOpponent_0;
    }

    if (FindAbeOrMud_4A4FD0())
    {
        auto pOtherScrab = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
        SetBrain(&Scrab::Brain_1_ChasingEnemy_4A6470);
        if (pOtherScrab->Type() == AETypes::eScrab_112 && pOtherScrab->field_114_flags.Get(Flags_114::e114_Bit4_bPossesed))
        {
            field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
            field_150_attack_delay_timer = sGnFrame_5C1B84 + 90;
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;
        }

        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        return Brain_1_ChasingEnemy::eBrain1_Inactive_0;
    }

    if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, -1) && !sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
        return Brain_0_Patrol::eBrain0_Howling_4;
    }

    auto pSwitch = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pSwitch && pSwitch->Type() == AETypes::eLiftPoint_78 && !(static_cast<LiftPoint*>(pSwitch)->vOnAnyFloor_461920() || field_11C_brain_sub_state != Brain_0_Patrol::eBrain0_OnLift_6))
    {
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        return Brain_0_Patrol::eBrain0_OnLift_6;
    }

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eTension_4, this, 0, 0);
    }

    if (field_106_current_motion == eScrabMotions::M_JumpToFall_8_4A9220 && field_11C_brain_sub_state != Brain_0_Patrol::eBrain0_BeingSpawned_11)
    {
        field_11C_brain_sub_state = Brain_0_Patrol::eBrain0_BeingSpawned_11;
    }

    switch (field_11C_brain_sub_state)
    {
        case Brain_0_Patrol::eBrain0_ToMoving_0:
            if (field_106_current_motion == eScrabMotions::M_Stand_0_4A8220)
            {
                field_194_speak = LastSpeak_4A56F0();
                if (field_1A2_speak_counter < field_1A0_speak_max && (field_194_speak == GameSpeakEvents::Scrab_Howl_53 || field_194_speak == GameSpeakEvents::Scrab_Shriek_54))
                {
                    return Brain_0_Patrol::eBrain0_ToSpeak_8;
                }
            }

            if (Check_IsOnEndOfLine_408E90(
                    field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX),
                    field_106_current_motion != eScrabMotions::M_Run_2_4A89C0 ? 1 : 3)
                || Handle_SlamDoor_or_EnemyStopper_4A4830(field_C4_velx, 1))
            {
                field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
                return Brain_0_Patrol::eBrain0_Turning_2;
            }

            if (Math_NextRandom() >= kPatrolTypeRunOrWalkChance_546FD8[field_12A_patrol_type_run_or_walk_chance])
            {
                field_108_next_motion = eScrabMotions::M_Run_2_4A89C0;
            }
            else
            {
                field_108_next_motion = eScrabMotions::M_Walk_1_4A84D0;
            }
            return Brain_0_Patrol::eBrain0_Moving_1;

        case Brain_0_Patrol::eBrain0_Moving_1:
            field_194_speak = Scrab::LastSpeak_4A56F0();
            if (field_1A2_speak_counter >= field_1A0_speak_max || (field_194_speak != GameSpeakEvents::Scrab_Howl_53 && field_194_speak != GameSpeakEvents::Scrab_Shriek_54))
            {
                if (Find_Fleech_4A4C90())
                {
                    field_108_next_motion = eScrabMotions::M_LegKick_38_4AA120;
                    return Brain_0_Patrol::eBrain0_KickFleech_7;
                }

                if (Check_IsOnEndOfLine_408E90(
                        field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX),
                        field_106_current_motion != eScrabMotions::M_Run_2_4A89C0 ? 1 : 3)
                    || // TODO: check order is correct
                    Handle_SlamDoor_or_EnemyStopper_4A4830(field_C4_velx, 1))
                {
                    field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
                    return Brain_0_Patrol::eBrain0_Turning_2;
                }

                if (Math_NextRandom() >= 3u || static_cast<s32>(sGnFrame_5C1B84 - field_154_movement_timer) <= 150)
                {
                    return field_11C_brain_sub_state;
                }

                field_108_next_motion = eScrabMotions::M_Shriek_30_4A9EA0;
                field_154_movement_timer = sGnFrame_5C1B84;
                return Brain_0_Patrol::eBrain0_Shriek_5;
            }
            else
            {
                field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                return Brain_0_Patrol::eBrain0_ToSpeak_8;
            }
            break;

        case Brain_0_Patrol::eBrain0_Turning_2:
        {
            if (field_106_current_motion != eScrabMotions::M_Turn_3_4A91A0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }

            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;

            s16 maxTimer = 0;
            s16 minTimer = 0;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                maxTimer = field_15A_left_max_delay;
                minTimer = field_158_left_min_delay;
            }
            else
            {
                maxTimer = field_15E_right_max_delay;
                minTimer = field_15C_right_min_delay;
            }
            field_12C_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(minTimer, maxTimer);
            return Brain_0_Patrol::eBrain0_Idle_3;
        }

        case Brain_0_Patrol::eBrain0_Idle_3:
            field_194_speak = LastSpeak_4A56F0();
            if (field_1A2_speak_counter >= field_1A0_speak_max || (field_194_speak != GameSpeakEvents::Scrab_Howl_53 && field_194_speak != GameSpeakEvents::Scrab_Shriek_54))
            {
                if (Find_Fleech_4A4C90())
                {
                    field_108_next_motion = eScrabMotions::M_LegKick_38_4AA120;
                    return Brain_0_Patrol::eBrain0_KickFleech_7;
                }
                else
                {
                    if (static_cast<s32>(sGnFrame_5C1B84) <= field_12C_timer)
                    {
                        return field_11C_brain_sub_state;
                    }

                    if (Math_NextRandom() >= 30u || static_cast<s32>(sGnFrame_5C1B84 - field_154_movement_timer) <= 150)
                    {
                        return Brain_0_Patrol::eBrain0_ToMoving_0;
                    }
                    field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
                    field_154_movement_timer = sGnFrame_5C1B84;
                    return Brain_0_Patrol::eBrain0_Howling_4;
                }
            }
            else
            {
                field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                return Brain_0_Patrol::eBrain0_ToSpeak_8;
            }
            break;

        case Brain_0_Patrol::eBrain0_Howling_4:
            if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, -1))
            {
                return field_11C_brain_sub_state;
            }
            if (field_106_current_motion == eScrabMotions::M_HowlBegin_26_4A9DA0 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                return Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            return field_11C_brain_sub_state;

        case Brain_0_Patrol::eBrain0_Shriek_5:
            if (field_106_current_motion == eScrabMotions::M_Shriek_30_4A9EA0 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                return Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            return field_11C_brain_sub_state;

        case Brain_0_Patrol::eBrain0_OnLift_6:
            if (pSwitch)
            {
                if (!static_cast<LiftPoint*>(pSwitch)->vOnAnyFloor_461920())
                {
                    return field_11C_brain_sub_state;
                }
                return Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            else
            {
                field_110_id = -1;
                return Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            break;

        case Brain_0_Patrol::eBrain0_KickFleech_7:
            if (field_106_current_motion == eScrabMotions::M_LegKick_38_4AA120 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                return Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            return field_11C_brain_sub_state;


        case Brain_0_Patrol::eBrain0_ToSpeak_8:
            if (field_106_current_motion != eScrabMotions::M_Stand_0_4A8220)
            {
                return field_11C_brain_sub_state;
            }

            field_1A2_speak_counter++;

            if (field_194_speak == GameSpeakEvents::Scrab_Shriek_54)
            {
                field_108_next_motion = eScrabMotions::M_Shriek_30_4A9EA0;
                field_154_movement_timer = sGnFrame_5C1B84;
                return Brain_0_Patrol::eBrain0_Shriek_5;
            }
            else if (field_194_speak == GameSpeakEvents::Scrab_Howl_53)
            {
                field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
                return Brain_0_Patrol::eBrain0_Howling_4;
            }
            else
            {
                return field_11C_brain_sub_state;
            }
            break;

        case Brain_0_Patrol::eBrain0_UsingInvisibility_9:
            if (field_106_current_motion != eScrabMotions::M_HowlBegin_26_4A9DA0 || !(field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame)))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Stamp_21_4A9CC0;
            field_12C_timer = sGnFrame_5C1B84 + 30;
            return Brain_0_Patrol::eBrain0_PostInvisibilitySmash_10;

        case Brain_0_Patrol::eBrain0_PostInvisibilitySmash_10:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame) && static_cast<s32>(sGnFrame_5C1B84) > field_12C_timer)
            {
                field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                return Brain_0_Patrol::eBrain0_ToMoving_0;
            }
            return field_11C_brain_sub_state;

        case Brain_0_Patrol::eBrain0_BeingSpawned_11:
            if (field_106_current_motion != eScrabMotions::M_Stand_0_4A8220)
            {
                return field_11C_brain_sub_state;
            }
            return Brain_0_Patrol::eBrain0_ToMoving_0;

        default:
            return field_11C_brain_sub_state;
    }
}

s16 Scrab::Brain_1_ChasingEnemy_4A6470()
{
    Scrab* pScrabToFight = FindScrabToFight_4A4E20();
    if (pScrabToFight)
    {
        SetBrain(&Scrab::Brain_2_Fighting_4A5840);
        field_124_fight_target_obj_id = pScrabToFight->field_8_object_id;
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        return Brain_2_Fighting::eBrain2_LookingForOpponent_0;
    }

    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    if (!pObj || field_6_flags.Get(BaseGameObject::eDead_Bit3) || (static_cast<s32>(sGnFrame_5C1B84) > field_14C_pause_after_chase_timer && !CanSeeAbe_4A51A0(pObj)))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        ToPatrol_4AA600();
        return Brain_0_Patrol::eBrain0_ToMoving_0;
    }

    if (pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        field_120_obj_id = -1;
        field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
        ToPatrol_4AA600();
        return Brain_0_Patrol::eBrain0_UsingInvisibility_9;
    }

    if (CanSeeAbe_4A51A0(pObj))
    {
        field_14C_pause_after_chase_timer = sGnFrame_5C1B84 + field_148_pause_after_chase_delay;
    }

    if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, -1) && field_11C_brain_sub_state != 26) //TODO OG bug? field_108_next_motion instead of field_11C_sub_state
    {
        field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
        return Brain_1_ChasingEnemy::eBrain1_Panic_4;
    }

    LiftPoint* pLiftPoint = static_cast<LiftPoint*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pLiftPoint && pLiftPoint->Type() != AETypes::eLiftPoint_78)
    {
        pLiftPoint = nullptr; //OG bug fix: Before it could use the pointer as a LiftPoint even if it, in fact, wasn't one
    }
    if (pLiftPoint && !pLiftPoint->vOnAnyFloor_461920() && field_11C_brain_sub_state != Brain_1_ChasingEnemy::eBrain1_Panic_4)
    {
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        return Brain_1_ChasingEnemy::eBrain1_Panic_4;
    }

    if (gMap_5C3030.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos)
        >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    switch (field_11C_brain_sub_state)
    {
        case Brain_1_ChasingEnemy::eBrain1_Inactive_0:
        {
            field_150_attack_delay_timer = sGnFrame_5C1B84 + field_128_attack_delay;
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;
        }
        case Brain_1_ChasingEnemy::eBrain1_Idle_1:
        {
            if (field_106_current_motion == eScrabMotions::M_Stand_0_4A8220)
            {
                field_194_speak = LastSpeak_4A56F0();
                if (field_1A2_speak_counter < field_1A0_speak_max
                    && (LastSpeak_4A56F0() == GameSpeakEvents::Scrab_Howl_53
                        || LastSpeak_4A56F0() == GameSpeakEvents::Scrab_Shriek_54))
                {
                    return Brain_1_ChasingEnemy::eBrain1_PreparingToHowlOrShriek_15;
                }
            }

            if (!vIsFacingMe_4254A0(pObj))
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                {
                    field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
                }
                else
                {
                    field_106_current_motion = eScrabMotions::M_Turn_3_4A91A0;
                    field_108_next_motion = -1;
                    MapFollowMe_408D10(TRUE);
                }
                return Brain_1_ChasingEnemy::eBrain1_Turning_3;
            }

            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromDouble(1.5), pObj)
                && vOnSameYLevel_425520(pObj))
            {
                if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pObj))
                {
                    field_108_next_motion = eScrabMotions::M_AttackLunge_37_4AA0B0;
                    return Brain_1_ChasingEnemy::eBrain1_Attacking_8;
                }
                field_108_next_motion = eScrabMotions::M_LegKick_38_4AA120;
                return Brain_1_ChasingEnemy::eBrain1_Attacking_8;
            }
            else if (CanSeeAbe_4A51A0(pObj) && static_cast<s32>(sGnFrame_5C1B84) >= field_150_attack_delay_timer)
            {
                if (Handle_SlamDoor_or_EnemyStopper_4A4830(field_C4_velx, 0))
                {
                    return field_11C_brain_sub_state;
                }
                field_108_next_motion = eScrabMotions::M_Run_2_4A89C0;
                return Brain_1_ChasingEnemy::eBrain1_Running_2;
            }
            else
            {
                if (Math_NextRandom() >= 16u || static_cast<s32>(sGnFrame_5C1B84 - field_154_movement_timer) <= 60)
                {
                    return Brain_1_ChasingEnemy::eBrain1_Idle_1;
                }
                field_108_next_motion = eScrabMotions::M_Shriek_30_4A9EA0;
                field_154_movement_timer = sGnFrame_5C1B84;
                return Brain_1_ChasingEnemy::eBrain1_Shriek_14;
            }
        }
        case Brain_1_ChasingEnemy::eBrain1_Running_2:
            return Brain_ChasingEnemy_State_2_Running(pObj);

        case Brain_1_ChasingEnemy::eBrain1_Turning_3:
            if (field_106_current_motion == eScrabMotions::M_Turn_3_4A91A0)
            {
                if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    return field_11C_brain_sub_state;
                }
                field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                return Brain_1_ChasingEnemy::eBrain1_Idle_1;
            }

            if (field_108_next_motion != eScrabMotions::M_Turn_3_4A91A0)
            {
                field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
            }

            if (field_106_current_motion != eScrabMotions::M_Shriek_30_4A9EA0)
            {
                return field_11C_brain_sub_state;
            }

            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Panic_4:
            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pObj)
                && vOnSameYLevel_425520(pObj))
            {
                field_108_next_motion = eScrabMotions::M_LegKick_38_4AA120;
                return Brain_1_ChasingEnemy::eBrain1_Attacking_8;
            }

            if (pLiftPoint)
            {
                if (!pLiftPoint->vOnAnyFloor_461920())
                {
                    return field_11C_brain_sub_state;
                }
            }
            else
            {
                field_110_id = -1;
            }
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Falling_5:
        case Brain_1_ChasingEnemy::eBrain1_ToIdle_6:
            if (field_106_current_motion)
            {
                return field_11C_brain_sub_state;
            }
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Jumping_7:
            if (field_106_current_motion != eScrabMotions::M_RunJumpEnd_13_4A9BE0)
            {
                return field_11C_brain_sub_state;
            }
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Attacking_8:
            if ((field_106_current_motion != eScrabMotions::M_AttackLunge_37_4AA0B0
                 && field_106_current_motion != eScrabMotions::M_LegKick_38_4AA120)
                || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }

            if (pObj->field_10C_health > FP_FromInteger(0))
            {
                return Brain_1_ChasingEnemy::eBrain1_Idle_1;
                break;
            }

            if (vIsFacingMe_4254A0(pObj))
            {
                if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pObj))
                {
                    if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pObj))
                    {
                        field_108_next_motion = eScrabMotions::M_Walk_1_4A84D0;
                    }
                }
                return Brain_1_ChasingEnemy::eBrain1_Walking_9;
            }
            else
            {
                field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
                return Brain_1_ChasingEnemy::eBrain1_EnemyDead_10;
            }

        case Brain_1_ChasingEnemy::eBrain1_Walking_9:
            if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pObj))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Stamp_21_4A9CC0;
            field_12C_timer = sGnFrame_5C1B84 + 30;
            return Brain_1_ChasingEnemy::eBrain1_SmashingEnemy_11;

        case Brain_1_ChasingEnemy::eBrain1_EnemyDead_10:
            if (field_106_current_motion != eScrabMotions::M_Turn_3_4A91A0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }

            if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pObj))
            {
                field_108_next_motion = eScrabMotions::M_Walk_1_4A84D0;
            }
            return Brain_1_ChasingEnemy::eBrain1_Walking_9;

        case Brain_1_ChasingEnemy::eBrain1_SmashingEnemy_11:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_12C_timer)
            {
                return field_11C_brain_sub_state;
            }

            if (pObj->field_BC_ypos - field_BC_ypos < FP_FromInteger(0))
            {
                pObj->field_BC_ypos - field_BC_ypos = -(pObj->field_BC_ypos - field_BC_ypos);
            }

            if (pObj->field_BC_ypos - field_BC_ypos >= FP_FromInteger(5))
            {
                field_108_next_motion = eScrabMotions::M_Shriek_30_4A9EA0;
                return Brain_1_ChasingEnemy::eBrain1_Shriek_14;
            }
            else
            {
                field_108_next_motion = eScrabMotions::M_StandToFeed_35_4AA010;
                return Brain_1_ChasingEnemy::eBrain1_Eating_12;
            }

        case Brain_1_ChasingEnemy::eBrain1_Eating_12:
            if (field_106_current_motion == eScrabMotions::M_FeedToGulp_33_4A9FA0)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
                {
                    field_108_next_motion = eScrabMotions::M_StandToFeed_35_4AA010;
                }
            }
            return field_11C_brain_sub_state;

        case Brain_1_ChasingEnemy::eBrain1_Howl_13:
            if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, -1))
            {
                return field_11C_brain_sub_state;
            }

            field_106_current_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
            if (field_106_current_motion != eScrabMotions::M_Shriek_30_4A9EA0)
            {
                return field_11C_brain_sub_state;
            }
            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_Shriek_14:
            if (field_106_current_motion != eScrabMotions::M_Shriek_30_4A9EA0)
            {
                return field_11C_brain_sub_state;
            }

            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            return Brain_1_ChasingEnemy::eBrain1_Idle_1;

        case Brain_1_ChasingEnemy::eBrain1_PreparingToHowlOrShriek_15:
            if (field_106_current_motion)
            {
                return field_11C_brain_sub_state;
            }

            field_1A2_speak_counter++;
            if (field_194_speak == GameSpeakEvents::Scrab_Shriek_54)
            {
                field_108_next_motion = eScrabMotions::M_Shriek_30_4A9EA0;
                field_154_movement_timer = sGnFrame_5C1B84;
                return Brain_1_ChasingEnemy::eBrain1_Shriek_14;
            }

            if (field_194_speak != GameSpeakEvents::Scrab_Howl_53)
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
            return Brain_1_ChasingEnemy::eBrain1_Howl_13;

        case Brain_1_ChasingEnemy::eBrain1_KilledPossessedScrab_16:
            if (field_106_current_motion == eScrabMotions::M_JumpToFall_8_4A9220)
            {
                return Brain_1_ChasingEnemy::eBrain1_Falling_5;
            }
            else if (field_106_current_motion == eScrabMotions::M_AttackSpin_32_4A8DC0)
            {
                return field_11C_brain_sub_state;
            }
            else
            {
                return Brain_1_ChasingEnemy::eBrain1_Idle_1;
            }
        default:
            return field_11C_brain_sub_state;
    }
}

s16 Scrab::Brain_ChasingEnemy_State_2_Running(BaseAliveGameObject* pObj)
{
    field_194_speak = LastSpeak_4A56F0();
    if (field_1A2_speak_counter < field_1A0_speak_max
        && (LastSpeak_4A56F0() == GameSpeakEvents::Scrab_Howl_53
            || LastSpeak_4A56F0() == GameSpeakEvents::Scrab_Shriek_54))
    {
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        return Brain_1_ChasingEnemy::eBrain1_PreparingToHowlOrShriek_15;
    }

    if (Handle_SlamDoor_or_EnemyStopper_4A4830(field_C4_velx, 0))
    {
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        return Brain_1_ChasingEnemy::eBrain1_Idle_1;
    }

    s32 xPosition = abs(FP_GetExponent(field_B8_xpos) - SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)));

    //Jump at the end of the ledge
    FP xOffset = (field_C4_velx >= FP_FromInteger(0)) ? ScaleToGridSize_4498B0(field_CC_sprite_scale) : -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (xPosition < 6
        && Check_IsOnEndOfLine_408E90(field_C4_velx < FP_FromInteger(0), 1)
        && ((pObj->field_BC_ypos - field_BC_ypos < FP_FromInteger(5))
            || sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos + xOffset),
                FP_GetExponent(field_BC_ypos + FP_FromInteger(10)),
                FP_GetExponent(field_B8_xpos + xOffset),
                FP_GetExponent(field_BC_ypos + FP_FromInteger(10)),
                TlvTypes::ElectricWall_38))
        && !Check_IsOnEndOfLine_408E90(field_C4_velx < FP_FromInteger(0), 3))
    {
        ToJump_4A75E0();
        field_108_next_motion = -1;
        return Brain_1_ChasingEnemy::eBrain1_Jumping_7;
    }

    if (vIsFacingMe_4254A0(pObj))
    {
        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(7), pObj)
            && pObj->field_114_flags.Get(Flags_114::e114_Bit4_bPossesed)
            && pObj->Type() == AETypes::eScrab_112)
        {
            if (!field_178_shred_power_active)
            {
                field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
                return Brain_1_ChasingEnemy::eBrain1_Howl_13;
            }
            if (!Math_NextRandom())
            {
                field_108_next_motion = eScrabMotions::M_AttackSpin_32_4A8DC0;
                return Brain_1_ChasingEnemy::eBrain1_KilledPossessedScrab_16;
            }
        }

        if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pObj)
            && field_106_current_motion == eScrabMotions::M_Run_2_4A89C0
            && vOnSameYLevel_425520(pObj))
        {
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), pObj->field_B8_xpos - field_B8_xpos))
            {
                field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                return Brain_1_ChasingEnemy::eBrain1_Idle_1;
            }
            else
            {
                field_108_next_motion = eScrabMotions::M_AttackLunge_37_4AA0B0;
                return Brain_1_ChasingEnemy::eBrain1_Attacking_8;
            }
        }
        else
        {
            if (field_106_current_motion != eScrabMotions::M_JumpToFall_8_4A9220)
            {
                //A patch workaround: This situation should probably never happen in the first place, but since it does...
                if (field_106_current_motion == eScrabMotions::M_Stand_0_4A8220 && field_108_next_motion == -1)
                {
                    field_108_next_motion = eScrabMotions::M_Run_2_4A89C0;
                }
                return field_11C_brain_sub_state;
            }
            return Brain_1_ChasingEnemy::eBrain1_Falling_5;
        }
    }
    else
    {
        if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
        }
        else
        {
            field_106_current_motion = eScrabMotions::M_Turn_3_4A91A0;
            field_108_next_motion = -1;
            MapFollowMe_408D10(TRUE);
        }
        return Brain_1_ChasingEnemy::eBrain1_Turning_3;
    }
}

s16 Scrab::Brain_2_Fighting_4A5840()
{
    auto pTarget = static_cast<Scrab*>(sObjectIds_5C1B70.Find_449CF0(field_124_fight_target_obj_id));
    if (field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_Battling_10 && field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_Victorious_11 && field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_SmashingOpponent_12 && field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_VictoryYell_13 && (!pTarget || pTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3) || (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), pTarget->field_B8_xpos - field_B8_xpos)) || !vOnSameYLevel_425520(pTarget)))
    {
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        ToPatrol_4AA600();
        return Brain_0_Patrol::eBrain0_ToMoving_0;
    }

    if (pTarget && pTarget->field_114_flags.Get(Flags_114::e114_Bit4_bPossesed) && field_10C_health > FP_FromInteger(0) && pTarget->field_10C_health > FP_FromInteger(0))
    {
        field_120_obj_id = field_124_fight_target_obj_id;
        field_124_fight_target_obj_id = -1;
        SetBrain(&Scrab::Brain_1_ChasingEnemy_4A6470);
        field_150_attack_delay_timer = sGnFrame_5C1B84 + 90;
        field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
        return Brain_2_Fighting::eBrain2_SpottedOpponent_1;
    }

    if (Event_Get_422C00(kEventAbeOhm))
    {
        if (field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_Running_9 && field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_Battling_10)
        {
            if (field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_Victorious_11 && field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_SmashingOpponent_12 && field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_VictoryYell_13)
            {
                field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
                ToPatrol_4AA600();
                return Brain_0_Patrol::eBrain0_Howling_4;
            }

            if (pTarget->field_11C_brain_sub_state != Brain_2_Fighting::eBrain2_Running_9 && field_108_next_motion == -1)
            {
                field_11C_brain_sub_state = Brain_2_Fighting::eBrain2_InterruptVictoryStates_14;
                field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
            }
        }
    }

    if (gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) >= CameraPos::eCamCurrent_0)
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    }

    switch (field_11C_brain_sub_state)
    {
        case Brain_2_Fighting::eBrain2_LookingForOpponent_0:
            if (field_106_current_motion)
            {
                if (field_108_next_motion != eScrabMotions::M_Stand_0_4A8220)
                {
                    field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
                }
                return Brain_2_Fighting::eBrain2_LookingForOpponent_0;
            }

            field_1AA_flags.Clear(Flags_1AA::eBit1_attacking);
            field_108_next_motion = -1;
            if (pTarget->field_124_fight_target_obj_id == -1 || pTarget->field_124_fight_target_obj_id == field_8_object_id)
            {
                if (vIsFacingMe_4254A0(pTarget))
                {
                    if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(8), pTarget))
                    {
                        if (pTarget->field_106_current_motion == eScrabMotions::M_HowlBegin_26_4A9DA0)
                        {
                            return field_11C_brain_sub_state;
                        }
                        field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
                        return Brain_2_Fighting::eBrain2_Yelling_6;
                    }
                    field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
                    return Brain_2_Fighting::eBrain2_Turning_2;
                }
                else
                {
                    field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
                    return Brain_2_Fighting::eBrain2_SpottedOpponent_1;
                }
            }
            else
            {
                field_124_fight_target_obj_id = -1;
                field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
                return Brain_2_Fighting::eBrain2_WaitingForBattle_15;
            }
            break;

        case Brain_2_Fighting::eBrain2_SpottedOpponent_1:
            if (field_106_current_motion != eScrabMotions::M_Turn_3_4A91A0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }

            if (vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(8), pTarget))
            {
                field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
                return Brain_2_Fighting::eBrain2_Turning_2;
            }
            else
            {
                field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
                return Brain_2_Fighting::eBrain2_Yelling_6;
            }
            break;

        case Brain_2_Fighting::eBrain2_Turning_2:
            if (field_106_current_motion != eScrabMotions::M_Turn_3_4A91A0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Walk_1_4A84D0;
            return Brain_2_Fighting::eBrain2_Walking_3;

        case Brain_2_Fighting::eBrain2_Walking_3:
        {
            auto xPos = field_B8_xpos;
            s16 sIsFlipped = 0;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                sIsFlipped = 1;
                xPos -= FP_FromInteger(50);
            }
            else
            {
                xPos += FP_FromInteger(50);
            }
            if (!Handle_SlamDoor_or_EnemyStopper_4A4830(field_C4_velx, 0) && !Check_IsOnEndOfLine_408E90(sIsFlipped, 2) && vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(10), pTarget))
            {
                if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, xPos, field_BC_ypos, 0))
                {
                    return field_11C_brain_sub_state;
                }
            }
            field_108_next_motion = eScrabMotions::M_Turn_3_4A91A0;
            return Brain_2_Fighting::eBrain2_SetInPosition_4;
        }
        case Brain_2_Fighting::eBrain2_SetInPosition_4:
            if (field_106_current_motion != eScrabMotions::M_Turn_3_4A91A0)
            {
                return field_11C_brain_sub_state;
            }
            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
            return Brain_2_Fighting::eBrain2_Yelling_6;

        case Brain_2_Fighting::eBrain2_Idle_5:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_12C_timer)
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Shriek_30_4A9EA0;
            return Brain_2_Fighting::eBrain2_Shriek_7;

        case Brain_2_Fighting::eBrain2_Yelling_6:
            if (field_106_current_motion != eScrabMotions::M_HowlBegin_26_4A9DA0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            field_12C_timer = sGnFrame_5C1B84 + 20;
            return Brain_2_Fighting::eBrain2_Idle_5;

        case Brain_2_Fighting::eBrain2_Shriek_7:
            if (field_106_current_motion != eScrabMotions::M_Shriek_30_4A9EA0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }
            field_1AA_flags.Set(Flags_1AA::eBit1_attacking);
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            return Brain_2_Fighting::eBrain2_ToRun_8;

        case Brain_2_Fighting::eBrain2_ToRun_8:
            if (!pTarget->field_1AA_flags.Get(Flags_1AA::eBit1_attacking))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Run_2_4A89C0;
            return Brain_2_Fighting::eBrain2_Running_9;

        case Brain_2_Fighting::eBrain2_Running_9:
            if (!vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale), pTarget) || !field_100_pCollisionLine || !pTarget->field_100_pCollisionLine)
            {
                return field_11C_brain_sub_state;
            }

            if (pTarget->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_10C_health = FP_FromInteger(0);
            }

            MapFollowMe_408D10(TRUE);
            field_106_current_motion = eScrabMotions::M_ScrabBattleAnim_31_4A9F30;
            field_12C_timer = sGnFrame_5C1B84 + 40;
            return Brain_2_Fighting::eBrain2_Battling_10;

        case Brain_2_Fighting::eBrain2_Battling_10:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_12C_timer)
            {
                return field_11C_brain_sub_state;
            }

            SND_Stop_Channels_Mask_4CA810(field_160_sfx_bitmask);
            field_160_sfx_bitmask = 0;
            Scrab_SFX_4AADB0(ScrabSounds::eDeathHowl_1, 0, -1571, 1);
            Scrab_SFX_4AADB0(ScrabSounds::eYell_8, 0, -1571, 1);
            Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 0, -383, 0);

            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                SetBrain(&Scrab::Brain_3_Death_4A62B0);
                field_B8_xpos = pTarget->field_B8_xpos;
                field_106_current_motion = eScrabMotions::M_DeathBegin_39_4AA190;
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_12C_timer = sGnFrame_5C1B84 + 90;
                return field_11C_brain_sub_state;
            }
            field_106_current_motion = eScrabMotions::M_Stand_0_4A8220;
            field_12C_timer = sGnFrame_5C1B84 + 5;
            return Brain_2_Fighting::eBrain2_Victorious_11;

        case Brain_2_Fighting::eBrain2_Victorious_11:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_12C_timer)
            {
                return field_11C_brain_sub_state;
            }
            field_12C_timer = 5;
            field_108_next_motion = eScrabMotions::M_Stamp_21_4A9CC0;
            return Brain_2_Fighting::eBrain2_SmashingOpponent_12;

        case Brain_2_Fighting::eBrain2_SmashingOpponent_12:
            if (field_106_current_motion != eScrabMotions::M_Stamp_21_4A9CC0)
            {
                return field_11C_brain_sub_state;
            }

            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }

            field_12C_timer--;
            if (field_12C_timer > 0)
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
            return Brain_2_Fighting::eBrain2_VictoryYell_13;

        case Brain_2_Fighting::eBrain2_VictoryYell_13:
            if (field_106_current_motion != eScrabMotions::M_HowlBegin_26_4A9DA0 || !field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }
            field_124_fight_target_obj_id = -1;
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            ToPatrol_4AA600();
            return Brain_0_Patrol::eBrain0_ToMoving_0;

        case Brain_2_Fighting::eBrain2_InterruptVictoryStates_14:
            if (Event_Get_422C00(kEventAbeOhm))
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            return Brain_2_Fighting::eBrain2_LookingForOpponent_0;

        case Brain_2_Fighting::eBrain2_WaitingForBattle_15:
        {
            if (field_106_current_motion != eScrabMotions::M_HowlBegin_26_4A9DA0)
            {
                return field_11C_brain_sub_state;
            }
            if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                return field_11C_brain_sub_state;
            }

            auto pFoundTarget = FindScrabToFight_4A4E20();
            if (!pFoundTarget)
            {
                return field_11C_brain_sub_state;
            }
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            field_124_fight_target_obj_id = pFoundTarget->field_8_object_id;
            return Brain_2_Fighting::eBrain2_LookingForOpponent_0;
        }

        default:
            return field_11C_brain_sub_state;
    }
}

s16 Scrab::Brain_3_Death_4A62B0()
{
    if (field_12C_timer < static_cast<s32>((sGnFrame_5C1B84 + 80)))
    {
        field_CC_sprite_scale -= FP_FromDouble(0.008);
        field_D0_r -= 2;
        field_D2_g -= 2;
        field_D4_b -= 2;
    }

    if (static_cast<s32>(sGnFrame_5C1B84) < field_12C_timer - 24)
    {
        DeathSmokeEffect(true);
    }

    if (field_12C_timer < static_cast<s32>(sGnFrame_5C1B84))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return 0;
}

s16 Scrab::Brain_4_ShrinkDeath_4A6420()
{
    if (field_D6_scale == 1)
    {
        field_CC_sprite_scale -= FP_FromDouble(0.06);
    }
    else
    {
        field_CC_sprite_scale -= FP_FromDouble(0.03);
    }

    if (field_12C_timer < static_cast<s32>(sGnFrame_5C1B84))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return 0;
}

s16 Scrab::Brain_5_Possessed_4A6180()
{
    // Abe is dead, go back to patrolling
    if (sActiveHero_5C1B68->field_10C_health <= FP_FromInteger(0))
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
        field_1A2_speak_counter = 0;
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
        ToPatrol_4AA600();
        field_11C_brain_sub_state = Brain_0_Patrol::eBrain0_ToMoving_0;
        gMap_5C3030.SetActiveCam_480D30(field_166_level, field_168_path, field_16A_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        return field_11C_brain_sub_state;
    }

    if (field_11C_brain_sub_state == 0)
    {
        if (static_cast<s32>(sGnFrame_5C1B84) <= field_12C_timer)
        {
            return field_11C_brain_sub_state;
        }
        field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
        return 1;
    }
    else
    {
        if (field_11C_brain_sub_state != 1 || gMap_5C3030.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos) == CameraPos::eCamInvalid_m1)
        {
            return field_11C_brain_sub_state;
        }
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
        return field_11C_brain_sub_state;
    }
}

void Scrab::M_Stand_0_4A8220()
{
    if (BrainIs(&Scrab::Brain_5_Possessed_4A6180))
    {
        if (field_11C_brain_sub_state != 1)
        {
            return;
        }

        if (Input_IsChanting_45F260() && !field_164_prevent_depossession)
        {
            field_130_depossession_timer = sGnFrame_5C1B84 + 30;
            SFX_Play_46FA90(SoundEffect::PossessEffect_17, 0);
            field_106_current_motion = eScrabMotions::M_GetDepossessedBegin_28_4AA200;
            return;
        }

        if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak2_5550F8))
        {
            field_106_current_motion = eScrabMotions::M_Shriek_30_4A9EA0;
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            return;
        }

        if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak1_555104))
        {
            field_106_current_motion = eScrabMotions::M_HowlBegin_26_4A9DA0;
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            return;
        }

        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eThrowItem | InputCommands::Enum::eDoAction) && field_178_shred_power_active)
        {
            field_106_current_motion = eScrabMotions::M_AttackSpin_32_4A8DC0;
            field_108_next_motion = -1;
            field_12C_timer = sGnFrame_5C1B84 + field_174_possessed_max_whirl_attack_duration;
            return;
        }

        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eThrowItem | InputCommands::Enum::eDoAction))
        {
            field_106_current_motion = eScrabMotions::M_Stamp_21_4A9CC0;
            field_108_next_motion = -1;
            return;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Hop_5550E0))
        {
            const FP k45Scaled = (field_CC_sprite_scale * FP_FromInteger(45));
            const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                if (!WallHit_408750(k45Scaled, -kGridSize))
                {
                    field_106_current_motion = eScrabMotions::M_HopBegin_5_4A96C0;
                    field_108_next_motion = -1;
                    return;
                }
            }
            else
            {
                if (!WallHit_408750(k45Scaled, kGridSize))
                {
                    field_106_current_motion = eScrabMotions::M_HopBegin_5_4A96C0;
                    field_108_next_motion = -1;
                    return;
                }
            }
        }
    }
    ToNextMotion_4A7920();
}

const FP sScrabWalkXVels_546E54[22] = {
    FP_FromDouble(1.570587158203125),
    FP_FromDouble(1.5780487060546875),
    FP_FromDouble(1.5256805419921875),
    FP_FromDouble(1.5651397705078125),
    FP_FromDouble(1.5966339111328125),
    FP_FromDouble(3.694122314453125),
    FP_FromDouble(3.9574737548828125),
    FP_FromDouble(3.9715423583984375),
    FP_FromDouble(3.3073272705078125),
    FP_FromDouble(2.629669189453125),
    FP_FromDouble(2.1679229736328125),
    FP_FromDouble(2.0220794677734375),
    FP_FromDouble(1.804046630859375),
    FP_FromDouble(1.6065216064453125),
    FP_FromDouble(2.6418609619140625),
    FP_FromDouble(2.685821533203125),
    FP_FromDouble(2.673095703125),
    FP_FromDouble(2.691070556640625),
    FP_FromDouble(1.3966827392578125),
    FP_FromDouble(1.376983642578125),
    FP_FromDouble(1.3767242431640625),
    FP_FromDouble(2.1646728515625)};

void Scrab::M_Walk_1_4A84D0()
{
    field_C4_velx = field_CC_sprite_scale * sScrabWalkXVels_546E54[field_20_animation.field_92_current_frame];

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
    }

    MoveOnLine_4A7D20();

    if (field_106_current_motion != eScrabMotions::M_Walk_1_4A84D0)
    {
        return;
    }

    FP kGridSize = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        kGridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), kGridSize * FP_FromDouble(0.5)))
    {
        KnockBack_4AA530();
        return;
    }

    switch (field_20_animation.field_92_current_frame)
    {
        case 3:
        case 13:
            Scrab_SFX_4AADB0(ScrabSounds::eWalk1_6, Math_RandomRange_496AB0(40, 50), 0x7FFF, 1);
            return;

        case 5:
        case 15:
            if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), kGridSize * FP_FromInteger(1)))
            {
                field_106_current_motion = eScrabMotions::M_WalkToStand_11_4A8880;
            }
            else if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion == eScrabMotions::M_Stand_0_4A8220 || field_108_next_motion == eScrabMotions::M_Turn_3_4A91A0 || field_108_next_motion == eScrabMotions::M_Stamp_21_4A9CC0 || field_108_next_motion == eScrabMotions::M_Empty_25_4A34D0 || field_108_next_motion == eScrabMotions::M_Shriek_30_4A9EA0 || field_108_next_motion == eScrabMotions::M_HowlBegin_26_4A9DA0 || field_108_next_motion == eScrabMotions::M_HopMidair_6_4A9490 || field_108_next_motion == eScrabMotions::M_AttackLunge_37_4AA0B0 || field_108_next_motion == eScrabMotions::M_LegKick_38_4AA120)
                {
                    field_106_current_motion = eScrabMotions::M_WalkToStand_11_4A8880;
                }
            }
            else
            {
                if ((field_C4_velx > FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft)) || (field_C4_velx < FP_FromInteger(0) && sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight)) || !sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight | InputCommands::Enum::eLeft))
                {
                    field_106_current_motion = eScrabMotions::M_WalkToStand_11_4A8880;
                }
            }
            return;

        case 7:
        case 18:
            Scrab_SFX_4AADB0(ScrabSounds::eWalk1_6, Math_RandomRange_496AB0(40, 50), 0x7FFF, 1);
            if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
            {
                if (field_108_next_motion != eScrabMotions::M_Run_2_4A89C0)
                {
                    MapFollowMe_408D10(TRUE);
                    return;
                }
                field_106_current_motion = eScrabMotions::M_WalkToRun_16_4A8D60;
                field_108_next_motion = -1;
                MapFollowMe_408D10(TRUE);
                return;
            }

            if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eThrowItem | InputCommands::Enum::eDoAction) && field_178_shred_power_active)
            {
                field_106_current_motion = eScrabMotions::M_AttackSpin_32_4A8DC0;
                field_12C_timer = sGnFrame_5C1B84 + field_174_possessed_max_whirl_attack_duration;
                field_108_next_motion = -1;
                MapFollowMe_408D10(TRUE);
                return;
            }

            if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRun))
            {
                field_106_current_motion = eScrabMotions::M_WalkToRun_16_4A8D60;
                field_108_next_motion = -1;
                MapFollowMe_408D10(TRUE);
                return;
            }

            if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eHop))
            {
                field_106_current_motion = eScrabMotions::M_HopBegin_5_4A96C0;
                field_108_next_motion = -1;
                MapFollowMe_408D10(TRUE);
                return;
            }

            MapFollowMe_408D10(TRUE);
            return;

        default:
            return;
    }
}

static const FP sScrabRunVelX_546EC4[14] = {
    FP_FromDouble(9.498733520507812),
    FP_FromDouble(9.733749389648438),
    FP_FromDouble(9.584152221679688),
    FP_FromDouble(9.141021728515625),
    FP_FromDouble(9.099105834960938),
    FP_FromDouble(9.576034545898438),
    FP_FromDouble(9.327011108398438),
    FP_FromDouble(8.654891967773438),
    FP_FromDouble(5.2911224365234375),
    FP_FromDouble(4.2801666259765625),
    FP_FromDouble(3.446380615234375),
    FP_FromDouble(3.3974456787109375),
    FP_FromDouble(3.8137969970703125),
    FP_FromDouble(5.15692138671875)};

void Scrab::M_Run_2_4A89C0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    field_C4_velx = field_CC_sprite_scale * sScrabRunVelX_546EC4[field_20_animation.field_92_current_frame];

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
    }

    FP gridSize = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), gridSize * FP_FromDouble(0.5)))
    {
        KnockBack_4AA530();
        return;
    }

    MoveOnLine_4A7D20();

    if (field_106_current_motion == eScrabMotions::M_Run_2_4A89C0)
    {
        KillTarget_4A7F20(pTarget);
        switch (field_20_animation.field_92_current_frame)
        {
            case 0:
            case 5:
            case 7:
            case 12:
                Scrab_SFX_4AADB0(ScrabSounds::eWalk1_6, Math_RandomRange_496AB0(40, 50), 0x7FFF, 1);
                return;

            case 3:
            case 10:
                if (sControlledCharacter_5C1B8C != this || field_10C_health <= FP_FromInteger(0))
                {
                    if (field_108_next_motion == eScrabMotions::M_Walk_1_4A84D0)
                    {
                        field_106_current_motion = eScrabMotions::M_RunToWalk_17_4A8D90;
                        field_108_next_motion = -1;
                        MapFollowMe_408D10(TRUE);
                        return;
                    }

                    if (field_108_next_motion != eScrabMotions::M_Stand_0_4A8220 && field_108_next_motion != eScrabMotions::M_Turn_3_4A91A0 && field_108_next_motion != eScrabMotions::M_HopMidair_6_4A9490 && field_108_next_motion != eScrabMotions::M_AttackLunge_37_4AA0B0 && field_108_next_motion != eScrabMotions::M_Empty_25_4A34D0 && field_108_next_motion != eScrabMotions::M_HowlBegin_26_4A9DA0 && field_108_next_motion != eScrabMotions::M_LegKick_38_4AA120 && field_108_next_motion != eScrabMotions::M_Shriek_30_4A9EA0)
                    {
                        if (field_108_next_motion == eScrabMotions::M_AttackSpin_32_4A8DC0)
                        {
                            field_106_current_motion = eScrabMotions::M_AttackSpin_32_4A8DC0;
                            MapFollowMe_408D10(TRUE);
                            return;
                        }

                        if (field_108_next_motion == eScrabMotions::M_ScrabBattleAnim_31_4A9F30)
                        {
                            ToStand_4A75A0();
                            field_106_current_motion = eScrabMotions::M_ScrabBattleAnim_31_4A9F30;
                            field_108_next_motion = -1;
                            MapFollowMe_408D10(TRUE);
                            return;
                        }

                        MapFollowMe_408D10(TRUE);
                        return;
                    }
                }
                else
                {
                    if ((field_C4_velx <= FP_FromInteger(0) || !sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4)) && (field_C4_velx >= FP_FromInteger(0) || !sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0)))
                    {
                        if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
                        {
                            if (!sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
                            {
                                field_106_current_motion = eScrabMotions::M_RunToWalk_17_4A8D90;
                                MapFollowMe_408D10(TRUE);
                                return;
                            }

                            if (sInputObject_5BD4E0.isPressed(sInputKey_Hop_5550E0))
                            {
                                ToJump_4A75E0();
                                MapFollowMe_408D10(TRUE);
                                return;
                            }

                            if (sInputObject_5BD4E0.isPressed(0xA0) && field_178_shred_power_active)
                            {
                                field_12C_timer = MakeTimer(field_174_possessed_max_whirl_attack_duration);
                                field_106_current_motion = eScrabMotions::M_AttackSpin_32_4A8DC0;
                                field_108_next_motion = -1;
                                MapFollowMe_408D10(TRUE);
                                return;
                            }

                            MapFollowMe_408D10(TRUE);
                            return;
                        }
                    }
                }
                field_106_current_motion = eScrabMotions::M_RunToStand_4_4A90C0;
                MapFollowMe_408D10(TRUE);
                return;

            default:
                return;
        }
    }
}

void Scrab::M_Turn_3_4A91A0()
{
    if (field_20_animation.field_92_current_frame == 5 || field_20_animation.field_92_current_frame == 9 || field_20_animation.field_92_current_frame == 11)
    {
        Scrab_SFX_4AADB0(ScrabSounds::eWalk1_6, Math_RandomRange_496AB0(40, 50), 0x7FFF, 1);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        TryMoveOrStand_4A7570();
    }
}

const FP dword_546EFC[10] = {
    490908,
    453112,
    254902,
    155230,
    49871,
    42004,
    46393,
    50715,
    47541,
    47764};

void Scrab::M_RunToStand_4_4A90C0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    field_C4_velx = field_CC_sprite_scale * dword_546EFC[field_20_animation.field_92_current_frame]; // TODO: check size
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        KnockBack_4AA530();
    }
    else
    {
        MoveOnLine_4A7D20();

        if (field_106_current_motion == eScrabMotions::M_RunToStand_4_4A90C0)
        {
            KillTarget_4A7F20(pTarget);

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                MapFollowMe_408D10(TRUE);
                TryMoveOrStand_4A7570();
            }
        }
    }
}

const FP sScrabHopBeginVelX_546F24[4] = {
    FP_FromDouble(-1.0283050537109375),
    FP_FromDouble(-1.8401947021484375),
    FP_FromDouble(-0.2052764892578125),
    FP_FromDouble(2.77862548828125)};

void Scrab::M_HopBegin_5_4A96C0()
{
    Event_Broadcast_422BC0(kEventNoise, this);

    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sScrabHopBeginVelX_546F24[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sScrabHopBeginVelX_546F24[field_20_animation.field_92_current_frame];
    }

    field_C4_velx = field_CC_sprite_scale * frameVelX;

    FP gridSize = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), gridSize * FP_FromDouble(0.75)))
    {
        KnockBack_4AA530();
        field_106_current_motion = eScrabMotions::M_JumpToFall_8_4A9220;
    }
    else
    {
        field_B8_xpos += field_C4_velx;
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_46FBA0(SoundEffect::PickupItem_28, 50, -800);
            field_F8_LastLineYPos = field_BC_ypos;
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(-5.67);
            }
            else
            {
                field_C4_velx = field_CC_sprite_scale * FP_FromDouble(5.67);
            }
            field_C8_vely = field_CC_sprite_scale * FP_FromDouble(-9.8);
            field_BC_ypos += field_C8_vely;
            VOnTrapDoorOpen();
            field_106_current_motion = eScrabMotions::M_HopMidair_6_4A9490;
            field_100_pCollisionLine = nullptr;
        }
    }
}

const FP sScrabHopMidAirVelX_546F34[8] = {
    FP_FromDouble(5.00537109375),
    FP_FromDouble(6.8809356689453125),
    FP_FromDouble(15.796859741210938),
    FP_FromDouble(18.309295654296875),
    FP_FromDouble(15.494369506835938),
    FP_FromDouble(15.187164306640625),
    FP_FromDouble(6.1830902099609375),
    FP_FromDouble(5.00537109375)};

void Scrab::M_HopMidair_6_4A9490()
{
    Event_Broadcast_422BC0(kEventNoise, this);

    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sScrabHopMidAirVelX_546F34[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sScrabHopMidAirVelX_546F34[field_20_animation.field_92_current_frame];
    }

    field_C4_velx = field_CC_sprite_scale * frameVelX;

    FP kGridSize = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        kGridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), kGridSize * FP_FromDouble(0.75)))
    {
        KnockBack_4AA530();
    }
    else
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        const auto bCollision = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));

        if (sControlledCharacter_5C1B8C == this)
        {
            SetActiveCameraDelayedFromDir_408C40();
        }

        if (bCollision)
        {
            switch (pLine->field_8_type)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eUnknown_32:
                case eLineTypes::eUnknown_36:
                    field_100_pCollisionLine = pLine;
                    ToStand_4A75A0();
                    field_106_current_motion = eScrabMotions::M_HopLand_7_4A9890;
                    PlatformCollide_4A7E50();
                    field_B8_xpos = hitX;
                    field_BC_ypos = hitY;
                    return;

                case eLineTypes::eWallLeft_1:
                case eLineTypes::eWallRight_2:
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                    return;
            }
        }

        if (field_BC_ypos - field_F8_LastLineYPos > FP_FromInteger(5))
        {
            field_134_falling_velx_scale_factor = FP_FromDouble(1.25);
            field_106_current_motion = eScrabMotions::M_JumpToFall_8_4A9220;
        }
    }
}

const FP sScrabHopLandVelX_546F54[4] = {
    FP_FromDouble(2.77862548828125),
    FP_FromDouble(0.9429779052734375),
    FP_FromDouble(0.4394683837890625),
    FP_FromDouble(0.388397216796875)};

void Scrab::M_HopLand_7_4A9890()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, 400, this);
        Scrab_SFX_4AADB0(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    Event_Broadcast_422BC0(kEventNoise, this);

    FP frameVelX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        frameVelX = -sScrabHopLandVelX_546F54[field_20_animation.field_92_current_frame];
    }
    else
    {
        frameVelX = sScrabHopLandVelX_546F54[field_20_animation.field_92_current_frame];
    }
    field_C4_velx = (field_CC_sprite_scale * frameVelX);

    FP gridSize = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        gridSize = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), gridSize * FP_FromDouble(0.75)))
    {
        KnockBack_4AA530();
    }
    else
    {
        MoveOnLine_4A7D20();
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            ToStand_4A75A0();
        }
    }
}

void Scrab::M_JumpToFall_8_4A9220()
{
    if (field_C4_velx > FP_FromInteger(0))
    {
        field_C4_velx = field_C4_velx - (field_CC_sprite_scale * field_134_falling_velx_scale_factor);
        if (field_C4_velx < FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else if (field_C4_velx < FP_FromInteger(0))
    {
        field_C4_velx = (field_CC_sprite_scale * field_134_falling_velx_scale_factor) + field_C4_velx;
        if (field_C4_velx > FP_FromInteger(0))
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bHit = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
    if (sControlledCharacter_5C1B8C == this)
    {
        SetActiveCameraDelayedFromDir_408C40();
    }

    if (bHit)
    {
        switch (pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackgroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                field_100_pCollisionLine = pLine;
                field_106_current_motion = eScrabMotions::M_Fall_20_4A93E0;
                PlatformCollide_4A7E50();
                field_BC_ypos = hitY;
                field_B8_xpos = hitX;
                MapFollowMe_408D10(TRUE);
                break;
            case eLineTypes::eWallLeft_1:
            case eLineTypes::eWallRight_2:
                field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                break;
            default:
                return;
        }
    }
}

const FP sStandToWalkVels_546E48[3] = {
    FP_FromDouble(1.12),
    FP_FromDouble(2.37),
    FP_FromDouble(3.20)};

void Scrab::M_StandToWalk_9_4A8450()
{
    field_C4_velx = (field_CC_sprite_scale * sStandToWalkVels_546E48[field_20_animation.field_92_current_frame & 3]); // TODO: Check size of 3 is correct
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
    }

    MoveOnLine_4A7D20();

    if (field_106_current_motion == eScrabMotions::M_StandToWalk_9_4A8450)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eScrabMotions::M_Walk_1_4A84D0;
        }
    }
}

const FP sStandToRunVels_546EB8[3] = {
    FP_FromDouble(2.29),
    FP_FromDouble(2.69),
    FP_FromDouble(3.88)};

void Scrab::M_StandToRun_10_4A8900()
{
    field_C4_velx = field_CC_sprite_scale * sStandToRunVels_546EB8[field_20_animation.field_92_current_frame & 3]; // TODO: Check size of 3 is correct
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        KnockBack_4AA530();
    }
    else
    {
        MoveOnLine_4A7D20();

        if (field_106_current_motion == eScrabMotions::M_StandToRun_10_4A8900)
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_106_current_motion = eScrabMotions::M_Run_2_4A89C0;
            }
        }
    }
}

const FP sWalkToStandVels_546EAC[3] = {
    FP_FromDouble(1.04),
    FP_FromDouble(3.29),
    FP_FromDouble(2.86)};

void Scrab::M_WalkToStand_11_4A8880()
{
    field_C4_velx = (field_CC_sprite_scale * sWalkToStandVels_546EAC[field_20_animation.field_92_current_frame & 3]); // TODO: Check size of 3 is correct
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
    }

    MoveOnLine_4A7D20();

    if (field_106_current_motion == eScrabMotions::M_WalkToStand_11_4A8880)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            TryMoveOrStand_4A7570();
        }
    }
}

const FP dword_546F64[8] = {
    FP_FromDouble(11.25),
    FP_FromDouble(10.49),
    FP_FromDouble(9.67),
    FP_FromDouble(9.01),
    FP_FromDouble(7.47),
    FP_FromDouble(7.03),
    FP_FromDouble(6.69),
    FP_FromDouble(6.44)};

void Scrab::M_RunJumpBegin_12_4A99C0()
{
    if (field_20_animation.field_92_current_frame == 1)
    {
        SFX_Play_46FBA0(SoundEffect::PickupItem_28, 50, -800);
    }

    Event_Broadcast_422BC0(kEventNoise, this);

    FP velX = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        velX = -dword_546F64[field_20_animation.field_92_current_frame & 8]; // TODO: Check size
    }
    else
    {
        velX = dword_546F64[field_20_animation.field_92_current_frame & 8]; // TODO: Check size
    }

    field_C4_velx = field_CC_sprite_scale * velX;

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        KnockBack_4AA530();
    }
    else
    {
        FP hitX = {};
        FP hitY = {};
        PathLine* pLine = nullptr;
        const auto bHit = InAirCollision_408810(&pLine, &hitX, &hitY, FP_FromDouble(1.8));
        if (sControlledCharacter_5C1B8C == this)
        {
            SetActiveCameraDelayedFromDir_408C40();
        }

        if (bHit)
        {
            switch (pLine->field_8_type)
            {
                case eLineTypes::eFloor_0:
                case eLineTypes::eBackgroundFloor_4:
                case eLineTypes::eUnknown_32:
                case eLineTypes::eUnknown_36:
                    field_100_pCollisionLine = pLine;
                    ToStand_4A75A0();
                    field_106_current_motion = eScrabMotions::M_RunJumpEnd_13_4A9BE0;
                    PlatformCollide_4A7E50();
                    field_B8_xpos = hitX;
                    field_BC_ypos = hitY;
                    return;

                case eLineTypes::eWallLeft_1:
                case eLineTypes::eWallRight_2:
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                    return;
                default:
                    break;
            }
        }

        if (field_BC_ypos - field_F8_LastLineYPos > FP_FromInteger(5))
        {
            field_134_falling_velx_scale_factor = FP_FromDouble(1.25);
            field_106_current_motion = eScrabMotions::M_JumpToFall_8_4A9220;
        }
    }
}

const FP sEndRunJumpVels_546F84[21] = {
    FP_FromDouble(2.68),
    FP_FromDouble(2.65),
    FP_FromDouble(2.72),
    FP_FromDouble(2.89),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0),
    FP_FromInteger(0)};


void Scrab::M_RunJumpEnd_13_4A9BE0()
{
    Event_Broadcast_422BC0(kEventNoise, this);
    field_C4_velx = (field_CC_sprite_scale * sEndRunJumpVels_546F84[field_20_animation.field_92_current_frame & 21]); // TODO: Check size
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = -field_C4_velx;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        KnockBack_4AA530();
    }
    else
    {
        MoveOnLine_4A7D20();
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            Scrab_SFX_4AADB0(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
            if (!ToNextMotion_4A7920())
            {
                ToStand_4A75A0();
            }
        }
    }
}

void Scrab::M_WalkToFall_14_4A9460()
{
    M_JumpToFall_8_4A9220();
    if (field_106_current_motion == eScrabMotions::M_Stand_0_4A8220)
    {
        field_106_current_motion = eScrabMotions::M_HopLand_7_4A9890;
    }
}

void Scrab::M_RunToFall_15_4A9430()
{
    M_JumpToFall_8_4A9220();
    if (field_106_current_motion == eScrabMotions::M_Stand_0_4A8220)
    {
        field_106_current_motion = eScrabMotions::M_HopLand_7_4A9890;
    }
}

void Scrab::M_WalkToRun_16_4A8D60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eScrabMotions::M_Run_2_4A89C0;
        field_108_next_motion = -1;
    }
}

void Scrab::M_RunToWalk_17_4A8D90()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eScrabMotions::M_Walk_1_4A84D0;
        field_108_next_motion = -1;
    }
}

void Scrab::M_Knockback_18_4AA490()
{
    if (field_100_pCollisionLine)
    {
        MoveOnLine_4A7D20();
    }
    else
    {
        M_JumpToFall_8_4A9220();
    }

    if (field_20_animation.field_92_current_frame == 0)
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, 400, this);
    }

    else if (field_20_animation.field_92_current_frame == 2)
    {
        Scrab_SFX_4AADB0(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_100_pCollisionLine)
        {
            ToStand_4A75A0();
        }
        else
        {
            field_106_current_motion = eScrabMotions::M_JumpToFall_8_4A9220;
        }
    }
}

void Scrab::M_GetEaten_19_4AA3E0()
{
    if (field_20_animation.field_92_current_frame == 3)
    {
        SFX_Play_46FBA0(SoundEffect::PickupItem_28, 120, -1000);
    }
}

void Scrab::M_Fall_20_4A93E0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Environment_SFX_457A40(EnvironmentSfx::eHitGroundSoft_6, 80, 400, this);
        Scrab_SFX_4AADB0(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        ToStand_4A75A0();
        field_108_next_motion = -1;
    }
}

void Scrab::M_Stamp_21_4A9CC0()
{
    if (field_20_animation.field_92_current_frame == 9)
    {
        if (BrainIs(&Scrab::Brain_5_Possessed_4A6180))
        {
            KillTarget_4A7F20(nullptr);
        }

        Scrab_SFX_4AADB0(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
        SFX_Play_46FBA0(SoundEffect::KillEffect_64, 60, Math_RandomRange_496AB0(-255, 255));
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        Scrab_SFX_4AADB0(ScrabSounds::eHowl_0, 60, 511, 1);

        if (BrainIs(&Scrab::Brain_5_Possessed_4A6180))
        {
            ToStand_4A75A0();
        }
        else
        {
            ToNextMotion_4A7920();
        }
    }
}

void Scrab::M_GetPossessed_22_4AA420()
{
    if (field_20_animation.field_92_current_frame == 9)
    {
        Scrab_SFX_4AADB0(ScrabSounds::eHitCollision_4, 0, 0x7FFF, 1);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_108_next_motion)
        {
            field_1AA_flags.Set(Flags_1AA::eBit4_force_update_animation);
            field_106_current_motion = eScrabMotions::M_GetPossessed_22_4AA420;
        }
        else
        {
            ToStand_4A75A0();
        }
    }
}

void Scrab::M_Empty_23_4A9D80()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_DeathEnd_24_4AA140()
{
    if (!field_100_pCollisionLine)
    {
        field_C4_velx = FP_FromInteger(0);
        M_JumpToFall_8_4A9220();
        if (field_106_current_motion != eScrabMotions::M_DeathEnd_24_4AA140)
        {
            field_106_current_motion = eScrabMotions::M_DeathEnd_24_4AA140;
        }
    }
}

void Scrab::M_Empty_25_4A34D0()
{
    NOT_IMPLEMENTED();
}

void Scrab::M_HowlBegin_26_4A9DA0()
{
    if (field_20_animation.field_92_current_frame == 2)
    {
        if (sGnFrame_5C1B84 & 1)
        {
            Event_Broadcast_422BC0(kEventLoudNoise, this);
            Scrab_SFX_4AADB0(ScrabSounds::eYell_8, 0, Math_RandomRange_496AB0(-1600, -900), 1);
            if (BrainIs(&Scrab::Brain_5_Possessed_4A6180))
            {
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Scrab_Howl_53);
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_178_shred_power_active = 1;

        if (field_108_next_motion == -1)
        {
            ToStand_4A75A0();
        }
        else
        {
            field_106_current_motion = eScrabMotions::M_HowlEnd_27_4A9E60;
        }
    }
}

void Scrab::M_HowlEnd_27_4A9E60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (BrainIs(&Scrab::Brain_5_Possessed_4A6180))
        {
            if (!ToNextMotion_4A7920())
            {
                ToStand_4A75A0();
            }
        }
        else
        {
            ToNextMotion_4A7920();
        }
    }
}

void Scrab::M_GetDepossessedBegin_28_4AA200()
{
    field_178_shred_power_active = 0;

    if (sControlledCharacter_5C1B8C == this)
    {
        if (field_20_animation.field_92_current_frame == 2)
        {
            Scrab_SFX_4AADB0(ScrabSounds::eYell_8, 0, Math_RandomRange_496AB0(-1600, -900), 1);
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!Input_IsChanting_45F260())
            {
                field_106_current_motion = eScrabMotions::M_GetDepossessedEnd_29_4AA3C0;
            }
        }

        if (!(static_cast<s32>(sGnFrame_5C1B84) % 4))
        {
            const FP xRnd = FP_FromInteger(Math_RandomRange_496AB0(-20, 20));
            const FP yRnd = FP_FromInteger(Math_RandomRange_496AB0(20, 50));
            const FP ypos = field_BC_ypos - (field_CC_sprite_scale * yRnd);
            const FP xpos = (field_CC_sprite_scale * xRnd) + field_B8_xpos;
            New_TintChant_Particle_426BE0(xpos, ypos, field_CC_sprite_scale, Layer::eLayer_0);
        }

        if (static_cast<s32>(sGnFrame_5C1B84) > field_130_depossession_timer || sActiveHero_5C1B68->field_10C_health <= FP_FromInteger(0))
        {
            sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
            field_114_flags.Clear(Flags_114::e114_Bit4_bPossesed);
            field_1A2_speak_counter = 0;
            MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
            field_106_current_motion = eScrabMotions::M_GetDepossessedEnd_29_4AA3C0;
            ToPatrol_4AA600();
            field_11C_brain_sub_state = Brain_0_Patrol::eBrain0_ToMoving_0;
            gMap_5C3030.SetActiveCam_480D30(field_166_level, field_168_path, field_16A_camera, CameraSwapEffects::eInstantChange_0, 0, 0);
        }
    }
}

void Scrab::M_GetDepossessedEnd_29_4AA3C0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4A75A0();
    }
}

void Scrab::M_Shriek_30_4A9EA0()
{
    if (field_20_animation.field_92_current_frame == 4)
    {
        if (sGnFrame_5C1B84 & 1)
        {
            Event_Broadcast_422BC0(kEventLoudNoise, this);
            Scrab_SFX_4AADB0(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
            if (BrainIs(&Scrab::Brain_5_Possessed_4A6180))
            {
                pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Scrab_Shriek_54);
            }
        }
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (!ToNextMotion_4A7920())
        {
            ToStand_4A75A0();
        }
    }
}

void Scrab::M_ScrabBattleAnim_31_4A9F30()
{
    if (field_20_animation.field_92_current_frame == 0)
    {
        field_160_sfx_bitmask = Scrab_SFX_4AADB0(ScrabSounds::eShredding_5, 100, Math_RandomRange_496AB0(-600, 200), 1);
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToNextMotion_4A7920();
    }
}

void Scrab::M_AttackSpin_32_4A8DC0()
{
    auto pObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));

    field_178_shred_power_active = 0;
    if (Expired(field_12C_timer))
    {
        field_106_current_motion = eScrabMotions::M_Run_2_4A89C0;
        return;
    }

    if (field_20_animation.field_92_current_frame == 0)
    {
        field_160_sfx_bitmask = Scrab_SFX_4AADB0(ScrabSounds::eShredding_5, 100, Math_RandomRange_496AB0(-600, 200), 1);
    }

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (sControlledCharacter_5C1B8C != this)
    {
        if (pObj)
        {
            if (pObj->field_B8_xpos <= field_B8_xpos)
            {
                field_C4_velx = -(kGridSize / FP_FromDouble(3.5));
            }
            else
            {
                field_C4_velx = (kGridSize / FP_FromDouble(3.5));
            }
        }
        else
        {
            field_C4_velx = FP_FromInteger(0);
        }
    }
    else
    {
        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eThrowItem | InputCommands::Enum::eDoAction))
        {
            if (!sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4 | sInputKey_Right_5550D0))
            {
                field_C4_velx = FP_FromInteger(0);
            }

            if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
                {
                    field_C4_velx = -(kGridSize / FP_FromDouble(3.5));
                }

                if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                {
                    field_C4_velx = (kGridSize / FP_FromDouble(3.5));
                }
            }
            else
            {
                if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
                {
                    field_C4_velx = -(kGridSize / FP_FromInteger(7));
                }

                if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
                {
                    field_C4_velx = (kGridSize / FP_FromInteger(7));
                }
            }
        }
        else
        {
            field_106_current_motion = eScrabMotions::M_Run_2_4A89C0;
        }
    }

    MapFollowMe_408D10(0);

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), field_C4_velx))
    {
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    }
    else
    {
        MoveOnLine_4A7D20();
        if (field_106_current_motion == eScrabMotions::M_AttackSpin_32_4A8DC0)
        {
            KillTarget_4A7F20(pObj);
        }
    }
}

void Scrab::M_FeedToGulp_33_4A9FA0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_108_next_motion == eScrabMotions::M_StandToFeed_35_4AA010)
        {
            field_106_current_motion = eScrabMotions::M_StandToFeed_35_4AA010;
        }
        else
        {
            field_106_current_motion = eScrabMotions::M_GulpToStand_34_4A9FF0;
        }
        field_108_next_motion = -1;
    }
}

void Scrab::M_GulpToStand_34_4A9FF0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4A75A0();
    }
}

void Scrab::M_StandToFeed_35_4AA010()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eScrabMotions::M_Feed_36_4AA030;
    }
}

void Scrab::M_Feed_36_4AA030()
{
    if (field_20_animation.field_92_current_frame)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_106_current_motion = eScrabMotions::M_FeedToGulp_33_4A9FA0;
        }
    }
    else
    {
        SFX_Play_46FA90((Math_NextRandom() & 1) ? SoundEffect::Eating2_66 : SoundEffect::Eating1_65, 0);
    }
}

void Scrab::M_AttackLunge_37_4AA0B0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_120_obj_id));
    KillTarget_4A7F20(pTarget);

    if (field_20_animation.field_92_current_frame == 4)
    {
        Scrab_SFX_4AADB0(ScrabSounds::eHowl_0, 0, 0x7FFF, 1);
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        ToStand_4A75A0();
    }
}

void Scrab::M_LegKick_38_4AA120()
{
    M_AttackLunge_37_4AA0B0();
}

void Scrab::M_DeathBegin_39_4AA190()
{
    if (!field_100_pCollisionLine)
    {
        field_C4_velx = FP_FromInteger(0);
        M_JumpToFall_8_4A9220();
        if (field_106_current_motion != eScrabMotions::M_DeathBegin_39_4AA190)
        {
            field_106_current_motion = eScrabMotions::M_DeathBegin_39_4AA190;
        }
    }
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_106_current_motion = eScrabMotions::M_DeathEnd_24_4AA140;
    }
}

s16 Scrab::vOnSameYLevel_4A5400(BaseAnimatedWithPhysicsGameObject* pOther)
{
    PSX_RECT ourRect = {};
    vGetBoundingRect_424FD0(&ourRect, 1);

    PSX_RECT otherRect = {};
    pOther->vGetBoundingRect_424FD0(&otherRect, 1);

    const FP k10Scaled = (FP_FromInteger(10) * field_CC_sprite_scale);
    if (FP_FromInteger(ourRect.y) <= (FP_FromInteger(otherRect.h) - k10Scaled) && ourRect.y >= otherRect.y)
    {
        return TRUE;
    }

    if (ourRect.h <= otherRect.h)
    {
        if (FP_FromInteger(ourRect.h) >= (k10Scaled + FP_FromInteger(otherRect.y)))
        {
            return TRUE;
        }
    }

    if (ourRect.y >= otherRect.y)
    {
        if (ourRect.h <= otherRect.h)
        {
            return TRUE;
        }

        if (ourRect.y > otherRect.y)
        {
            return FALSE;
        }
    }

    if (ourRect.h < otherRect.h)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void Scrab::ToPatrol_4AA600()
{
    SetBrain(&Scrab::Brain_0_Patrol_4AA630);
    field_138_unused = field_B8_xpos;
}

void Scrab::ToStand_4A75A0()
{
    field_134_falling_velx_scale_factor = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_106_current_motion = eScrabMotions::M_Stand_0_4A8220;
    MapFollowMe_408D10(TRUE);
}

void Scrab::MoveOnLine_4A7D20()
{
    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_110_id);
    const FP oldXPos = field_B8_xpos;
    if (field_100_pCollisionLine)
    {
        field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
        if (field_100_pCollisionLine)
        {
            if (pObj)
            {
                if (field_100_pCollisionLine->field_8_type != eLineTypes::eUnknown_32 &&
                    field_100_pCollisionLine->field_8_type != eLineTypes::eUnknown_36)
                {
                    const auto oldMotion = field_106_current_motion;
                    VOnTrapDoorOpen();
                    field_106_current_motion = oldMotion;
                }
            }
            else if (field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_32 ||
                     field_100_pCollisionLine->field_8_type == eLineTypes::eUnknown_36)
            {
                PlatformCollide_4A7E50();
            }
        }
        else
        {
            VOnTrapDoorOpen();
            field_F8_LastLineYPos = field_BC_ypos;
            field_134_falling_velx_scale_factor = FP_FromInteger(1);
            field_B8_xpos = oldXPos + field_C4_velx;
            if (field_106_current_motion == eScrabMotions::M_Walk_1_4A84D0)
            {
                field_106_current_motion = eScrabMotions::M_WalkToFall_14_4A9460;
            }
            else
            {
                field_106_current_motion = eScrabMotions::M_RunToFall_15_4A9430;
            }
        }
    }
    else
    {
        field_106_current_motion = eScrabMotions::M_JumpToFall_8_4A9220;
        field_F8_LastLineYPos = field_BC_ypos;
    }
}

void Scrab::PlatformCollide_4A7E50()
{
    PSX_RECT bRect = {};
    vGetBoundingRect_424FD0(&bRect, 1);

    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
    const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
    vOnCollisionWith_424EE0(xy, wh, ObjList_5C1B78, 1, (TCollisionCallBack) &BaseAliveGameObject::OnTrapDoorIntersection_408BA0);
}

BaseAliveGameObject* Scrab::Find_Fleech_4A4C90()
{
    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        auto pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eFleech_50)
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (pAliveObj->field_10C_health > FP_FromInteger(0))
            {
                if (pAliveObj->vOnSameYLevel_425520(pAliveObj))
                {
                    if (pAliveObj->vIsObjNearby_4253B0(ScaleToGridSize_4498B0(field_CC_sprite_scale) * FP_FromInteger(3), pAliveObj))
                    {
                        if (pAliveObj->vIsFacingMe_4254A0(pAliveObj))
                        {
                            if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), pAliveObj->field_B8_xpos - field_B8_xpos) && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pAliveObj->field_C2_lvl_number, pAliveObj->field_C0_path_number, pAliveObj->field_B8_xpos, pAliveObj->field_BC_ypos, 0) && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                            {
                                return pAliveObj;
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

void Scrab::vPossesed_4A5620()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_164_prevent_depossession = 1;
    field_178_shred_power_active = 0;
    field_106_current_motion = eScrabMotions::M_GetPossessed_22_4AA420;
    field_108_next_motion = -1;
    vUpdateAnim_4A34F0();
    SetBrain(&Scrab::Brain_5_Possessed_4A6180);
    field_11C_brain_sub_state = 0;
    field_120_obj_id = -1;
    field_124_fight_target_obj_id = -1;
    field_12C_timer = sGnFrame_5C1B84 + 35;
    field_166_level = gMap_5C3030.field_0_current_level;
    field_168_path = gMap_5C3030.field_2_current_path;
    field_16A_camera = gMap_5C3030.field_4_current_camera;
}

u8** Scrab::ResBlockForMotion_4A43E0(s16 motion)
{
    if (motion < eScrabMotions::M_Stamp_21_4A9CC0)
    {
        field_140_motion_resource_block_index = 0;
    }
    else if (motion < eScrabMotions::M_Empty_23_4A9D80)
    {
        field_140_motion_resource_block_index = 6;
    }
    else if (motion < eScrabMotions::M_Empty_25_4A34D0)
    {
        field_140_motion_resource_block_index = 8;
    }
    else if (motion < eScrabMotions::M_HowlBegin_26_4A9DA0)
    {
        field_140_motion_resource_block_index = 10;
    }
    else if (motion < eScrabMotions::M_Shriek_30_4A9EA0)
    {
        field_140_motion_resource_block_index = 5;
    }
    else if (motion < eScrabMotions::M_ScrabBattleAnim_31_4A9F30)
    {
        field_140_motion_resource_block_index = 9;
    }
    else if (motion < eScrabMotions::M_FeedToGulp_33_4A9FA0)
    {
        field_140_motion_resource_block_index = 4;
    }
    else if (motion < eScrabMotions::M_StandToFeed_35_4AA010)
    {
        field_140_motion_resource_block_index = 11;
    }
    else if (motion < eScrabMotions::M_AttackLunge_37_4AA0B0)
    {
        field_140_motion_resource_block_index = 1;
    }
    else if (motion < eScrabMotions::M_LegKick_38_4AA120)
    {
        field_140_motion_resource_block_index = 2;
    }
    else if (motion >= eScrabMotions::M_DeathBegin_39_4AA190)
    {
        field_140_motion_resource_block_index = motion >= 40 ? 0 : 13; // 40 == max motions ??
    }
    else
    {
        field_140_motion_resource_block_index = 3;
    }
    return field_10_resources_array.ItemAt(field_140_motion_resource_block_index);
}

void Scrab::vScreenChanged_4A5560()
{
    BaseGameObject* pChaseTarget = sObjectIds_5C1B70.Find_449CF0(field_120_obj_id);

    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else if (pChaseTarget)
    {
        if (pChaseTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_120_obj_id = -1;
            field_108_next_motion = eScrabMotions::M_Stand_0_4A8220;
            ToPatrol_4AA600();
            field_11C_brain_sub_state = Brain_0_Patrol::eBrain0_ToMoving_0;
        }
    }
}

void Scrab::vRender_4A45B0(PrimHeader** ppOt)
{
    if (UpdateDelay() == 0)
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Scrab::TryMoveOrStand_4A7570()
{
    if (!ToNextMotion_4A7920())
    {
        ToStand_4A75A0();
    }
}

s16 Scrab::ToNextMotion_4A7920()
{
    MapFollowMe_408D10(TRUE);

    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0) && field_11C_brain_sub_state != 0)
    {
        return PlayerControlled_4A76A0();
    }

    if (field_108_next_motion == eScrabMotions::M_Turn_3_4A91A0 || field_108_next_motion == eScrabMotions::M_Stamp_21_4A9CC0 || field_108_next_motion == eScrabMotions::M_Empty_25_4A34D0 || field_108_next_motion == eScrabMotions::M_HowlBegin_26_4A9DA0 || field_108_next_motion == eScrabMotions::M_Shriek_30_4A9EA0 || field_108_next_motion == eScrabMotions::M_ScrabBattleAnim_31_4A9F30 || field_108_next_motion == eScrabMotions::M_AttackLunge_37_4AA0B0 || field_108_next_motion == eScrabMotions::M_LegKick_38_4AA120 || field_108_next_motion == eScrabMotions::M_StandToFeed_35_4AA010)
    {
        field_106_current_motion = field_108_next_motion;
        field_108_next_motion = -1;
        return 1;
    }

    const FP k45Scaled = (field_CC_sprite_scale * FP_FromInteger(45));
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    // Check if going to run into a wall.
    if (field_108_next_motion == eScrabMotions::M_Run_2_4A89C0)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (WallHit_408750(k45Scaled, -kGridSize))
            {
                return 0;
            }
            else
            {
                field_C4_velx = -(kGridSize / FP_FromDouble(3.5));
                field_106_current_motion = eScrabMotions::M_StandToRun_10_4A8900;
                field_108_next_motion = -1;
                return 1;
            }
        }
        else
        {
            if (WallHit_408750(k45Scaled, kGridSize))
            {
                return 0;
            }
            else
            {
                field_C4_velx = (kGridSize / FP_FromDouble(3.5));
                field_106_current_motion = eScrabMotions::M_StandToRun_10_4A8900;
                field_108_next_motion = -1;
                return 1;
            }
        }
    }

    if (field_108_next_motion != eScrabMotions::M_Walk_1_4A84D0)
    {
        if (field_108_next_motion == eScrabMotions::M_Stand_0_4A8220)
        {
            ToStand_4A75A0();
            return 1;
        }

        if (field_108_next_motion != eScrabMotions::M_HopMidair_6_4A9490)
        {
            return 0;
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (WallHit_408750(k45Scaled, -kGridSize))
            {
                return 0;
            }
            else
            {
                field_106_current_motion = eScrabMotions::M_HopBegin_5_4A96C0;
                field_108_next_motion = -1;
                return 1;
            }
        }
        else
        {
            if (WallHit_408750(k45Scaled, kGridSize))
            {
                return 0;
            }
            else
            {
                field_106_current_motion = eScrabMotions::M_HopBegin_5_4A96C0;
                field_108_next_motion = -1;
                return 1;
            }
        }
    }
    else
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            if (WallHit_408750(k45Scaled, -kGridSize))
            {
                return 0;
            }
            else
            {
                field_106_current_motion = eScrabMotions::M_StandToWalk_9_4A8450;
                field_C4_velx = -(kGridSize / FP_FromInteger(7));
                field_108_next_motion = -1;
                return 1;
            }
        }
        else
        {
            if (WallHit_408750(k45Scaled, kGridSize))
            {
                return 0;
            }
            else
            {
                field_106_current_motion = eScrabMotions::M_StandToWalk_9_4A8450;
                field_C4_velx = (kGridSize / FP_FromInteger(7));
                field_108_next_motion = -1;
                return 1;
            }
        }
    }
}

s16 Scrab::PlayerControlled_4A76A0()
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    const FP k45Scaled = (field_CC_sprite_scale * FP_FromInteger(45));

    if (sInputObject_5BD4E0.isPressed(sInputKey_Right_5550D0))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_106_current_motion = eScrabMotions::M_Turn_3_4A91A0;
            return 1;
        }
        else
        {
            if (WallHit_408750(k45Scaled, kGridSize))
            {
                return 0;
            }

            if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
            {
                field_C4_velx = (kGridSize / FP_FromDouble(3.5));
                field_106_current_motion = eScrabMotions::M_StandToRun_10_4A8900;
                return 1;
            }
            else
            {
                field_C4_velx = (kGridSize / FP_FromInteger(7));
                field_106_current_motion = eScrabMotions::M_StandToWalk_9_4A8450;
                return 1;
            }
        }
    }
    else if (sInputObject_5BD4E0.isPressed(sInputKey_Left_5550D4))
    {
        if (!field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            field_106_current_motion = eScrabMotions::M_Turn_3_4A91A0;
            return 1;
        }
        else
        {
            if (WallHit_408750(k45Scaled, -kGridSize))
            {
                return 0;
            }

            if (sInputObject_5BD4E0.isPressed(sInputKey_Run_5550E8))
            {
                field_C4_velx = -(kGridSize / FP_FromDouble(3.5));
                field_106_current_motion = eScrabMotions::M_StandToRun_10_4A8900;
                return 1;
            }
            else
            {
                field_C4_velx = -(kGridSize / FP_FromInteger(7));
                field_106_current_motion = eScrabMotions::M_StandToWalk_9_4A8450;
                return 1;
            }
        }
    }
    else
    {
        if (sInputObject_5BD4E0.isPressed(sInputKey_Up_5550D8))
        {
            field_106_current_motion = eScrabMotions::M_StandToFeed_35_4AA010;
            return 1;
        }

        if (sInputObject_5BD4E0.isPressed(sInputKey_Down_5550DC))
        {
            field_106_current_motion = eScrabMotions::M_Stamp_21_4A9CC0;
            return 1;
        }

        return 0;
    }
}

void Scrab::ToJump_4A75E0()
{
    field_F8_LastLineYPos = field_BC_ypos;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_C4_velx = field_CC_sprite_scale * -FP_FromDouble(-5.67);
    }
    else
    {
        field_C4_velx = field_CC_sprite_scale * -FP_FromDouble(5.67);
    }

    field_C8_vely = field_CC_sprite_scale * FP_FromDouble(-9.6);
    field_BC_ypos += field_C8_vely;

    VOnTrapDoorOpen();

    field_106_current_motion = eScrabMotions::M_RunJumpBegin_12_4A99C0;
    field_100_pCollisionLine = nullptr;
}

s16 Scrab::vTakeDamage_4A45E0(BaseGameObject* pFrom)
{
    if (field_10C_health <= FP_FromInteger(0))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case AETypes::eFleech_50:
            field_10C_health = field_10C_health - FP_FromDouble(0.13);
            if (field_10C_health < FP_FromInteger(0))
            {
                field_10C_health = FP_FromInteger(0);
            }

            if (field_10C_health > FP_FromInteger(0))
            {
                return 1;
            }

            if (static_cast<BaseAliveGameObject*>(pFrom)->field_B8_xpos >= field_B8_xpos)
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
            }
            else
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            }

            Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
            SetBrain(&Scrab::Brain_4_ShrinkDeath_4A6420);
            field_106_current_motion = eScrabMotions::M_GetEaten_19_4AA3E0;
            field_12C_timer = sGnFrame_5C1B84 + 90;
            vUpdateAnim_4A34F0();
            if (sControlledCharacter_5C1B8C == this)
            {
                SND_SEQ_Play_4CAB10(SeqId::DeathDrums_29, 1, 127, 127);
            }
            return 0;

        case AETypes::eAbilityRing_104:
            return 0;

        case AETypes::eBullet_15:
        case AETypes::eNeverSet_107:
        case AETypes::eScrab_112:
            break;

        default:
            SFX_Play_46FA90(SoundEffect::KillEffect_64, 127);
            SFX_Play_46FA90(SoundEffect::FallingItemHit_47, 90);
            break;
    }

    Event_Broadcast_422BC0(kEventMudokonComfort | kEventSpeaking, this);
    field_10C_health = FP_FromInteger(0);
    SetBrain(&Scrab::Brain_3_Death_4A62B0);
    field_12C_timer = sGnFrame_5C1B84 + 90;
    field_106_current_motion = eScrabMotions::M_DeathBegin_39_4AA190;
    vUpdateAnim_4A34F0();

    if (sControlledCharacter_5C1B8C == this)
    {
        SND_SEQ_Play_4CAB10(SeqId::DeathDrums_29, 1, 127, 127);
    }

    return 1;
}

void Scrab::KnockBack_4AA530()
{
    field_134_falling_velx_scale_factor = FP_FromDouble(0.35);

    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    FP v4 = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        v4 = (kGridSize / FP_FromDouble(3.5));
    }
    else
    {
        v4 = -(kGridSize / FP_FromDouble(3.5));
    }
    field_C4_velx = (v4 / FP_FromInteger(2));

    MapFollowMe_408D10(TRUE);

    if (field_C8_vely < FP_FromInteger(0))
    {
        field_C8_vely = FP_FromInteger(0);
    }

    field_106_current_motion = eScrabMotions::M_Knockback_18_4AA490;
    field_130_depossession_timer = sGnFrame_5C1B84 + 10;
}

const SfxDefinition getSfxDef(ScrabSounds effectId)
{
    return scrab_sScrabSfx_560330[static_cast<s32>(effectId)];
}

s32 Scrab::Scrab_SFX_4AADB0(ScrabSounds soundId, s32 vol, s32 pitch, s16 applyDirection)
{
    s16 volumeLeft = 0;
    s16 volumeRight = 0;
    const CameraPos direction = gMap_5C3030.GetDirection_4811A0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos);

    const SfxDefinition effectDef = getSfxDef(soundId);
    const s16 defaultSndIdxVol = effectDef.field_3_default_volume;

    volumeRight = static_cast<s16>(vol);
    if (vol <= 0)
    {
        volumeRight = defaultSndIdxVol;
    }

    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        volumeRight /= 2;
    }

    if (applyDirection)
    {
        PSX_RECT pRect = {};
        gMap_5C3030.Get_Camera_World_Rect_481410(direction, &pRect);
        switch (direction)
        {
            case CameraPos::eCamCurrent_0:
                volumeLeft = volumeRight;
                break;
            case CameraPos::eCamTop_1:
            case CameraPos::eCamBottom_2:
            {
                volumeLeft = FP_GetExponent(FP_FromInteger(defaultSndIdxVol * 2) / FP_FromInteger(3));
                volumeRight = volumeLeft;
            }
            break;
            case CameraPos::eCamLeft_3:
            {
                const FP percentHowFar = (FP_FromInteger(pRect.w) - field_B8_xpos) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
                volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
            }
            break;
            case CameraPos::eCamRight_4:
            {
                const FP percentHowFar = (field_B8_xpos - FP_FromInteger(pRect.x)) / FP_FromInteger(368);
                volumeLeft = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight));
                volumeRight = volumeRight - FP_GetExponent(percentHowFar * FP_FromInteger(volumeRight - (volumeRight / 3)));
            }
            break;
            default:
                return 0;
        }
    }
    else
    {
        volumeLeft = volumeRight;
    }

    return SFX_SfxDefinition_Play_4CA700(
        &effectDef,
        volumeLeft,
        volumeRight,
        static_cast<s16>(pitch),
        static_cast<s16>(pitch));
}

void Scrab::KillTarget_4A7F20(BaseAliveGameObject* pTarget)
{
    bool bKilledTarget = false;
    bool bKillSpecific = false;

    if (Is_In_Current_Camera_424A70() == CameraPos::eCamCurrent_0)
    {
        if (!BrainIs(&Scrab::Brain_2_Fighting_4A5840))
        {
            if (!BrainIs(&Scrab::Brain_5_Possessed_4A6180) || field_106_current_motion == eScrabMotions::M_LegKick_38_4AA120 || field_106_current_motion == eScrabMotions::M_Stamp_21_4A9CC0 || field_106_current_motion == eScrabMotions::M_AttackSpin_32_4A8DC0)
            {
                PSX_RECT bOurRect = {};
                vGetBoundingRect_424FD0(&bOurRect, 1);
                BaseAliveGameObject* pObj = pTarget;

                s32 list_idx = 0;
                if (pTarget)
                {
                    bKillSpecific = true;
                }
                else if (gBaseAliveGameObjects_5C1B7C->field_4_used_size > 0)
                {
                    list_idx = 1;
                    pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(0);
                }
                else
                {
                    pObj = nullptr;
                }

                do
                {
                    if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
                    {
                        if (pObj != this)
                        {
                            if ((pObj->Type() == AETypes::eAbe_69 || 
                                pObj->Type() == AETypes::eMudokon2_81 ||
                                pObj->Type() == AETypes::eMudokon_110 ||
                                pObj->Type() == AETypes::eNevetSet_127 ||
                                pObj->Type() == AETypes::eFleech_50 ||
                                pObj->Type() == AETypes::eScrab_112) &&
                                field_D6_scale == pObj->field_D6_scale && pObj->field_10C_health > FP_FromInteger(0))
                            {
                                const FP xDist = pObj->field_B8_xpos - field_B8_xpos;
                                if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), xDist))
                                {
                                    if (!pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
                                    {
                                        if (pObj->Type() != AETypes::eScrab_112 ||
                                            !pObj->field_114_flags.Get(Flags_114::e114_Bit4_bPossesed) ||
                                            (pObj->field_106_current_motion != eScrabMotions::M_AttackSpin_32_4A8DC0 &&
                                            (pObj->Type() != AETypes::eFleech_50 || BrainIs(&Scrab::Brain_5_Possessed_4A6180) || field_1A8_bKill_enemy == Choice_short::eYes_1)))

                                        {
                                            PSX_RECT objRect = {};
                                            pObj->vGetBoundingRect_424FD0(&objRect, 1);

                                            if (PSX_Rects_overlap_no_adjustment(&objRect, &bOurRect))
                                            {
                                                if (pObj->VTakeDamage_408730(this))
                                                {
                                                    bKilledTarget = true;
                                                    SFX_Play_46FA90(SoundEffect::KillEffect_64, 0);
                                                    if (pObj->Type() == AETypes::eAbe_69)
                                                    {
                                                        Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 0, 0, sActiveHero_5C1B68);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (bKillSpecific)
                    {
                        break;
                    }

                    if (bKilledTarget)
                    {
                        break;
                    }

                    if (list_idx >= gBaseAliveGameObjects_5C1B7C->Size())
                    {
                        break;
                    }

                    pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(list_idx++);
                }
                while (pObj);
            }
        }
    }
}

s16 Scrab::FindAbeOrMud_4A4FD0()
{
    if (CanSeeAbe_4A51A0(sActiveHero_5C1B68) && sActiveHero_5C1B68->field_10C_health > FP_FromInteger(0) && sActiveHero_5C1B68->field_CC_sprite_scale == field_CC_sprite_scale && !sActiveHero_5C1B68->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), sActiveHero_5C1B68->field_B8_xpos - field_B8_xpos))
        {
            field_120_obj_id = sActiveHero_5C1B68->field_8_object_id;
            return TRUE;
        }
    }

    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if ((pAliveObj->Type() == AETypes::eMudokon2_81 || pAliveObj->Type() == AETypes::eMudokon_110 || pAliveObj->Type() == AETypes::eNevetSet_127 || pAliveObj->Type() == AETypes::eScrab_112) && (pAliveObj->Type() != AETypes::eScrab_112 || pAliveObj->field_114_flags.Get(Flags_114::e114_Bit4_bPossesed)) && CanSeeAbe_4A51A0(pAliveObj) && pAliveObj->field_10C_health > FP_FromInteger(0) && pAliveObj->field_CC_sprite_scale == field_CC_sprite_scale)
            {
                if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), pAliveObj->field_B8_xpos - field_B8_xpos))
                {
                    field_120_obj_id = pAliveObj->field_8_object_id;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

s16 Scrab::CanSeeAbe_4A51A0(BaseAliveGameObject* pObj)
{
    if (pObj->field_CC_sprite_scale != field_CC_sprite_scale)
    {
        return 0;
    }

    if (pObj == sActiveHero_5C1B68)
    {
        if (sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_67_LedgeHang_454E20 || sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_69_LedgeHangWobble_454EF0)
        {
            return vOnSameYLevel_425520(pObj);
        }
    }

    if (pObj->field_BC_ypos <= (field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(45))) || !LineOfSightTo_4A52D0(this, pObj))
    {
        return 0;
    }

    if (vOnSameYLevel_425520(pObj))
    {
        FP offX = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            offX = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }
        else
        {
            offX = ScaleToGridSize_4498B0(field_CC_sprite_scale);
        }

        if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), offX))
        {
            return 0;
        }
    }

    return 1;
}


Bool32 Scrab::LineOfSightTo_4A52D0(Scrab* pThis, BaseAliveGameObject* pObj)
{
    PSX_RECT objRect = {};
    pObj->vGetBoundingRect_424FD0(&objRect, 1);

    PSX_RECT bRect = {};
    pThis->vGetBoundingRect_424FD0(&bRect, 1);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_5C1128->Raycast_417A60(
               pThis->field_B8_xpos,
               FP_FromInteger((bRect.y + bRect.h) / 2),
               pObj->field_B8_xpos,
               FP_FromInteger((objRect.y + objRect.h) / 2),
               &pLine,
               &hitX,
               &hitY,
               (pThis->field_D6_scale != 0 ? 1 : 16) | (pThis->field_D6_scale != 0 ? 6 : 0x60) | (pThis->field_D6_scale != 0 ? 8 : 0x80))
        != 1;
}


Scrab* Scrab::FindScrabToFight_4A4E20()
{
    Scrab* pScrabIAmFightingAlready = nullptr;
    Scrab* pScrabNotInAFight = nullptr;
    Scrab* pScrabInFightWithSomeoneElse = nullptr;

    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eScrab_112)
        {
            auto pScrab = static_cast<Scrab*>(pObj);

            if (pScrab != this && !pScrab->field_114_flags.Get(Flags_114::e114_Bit4_bPossesed) && !BrainIs(&Scrab::Brain_3_Death_4A62B0))
            {
                if (vOnSameYLevel_425520(pScrab))
                {
                    if (!WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), pScrab->field_B8_xpos - field_B8_xpos) && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(pScrab->field_C2_lvl_number, pScrab->field_C0_path_number, pScrab->field_B8_xpos, pScrab->field_BC_ypos, 0) && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
                    {
                        if (pScrab->field_124_fight_target_obj_id == -1)
                        {
                            pScrabNotInAFight = pScrab;
                        }
                        else
                        {
                            if (pScrab->field_124_fight_target_obj_id == field_8_object_id)
                            {
                                pScrabIAmFightingAlready = pScrab;
                            }
                            else
                            {
                                pScrabInFightWithSomeoneElse = pScrab;
                            }
                        }
                    }
                }
            }
        }
    }

    if (pScrabIAmFightingAlready)
    {
        return pScrabIAmFightingAlready;
    }

    if (pScrabNotInAFight)
    {
        return pScrabNotInAFight;
    }

    // This can be nullptr
    return pScrabInFightWithSomeoneElse;
}

s16 Scrab::Handle_SlamDoor_or_EnemyStopper_4A4830(FP velX, s16 bCheckLeftRightBounds)
{
    if (velX < FP_FromInteger(0))
    {
        velX = -velX;
    }

    FP gridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    if (velX > FP_FromInteger(5))
    {
        gridSize = (gridSize * FP_FromInteger(2));
    }

    TlvTypes objectType = {};
    Path_EnemyStopper::StopDirection stopDirection = Path_EnemyStopper::StopDirection::Both_2;
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        objectType = TlvTypes::ScrabLeftBound_43;
        stopDirection = Path_EnemyStopper::StopDirection::Left_0;
        gridSize = -gridSize;
    }
    else
    {
        objectType = TlvTypes::ScrabRightBound_44;
        stopDirection = Path_EnemyStopper::StopDirection::Right_1;
    }

    if (WallHit_408750(field_CC_sprite_scale * FP_FromInteger(45), gridSize * FP_FromInteger(1)))
    {
        return 1;
    }

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(FP_Abs(field_BC_ypos)),
        FP_GetExponent(field_B8_xpos + gridSize),
        FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
        TlvTypes::SlamDoor_85);

    auto pSlamDoorTlv = static_cast<Path_SlamDoor*>(field_FC_pPathTLV);
    if (pSlamDoorTlv && ((pSlamDoorTlv->field_10_bStart_closed == Choice_short::eYes_1 && !SwitchStates_Get_466020(pSlamDoorTlv->field_14_switch_id)) || (pSlamDoorTlv->field_10_bStart_closed == Choice_short::eNo_0 && SwitchStates_Get_466020(pSlamDoorTlv->field_14_switch_id))))
    {
        return 1;
    }

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
        FP_GetExponent(field_B8_xpos),
        FP_GetExponent(field_BC_ypos),
        FP_GetExponent(field_B8_xpos + gridSize),
        FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
        TlvTypes::EnemyStopper_47);

    auto pPathEnemyStopper = static_cast<Path_EnemyStopper*>(field_FC_pPathTLV);
    if (pPathEnemyStopper && (pPathEnemyStopper->field_10_stop_direction == stopDirection || pPathEnemyStopper->field_10_stop_direction == Path_EnemyStopper::StopDirection::Both_2) && SwitchStates_Get_466020(pPathEnemyStopper->field_12_switch_id))
    {
        return 1;
    }

    if (bCheckLeftRightBounds)
    {
        if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
                FP_GetExponent(field_B8_xpos),
                FP_GetExponent(FP_Abs(field_BC_ypos)),
                FP_GetExponent(field_B8_xpos + gridSize),
                FP_GetExponent(field_BC_ypos - ScaleToGridSize_4498B0(field_CC_sprite_scale)),
                objectType))
        {
            return 1;
        }
    }
    return 0;
}

GameSpeakEvents Scrab::LastSpeak_4A56F0()
{
    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 1))
    {
        return GameSpeakEvents::eNone_m1;
    }

    if (field_17C_last_event == pEventSystem_5BC11C->field_28_last_event_index)
    {
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            return GameSpeakEvents::eNone_m1;
        }
        else
        {
            return GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_17C_last_event = pEventSystem_5BC11C->field_28_last_event_index;
        return pEventSystem_5BC11C->field_20_last_event;
    }
}
