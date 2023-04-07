#include "stdafx.h"
#include "FlyingSlig.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Collisions.hpp"
#include "Shadow.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "MusicController.hpp"
#include "DDCheat.hpp"
#include "Slig.hpp"
#include "Grenade.hpp"
#include "Dove.hpp"
#include "CrawlingSlig.hpp"
#include "Particle.hpp"
#include "Gibs.hpp"
#include "Blood.hpp"
#include "GameSpeak.hpp"
#include "Explosion.hpp"
#include "Bullet.hpp"
#include "ParticleBurst.hpp"
#include "Lever.hpp"
#include "Sys_common.hpp"
#include "Grid.hpp"

ALIVE_ARY(1, 0x5523A0, TFlyingSligBrainFn, 26, sFlyingSlig_motion_table_5523A0,
          {
              &FlyingSlig::M_Idle_0_4385E0,
              &FlyingSlig::M_HorizontalMovement_1_4386A0,
              &FlyingSlig::M_IdleToTurn_2_4388B0,
              &FlyingSlig::M_DownMovement_3_438AA0,
              &FlyingSlig::M_DownMovementToTurn_4_438CC0,
              &FlyingSlig::M_UpMovement_5_438DD0,
              &FlyingSlig::M_UpMovementToTurn_6_439030,
              &FlyingSlig::M_LeverPull_7_439150,
              &FlyingSlig::M_GameSpeak_8_4391D0,
              &FlyingSlig::M_Possession_9_434290,
              &FlyingSlig::M_EndHorizontalMovement_10_4387D0,
              &FlyingSlig::M_BeginUpMovement_11_438E40,
              &FlyingSlig::M_HorizontalToDownMovement_12_438B10,
              &FlyingSlig::M_UpToHorizontalMovement_13_438F60,
              &FlyingSlig::M_DownToHorizontalMovement_14_438BF0,
              &FlyingSlig::M_QuickTurn_15_4387F0,
              &FlyingSlig::M_IdleToHorizontalMovement_16_438730,
              &FlyingSlig::M_BeginDownMovement_17_438B80,
              &FlyingSlig::M_EndDownMovement_18_438C90,
              &FlyingSlig::M_DownKnockback_19_4390D0,
              &FlyingSlig::M_UpKnockback_20_439110,
              &FlyingSlig::M_EndUpMovement_21_438EB0,
              &FlyingSlig::M_InstantUpXTurn_22_438EE0,
              &FlyingSlig::M_InstantDownXTurn_23_438F20,
              &FlyingSlig::M_HorizontalToUpMovement_24_438D60,
              &FlyingSlig::M_TurnToHorizontalMovement_25_4389E0,
          });

const TFlyingSligBrainFn sFlyingSlig_Brain_table_552350[18] = {
    &FlyingSlig::Brain_0_Inactive_4355B0,
    &FlyingSlig::Brain_1_Death_4364E0,
    &FlyingSlig::Brain_2_Moving_4356D0,
    &FlyingSlig::Brain_3_GetAlerted_435750,
    &FlyingSlig::Brain_4_ChasingEnemy_435BC0,
    &FlyingSlig::Brain_5_Idle_435820,
    &FlyingSlig::Brain_6_GameSpeakToMoving_435940,
    &FlyingSlig::Brain_7_PanicMoving_435990,
    &FlyingSlig::Brain_8_PanicIdle_435AC0,
    &FlyingSlig::Brain_9_SpottedEnemy_435E40,
    &FlyingSlig::Brain_10_LaunchingGrenade_435F10,
    &FlyingSlig::Brain_11_AbeDead_435FD0,
    &FlyingSlig::Brain_12_Possessed_436040,
    &FlyingSlig::Brain_13_Possession_4360F0,
    &FlyingSlig::Brain_14_DePossession_436180,
    &FlyingSlig::Brain_15_FlyingSligSpawn_4362C0,
    &FlyingSlig::Brain_1_Death_4364E0,
    &FlyingSlig::Brain_17_FromCrawlingSlig_4355E0,
};

const static BrainFunctionData<TFlyingSligBrainFn> sFlyingSligBrainTable[18] = {
    {&FlyingSlig::Brain_0_Inactive_4355B0, 0x401802, "Brain_0_Inactive"},
    {&FlyingSlig::Brain_1_Death_4364E0, 0x402ABD, "Brain_1_Death (first)"},
    {&FlyingSlig::Brain_2_Moving_4356D0, 0x4046BF, "Brain_2_Moving"},
    {&FlyingSlig::Brain_3_GetAlerted_435750, 0x402603, "Brain_3_GetAlerted"},
    {&FlyingSlig::Brain_4_ChasingEnemy_435BC0, 0x402086, "Brain_4_ChasingEnemy"},
    {&FlyingSlig::Brain_5_Idle_435820, 0x4028A6, "Brain_5_Idle"},
    {&FlyingSlig::Brain_6_GameSpeakToMoving_435940, 0x403E31, "Brain_6_GameSpeakToMoving"},
    {&FlyingSlig::Brain_7_PanicMoving_435990, 0x404453, "Brain_7_PanicMoving"},
    {&FlyingSlig::Brain_8_PanicIdle_435AC0, 0x4013A2, "Brain_8_PanicIdle"},
    {&FlyingSlig::Brain_9_SpottedEnemy_435E40, 0x40394A, "Brain_9_SpottedEnemy"},
    {&FlyingSlig::Brain_10_LaunchingGrenade_435F10, 0x40253B, "Brain_10_LaunchingGrenade"},
    {&FlyingSlig::Brain_11_AbeDead_435FD0, 0x40469C, "Brain_11_AbeDead"},
    {&FlyingSlig::Brain_12_Possessed_436040, 0x404480, "Brain_12_Possessed"},
    {&FlyingSlig::Brain_13_Possession_4360F0, 0x40241E, "Brain_13_Possession"},
    {&FlyingSlig::Brain_14_DePossession_436180, 0x403DBE, "Brain_14_DePossession"},
    {&FlyingSlig::Brain_15_FlyingSligSpawn_4362C0, 0x40128A, "Brain_15_FlyingSligSpawn"},
    {&FlyingSlig::Brain_1_Death_4364E0, 0x402ABD, "Brain_1_Death (second)"},
    {&FlyingSlig::Brain_17_FromCrawlingSlig_4355E0, 0x401749, "Brain_17_FromNakedSlig"},
};

void FlyingSlig::SetBrain(TFlyingSligBrainFn fn)
{
    ::SetBrain(fn, field_29C_brain_state, sFlyingSligBrainTable);
}

bool FlyingSlig::BrainIs(TFlyingSligBrainFn fn)
{
    return ::BrainIs(fn, field_29C_brain_state, sFlyingSligBrainTable);
}

FlyingSlig* FlyingSlig::ctor_4342B0(Path_FlyingSlig* pTlv, s32 tlvInfo)
{
    //BaseCtor_4340B0(9); // Omitted for direct call
    ctor_408240(9);

    field_178_unused = 0;
    field_164_unused = -1;
    field_17A_unused = -1;

    SetVTable(this, 0x545120);

    if (tlvInfo != -1)
    {
        field_C_objectId = tlvInfo;
    }

    SetType(AETypes::eFlyingSlig_54);

    field_10_resources_array.SetAt(0, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kFlySligResID, TRUE, FALSE));
    field_10_resources_array.SetAt(1, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, TRUE, FALSE));
    field_10_resources_array.SetAt(2, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, TRUE, FALSE));
    field_10_resources_array.SetAt(3, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, TRUE, FALSE));
    field_10_resources_array.SetAt(4, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, TRUE, FALSE));
    field_10_resources_array.SetAt(5, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, TRUE, FALSE));
    field_10_resources_array.SetAt(6, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, TRUE, FALSE));
    field_10_resources_array.SetAt(7, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kVaporResID, TRUE, FALSE));
    field_10_resources_array.SetAt(8, ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, TRUE, FALSE));

    const AnimRecord& rec = AnimRec(AnimId::FlyingSlig_Idle);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    //Animation_Init_424E10(116888, 107, 48u, field_10_resources_array.ItemAt(0), 1, 1u);

    field_15E_useless = 0;

    field_20_animation.field_1C_fn_ptr_array = kFlyingSlig_Anim_Frames_Fns_55EFC4;

    field_114_flags.Set(Flags_114::e114_Bit3_Can_Be_Possessed);
    field_114_flags.Set(Flags_114::e114_Bit6_SetOffExplosives);

    field_6_flags.Set(BaseGameObject::eCanExplode_Bit7);

    field_14C_timer = 0;
    field_DC_bApplyShadows |= 2u;

    field_15C_voice_pitch_min = 45 * ((Math_NextRandom() % 5) - 2);

    field_150_grenade_delay = 0;
    field_154_collision_reaction_timer = 0;

    field_17E_flags.Clear(Flags_17E::eBit1_Speaking_flag1);
    field_17E_flags.Clear(Flags_17E::eBit5_Throw);
    field_17E_flags.Clear(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
    field_17E_flags.Clear(Flags_17E::eBit7_DoAction);
    field_17E_flags.Clear(Flags_17E::eBit8_Unused);
    field_17E_flags.Clear(Flags_17E::eBit9_Chanting);
    field_17E_flags.Clear(Flags_17E::eBit10_Speaking_flag2);

    field_158_obj_id = -1;

    field_288_unused = 0;
    field_290_bobbing_values_index = 0;
    field_284_bobbing_value = FP_FromInteger(0);
    field_28C_bobbing_values_table_index = 0;
    field_1E8_unused = 0;
    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_118_data = *pTlv;

    field_148_tlvInfo = tlvInfo;

    SetBrain(&FlyingSlig::Brain_0_Inactive_4355B0);

    field_106_current_motion = eFlyingSligMotions::M_Idle_0_4385E0;

    if (field_118_data.field_10_data.field_2_state == Path_FlyingSlig_Data::SpawnDelayStates::eUseCustomSpawnMoveDelay_1)
    {
        field_14C_timer = sGnFrame_5C1B84 + field_118_data.field_10_data.field_4_spawn_move_delay;
    }
    else if (field_118_data.field_10_data.field_2_state == Path_FlyingSlig_Data::SpawnDelayStates::eMoveImmediately_0)
    {
        field_14C_timer = sGnFrame_5C1B84 + 1;
    }

    field_2A8_max_x_speed = FP_FromInteger(field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2AC_up_vel = FP_FromInteger(-field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2B0_down_vel = FP_FromInteger(field_118_data.field_10_data.field_1A_max_velocity) * field_CC_sprite_scale;
    field_2B4_max_slow_down = FP_FromDouble(0.4) * field_CC_sprite_scale;
    field_2B8_max_speed_up = FP_FromDouble(0.4) * field_CC_sprite_scale;

    field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, field_118_data.field_10_data.field_A_direction == XDirection_short::eLeft_0);

    if (field_118_data.field_10_data.field_0_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_SligGreeterFarts_Half_14;
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = Layer::eLayer_SligGreeterFarts_33;
        field_D6_scale = 1;
    }

    field_17E_flags.Set(Flags_17E::eBit13_Persistant, field_118_data.field_10_data.field_1E_persistant == Choice_short::eYes_1);

    field_17C_launch_switch_id |= field_118_data.field_10_data.field_1C_launch_switch_id;

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    FP hitX = {};
    FP hitY = {};
    sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(pTlv->field_8_top_left.field_0_x),
        FP_FromInteger(pTlv->field_8_top_left.field_2_y),
        FP_FromInteger(pTlv->field_C_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_C_bottom_right.field_2_y),
        &field_100_pCollisionLine,
        &hitX,
        &hitY,
        0x100);

    if (field_100_pCollisionLine)
    {
        field_B8_xpos = FP_FromInteger(field_100_pCollisionLine->field_0_rect.x);
        field_BC_ypos = FP_FromInteger(field_100_pCollisionLine->field_0_rect.y);
        sub_4348A0();
        field_194 = FP_FromInteger(0);
    }

    field_1D8_unused = 0;
    field_1DC_unused = 0;
    field_1E0_unused = 0;
    field_1E4_unused = 0;

    field_E0_pShadow = ae_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

const AnimId sFlyingSligFrameTables_552408[28] = {
    AnimId::FlyingSlig_Idle,
    AnimId::FlyingSlig_MoveHorizontal,
    AnimId::FlyingSlig_IdleTurnAround,
    AnimId::FlyingSlig_MoveDown,
    AnimId::FlyingSlig_MoveDownTurnAround,
    AnimId::FlyingSlig_MoveUp,
    AnimId::FlyingSlig_MoveUpTurnAround,
    AnimId::FlyingSlig_PullLever,
    AnimId::FlyingSlig_Speak,
    AnimId::FlyingSlig_Possession,
    AnimId::FlyingSlig_MoveHorizontalEnd,
    AnimId::FlyingSlig_MoveUpStart,
    AnimId::FlyingSlig_MoveHorizontalToDown,
    AnimId::FlyingSlig_MoveUpToHorizontal,
    AnimId::FlyingSlig_MoveDownToHorizontal,
    AnimId::FlyingSlig_TurnQuick,
    AnimId::FlyingSlig_IdleToHorizontal,
    AnimId::FlyingSlig_BeginDownMovement,
    AnimId::FlyingSlig_EndDownMovement,
    AnimId::FlyingSlig_DownKnockback,
    AnimId::FlyingSlig_UpKnockback,
    AnimId::FlyingSlig_EndUpMovement,
    AnimId::FlyingSlig_InstantUpXTurn,
    AnimId::FlyingSlig_InstantDownXTurn,
    AnimId::FlyingSlig_HorizontalToUpMovement,
    AnimId::FlyingSlig_TurnToHorizontalMovement,
};

s32 CC FlyingSlig::CreateFromSaveState_437E40(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const FlyingSlig_State*>(pBuffer);

    auto pTlv = static_cast<Path_FlyingSlig*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pSaveState->field_3C_tlvInfo));
    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kFlySligResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("FLYSLIG.BND", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSligBlowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLGBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("GRENADE.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSmallExplo2ResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SMEXP.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kMetalGibResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("METAL.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kAbeblowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEBLOW.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kBigflashResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("BIGFLASH.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kVaporResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("VAPOR.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlogBlowResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("DOGBLOW.BAN", nullptr);
    }

    auto pFlyingSlig = ae_new<FlyingSlig>();
    pFlyingSlig->ctor_4342B0(pTlv, pSaveState->field_3C_tlvInfo);
    pFlyingSlig->field_FC_pPathTLV = nullptr;
    pFlyingSlig->field_100_pCollisionLine = nullptr;

    pFlyingSlig->field_B8_xpos = pSaveState->field_4_xpos;
    pFlyingSlig->field_BC_ypos = pSaveState->field_8_ypos;

    pFlyingSlig->field_C4_velx = pSaveState->field_C_velx;
    pFlyingSlig->field_C8_vely = pSaveState->field_10_vely;

    pFlyingSlig->field_C0_path_number = pSaveState->field_14_path_number;
    pFlyingSlig->field_C2_lvl_number = pSaveState->field_16_lvl_number;
    pFlyingSlig->field_CC_sprite_scale = pSaveState->field_18_sprite_scale;

    pFlyingSlig->field_27C_r = pSaveState->field_1C_oldr;
    pFlyingSlig->field_D0_r = pSaveState->field_1C_oldr;

    pFlyingSlig->field_27E_g = pSaveState->field_1E_oldg;
    pFlyingSlig->field_D2_g = pSaveState->field_1E_oldg;

    pFlyingSlig->field_280_b = pSaveState->field_20_oldb;
    pFlyingSlig->field_D4_b = pSaveState->field_20_oldb;

    pFlyingSlig->field_106_current_motion = pSaveState->field_24_current_state;

    u8** ppRes = pFlyingSlig->ResBlockForMotion_4350F0(pSaveState->field_24_current_state);
    const AnimRecord& animRec = AnimRec(sFlyingSligFrameTables_552408[pFlyingSlig->field_106_current_motion]);
    pFlyingSlig->field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);

    pFlyingSlig->field_20_animation.field_92_current_frame = pSaveState->field_26_current_frame;

    pFlyingSlig->field_20_animation.field_E_frame_change_counter = pSaveState->field_28_frame_change_counter;

    pFlyingSlig->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pSaveState->field_2A_bAnimRender & 1);
    pFlyingSlig->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pSaveState->field_22_bAnimFlipX & 1);
    pFlyingSlig->field_6_flags.Set(BaseGameObject::eDrawable_Bit4, pSaveState->field_2B_bDrawable & 1);

    if (IsLastFrame(&pFlyingSlig->field_20_animation))
    {
        pFlyingSlig->field_20_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pFlyingSlig->field_10C_health = pSaveState->field_2C_current_health;
    pFlyingSlig->field_106_current_motion = pSaveState->field_30_current_state;
    pFlyingSlig->field_108_next_motion = pSaveState->field_32_delayed_state;
    pFlyingSlig->field_F8_LastLineYPos = FP_FromInteger(pSaveState->field_34_lastLineYPos);
    pFlyingSlig->field_114_flags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
    pFlyingSlig->field_104_collision_line_type = -1;

    if (pSaveState->field_36_line_idx != -1)
    {
        pFlyingSlig->field_104_collision_line_type = pSaveState->field_36_line_idx;
    }

    if (pSaveState->field_3A.Get(FlyingSlig_State::eBit1_bPossessed))
    {
        sControlledCharacter_5C1B8C = pFlyingSlig;
        pFlyingSlig->field_2A8_max_x_speed = (FP_FromDouble(5.5) * pFlyingSlig->field_CC_sprite_scale);
        pFlyingSlig->field_2AC_up_vel = (FP_FromDouble(-5.5) * pFlyingSlig->field_CC_sprite_scale);
        pFlyingSlig->field_2B0_down_vel = (FP_FromDouble(5.5) * pFlyingSlig->field_CC_sprite_scale);
        pFlyingSlig->field_2B4_max_slow_down = (FP_FromDouble(0.25) * pFlyingSlig->field_CC_sprite_scale);
        pFlyingSlig->field_2B8_max_speed_up = (FP_FromDouble(0.7) * pFlyingSlig->field_CC_sprite_scale);
    }

    pFlyingSlig->field_17C_launch_switch_id = pSaveState->field_38_launch_switch_id;


    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit5_Throw, pSaveState->field_3A.Get(FlyingSlig_State::eBit2_Throw));
    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit6_bAlertedAndNotFacingAbe, pSaveState->field_3A.Get(FlyingSlig_State::eBit3_bAlertedAndNotFacingAbe));
    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit7_DoAction, pSaveState->field_3A.Get(FlyingSlig_State::eBit4_DoAction));
    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit9_Chanting, pSaveState->field_3A.Get(FlyingSlig_State::eBit5_Chanting));
    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit10_Speaking_flag2, pSaveState->field_3A.Get(FlyingSlig_State::eBit6_Speaking_flag2));
    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1, pSaveState->field_3A.Get(FlyingSlig_State::eBit7_Speaking_flag1));
    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit2_bLastLine, pSaveState->field_3A.Get(FlyingSlig_State::eBit8_bLastLine));
    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit3, pSaveState->field_3A.Get(FlyingSlig_State::eBit9));
    pFlyingSlig->field_17E_flags.Set(Flags_17E::eBit4, pSaveState->field_3A.Get(FlyingSlig_State::eBit10));


    pFlyingSlig->field_14C_timer = pSaveState->field_40_timer;
    pFlyingSlig->field_150_grenade_delay = pSaveState->field_44_grenade_delay;
    pFlyingSlig->field_154_collision_reaction_timer = pSaveState->field_48_collision_reaction_timer;
    pFlyingSlig->field_184_xSpeed = pSaveState->field_4C_xSpeed;
    pFlyingSlig->field_188_ySpeed = pSaveState->field_50_ySpeed;
    pFlyingSlig->field_17D_next_speak = pSaveState->field_54_next_speak;
    pFlyingSlig->field_160_voice_pitch_min = pSaveState->field_56_voice_pitch_min;
    pFlyingSlig->field_158_obj_id = pSaveState->field_58_obj_id;
    pFlyingSlig->field_18C = pSaveState->field_5C;
    pFlyingSlig->field_190 = pSaveState->field_60;
    pFlyingSlig->field_194 = pSaveState->field_64;
    pFlyingSlig->field_198_line_length = pSaveState->field_68_line_length;
    pFlyingSlig->field_1C4 = pSaveState->field_6C;
    pFlyingSlig->field_1C8_lever_pull_range_xpos = pSaveState->field_70_lever_pull_range_xpos;
    pFlyingSlig->field_1CC_lever_pull_range_ypos = pSaveState->field_74_lever_pull_range_ypos;
    pFlyingSlig->field_1D8_unused = pSaveState->field_78_unused;
    pFlyingSlig->field_1DC_unused = pSaveState->field_7C_unused;
    pFlyingSlig->field_1E0_unused = pSaveState->field_80_unused;
    pFlyingSlig->field_1E4_unused = pSaveState->field_84_unused;
    pFlyingSlig->field_294_nextXPos = pSaveState->field_88_nextXPos;
    pFlyingSlig->field_298_nextYPos = pSaveState->field_8C_nextYPos;
    pFlyingSlig->SetBrain(sFlyingSlig_Brain_table_552350[pSaveState->field_90_fns1_idx]);
    pFlyingSlig->field_1E8_unused = pSaveState->field_98_unused;
    pFlyingSlig->field_2A0_abe_level = pSaveState->field_9A_abe_level;
    pFlyingSlig->field_2A2_abe_path = pSaveState->field_9C_abe_path;
    pFlyingSlig->field_2A4_abe_camera = pSaveState->field_9E_abe_camera;
    pFlyingSlig->field_290_bobbing_values_index = pSaveState->field_A4_bobbing_values_index;
    pFlyingSlig->field_284_bobbing_value = pSaveState->field_A8_bobbing_value;
    pFlyingSlig->field_28C_bobbing_values_table_index = pSaveState->field_A0_bobbing_values_table_index;
    return sizeof(FlyingSlig_State);
}

s32 FlyingSlig::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_43B1E0(reinterpret_cast<FlyingSlig_State*>(pSaveBuffer));
}

s32 FlyingSlig::vGetSaveState_43B1E0(FlyingSlig_State* pState)
{
    if (field_114_flags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    pState->field_0_type = AETypes::eFlyingSlig_54;

    pState->field_4_xpos = field_B8_xpos;
    pState->field_8_ypos = field_BC_ypos;
    pState->field_C_velx = field_C4_velx;
    pState->field_10_vely = field_C8_vely;

    pState->field_14_path_number = field_C0_path_number;
    pState->field_16_lvl_number = field_C2_lvl_number;
    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_1C_oldr = field_D0_r;
    pState->field_1E_oldg = field_D2_g;
    pState->field_20_oldb = field_D4_b;

    pState->field_22_bAnimFlipX = field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX);
    pState->field_24_current_state = field_106_current_motion;
    pState->field_26_current_frame = field_20_animation.field_92_current_frame;
    pState->field_28_frame_change_counter = field_20_animation.field_E_frame_change_counter;

    pState->field_2B_bDrawable = field_6_flags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_2A_bAnimRender = field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render);
    pState->field_2C_current_health = field_10C_health;
    pState->field_30_current_state = field_106_current_motion;
    pState->field_32_delayed_state = field_108_next_motion;

    pState->field_34_lastLineYPos = FP_GetExponent(field_F8_LastLineYPos);

    pState->field_36_line_idx = -1; // OG bug - actually becomes 0 due to impossible case removed below ?

    if (field_100_pCollisionLine)
    {
        pState->field_36_line_idx = static_cast<s16>(field_100_pCollisionLine - sCollisions_DArray_5C1128->field_0_pArray);
    }

    pState->field_38_launch_switch_id = field_17C_launch_switch_id;

    pState->field_3A.Set(FlyingSlig_State::eBit1_bPossessed, this == sControlledCharacter_5C1B8C);
    pState->field_3A.Set(FlyingSlig_State::eBit2_Throw, field_17E_flags.Get(Flags_17E::eBit5_Throw));
    pState->field_3A.Set(FlyingSlig_State::eBit3_bAlertedAndNotFacingAbe, field_17E_flags.Get(Flags_17E::eBit6_bAlertedAndNotFacingAbe));
    pState->field_3A.Set(FlyingSlig_State::eBit4_DoAction, field_17E_flags.Get(Flags_17E::eBit7_DoAction));
    pState->field_3A.Set(FlyingSlig_State::eBit5_Chanting, field_17E_flags.Get(Flags_17E::eBit9_Chanting));
    pState->field_3A.Set(FlyingSlig_State::eBit6_Speaking_flag2, field_17E_flags.Get(Flags_17E::eBit10_Speaking_flag2));
    pState->field_3A.Set(FlyingSlig_State::eBit7_Speaking_flag1, field_17E_flags.Get(Flags_17E::eBit1_Speaking_flag1));
    pState->field_3A.Set(FlyingSlig_State::eBit8_bLastLine, field_17E_flags.Get(Flags_17E::eBit2_bLastLine));
    pState->field_3A.Set(FlyingSlig_State::eBit9, field_17E_flags.Get(Flags_17E::eBit3));
    pState->field_3A.Set(FlyingSlig_State::eBit10, field_17E_flags.Get(Flags_17E::eBit4));

    pState->field_3C_tlvInfo = field_148_tlvInfo;
    pState->field_40_timer = field_14C_timer;
    pState->field_44_grenade_delay = field_150_grenade_delay;
    pState->field_48_collision_reaction_timer = field_154_collision_reaction_timer;

    pState->field_4C_xSpeed = field_184_xSpeed;
    pState->field_50_ySpeed = field_188_ySpeed;

    pState->field_54_next_speak = field_17D_next_speak;
    pState->field_56_voice_pitch_min = field_160_voice_pitch_min;

    pState->field_58_obj_id = -1;
    if (field_158_obj_id != -1)
    {
        auto pObj = sObjectIds_5C1B70.Find_449CF0(field_158_obj_id);
        if (pObj)
        {
            pState->field_58_obj_id = pObj->field_C_objectId;
        }
    }

    pState->field_5C = field_18C;
    pState->field_60 = field_190;
    pState->field_64 = field_194;
    pState->field_68_line_length = field_198_line_length;
    pState->field_6C = field_1C4;
    pState->field_70_lever_pull_range_xpos = field_1C8_lever_pull_range_xpos;
    pState->field_74_lever_pull_range_ypos = field_1CC_lever_pull_range_ypos;
    pState->field_78_unused = field_1D8_unused;
    pState->field_7C_unused = field_1DC_unused;
    pState->field_80_unused = field_1E0_unused;
    pState->field_84_unused = field_1E4_unused;
    pState->field_88_nextXPos = field_294_nextXPos;
    pState->field_8C_nextYPos = field_298_nextYPos;
    pState->field_90_fns1_idx = 0;

    s32 idx = 0;
    for (const auto& fn : sFlyingSlig_Brain_table_552350)
    {
        if (BrainIs(fn))
        {
            pState->field_90_fns1_idx = idx;
            break;
        }
        idx++;
    }

    pState->field_98_unused = field_1E8_unused;

    pState->field_9A_abe_level = field_2A0_abe_level;
    pState->field_9C_abe_path = field_2A2_abe_path;
    pState->field_9E_abe_camera = field_2A4_abe_camera;

    pState->field_A4_bobbing_values_index = field_290_bobbing_values_index;
    pState->field_A8_bobbing_value = field_284_bobbing_value;
    pState->field_A0_bobbing_values_table_index = field_28C_bobbing_values_table_index;

    return sizeof(FlyingSlig_State);
}

void FlyingSlig::dtor_434990()
{
    SetVTable(this, 0x545120);

    if (sControlledCharacter_5C1B8C == this)
    {
        sControlledCharacter_5C1B8C = sActiveHero_5C1B68;
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
        if (gMap_5C3030.field_A_level != LevelIds::eMenu_0)
        {
            gMap_5C3030.SetActiveCam_480D30(
                field_2A0_abe_level,
                field_2A2_abe_path,
                field_2A4_abe_camera,
                CameraSwapEffects::eInstantChange_0,
                0,
                0);
        }
    }

    Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_148_tlvInfo);
    if (pTlv)
    {
        if (pTlv->field_4_type.mType != TlvTypes::SligGetWings_105 && pTlv->field_4_type.mType != TlvTypes::FlyingSligSpawner_92)
        {
            if (field_10C_health <= FP_FromInteger(0))
            {
                Path::TLV_Reset_4DB8E0(field_148_tlvInfo, -1, 0, 1);
            }
            else
            {
                Path::TLV_Reset_4DB8E0(field_148_tlvInfo, -1, 0, 0);
            }
        }
    }

    BaseAliveGameObject::dtor_4080B0();
}

BaseGameObject* FlyingSlig::VDestructor(s32 flags)
{
    return vdtor_434870(flags);
}

FlyingSlig* FlyingSlig::vdtor_434870(s32 flags)
{
    dtor_434990();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void FlyingSlig::VScreenChanged()
{
    vScreenChanged_434C10();
}

void FlyingSlig::vScreenChanged_434C10()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || (gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path && (this != sControlledCharacter_5C1B8C || field_17E_flags.Get(Flags_17E::eBit13_Persistant))))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void FlyingSlig::VUpdate()
{
    vUpdate_434AD0();
}

void FlyingSlig::vUpdate_434AD0()
{
    if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);

        if (!IsPossessed_436A90())
        {
            if (field_104_collision_line_type != -1)
            {
                const FP savedNextXPos = field_294_nextXPos;
                const FP savedNextYPos = field_298_nextYPos;
                field_100_pCollisionLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(field_104_collision_line_type);
                sub_4348A0();
                field_294_nextXPos = savedNextXPos;
                field_298_nextYPos = savedNextYPos;
            }
        }

        if (field_158_obj_id != -1)
        {
            for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pObj)
                {
                    break;
                }

                if (pObj->field_C_objectId == field_158_obj_id)
                {
                    field_158_obj_id = pObj->field_8_object_id;
                    break;
                }
            }
        }
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        field_B8_xpos = field_294_nextXPos;
        field_BC_ypos = field_298_nextYPos;

        const auto oldBrain = field_29C_brain_state;

        (this->*(field_29C_brain_state))();

        (this->*(sFlyingSlig_motion_table_5523A0)[field_106_current_motion])();

        if (oldBrain != field_29C_brain_state)
        {
            //LOG_INFO("FlyingSlig: Old brain = " << GetOriginalFn(oldBrain, sFlyingSligAITable).fnName << " new brain = " << GetOriginalFn(field_29C_brain_state, sFlyingSligAITable).fnName);
            //LOG_INFO("FlyingSlig: Old motion = " << oldMotion << " new motion = " << field_106_current_motion);
        }

        Movement_4396E0();
    }
}

void FlyingSlig::VRender(PrimHeader** ppOt)
{
    vRender_435130(ppOt);
}

void FlyingSlig::vRender_435130(PrimHeader** ot)
{
    const s16 eyeIndices[] = {60, 62};
    renderWithGlowingEyes(ot, this, &field_1F4_pPalAlloc[0], ALIVE_COUNTOF(field_1F4_pPalAlloc),
                          &field_274_pal_rect, field_27C_r, field_27E_g, field_280_b, &eyeIndices[0], ALIVE_COUNTOF(eyeIndices));
}

void FlyingSlig::sub_4348A0()
{
    field_17E_flags.Clear(Flags_17E::eBit3);
    field_17E_flags.Clear(Flags_17E::eBit2_bLastLine);
    field_298_nextYPos = field_BC_ypos;
    field_294_nextXPos = field_B8_xpos;
    field_18C = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);
    sub_437C70(field_100_pCollisionLine);
    const s16 v5 = FP_GetExponent(field_BC_ypos - field_1A4_rect.y);
    const s16 v6 = FP_GetExponent(field_B8_xpos - field_1A4_rect.x);
    field_194 = FP_FromInteger(Math_SquareRoot_Int_496E70(v5 * v5 + v6 * v6));
    field_17E_flags.Set(Flags_17E::eBit4, field_118_data.field_10_data.field_A_direction == XDirection_short::eLeft_0);
}

const s32 sBobbingValuesHorizontalMovement_552500[9] = {
    -163840,
    -245760,
    -286720,
    -327680,
    -286720,
    -245760,
    -163840,
    -65536,
    65535999};

const s32 sBobbingValuesIdle_552524[11] = {
    74785,
    85101,
    79944,
    59312,
    23209,
    -34563,
    -82422,
    -98373,
    -82422,
    -34563,
    65535999};

const s32 sBobbingValuesTurning_552550[11] = {
    38603,
    84930,
    84928,
    38605,
    -187040,
    -344791,
    -178048,
    70049,
    100043,
    38273,
    65535999};



ALIVE_ARY(1, 0x55257C, const s32*, 4, sBobbingValuesTables_55257C, {nullptr, sBobbingValuesHorizontalMovement_552500, sBobbingValuesIdle_552524, sBobbingValuesTurning_552550});

void FlyingSlig::Movement_4396E0()
{
    if (!IsPossessed_436A90())
    {
        if (field_190 > FP_FromInteger(0))
        {
            if (field_18C + field_190 > field_2A8_max_x_speed)
            {
                field_18C = field_2A8_max_x_speed;
            }
            else
            {
                field_18C += field_190;
            }
        }
        else if (field_190 < FP_FromInteger(0))
        {
            if (field_18C + field_190 < -field_2A8_max_x_speed)
            {
                field_18C = -field_2A8_max_x_speed;
            }
            else
            {
                field_18C += field_190;
            }
        }
        else if (field_190 == FP_FromInteger(0))
        {
            if (field_18C >= -field_2B4_max_slow_down)
            {
                if (field_18C <= field_2B4_max_slow_down)
                {
                    field_18C = FP_FromInteger(0);
                }
                else
                {
                    field_18C -= field_2B4_max_slow_down;
                }
            }
            else
            {
                field_18C += field_2B4_max_slow_down;
            }
        }

        FP_Point posXY = {};
        sub_437AC0(field_18C, &posXY);
        field_C4_velx = posXY.field_0_x - field_B8_xpos;
        field_C8_vely = posXY.field_4_y - field_BC_ypos;
    }
    else
    {
        const s32 newVel = Math_SquareRoot_Int_496E70(FP_GetExponent((field_C8_vely * field_C8_vely) + (field_C4_velx * field_C4_velx)));
        if (FP_Abs(FP_FromInteger(newVel)) > FP_FromDouble(0.05))
        {
            field_C4_velx = field_C4_velx - ((field_C4_velx / FP_FromInteger(newVel)) * field_2B4_max_slow_down) + field_184_xSpeed;
            field_C8_vely = field_C8_vely - ((field_C8_vely / FP_FromInteger(newVel)) * field_2B4_max_slow_down) + field_188_ySpeed;
        }
        else
        {
            if (field_184_xSpeed != FP_FromInteger(0) || field_188_ySpeed != FP_FromInteger(0))
            {
                field_C4_velx += field_184_xSpeed;
                field_C8_vely = field_C8_vely + field_188_ySpeed;
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);
                field_C8_vely = FP_FromInteger(0);
            }
        }

        const FP v15 = FP_FromInteger(Math_SquareRoot_Int_496E70(FP_GetExponent((field_C8_vely * field_C8_vely) + (field_C4_velx * field_C4_velx))));
        if (v15 > field_2A8_max_x_speed)
        {
            field_C4_velx = ((field_C4_velx / v15) * field_2A8_max_x_speed);
            field_C8_vely = ((field_C8_vely / v15) * field_2A8_max_x_speed);
        }

        if (field_C8_vely < FP_FromInteger(0))
        {
            if (CollisionUp_43A640(field_C8_vely))
            {
                field_C8_vely = -field_C8_vely;
            }
        }

        if (field_C8_vely > FP_FromInteger(0))
        {
            if (CollisionDown_43A9E0(field_C8_vely))
            {
                field_C8_vely = -field_C8_vely;
            }
        }

        if (field_C4_velx != FP_FromInteger(0))
        {
            if (CollisionLeftRight_43AC80(field_C4_velx))
            {
                field_C4_velx = -field_C4_velx;
            }
        }
    }

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    SetActiveCameraDelayedFromDir_408C40();

    if (field_17E_flags.Get(Flags_17E::eBit5_Throw))
    {
        if (static_cast<s32>(sGnFrame_5C1B84) > field_150_grenade_delay && 
            (field_114_flags.Get(Flags_114::e114_Bit4_bPossesed) || SwitchStates_Get_466020(field_17C_launch_switch_id) ||
             field_114_flags.Get(Flags_114::e114_Bit4_bPossesed)) && CanThrowGrenade_43A490())
        {
            ThrowGrenade_43A1E0();
        }
    }

    if (field_17E_flags.Get(Flags_17E::eBit10_Speaking_flag2))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_GameSpeak_8_4391D0);
    }

    if (field_17E_flags.Get(Flags_17E::eBit6_bAlertedAndNotFacingAbe))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_IdleToTurn_2_4388B0);
    }

    if (field_17E_flags.Get(Flags_17E::eBit7_DoAction))
    {
        if (field_106_current_motion == eFlyingSligMotions::M_Idle_0_4385E0)
        {
            TryPullLever_439DB0();
        }
    }

    field_294_nextXPos = field_B8_xpos;
    field_298_nextYPos = field_BC_ypos;

    field_BC_ypos += field_CC_sprite_scale * FP_FromInteger(20);

    if (field_28C_bobbing_values_table_index)
    {
        if (field_28C_bobbing_values_table_index > 3)
        {
            ALIVE_FATAL("FlyingSlig array out of bounds");
        }
        const FP* pTable = reinterpret_cast<const FP*>(sBobbingValuesTables_55257C[field_28C_bobbing_values_table_index]); // TODO: Type conversion !!
        field_288_unused = pTable;
        field_284_bobbing_value = pTable[field_290_bobbing_values_index];
        if (field_284_bobbing_value <= FP_FromInteger(990))
        {
            field_290_bobbing_values_index++;
        }
        else
        {
            field_284_bobbing_value = FP_FromInteger(0);
            field_28C_bobbing_values_table_index = 0;
            field_290_bobbing_values_index = 0;
        }
        field_BC_ypos = field_BC_ypos + field_284_bobbing_value;
    }
    else
    {
        if (field_284_bobbing_value != FP_FromInteger(0))
        {
            if (field_284_bobbing_value <= FP_FromInteger(0))
            {
                if (field_284_bobbing_value <= FP_FromInteger(-1))
                {
                    field_284_bobbing_value = field_284_bobbing_value + FP_FromInteger(1);
                }
                else
                {
                    field_284_bobbing_value = FP_FromInteger(0);
                }
            }
            else if (field_284_bobbing_value < FP_FromInteger(1))
            {
                field_284_bobbing_value = FP_FromInteger(0);
            }
            else
            {
                field_284_bobbing_value = field_284_bobbing_value - FP_FromInteger(1);
            }
            field_BC_ypos = field_BC_ypos + field_284_bobbing_value;
        }
    }

    s32 v37 = {};
    if (Math_Distance_496EB0(0, 0, FP_GetExponent(field_C4_velx), FP_GetExponent(field_C8_vely)) >= 0)
    {
        v37 = Math_Distance_496EB0(0, 0, FP_GetExponent(field_C4_velx), FP_GetExponent(field_C8_vely));
    }
    else
    {
        v37 = -Math_Distance_496EB0(0, 0, FP_GetExponent(field_C4_velx), FP_GetExponent(field_C8_vely));
    }

    const s32 v38 = static_cast<s32>(sGnFrame_5C1B84) % ((FP_FromInteger(v37) < field_2A8_max_x_speed) + 2);
    if (!v38 && field_10C_health > FP_FromInteger(0))
    {
        Slig_SoundEffect_4BFFE0((Math_NextRandom() % 3) ? SligSfx::ePropeller2_10 : SligSfx::ePropeller1_9, this);
    }

    field_17E_flags.Clear(Flags_17E::eBit5_Throw);
    field_17E_flags.Clear(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
    field_17E_flags.Clear(Flags_17E::eBit7_DoAction);
    field_17E_flags.Clear(Flags_17E::eBit10_Speaking_flag2);

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    field_190 = FP_FromInteger(0);
}

s16 FlyingSlig::VTakeDamage_408730(BaseGameObject* pFrom)
{
    return vTakeDamage_434C90(pFrom);
}

s16 FlyingSlig::vTakeDamage_434C90(BaseGameObject* pFrom)
{
    switch (pFrom->Type())
    {
        case AETypes::eBullet_15:
        {
            if (static_cast<Bullet*>(pFrom)->field_20_type == BulletType::eZBullet_3)
            {
                PSX_RECT bRect = {};
                vGetBoundingRect_424FD0(&bRect, 1);
                Path_TLV* pTlv = nullptr;
                do
                {
                    pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv,
                                                                 field_B8_xpos,
                                                                 FP_FromInteger(bRect.y),
                                                                 field_B8_xpos,
                                                                 FP_FromInteger(bRect.y));
                    if (!pTlv)
                    {
                        break;
                    }
                    if (pTlv->field_4_type == TlvTypes::ZSligCover_50)
                    {
                        // Left/right in cover
                        if (bRect.x >= pTlv->field_8_top_left.field_0_x && bRect.x <= pTlv->field_C_bottom_right.field_0_x && bRect.y >= pTlv->field_8_top_left.field_2_y && bRect.y <= pTlv->field_C_bottom_right.field_2_y)
                        {
                            // Top/bottom in cover
                            if (bRect.w >= pTlv->field_8_top_left.field_0_x && bRect.w <= pTlv->field_C_bottom_right.field_0_x && bRect.h >= pTlv->field_8_top_left.field_2_y && bRect.h <= pTlv->field_C_bottom_right.field_2_y)
                            {
                                return 0;
                            }
                        }
                    }
                }
                while (pTlv);
            }
            // Not in Z-Cover, fall through and be shot
            [[fallthrough]];
        }
        case AETypes::eDrill_30:
        case AETypes::eRockSpawner_48:
        case AETypes::eAbe_69:
        case AETypes::eMeatSaw_86:
        case AETypes::eMineCar_89:
        case AETypes::eNeverSet_107:
        case AETypes::eSlog_126:
        {
            if (BrainIs(&FlyingSlig::Brain_1_Death_4364E0))
            {
                return 1;
            }
            BlowUp_436510();
            auto pExplosion = ae_new<Explosion>();
            if (!pExplosion)
            {
                return 1;
            }
            pExplosion->ctor_4A1200(field_B8_xpos, field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)), field_CC_sprite_scale, 1);
            return 1;
        }

        case AETypes::eElectricWall_39:
            Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_15C_voice_pitch_min, this);
            break;

        case AETypes::eBaseBomb_46:
        case AETypes::eExplosion_109:
            if (!BrainIs(&FlyingSlig::Brain_1_Death_4364E0))
            {
                BlowUp_436510();
            }
            break;

        case AETypes::eElectrocute_150:
            if (!BrainIs(&FlyingSlig::Brain_1_Death_4364E0))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                field_10C_health = FP_FromInteger(0);
                SetBrain(&FlyingSlig::Brain_1_Death_4364E0);
                field_14C_timer = sGnFrame_5C1B84;
            }
            break;

        default:
            return 1;
    }
    return 1;
}

void FlyingSlig::Brain_0_Inactive_4355B0()
{
    if (static_cast<s32>(sGnFrame_5C1B84) >= field_14C_timer)
    {
        ToMoving_435720();
    }
}

void FlyingSlig::Brain_1_Death_4364E0()
{
    if (static_cast<s32>(sGnFrame_5C1B84) >= field_14C_timer)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        Event_Broadcast_422BC0(kEventMudokonComfort, this);
    }
}

void FlyingSlig::Brain_2_Moving_4356D0()
{
    if (!sub_436730() && sub_4374A0(1) == 1)
    {
        field_17E_flags.Toggle(Flags_17E::eBit4);
        PatrolDelay_435860();
    }
}

void FlyingSlig::Brain_3_GetAlerted_435750()
{
    if (field_106_current_motion != eFlyingSligMotions::M_Idle_0_4385E0)
    {
        field_14C_timer++;
    }
    else if (vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
    {
        if (!sub_436730() && static_cast<s32>(sGnFrame_5C1B84) >= field_14C_timer)
        {
            ToMoving_435720();
        }
    }
    else
    {
        field_17E_flags.Set(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
        field_14C_timer++;
    }
}

void FlyingSlig::Brain_4_ChasingEnemy_435BC0()
{
    field_17E_flags.Clear(Flags_17E::eBit3);

    if (Event_Get_422C00(kEventHeroDying) && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        ToAbeDead_436010();
        return;
    }

    if (Event_Get_422C00(kEventResetting) || sControlledCharacter_5C1B8C->field_CC_sprite_scale != field_CC_sprite_scale || IsInInvisibleZone_425710(sControlledCharacter_5C1B8C) || sControlledCharacter_5C1B8C->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible) || (!IsWallBetween_43A550(this, sControlledCharacter_5C1B8C) && (sControlledCharacter_5C1B8C != sActiveHero_5C1B68 || sActiveHero_5C1B68->field_106_current_motion != eAbeMotions::Motion_65_LedgeAscend_4548E0) && sControlledCharacter_5C1B8C->Type() != AETypes::eMineCar_89))
    {
        PatrolDelay_435860();
        return;
    }

    if (sub_436C60(&field_100_pCollisionLine->field_0_rect, 1))
    {
        if (FP_Abs(field_194 - field_1C4) < (FP_FromInteger(15) * field_CC_sprite_scale))
        {
            ToLaunchingGrenade_435F50();
            return;
        }

        field_17E_flags.Set(Flags_17E::eBit4, field_1C4 > field_194);
        field_17E_flags.Set(Flags_17E::eBit3);

        if (static_cast<s32>(sGnFrame_5C1B84) > field_150_grenade_delay && CanThrowGrenade_43A490())
        {
            if (vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
            {
                if (!(Math_NextRandom() & 15))
                {
                    ToLaunchingGrenade_435F50();
                    return;
                }
            }
        }
    }
    else if (!sub_436B20())
    {
        sub_4373B0();
    }

    sub_4374A0(0);
}

void FlyingSlig::Brain_5_Idle_435820()
{
    if (!sub_436730() && static_cast<s32>(sGnFrame_5C1B84) >= field_14C_timer)
    {
        ToMoving_435720();
    }
}

void FlyingSlig::Brain_6_GameSpeakToMoving_435940()
{
    if (field_106_current_motion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
    {
        ToMoving_435720();
    }
}

void FlyingSlig::Brain_7_PanicMoving_435990()
{
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToChase_435E10();
        return;
    }

    if (!Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, field_D6_scale))
    {
        ToMoving_435720();
        return;
    }

    if (static_cast<s32>(sGnFrame_5C1B84) < field_14C_timer)
    {
        if (sub_4374A0(0) != 1)
        {
            return;
        }
        field_17E_flags.Toggle(Flags_17E::eBit4);
    }

    ToPanicIdle_435B50();
}

void FlyingSlig::Brain_8_PanicIdle_435AC0()
{
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToChase_435E10();
    }
    else if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, field_D6_scale))
    {
        if (static_cast<s32>(sGnFrame_5C1B84) >= field_14C_timer && field_106_current_motion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
        {
            ToPanicMoving_435A50();
        }
    }
    else
    {
        ToMoving_435720();
    }
}

void FlyingSlig::Brain_9_SpottedEnemy_435E40()
{
    if (static_cast<s32>(sGnFrame_5C1B84) >= field_14C_timer)
    {
        ToChase_435E10();
    }
}

void FlyingSlig::Brain_10_LaunchingGrenade_435F10()
{
    if (vIsFacingMe_4254A0(sControlledCharacter_5C1B8C))
    {
        field_17E_flags.Set(Flags_17E::eBit5_Throw);
    }
    ToChase_435E10();
}

void FlyingSlig::Brain_11_AbeDead_435FD0()
{
    if (static_cast<s32>(sGnFrame_5C1B84) == field_14C_timer - 45)
    {
        Say_436A50(SligSpeak ::eGotYa_12, 0);
    }
    else if (static_cast<s32>(sGnFrame_5C1B84) == field_14C_timer)
    {
        Say_436A50(SligSpeak::eLaugh_3, 0);
    }
}

void FlyingSlig::Brain_12_Possessed_436040()
{
    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    if (!field_17E_flags.Get(Flags_17E::eBit9_Chanting))
    {
        if (Input_IsChanting_45F260())
        {
            ToChantShake_436270();
        }
    }

    if (!Input_IsChanting_45F260())
    {
        field_17E_flags.Clear(Flags_17E::eBit9_Chanting);
    }

    HandlePlayerControls_439340();
}

void FlyingSlig::Brain_13_Possession_4360F0()
{
    if (static_cast<s32>(sGnFrame_5C1B84) > field_14C_timer)
    {
        Say_436A50(SligSpeak ::eHi_0, 0x1FF);
        ToPlayerControlled_4360C0();
    }
}

void FlyingSlig::Brain_14_DePossession_436180()
{
    if (!Input_IsChanting_45F260())
    {
        ToPlayerControlled_4360C0();
        return;
    }

    if (static_cast<s32>(sGnFrame_5C1B84) <= field_14C_timer)
    {
        if (!(static_cast<s32>(sGnFrame_5C1B84) % 4))
        {
            const FP xOff = (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(-20, 20) + (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? -10 : 10)));
            const FP yOff = (field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(-20, 10)));
            New_TintChant_Particle_426BE0(
                xOff + field_B8_xpos,
                yOff + field_BC_ypos,
                field_CC_sprite_scale,
                Layer::eLayer_0);
        }
    }
    else
    {
        BlowUp_436510();
    }
}

void FlyingSlig::Brain_15_FlyingSligSpawn_4362C0()
{
    if (sControlledCharacter_5C1B8C == this && field_10C_health > FP_FromInteger(0))
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::ePossessed_9, this, 0, 0);
    }

    if (FP_Abs(field_B8_xpos - field_1C8_lever_pull_range_xpos) >= FP_FromInteger(1) || FP_Abs(field_BC_ypos - field_1CC_lever_pull_range_ypos) >= FP_FromInteger(1))
    {
        const FP delta1 = field_1C8_lever_pull_range_xpos - field_B8_xpos;
        const FP delta2 = field_1CC_lever_pull_range_ypos - field_BC_ypos;

        if (FP_Abs(field_1C8_lever_pull_range_xpos - field_B8_xpos) <= FP_FromInteger(2))
        {
            field_B8_xpos = field_1C8_lever_pull_range_xpos;
        }
        else if (delta1 >= FP_FromInteger(2))
        {
            field_B8_xpos += FP_FromInteger(2);
        }
        else
        {
            field_B8_xpos -= FP_FromInteger(2);
        }

        if (FP_Abs(field_1CC_lever_pull_range_ypos - field_BC_ypos) <= FP_FromInteger(2))
        {
            field_BC_ypos = field_1CC_lever_pull_range_ypos;
        }
        else if (delta2 >= FP_FromInteger(2))
        {
            field_BC_ypos += FP_FromInteger(2);
        }
        else
        {
            field_BC_ypos -= FP_FromInteger(2);
        }

        if ((FP_Abs(field_B8_xpos - field_1C8_lever_pull_range_xpos) < FP_FromInteger(1)) && (FP_Abs(field_BC_ypos - field_1CC_lever_pull_range_ypos) < FP_FromInteger(1)))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_LeverPull_7_439150);
        }
    }
    else if (field_106_current_motion == eFlyingSligMotions::M_Idle_0_4385E0)
    {
        ToPlayerControlled_4360C0();
    }
}

void FlyingSlig::Brain_17_FromCrawlingSlig_4355E0()
{
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos - FP_FromInteger(5),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            0x100)
        == 1)
    {
        field_B8_xpos = hitX;
        field_BC_ypos = hitY;
        sub_4348A0();
        field_C8_vely = FP_FromInteger(0);
        field_C4_velx = FP_FromInteger(0);
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = FP_FromInteger(0);
        PatrolDelay_435860();
    }
    else
    {
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromDouble(-4.5);
        field_184_xSpeed = FP_FromInteger(0);
        field_188_ySpeed = FP_FromDouble(-0.5);
    }
}

void FlyingSlig::M_Idle_0_4385E0()
{
    if (!field_28C_bobbing_values_table_index)
    {
        field_28C_bobbing_values_table_index = 2;
        field_290_bobbing_values_index = 0;
    }

    if (field_184_xSpeed != FP_FromInteger(0))
    {
        if (IsFacingMovementDirection_43A510())
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_IdleToHorizontalMovement_16_438730);
            field_28C_bobbing_values_table_index = 1;
            field_290_bobbing_values_index = 0;
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_IdleToTurn_2_4388B0);
        }
    }
    else
    {
        if (field_188_ySpeed > FP_FromInteger(0))
        {
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
            }
        }
    }
}

void FlyingSlig::M_HorizontalMovement_1_4386A0()
{
    if (field_188_ySpeed < FP_FromInteger(0))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
    }
    else if (field_188_ySpeed > FP_FromInteger(0))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
    }
    else if (field_184_xSpeed == FP_FromInteger(0))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
    }
    else if (!IsFacingMovementDirection_43A510())
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        field_28C_bobbing_values_table_index = 3;
        field_290_bobbing_values_index = 0;
    }
}

void FlyingSlig::M_IdleToTurn_2_4388B0()
{
    if (field_20_animation.field_92_current_frame == 4)
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
            if (field_188_ySpeed >= FP_FromInteger(0))
            {
                if (field_188_ySpeed <= FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection_43A510())
                    {
                        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
                        VSetMotion_4081C0(eFlyingSligMotions::M_TurnToHorizontalMovement_25_4389E0);
                    }
                    else
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                    field_28C_bobbing_values_table_index = 0;
                }
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
                field_28C_bobbing_values_table_index = 0;
            }
        }
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
        }
    }
}

void FlyingSlig::M_DownMovement_3_438AA0()
{
    if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection_43A510())
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            if (field_184_xSpeed != FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            }
        }
    }
    else
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
    }
}

void FlyingSlig::M_DownMovementToTurn_4_438CC0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection_43A510())
        {
            if (field_188_ySpeed > FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownMovement_3_438AA0);
            }
            else if (field_184_xSpeed != FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
        }
    }
}

void FlyingSlig::M_UpMovement_5_438DD0()
{
    if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection_43A510())
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_184_xSpeed != FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            }
        }
    }
    else
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
    }
}

void FlyingSlig::M_UpMovementToTurn_6_439030()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_184_xSpeed == FP_FromInteger(0) || IsFacingMovementDirection_43A510())
        {
            if (field_188_ySpeed < FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpMovement_5_438DD0);
            }
            else if (field_184_xSpeed != FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
        }
    }
}

void FlyingSlig::M_LeverPull_7_439150()
{
    if (field_20_animation.field_92_current_frame)
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
        }
    }
    else
    {
        auto pSwitch = static_cast<Lever*>(sObjectIds_5C1B70.Find_449CF0(field_158_obj_id));
        if (pSwitch)
        {
            pSwitch->VPull_4D6050(field_B8_xpos < pSwitch->field_B8_xpos);
        }
        field_158_obj_id = -1;
    }
}

void FlyingSlig::M_GameSpeak_8_4391D0()
{
    if (field_20_animation.field_92_current_frame == 1 && field_17E_flags.Get(Flags_17E::eBit1_Speaking_flag1))
    {
        field_17E_flags.Clear(Flags_17E::eBit1_Speaking_flag1);

        if (BrainIs(&FlyingSlig::Brain_12_Possessed_436040))
        {
            switch (field_17D_next_speak)
            {
                case SligSpeak::eHi_0:
                    pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_Hi_27);
                    break;
                case SligSpeak::eHereBoy_1:
                    pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_HereBoy_28);
                    break;
                case SligSpeak::eGetHim_2:
                    pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_GetEm_29);
                    break;
                case SligSpeak::eLaugh_3:
                    pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_Laugh_8);
                    break;
                case SligSpeak::eBullshit_5:
                    pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_BS_5);
                    break;
                case SligSpeak::eLookOut_6:
                    pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_LookOut_6);
                    break;
                case SligSpeak::eBullshit2_7:
                    pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_BS2_7);
                    break;
                case SligSpeak::eFreeze_8:
                    pEventSystem_5BC11C->PushEvent_4218D0(GameSpeakEvents::Slig_Freeze_31);
                    break;
                default:
                    break;
            }
        }
        Slig_GameSpeak_SFX_4C04F0(field_17D_next_speak, 0, field_160_voice_pitch_min, this);
        Event_Broadcast_422BC0(kEventSpeaking, this);
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        this->VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_Possession_9_434290()
{
    NOT_IMPLEMENTED();
}

void FlyingSlig::M_EndHorizontalMovement_10_4387D0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_BeginUpMovement_11_438E40()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184_xSpeed > FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
                return;
            }
        }
        else if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            return;
        }
        VSetMotion_4081C0(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_HorizontalToDownMovement_12_438B10()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
                return;
            }
        }
        else if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            return;
        }
        VSetMotion_4081C0(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_UpToHorizontalMovement_13_438F60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
        }
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_188_ySpeed > FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C_bobbing_values_table_index = 0;
            }
            else if (field_188_ySpeed < FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
                field_28C_bobbing_values_table_index = 0;
            }
            else if (field_184_xSpeed == FP_FromInteger(0))
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
            }
            else if (IsFacingMovementDirection_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
            }
        }
    }
}

void FlyingSlig::M_DownToHorizontalMovement_14_438BF0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                if (field_184_xSpeed > FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection_43A510())
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
                    }
                    else
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
                }
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C_bobbing_values_table_index = 0;
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C_bobbing_values_table_index = 0;
        }
    }
}

void FlyingSlig::M_QuickTurn_15_4387F0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_188_ySpeed < FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_188_ySpeed > FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_184_xSpeed == FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
        }
        else if (!IsFacingMovementDirection_43A510())
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
    }
}

void FlyingSlig::M_IdleToHorizontalMovement_16_438730()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed < FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_188_ySpeed > FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
            field_28C_bobbing_values_table_index = 0;
        }
        else if (field_184_xSpeed == FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
        }
        else if (IsFacingMovementDirection_43A510())
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
        }
    }
}

void FlyingSlig::M_BeginDownMovement_17_438B80()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_DownMovementToTurn_4_438CC0);
                return;
            }
        }
        else if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
            return;
        }
        VSetMotion_4081C0(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_EndDownMovement_18_438C90()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_BeginUpMovement_11_438E40);
        }
    }
}

void FlyingSlig::M_DownKnockback_19_4390D0()
{
    if (FP_Abs(field_184_xSpeed) > FP_FromDouble(0.1) || field_188_ySpeed < FP_FromDouble(0.1))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_EndDownMovement_18_438C90);
    }
}

void FlyingSlig::M_UpKnockback_20_439110()
{
    if (FP_Abs(field_184_xSpeed) > FP_FromDouble(0.1) || field_188_ySpeed > FP_FromDouble(-0.1))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
    }
}

void FlyingSlig::M_EndUpMovement_21_438EB0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_188_ySpeed <= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_BeginDownMovement_17_438B80);
        }
    }
}

void FlyingSlig::M_InstantUpXTurn_22_438EE0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        VSetMotion_4081C0(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_InstantDownXTurn_23_438F20()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);
        VSetMotion_4081C0(eFlyingSligMotions::M_DownMovement_3_438AA0);
    }
}

void FlyingSlig::M_HorizontalToUpMovement_24_438D60()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_184_xSpeed != FP_FromInteger(0))
        {
            if (!IsFacingMovementDirection_43A510())
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_UpMovementToTurn_6_439030);
                return;
            }
        }
        else if (field_188_ySpeed >= FP_FromInteger(0))
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_EndUpMovement_21_438EB0);
            return;
        }
        VSetMotion_4081C0(eFlyingSligMotions::M_UpMovement_5_438DD0);
    }
}

void FlyingSlig::M_TurnToHorizontalMovement_25_4389E0()
{
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_20_animation.field_4_flags.Toggle(AnimFlags::eBit5_FlipX);

        if (field_188_ySpeed >= FP_FromInteger(0))
        {
            if (field_188_ySpeed <= FP_FromInteger(0))
            {
                if (field_184_xSpeed != FP_FromInteger(0))
                {
                    if (IsFacingMovementDirection_43A510())
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalMovement_1_4386A0);
                    }
                    else
                    {
                        VSetMotion_4081C0(eFlyingSligMotions::M_QuickTurn_15_4387F0);
                    }
                }
                else
                {
                    VSetMotion_4081C0(eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0);
                }
            }
            else
            {
                VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10);
                field_28C_bobbing_values_table_index = 0;
            }
        }
        else
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_HorizontalToUpMovement_24_438D60);
            field_28C_bobbing_values_table_index = 0;
        }
    }
}

s16 FlyingSlig::IsFacingMovementDirection_43A510()
{
    return ((field_184_xSpeed > FP_FromInteger(0) && !(field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)))
            || (field_184_xSpeed < FP_FromInteger(0) && field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)));
}

void FlyingSlig::ToPlayerControlled_4360C0()
{
    VSetMotion_4081C0(eFlyingSligMotions::M_Idle_0_4385E0);
    SetBrain(&FlyingSlig::Brain_12_Possessed_436040);
}

void FlyingSlig::ToMoving_435720()
{
    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eTension_4, this, 0, 0);
    SetBrain(&FlyingSlig::Brain_2_Moving_4356D0);
}

void FlyingSlig::ToPanicIdle_435B50()
{
    Say_436A50(SligSpeak::eHelp_10, 0);
    field_14C_timer = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + field_118_data.field_10_data.field_C_panic_delay;
    SetBrain(&FlyingSlig::Brain_8_PanicIdle_435AC0);
}

void FlyingSlig::ToChase_435E10()
{
    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    SetBrain(&FlyingSlig::Brain_4_ChasingEnemy_435BC0);
}

s16 FlyingSlig::IsPossessed_436A90()
{
    return BrainIs(&FlyingSlig::Brain_12_Possessed_436040) || BrainIs(&FlyingSlig::Brain_15_FlyingSligSpawn_4362C0) || BrainIs(&FlyingSlig::Brain_17_FromCrawlingSlig_4355E0) || BrainIs(&FlyingSlig::Brain_13_Possession_4360F0) || BrainIs(&FlyingSlig::Brain_14_DePossession_436180);
}

s16 FlyingSlig::CanChase_436850(BaseAliveGameObject* pObj)
{
    if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) || !gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0) || Event_Get_422C00(kEventResetting) || IsAbeEnteringDoor_43B030(pObj) || sActiveHero_5C1B68->field_CC_sprite_scale != field_CC_sprite_scale || !IsWallBetween_43A550(this, pObj))
    {
        return 0;
    }

    if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, field_D6_scale))
    {
        return 1;
    }

    if (vIsFacingMe_4254A0(pObj) && !IsInInvisibleZone_425710(pObj) && !pObj->field_114_flags.Get(Flags_114::e114_Bit8_bInvisible))
    {
        return 1;
    }

    return 0;
}

void FlyingSlig::Say_436A50(SligSpeak speak, s16 pitch)
{
    if (field_106_current_motion != eFlyingSligMotions::M_GameSpeak_8_4391D0)
    {
        field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1);
        field_17E_flags.Set(Flags_17E::eBit10_Speaking_flag2);
        field_17D_next_speak = speak;
        field_160_voice_pitch_min = pitch;
    }
}

s16 FlyingSlig::sub_4374A0(s16 a2)
{
    field_17E_flags.Clear(Flags_17E::eBit2_bLastLine);

    if (field_17E_flags.Get(Flags_17E::eBit4))
    {
        if (!field_17E_flags.Get(Flags_17E::eBit12_bNoNextLine) && (field_182_bound1 == TlvTypes::ContinuePoint_0 || !a2)) // todo: change to None when we can break abi
        {
            if (!field_17E_flags.Get(Flags_17E::eBit3))
            {
                field_190 = field_2B8_max_speed_up;
            }
            else
            {
                const FP v64 = FP_Abs(((field_18C * field_18C) / field_2B4_max_slow_down) * FP_FromDouble(0.5));
                if (v64 < FP_Abs(field_1C4 - field_194))
                {
                    field_190 = field_2B8_max_speed_up;
                }
            }
        }
        else
        {
            FP directedXMaxSpeedUp = field_2B8_max_speed_up + field_18C;
            if (directedXMaxSpeedUp > field_2A8_max_x_speed)
            {
                directedXMaxSpeedUp = field_2A8_max_x_speed;
            }

            FP left = {};
            FP right = {};
            if (directedXMaxSpeedUp <= FP_FromInteger(0))
            {
                left = field_1A4_rect.x;
                right = field_1A4_rect.w;
            }
            else
            {
                left = field_1A4_rect.w;
                right = field_1A4_rect.x;
            }

            const FP width = left - right;
            if (width > FP_FromInteger(0) && field_182_bound1 == TlvTypes::SligBoundLeft_32)
            {
                field_190 = field_2B8_max_speed_up;
            }
            else if (width < FP_FromInteger(0) && field_182_bound1 == TlvTypes::SligBoundRight_45)
            {
                field_190 = field_2B8_max_speed_up;
            }
            else
            {
                if (width == FP_FromInteger(0) && !field_17E_flags.Get(Flags_17E::eBit12_bNoNextLine))
                {
                    field_190 = field_2B8_max_speed_up;
                }
                else
                {
                    field_17E_flags.Set(Flags_17E::eBit2_bLastLine);

                    const FP v65 = FP_Abs((((field_18C * field_18C) / field_2B4_max_slow_down) * FP_FromDouble(0.5)));
                    const FP v27 = field_198_line_length - field_194;
                    if (v27 < field_2A8_max_x_speed && field_18C == FP_FromInteger(0))
                    {
                        return 1;
                    }

                    if (v65 < v27)
                    {
                        field_190 = field_2B8_max_speed_up;
                    }
                }
            }
        }
    }
    else
    {
        if (!field_17E_flags.Get(Flags_17E::eBit11_bNoPrevLine) && (field_180_bound2 == TlvTypes::ContinuePoint_0 || !a2)) // todo: change to None when we can break abi
        {
            if (!field_17E_flags.Get(Flags_17E::eBit3))
            {
                field_190 = -field_2B8_max_speed_up;
            }
            else
            {
                const FP v66 = FP_Abs(((field_18C * field_18C) / field_2B4_max_slow_down) * FP_FromDouble(0.5));
                if (v66 < FP_Abs(field_1C4 - field_194))
                {
                    field_190 = -field_2B8_max_speed_up;
                }
            }
        }
        else
        {
            FP directedXMaxSpeed = -field_2A8_max_x_speed;
            if (field_18C - field_2B8_max_speed_up >= directedXMaxSpeed)
            {
                directedXMaxSpeed = field_18C - field_2B8_max_speed_up;
            }

            FP left = {};
            FP right = {};
            if (directedXMaxSpeed <= FP_FromInteger(0))
            {
                left = field_1A4_rect.x;
                right = field_1A4_rect.w;
            }
            else
            {
                left = field_1A4_rect.w;
                right = field_1A4_rect.x;
            }

            const FP width = left - right;
            if (width > FP_FromInteger(0) && field_180_bound2 == TlvTypes::SligBoundLeft_32)
            {
                field_190 = -field_2B8_max_speed_up;
            }
            else if (width < FP_FromInteger(0) && field_180_bound2 == TlvTypes::SligBoundRight_45)
            {
                field_190 = -field_2B8_max_speed_up;
            }
            else
            {
                if (width == FP_FromInteger(0) && !field_17E_flags.Get(Flags_17E::eBit11_bNoPrevLine))
                {
                    field_190 = -field_2B8_max_speed_up;
                }
                else
                {
                    field_17E_flags.Set(Flags_17E::eBit2_bLastLine);
                    const FP v2 = FP_Abs(((field_18C * field_18C) / field_2B4_max_slow_down) * FP_FromDouble(0.5));
                    if (field_194 < field_2A8_max_x_speed && field_18C == FP_FromInteger(0))
                    {
                        return 1;
                    }

                    if (v2 < field_194)
                    {
                        field_190 = -field_2B8_max_speed_up;
                    }
                }
            }
        }
    }

    if (field_190 > FP_FromInteger(0))
    {
        field_184_xSpeed = field_1A4_rect.w - field_1A4_rect.x;
        field_188_ySpeed = field_1A4_rect.h - field_1A4_rect.y;
    }
    else if (field_190 < FP_FromInteger(0))
    {
        field_184_xSpeed = field_1A4_rect.x - field_1A4_rect.w;
        field_188_ySpeed = field_1A4_rect.y - field_1A4_rect.h;
    }

    return 0;
}

s16 CCSTD FlyingSlig::IsAbeEnteringDoor_43B030(BaseAliveGameObject* pThis)
{
    return Slig::IsAbeEnteringDoor_4BB990(pThis);
}

Bool32 CCSTD FlyingSlig::IsWallBetween_43A550(BaseAliveGameObject* pThis, BaseAliveGameObject* pObj)
{
    // TODO: Duplicated like IsAbeEnteringDoor_4BB990 ??
    PSX_RECT bRect = {};
    pObj->vGetBoundingRect_424FD0(&bRect, 1);

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    return sCollisions_DArray_5C1128->Raycast_417A60(
               pThis->field_B8_xpos,
               pThis->field_BC_ypos,
               pObj->field_B8_xpos,
               FP_FromInteger((bRect.y + bRect.h) / 2),
               &pLine,
               &hitX,
               &hitY,
               (pThis->field_D6_scale != 0 ? 1 : 16) | (pThis->field_D6_scale != 0 ? 6 : 96) | (pThis->field_D6_scale != 0 ? 8 : 0x80))
        != 1;
}

void FlyingSlig::ThrowGrenade_43A1E0()
{
    FP grenadeXVel = (FP_FromInteger(Math_RandomRange_496AB0(50, 64)) / FP_FromInteger(10) * field_CC_sprite_scale);
    const FP grenadeYVel = (FP_FromInteger(-6) * field_CC_sprite_scale);

    FP grenadeXPos = (FP_FromInteger(0) * field_CC_sprite_scale);
    const FP grenadeYPos = (FP_FromInteger(-5) * field_CC_sprite_scale);

    const FP xpos = (FP_FromInteger(0) * field_CC_sprite_scale);
    const FP ypos = (FP_FromInteger(-20) * field_CC_sprite_scale);

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        grenadeXPos = -grenadeXPos;
        grenadeXVel = -grenadeXVel;
    }

    auto pGrenade = ae_new<Grenade>();
    if (pGrenade)
    {
        pGrenade->ctor_447F70(grenadeXPos + field_B8_xpos, grenadeYPos + field_BC_ypos, 0, 1, 0, this);
    }

    pGrenade->field_CC_sprite_scale = field_CC_sprite_scale;
    pGrenade->field_D6_scale = field_D6_scale;
    pGrenade->VThrow_49E460(grenadeXVel, grenadeYVel);

    New_ShootingFire_Particle_426890(xpos + field_B8_xpos, ypos + field_BC_ypos, field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX), field_CC_sprite_scale);
    Slig_SoundEffect_4BFFE0(SligSfx::eThrowGrenade_8, this);
    Event_Broadcast_422BC0(kEventShooting, this);
    Event_Broadcast_422BC0(kEventLoudNoise, this);

    Dove::All_FlyAway_41FA60(0);

    s32 randomisedGrenadeDelay = field_118_data.field_10_data.field_18_grenade_delay + (Math_NextRandom() & 7);
    if (randomisedGrenadeDelay < 20)
    {
        randomisedGrenadeDelay = 20;
    }
    field_150_grenade_delay = randomisedGrenadeDelay + sGnFrame_5C1B84;

    if (IsPossessed_436A90() == 0 && Math_NextRandom() < 168u)
    {
        Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHereBoy_1, 0, field_15C_voice_pitch_min, this);
    }
}

void FlyingSlig::BlowUp_436510()
{
    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
    auto pGibs = ae_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_40FB40(GibType::Slig_1, field_B8_xpos, field_BC_ypos, field_C4_velx, field_C8_vely, field_CC_sprite_scale, 0);
    }

    auto pBlood = ae_new<Blood>();
    if (pBlood)
    {
        pBlood->ctor_40F0B0(field_B8_xpos, field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale), FP_FromInteger(0), FP_FromInteger(0), field_CC_sprite_scale, 20);
    }

    New_Smoke_Particles_426C70(field_B8_xpos, field_BC_ypos - (FP_FromInteger(30) * field_CC_sprite_scale), field_CC_sprite_scale, 3, 128u, 128u, 128u);
    SFX_Play_46FA90(SoundEffect::KillEffect_64, 128, field_CC_sprite_scale);
    SFX_Play_46FA90(SoundEffect::FallingItemHit_47, 90, field_CC_sprite_scale);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_18C = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_C4_velx = FP_FromInteger(0);
    field_190 = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);
    field_184_xSpeed = FP_FromInteger(0);
    field_10C_health = FP_FromInteger(0);
    SetBrain(&FlyingSlig::Brain_1_Death_4364E0);
    field_14C_timer = sGnFrame_5C1B84 + 40;
}

s16 FlyingSlig::sub_436730()
{
    if (CanChase_436850(sControlledCharacter_5C1B8C))
    {
        ToSpottedEnemy_435E70();
        return 1;
    }
    else if (CanHearAbe_4369C0() && !BrainIs(&FlyingSlig::Brain_3_GetAlerted_435750))
    {
        ToAlerted_4357E0();
        return 1;
    }
    else if (Event_Is_Event_In_Range_422C30(kEventAbeOhm, field_B8_xpos, field_BC_ypos, field_D6_scale))
    {
        ToPanicMoving_435A50();
        return 1;
    }
    else
    {
        if (sActiveHero_5C1B68 && sActiveHero_5C1B68->field_10C_health < FP_FromInteger(0) && FP_Abs(field_B8_xpos - sActiveHero_5C1B68->field_B8_xpos) < FP_FromInteger(640) && FP_Abs(field_BC_ypos - sActiveHero_5C1B68->field_BC_ypos) < FP_FromInteger(240))
        {
            ToAbeDead_436010();
            return 1;
        }
        return 0;
    }
}

s16 FlyingSlig::CanHearAbe_4369C0()
{
    return sActiveHero_5C1B68 == Event_Is_Event_In_Range_422C30(kEventSuspiciousNoise, field_B8_xpos, field_BC_ypos, field_D6_scale) || sActiveHero_5C1B68 == Event_Is_Event_In_Range_422C30(kEventSpeaking, field_B8_xpos, field_BC_ypos, field_D6_scale);
}

void FlyingSlig::ToSpottedEnemy_435E70()
{
    if (field_118_data.field_10_data.field_10_prechase_delay)
    {
        Say_436A50(SligSpeak ::eFreeze_8, 0);
        field_14C_timer = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + field_118_data.field_10_data.field_10_prechase_delay;
        SetBrain(&FlyingSlig::Brain_9_SpottedEnemy_435E40);
    }
    else
    {
        Slig_GameSpeak_SFX_4C04F0(SligSpeak::eFreeze_8, 0, field_15C_voice_pitch_min, this);
        ToChase_435E10();
    }
}

void FlyingSlig::ToAbeDead_436010()
{
    SetBrain(&FlyingSlig::Brain_11_AbeDead_435FD0);
    field_14C_timer = sGnFrame_5C1B84 + 70;
}

void FlyingSlig::ToAlerted_4357E0()
{
    Say_436A50(SligSpeak ::eWhat_9, 0);
    SetBrain(&FlyingSlig::Brain_3_GetAlerted_435750);
    field_14C_timer = sGnFrame_5C1B84 + field_118_data.field_10_data.field_14_alerted_listen_time;
}

void FlyingSlig::ToPanicMoving_435A50()
{
    MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eSoftChase_8, this, 0, 0);
    field_14C_timer = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + 12;
    SetBrain(&FlyingSlig::Brain_7_PanicMoving_435990);
}

s16 FlyingSlig::IsTurning_436AE0()
{
    return field_106_current_motion == eFlyingSligMotions::M_IdleToTurn_2_4388B0 || field_106_current_motion == eFlyingSligMotions::M_QuickTurn_15_4387F0 || field_106_current_motion == eFlyingSligMotions::M_DownMovementToTurn_4_438CC0 || field_106_current_motion == eFlyingSligMotions::M_UpMovementToTurn_6_439030;
}

u8** FlyingSlig::ResBlockForMotion_4350F0(s32 /*motion*/)
{
    if (field_15E_useless == 0)
    {
        return field_10_resources_array.ItemAt(0);
    }
    else
    {
        field_15E_useless = 0;
        return field_10_resources_array.ItemAt(0);
    }
}

void FlyingSlig::ToChantShake_436270()
{
    SFX_Play_46FA90(SoundEffect::PossessEffect_17, 0);
    VSetMotion_4081C0(eFlyingSligMotions::M_Possession_9_434290);
    SetBrain(&FlyingSlig::Brain_14_DePossession_436180);
    field_14C_timer = sGnFrame_5C1B84 + 45;
}

void FlyingSlig::ToPossesed_436130()
{
    Slig_GameSpeak_SFX_4C04F0(SligSpeak::eHelp_10, 0, field_15C_voice_pitch_min, this);
    VSetMotion_4081C0(eFlyingSligMotions::M_Possession_9_434290);
    SetBrain(&FlyingSlig::Brain_13_Possession_4360F0);
    field_14C_timer = sGnFrame_5C1B84 + 35;
}

void FlyingSlig::vUpdateAnimRes_4350A0()
{
    u8** ppRes = ResBlockForMotion_4350F0(field_106_current_motion);
    const AnimRecord& animRec = AnimRec(sFlyingSligFrameTables_552408[field_106_current_motion]);
    field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, ppRes);
}

void FlyingSlig::PatrolDelay_435860()
{
    if (BrainIs(&FlyingSlig::Brain_4_ChasingEnemy_435BC0))
    {
        field_14C_timer = (Math_NextRandom() & 7) + sGnFrame_5C1B84 + field_118_data.field_10_data.field_E_give_up_chase_delay;
        SetBrain(&FlyingSlig::Brain_5_Idle_435820);
        return;
    }

    field_14C_timer = field_118_data.field_10_data.field_6_patrol_pause_min + sGnFrame_5C1B84;
    if (field_118_data.field_10_data.field_6_patrol_pause_min <= field_118_data.field_10_data.field_8_patrol_pause_max)
    {
        SetBrain(&FlyingSlig::Brain_5_Idle_435820);
        return;
    }

    field_14C_timer += Math_NextRandom() % (field_118_data.field_10_data.field_6_patrol_pause_min - field_118_data.field_10_data.field_8_patrol_pause_max);
    SetBrain(&FlyingSlig::Brain_5_Idle_435820);
}

s16 FlyingSlig::CanThrowGrenade_43A490()
{
    return field_106_current_motion == eFlyingSligMotions::M_Idle_0_4385E0 || field_106_current_motion == eFlyingSligMotions::M_HorizontalMovement_1_4386A0 || field_106_current_motion == eFlyingSligMotions::M_DownMovement_3_438AA0 || field_106_current_motion == eFlyingSligMotions::M_UpMovement_5_438DD0 || field_106_current_motion == eFlyingSligMotions::M_IdleToHorizontalMovement_16_438730 || field_106_current_motion == eFlyingSligMotions::M_EndHorizontalMovement_10_4387D0 || field_106_current_motion == eFlyingSligMotions::M_BeginUpMovement_11_438E40 || field_106_current_motion == eFlyingSligMotions::M_UpToHorizontalMovement_13_438F60 || field_106_current_motion == eFlyingSligMotions::M_HorizontalToDownMovement_12_438B10 || field_106_current_motion == eFlyingSligMotions::M_DownToHorizontalMovement_14_438BF0 || field_106_current_motion == eFlyingSligMotions::M_EndDownMovement_18_438C90 || field_106_current_motion == eFlyingSligMotions::M_BeginDownMovement_17_438B80;
}

void FlyingSlig::ToLaunchingGrenade_435F50()
{
    if (!vIsFacingMe_4254A0(sControlledCharacter_5C1B8C) && !IsTurning_436AE0())
    {
        if (field_18C == FP_FromInteger(0))
        {
            field_17E_flags.Set(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
        }
        field_17E_flags.Toggle(Flags_17E::eBit4);
    }
    SetBrain(&FlyingSlig::Brain_10_LaunchingGrenade_435F10);
}

void FlyingSlig::HandlePlayerControls_439340()
{
    /* TODO OG issue - these local statics are never used ??
    if (!(byte_5BC5F4 & 1))
    {
        byte_5BC5F4 |= 1u;
        dword_5BC5F8 = Math_FixedPoint_Divide_496B70(0x800000, field_2B8);
    }
    if (!(byte_5BC5F4 & 2))
    {
        dword_5BC5F0 = 0x3F0000;
        byte_5BC5F4 |= 2u;
    }
    */

    field_17E_flags.Clear(Flags_17E::eBit5_Throw);
    field_17E_flags.Clear(Flags_17E::eBit6_bAlertedAndNotFacingAbe);
    field_17E_flags.Clear(Flags_17E::eBit7_DoAction);

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eThrowItem))
    {
        field_17E_flags.Set(Flags_17E::eBit5_Throw);
    }

    if (sInputObject_5BD4E0.isHeld(InputCommands::Enum::eDoAction))
    {
        field_17E_flags.Set(Flags_17E::eBit7_DoAction);
    }

    if (!field_17E_flags.Get(Flags_17E::eBit1_Speaking_flag1))
    {
        field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1);
        field_17E_flags.Set(Flags_17E::eBit10_Speaking_flag2);

        field_160_voice_pitch_min = 0;

        if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak1_555104))
        {
            field_17D_next_speak = SligSpeak::eHi_0;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak3_555100))
        {
            field_17D_next_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? SligSpeak::eGetHim_2 : SligSpeak::eFreeze_8;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak4_5550FC))
        {
            field_17D_next_speak = sInputObject_5BD4E0.Is_Demo_Playing_45F220() != 0 ? SligSpeak::eFreeze_8 : SligSpeak::eGetHim_2;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak2_5550F8))
        {
            field_17D_next_speak = SligSpeak::eHereBoy_1;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak6_555108))
        {
            field_17D_next_speak = SligSpeak::eBullshit_5;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak7_555114))
        {
            field_17D_next_speak = SligSpeak::eLookOut_6;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak5_55510C))
        {
            field_17D_next_speak = SligSpeak::eBullshit2_7;
        }
        else if (sInputObject_5BD4E0.isHeld(sInputKey_GameSpeak8_555110))
        {
            field_17D_next_speak = SligSpeak::eLaugh_3;
        }
        else
        {
            field_17E_flags.Clear(Flags_17E::eBit1_Speaking_flag1);
            field_17E_flags.Clear(Flags_17E::eBit10_Speaking_flag2);
        }
    }

    if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eUp))
    {
        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft))
        {
            field_184_xSpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            return;
        }
        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight))
        {
            field_184_xSpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            return;
        }
        field_184_xSpeed = FP_FromInteger(0);
        field_188_ySpeed = -field_2B8_max_speed_up;
    }
    else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eDown))
    {
        if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft))
        {
            field_184_xSpeed = (-field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
        }
        else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight))
        {
            field_184_xSpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
            field_188_ySpeed = (field_2B8_max_speed_up * FP_FromDouble(0.707));
        }
        else
        {
            field_184_xSpeed = FP_FromInteger(0);
            field_188_ySpeed = field_2B8_max_speed_up;
        }
    }
    else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eLeft))
    {
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = -field_2B8_max_speed_up;
    }
    else if (sInputObject_5BD4E0.isPressed(InputCommands::Enum::eRight))
    {
        field_188_ySpeed = FP_FromInteger(0);
        field_184_xSpeed = field_2B8_max_speed_up;
    }
}

s16 FlyingSlig::sub_437C70(PathLine* pLine)
{
    field_100_pCollisionLine = pLine;
    if (!field_100_pCollisionLine)
    {
        return 0;
    }

    field_1A4_rect.x = FP_FromInteger(field_100_pCollisionLine->field_0_rect.x);
    field_1A4_rect.y = FP_FromInteger(field_100_pCollisionLine->field_0_rect.y);
    field_1A4_rect.w = FP_FromInteger(field_100_pCollisionLine->field_0_rect.w);
    field_1A4_rect.h = FP_FromInteger(field_100_pCollisionLine->field_0_rect.h);


    field_1EC_pNextLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(field_100_pCollisionLine->field_C_next);
    field_1F0_pPrevLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(field_100_pCollisionLine->field_A_previous);

    field_198_line_length = FP_FromInteger(field_100_pCollisionLine->field_12_line_length);

    field_17E_flags.Set(Flags_17E::eBit11_bNoPrevLine, field_1F0_pPrevLine == nullptr);
    field_17E_flags.Set(Flags_17E::eBit12_bNoNextLine, field_1EC_pNextLine == nullptr);

    field_182_bound1 = FindLeftOrRightBound_43B0A0(field_1A4_rect.w, field_1A4_rect.h);
    field_180_bound2 = FindLeftOrRightBound_43B0A0(field_1A4_rect.x, field_1A4_rect.y);
    field_1BC = Math_Tan_496F70(field_1A4_rect.y - field_1A4_rect.h, field_1A4_rect.w - field_1A4_rect.x);

    field_1C0 = field_1BC + FP_FromInteger(128);

    if (field_1C0 >= FP_FromInteger(256))
    {
        field_1C0 -= FP_FromInteger(256);
    }
    return 1;
}

TlvTypes FlyingSlig::FindLeftOrRightBound_43B0A0(FP xOrY, FP wOrH)
{
    const FP kGridSize = ScaleToGridSize_4498B0(field_CC_sprite_scale);

    const FP left = xOrY - kGridSize;
    const FP top = wOrH - kGridSize;
    const FP right = xOrY + kGridSize;
    const FP bottom = wOrH + kGridSize;

    // TODO: Check left is really Abs'd.
    TlvTypes found_type = {};

    if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(FP_Abs(left)), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), TlvTypes::SligBoundLeft_32))
    {
        found_type = TlvTypes::SligBoundLeft_32;
    }
    else if (sPath_dword_BB47C0->TLV_Get_At_4DB4B0(FP_GetExponent(left), FP_GetExponent(top), FP_GetExponent(right), FP_GetExponent(bottom), TlvTypes::SligBoundRight_45))
    {
        found_type = TlvTypes::SligBoundRight_45;
    }
    return found_type;
}

void FlyingSlig::VPossessed_408F70()
{
    vPossessed_434FB0();
}

void FlyingSlig::vPossessed_434FB0()
{
    field_114_flags.Set(Flags_114::e114_Bit4_bPossesed);
    field_17E_flags.Set(Flags_17E::eBit1_Speaking_flag1);

    field_2A0_abe_level = gMap_5C3030.field_0_current_level;
    field_2A2_abe_path = gMap_5C3030.field_2_current_path;
    field_2A4_abe_camera = gMap_5C3030.field_4_current_camera;

    field_2A8_max_x_speed = FP_FromDouble(5.5) * field_CC_sprite_scale;
    field_2AC_up_vel = FP_FromDouble(-5.5) * field_CC_sprite_scale;
    field_2B0_down_vel = FP_FromDouble(5.5) * field_CC_sprite_scale;
    field_2B4_max_slow_down = FP_FromDouble(0.25) * field_CC_sprite_scale;
    field_2B8_max_speed_up = FP_FromDouble(0.7) * field_CC_sprite_scale;

    ToPossesed_436130();
}

s16 FlyingSlig::sub_436C60(PSX_RECT* pRect, s16 arg_4)
{
    if (!pRect)
    {
        return 0;
    }

    FP v1 = {};
    if (FP_FromInteger(std::abs(pRect->w - pRect->x)) < FP_FromInteger(1))
    {
        // Set to a huge value if near 0.
        v1 = FP_FromInteger(9999);
    }
    else
    {
        const s32 a1 = pRect->h - pRect->y;
        const s32 a2 = pRect->w - pRect->x;
        v1 = FP_Abs(FP_FromInteger((a1) / a2));
    }

    if (v1 < FP_FromInteger(4))
    {
        const FP k80Scaled = (FP_FromInteger(80) * field_CC_sprite_scale);

        const FP rLeft = sControlledCharacter_5C1B8C->field_B8_xpos - k80Scaled;
        const FP rRight = sControlledCharacter_5C1B8C->field_B8_xpos + k80Scaled;

        const FP r_x = FP_FromInteger(std::min(pRect->x, pRect->w));
        const FP r_w = FP_FromInteger(std::max(pRect->w, pRect->x));

        const FP r_y = FP_FromInteger(std::min(pRect->y, pRect->h));
        const FP r_h = FP_FromInteger(std::max(pRect->h, pRect->y) + 150);

        s32 bLeftInRect = 0;
        if (rLeft < r_x || rLeft > r_w || sControlledCharacter_5C1B8C->field_BC_ypos < r_y || sControlledCharacter_5C1B8C->field_BC_ypos > r_h)
        {
            bLeftInRect = 0;
        }
        else
        {
            bLeftInRect = 1;
        }

        s32 bRightInRect = 0;
        if (rRight < r_x || rRight > r_w || sControlledCharacter_5C1B8C->field_BC_ypos < r_y || sControlledCharacter_5C1B8C->field_BC_ypos > r_h)
        {
            bRightInRect = 0;
        }
        else
        {
            bRightInRect = 1;
        }

        if (!bLeftInRect && !bRightInRect)
        {
            return 0;
        }

        if (!arg_4 && (bRightInRect || bLeftInRect))
        {
            return 1;
        }

        const FP v2 = (field_1A4_rect.h - field_1A4_rect.y) / (field_1A4_rect.w - field_1A4_rect.x);
        const FP v3 = field_1A4_rect.y - (v2 * field_1A4_rect.x);

        FP yOff1 = {};
        if (bRightInRect)
        {
            yOff1 = v3 + (v2 * rRight);
            if ((sControlledCharacter_5C1B8C->field_BC_ypos - (FP_FromInteger(60) * field_CC_sprite_scale) - yOff1) > (FP_FromInteger(35) * field_CC_sprite_scale))
            {
                FP hitX = {};
                FP hitY = {};
                PathLine* pLine = nullptr;

                if (sCollisions_DArray_5C1128->Raycast_417A60(
                        sControlledCharacter_5C1B8C->field_B8_xpos,
                        yOff1,
                        sControlledCharacter_5C1B8C->field_B8_xpos,
                        yOff1 + (FP_FromInteger(35) * field_CC_sprite_scale),
                        &pLine,
                        &hitX,
                        &hitY,
                        field_D6_scale != 0 ? 1 : 0x10)
                    == 1)
                {
                    bRightInRect = 0;
                }
            }
        }

        FP yOff2 = {};
        if (bLeftInRect)
        {
            yOff2 = (v2 * rLeft) + v3;

            FP hitX = {};
            FP hitY = {};
            PathLine* pLine = nullptr;

            if ((sControlledCharacter_5C1B8C->field_BC_ypos - (FP_FromInteger(60) * field_CC_sprite_scale) - yOff2) > (FP_FromInteger(35) * field_CC_sprite_scale))
            {
                if (sCollisions_DArray_5C1128->Raycast_417A60(
                        sControlledCharacter_5C1B8C->field_B8_xpos,
                        yOff2,
                        sControlledCharacter_5C1B8C->field_B8_xpos,
                        yOff2 + (FP_FromInteger(35) * field_CC_sprite_scale),
                        &pLine,
                        &hitX,
                        &hitY,
                        field_D6_scale != 0 ? 1 : 16)
                    == 1)
                {
                    bLeftInRect = 0;
                }
            }
        }

        if (bRightInRect || bLeftInRect)
        {
            FP sqrt1 = {};
            FP sqrt2 = {};
            if (bRightInRect)
            {
                sqrt1 = FP_FromInteger(Math_SquareRoot_Int_496E70(
                    FP_GetExponent(yOff1 - field_1A4_rect.y) * (FP_GetExponent(yOff1 - field_1A4_rect.y)) + FP_GetExponent(rRight - field_1A4_rect.x) * (FP_GetExponent(rRight - field_1A4_rect.x))));
            }

            if (bLeftInRect)
            {
                const s32 sqrt2_int = Math_SquareRoot_Int_496E70(
                    FP_GetExponent(yOff2 - field_1A4_rect.y) * (FP_GetExponent(yOff2 - field_1A4_rect.y)) + FP_GetExponent(rLeft - field_1A4_rect.x) * (FP_GetExponent(rLeft - field_1A4_rect.x)));

                sqrt2 = FP_FromInteger(sqrt2_int);
            }

            if (bRightInRect && bLeftInRect)
            {
                if (FP_Abs(sqrt2 - field_194) >= FP_Abs(sqrt1 - field_194))
                {
                    field_1C4 = sqrt1;
                    return 1;
                }
                else
                {
                    field_1C4 = sqrt2;
                    return 1;
                }
            }
            else if (bRightInRect)
            {
                field_1C4 = sqrt1;
                return 1;
            }
            else if (bLeftInRect)
            {
                field_1C4 = sqrt2;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        const FP k40Scaled = (FP_FromInteger(40) * field_CC_sprite_scale);
        const FP yTop = sControlledCharacter_5C1B8C->field_BC_ypos - k40Scaled;
        if (pRect->y >= pRect->h)
        {
            if (yTop < FP_FromInteger(pRect->h))
            {
                return 0;
            }

            if (yTop > FP_FromInteger(pRect->y))
            {
                return 0;
            }
        }
        else
        {
            if (yTop < FP_FromInteger(pRect->y))
            {
                return 0;
            }

            if (yTop > FP_FromInteger(pRect->h))
            {
                return 0;
            }
        }

        if (FP_Abs(sControlledCharacter_5C1B8C->field_B8_xpos - FP_FromInteger(pRect->x)) > (FP_FromInteger(120) * field_CC_sprite_scale))
        {
            return 0;
        }

        if (arg_4)
        {
            field_1C4 = FP_Abs(yTop - field_1A4_rect.y);
        }

        return 1;
    }
}

Bool32 FlyingSlig::sub_436B20()
{
    PathLine* pLastNextOrPrevLine = nullptr;

    FP lastNextSegmentLength = FP_FromInteger(9999);
    FP totalNextSegmentLength = {};

    PathLine* pNextLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(field_100_pCollisionLine->field_C_next);
    if (pNextLine)
    {
        while (pNextLine != field_100_pCollisionLine)
        {
            if (sub_436C60(&pNextLine->field_0_rect, 0))
            {
                field_17E_flags.Set(Flags_17E::eBit4);
                pLastNextOrPrevLine = pNextLine;
                if (pNextLine)
                {
                    lastNextSegmentLength = totalNextSegmentLength;
                }
                break;
            }
            else
            {
                totalNextSegmentLength += FP_FromInteger(pNextLine->field_12_line_length);
                pNextLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(pNextLine->field_C_next);
                if (!pNextLine)
                {
                    break;
                }
            }
        }
    }

    FP totalPrevSegmentLength = {};
    PathLine* pPrevLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(field_100_pCollisionLine->field_A_previous);
    if (pPrevLine)
    {
        while (pPrevLine != field_100_pCollisionLine)
        {
            if (sub_436C60(&pPrevLine->field_0_rect, 0))
            {
                if (totalPrevSegmentLength < lastNextSegmentLength)
                {
                    field_17E_flags.Clear(Flags_17E::eBit4);
                    pLastNextOrPrevLine = pPrevLine;
                }
                break;
            }
            else
            {
                totalPrevSegmentLength += FP_FromInteger(pPrevLine->field_12_line_length);
                pPrevLine = sCollisions_DArray_5C1128->Get_Line_At_Idx_418070(pPrevLine->field_A_previous);
                if (!pPrevLine)
                {
                    break;
                }
            }
        }
    }

    return pLastNextOrPrevLine != nullptr;
}

void FlyingSlig::sub_4373B0()
{
    const FP calc = Math_Tan_496F70(field_BC_ypos - sControlledCharacter_5C1B8C->field_BC_ypos, sControlledCharacter_5C1B8C->field_B8_xpos - field_B8_xpos);
    FP value1 = FP_Abs(field_1BC - calc);
    if (value1 > FP_FromInteger(128))
    {
        value1 = FP_FromInteger(256) - value1;
    }

    FP value2 = FP_Abs(field_1C0 - calc);
    if (value2 > FP_FromInteger(128))
    {
        value2 = FP_FromInteger(256) - value2;
    }

    if (value1 >= value2)
    {
        field_17E_flags.Clear(Flags_17E::eBit4);
    }
    else
    {
        field_17E_flags.Set(Flags_17E::eBit4);
    }
}

void FlyingSlig::VSetMotion_4081C0(s16 state)
{
    vSetMotion_43B1B0(state);
}

void FlyingSlig::vSetMotion_43B1B0(s16 newMotion)
{
    field_106_current_motion = newMotion;
    vUpdateAnimRes_4350A0();
}

s16 FlyingSlig::CollisionUp_43A640(FP velY)
{
    const FP y2 = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(20)) + velY;

    FP xOff1 = {};
    FP xOff2 = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff1 = (field_CC_sprite_scale * FP_FromInteger(17));
        xOff2 = ((field_CC_sprite_scale * FP_FromInteger(17)) / FP_FromInteger(3));
    }
    else
    {
        xOff2 = (field_CC_sprite_scale * FP_FromInteger(17));
        xOff1 = ((field_CC_sprite_scale * FP_FromInteger(17)) / FP_FromInteger(3));
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos - xOff1,
        field_BC_ypos,
        field_B8_xpos - xOff1,
        y2,
        &pLine,
        &hitX,
        &hitY,
        (field_D6_scale != 0 ? 0x20000 : 0x40000) | (field_D6_scale != 0 ? 1 : 0x10) | (field_D6_scale != 0 ? 8 : 0x80));

    if (!bCollision)
    {
        bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
            xOff2 + field_B8_xpos,
            field_BC_ypos,
            xOff2 + field_B8_xpos,
            y2,
            &pLine,
            &hitX,
            &hitY,
            (field_D6_scale != 0 ? 0x20000 : 0x40000) | (field_D6_scale != 0 ? 1 : 16) | (field_D6_scale != 0 ? 8 : 128));
    }

    if (bCollision)
    {
        if (FP_Abs(field_184_xSpeed) < FP_FromDouble(0.1) && field_106_current_motion != eFlyingSligMotions::M_UpKnockback_20_439110)
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_UpKnockback_20_439110);
        }

        if (static_cast<s32>(sGnFrame_5C1B84) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX_4C04F0(sGnFrame_5C1B84 & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange_496AB0(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame_5C1B84 + 10;
            auto pBurst = ae_new<ParticleBurst>();
            if (pBurst)
            {
                pBurst->ctor_41CF50(
                    field_B8_xpos,
                    hitY + (FP_FromInteger(7) * field_CC_sprite_scale),
                    5u,
                    field_CC_sprite_scale,
                    BurstType::eSmallPurpleSparks_6,
                    9);
            }
        }

        Slig_SoundEffect_4BFFE0(sGnFrame_5C1B84 & 1 ? SligSfx::eCollideWithWall1_12 : SligSfx::eCollideWithWall2_13, this);

        field_BC_ypos += velY + hitY - y2;
        return 1;
    }
    return 0;
}

s16 FlyingSlig::CollisionDown_43A9E0(FP velY)
{
    const FP y2 = (field_CC_sprite_scale * FP_FromInteger(10)) + field_BC_ypos + velY;

    FP xOff1 = {};
    FP xOff2 = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        xOff1 = (field_CC_sprite_scale * FP_FromInteger(17));
        xOff2 = ((field_CC_sprite_scale * FP_FromInteger(17)) / FP_FromInteger(3));
    }
    else
    {
        xOff1 = ((field_CC_sprite_scale * FP_FromInteger(17)) / FP_FromInteger(3));
        xOff2 = (field_CC_sprite_scale * FP_FromInteger(17));
    }

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos - xOff1,
        field_BC_ypos,
        field_B8_xpos - xOff1,
        y2,
        &pLine,
        &hitX,
        &hitY,
        (field_D6_scale != 0 ? 0x20000 : 0x40000) | (field_D6_scale != 0 ? 1 : 0x10));

    if (!bCollision)
    {
        bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
            xOff2 + field_B8_xpos,
            field_BC_ypos,
            xOff2 + field_B8_xpos,
            y2,
            &pLine,
            &hitX,
            &hitY,
            (field_D6_scale != 0 ? 0x20000 : 0x40000) | (field_D6_scale != 0 ? 1 : 0x10));
    }

    if (bCollision)
    {
        if (FP_Abs(field_184_xSpeed) < FP_FromDouble(0.1) && field_106_current_motion != eFlyingSligMotions::M_DownKnockback_19_4390D0)
        {
            VSetMotion_4081C0(eFlyingSligMotions::M_DownKnockback_19_4390D0);
        }

        if (static_cast<s32>(sGnFrame_5C1B84) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX_4C04F0(sGnFrame_5C1B84 & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange_496AB0(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame_5C1B84 + 10;
        }

        field_BC_ypos += velY + hitY - y2;
        return 1;
    }
    return 0;
}

s16 FlyingSlig::CollisionLeftRight_43AC80(FP velX)
{
    FP newX = {};
    if (velX <= FP_FromInteger(0))
    {
        newX = field_B8_xpos - (field_CC_sprite_scale * FP_FromInteger(17));
    }
    else
    {
        newX = (field_CC_sprite_scale * FP_FromInteger(17)) + field_B8_xpos;
    }

    const FP xOff = velX + newX;
    const FP yOff2 = field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(20));
    const FP yOff1 = (field_CC_sprite_scale * FP_FromInteger(20));

    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    auto bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos - yOff1,
        xOff,
        yOff2,
        &pLine,
        &hitX,
        &hitY,
        (field_D6_scale != 0 ? 0x20000 : 0x40000) | (field_D6_scale != 0 ? 6 : 96));

    FP sparkX = {};
    if (bCollision)
    {
        FP k25Directed = {};
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
        {
            k25Directed = FP_FromInteger(-25);
        }
        else
        {
            k25Directed = FP_FromInteger(25);
        }
        sparkX = (k25Directed * field_CC_sprite_scale) + field_B8_xpos;
        Slig_SoundEffect_4BFFE0(sGnFrame_5C1B84 & 1 ? SligSfx::eCollideWithWall1_12 : SligSfx::eCollideWithWall2_13, this);
    }
    else
    {
        bCollision = sCollisions_DArray_5C1128->Raycast_417A60(
            field_B8_xpos,
            (field_CC_sprite_scale * FP_FromInteger(10)) + field_BC_ypos,
            xOff,
            (field_CC_sprite_scale * FP_FromInteger(10)) + field_BC_ypos,
            &pLine,
            &hitX,
            &hitY,
            (field_D6_scale != 0 ? 0x20000 : 0x40000) | (field_D6_scale != 0 ? 6 : 96));

        if (bCollision)
        {
            FP k25Directed = {};
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
            {
                k25Directed = FP_FromInteger(-25);
            }
            else
            {
                k25Directed = FP_FromInteger(25);
            }
            sparkX = (k25Directed * field_CC_sprite_scale) + field_B8_xpos;
        }
    }

    if (bCollision)
    {
        if (static_cast<s32>(sGnFrame_5C1B84) > field_154_collision_reaction_timer)
        {
            Slig_GameSpeak_SFX_4C04F0(sGnFrame_5C1B84 & 1 ? SligSpeak::eOuch2_14 : SligSpeak::eOuch1_13, 127, Math_RandomRange_496AB0(256, 512), this);
            field_154_collision_reaction_timer = (Math_NextRandom() & 3) + sGnFrame_5C1B84 + 10;
            auto pBurst = ae_new<ParticleBurst>();
            if (pBurst)
            {
                pBurst->ctor_41CF50(sparkX, hitY + (FP_FromInteger(16) * field_CC_sprite_scale), 5u, field_CC_sprite_scale, BurstType::eSmallPurpleSparks_6, 9);
            }
        }
        field_B8_xpos += velX + hitX - xOff;
        return 1;
    }

    return 0;
}

void FlyingSlig::PullLever_436450()
{
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);

    field_184_xSpeed = FP_FromInteger(0);
    field_188_ySpeed = FP_FromInteger(0);

    if ((FP_Abs(field_B8_xpos - field_1C8_lever_pull_range_xpos) < FP_FromInteger(1)) &&
        (FP_Abs(field_BC_ypos - field_1CC_lever_pull_range_ypos) < FP_FromInteger(1)))
    {
        VSetMotion_4081C0(eFlyingSligMotions::M_LeverPull_7_439150);
    }
    SetBrain(&FlyingSlig::Brain_15_FlyingSligSpawn_4362C0);
}

s16 FlyingSlig::TryPullLever_439DB0()
{
    if (BrainIs(&FlyingSlig::Brain_15_FlyingSligSpawn_4362C0))
    {
        return 0;
    }

    FP kGridSizeDirected = {};
    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        kGridSizeDirected = -ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }
    else
    {
        kGridSizeDirected = ScaleToGridSize_4498B0(field_CC_sprite_scale);
    }

    const FP k15Scaled = FP_FromInteger(15) * field_CC_sprite_scale;
    const FP k2ScaledDirected = (kGridSizeDirected * FP_FromInteger(2));

    // TODO: These can be replaced with std::min/std::max.
    FP rect_w_fp = {};
    if (field_B8_xpos + kGridSizeDirected <= field_B8_xpos + k2ScaledDirected)
    {
        rect_w_fp = field_B8_xpos + kGridSizeDirected;
    }
    else
    {
        rect_w_fp = field_B8_xpos + k2ScaledDirected;
    }

    FP rect_x_fp = {};
    if (field_B8_xpos + k2ScaledDirected <= field_B8_xpos + kGridSizeDirected)
    {
        rect_x_fp = field_B8_xpos + kGridSizeDirected;
    }
    else
    {
        rect_x_fp = (k2ScaledDirected) + field_B8_xpos;
    }

    FP rect_h_fp = {};
    if (field_BC_ypos - k15Scaled <= k15Scaled + field_BC_ypos)
    {
        rect_h_fp = field_BC_ypos - k15Scaled;
    }
    else
    {
        rect_h_fp = k15Scaled + field_BC_ypos;
    }

    FP rect_y_fp = {};
    if (k15Scaled + field_BC_ypos <= field_BC_ypos - k15Scaled)
    {
        rect_y_fp = field_BC_ypos - k15Scaled;
    }
    else
    {
        rect_y_fp = k15Scaled + field_BC_ypos;
    }

    // TODO: Can be replaced with FP_Rect and PSX_RECT.
    const s16 rect_w = FP_GetExponent(rect_w_fp);
    const s16 rect_x = FP_GetExponent(rect_x_fp);
    const s16 rect_h = FP_GetExponent(rect_h_fp);
    const s16 rect_y = FP_GetExponent(rect_y_fp);

    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->Type() == AETypes::eLever_139)
        {
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            PSX_RECT bObjRect = {};
            pAliveObj->vGetBoundingRect_424FD0(&bObjRect, 1);
            if (rect_w <= bObjRect.w && rect_x >= bObjRect.x && rect_y >= bObjRect.y && rect_h <= bObjRect.h)
            {
                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    if (field_B8_xpos < pAliveObj->field_B8_xpos)
                    {
                        return FALSE;
                    }
                    field_1C8_lever_pull_range_xpos = (FP_FromInteger(45) * field_CC_sprite_scale) + pAliveObj->field_B8_xpos;
                }
                else
                {
                    if (field_B8_xpos > pAliveObj->field_B8_xpos)
                    {
                        return FALSE;
                    }
                    field_1C8_lever_pull_range_xpos = pAliveObj->field_B8_xpos - (FP_FromInteger(47) * field_CC_sprite_scale);
                }

                field_1CC_lever_pull_range_ypos = pAliveObj->field_BC_ypos - (FP_FromInteger(23) * field_CC_sprite_scale);
                field_158_obj_id = pAliveObj->field_8_object_id;
                PullLever_436450();
                return TRUE;
            }
        }
    }
    return FALSE;
}

void FlyingSlig::sub_437AC0(FP arg1, FP_Point* pPoint)
{
    FP unknown = arg1;
    if (arg1 == FP_FromInteger(0))
    {
        pPoint->field_0_x = field_B8_xpos;
        pPoint->field_4_y = field_BC_ypos;
        return;
    }

    if (arg1 > FP_FromInteger(0))
    {
        if (field_194 + arg1 > field_198_line_length)
        {
            if (!field_17E_flags.Get(Flags_17E::eBit2_bLastLine) && field_1EC_pNextLine)
            {
                unknown = field_194 + arg1 - field_198_line_length;
                if (sub_437C70(field_1EC_pNextLine))
                {
                    field_194 = FP_FromInteger(0);
                }
                else
                {
                    unknown = FP_FromInteger(0);
                    field_194 = field_198_line_length;
                }
            }
            else
            {
                unknown = FP_FromInteger(0);
                field_194 = field_198_line_length;
            }
        }
    }
    else
    {
        if (field_194 + arg1 < FP_FromInteger(0))
        {
            if (!field_17E_flags.Get(Flags_17E::eBit2_bLastLine) && field_1F0_pPrevLine)
            {
                unknown = field_194 + arg1;
                if (sub_437C70(field_1F0_pPrevLine))
                {
                    field_194 = field_198_line_length;
                }
                else
                {
                    unknown = FP_FromInteger(0);
                    field_194 = FP_FromInteger(0);
                }
            }
            else
            {
                unknown = FP_FromInteger(0);
                field_194 = FP_FromInteger(0);
            }
        }
    }

    field_194 += unknown;
    pPoint->field_0_x = (field_194 * ((field_1A4_rect.w - field_1A4_rect.x) / field_198_line_length)) + field_1A4_rect.x;
    pPoint->field_4_y = (field_194 * ((field_1A4_rect.h - field_1A4_rect.y) / field_198_line_length)) + field_1A4_rect.y;
}
