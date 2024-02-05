#pragma once

#include "nlohmann/json.hpp"
#include "CommonTypesSerialization.hpp"
#include "../../AliveLibAO/SaveGame.hpp"

// clang-format off

namespace AO {

NLOHMANN_JSON_SERIALIZE_ENUM(eAbeMotions, {
    {eAbeMotions::None_m1, "none"},
    {eAbeMotions::Motion_0_Idle, "idle"},
    {eAbeMotions::Motion_1_WalkLoop, "walk_loop"},
    {eAbeMotions::Motion_2_StandingTurn, "standing_turn"},
    {eAbeMotions::Motion_3_Fall, "fall"},
    {eAbeMotions::Motion_4_WalkToIdle, "walk_to_idle"},
    {eAbeMotions::Motion_5_MidWalkToIdle, "mid_walk_to_idle"},
    {eAbeMotions::Motion_6_WalkBegin, "walk_begin"},
    {eAbeMotions::Motion_7_Speak, "motion_7_speak"},
    {eAbeMotions::Motion_8_Speak, "motion_8_speak"},
    {eAbeMotions::Motion_9_Speak, "motion_9_speak"},
    {eAbeMotions::Motion_10_Speak, "motion_10_speak"},
    {eAbeMotions::Motion_11_Speak, "motion_11_speak"},
    {eAbeMotions::Motion_12_Speak, "motion_12_speak"},
    {eAbeMotions::Motion_13_Speak, "motion_13_speak"},
    {eAbeMotions::Motion_14_Speak, "motion_14_speak"},
    {eAbeMotions::Motion_15_Null, "motion_15_null"},
    {eAbeMotions::Motion_16_HoistBegin, "hoist_begin"},
    {eAbeMotions::Motion_17_HoistIdle, "hoist_idle"},
    {eAbeMotions::Motion_18_HoistLand, "hoist_land"},
    {eAbeMotions::Motion_19_CrouchIdle, "crouch_idle"},
    {eAbeMotions::Motion_20_CrouchToStand, "crouch_to_stand"},
    {eAbeMotions::Motion_21_StandToCrouch, "stand_to_crouch"},
    {eAbeMotions::Motion_22_CrouchSpeak, "motion_22_crouch_speak"},
    {eAbeMotions::Motion_23_CrouchSpeak, "motion_23_crouch_speak"},
    {eAbeMotions::Motion_24_RollBegin, "roll_begin"},
    {eAbeMotions::Motion_25_RollLoop, "roll_loop"},
    {eAbeMotions::Motion_26_RollEnd, "roll_end"},
    {eAbeMotions::Motion_27_RunSlideStop, "run_slide_stop"},
    {eAbeMotions::Motion_28_RunTurn, "run_turn"},
    {eAbeMotions::Motion_29_HopBegin, "hop_begin"},
    {eAbeMotions::Motion_30_HopMid, "hop_mid"},
    {eAbeMotions::Motion_31_HopLand, "hop_land"},
    {eAbeMotions::Motion_32_RunJumpBegin, "run_jump_begin"},
    {eAbeMotions::Motion_33_RunJumpMid, "run_jump_mid"},
    {eAbeMotions::Motion_34_RunJumpLand, "run_jump_land"},
    {eAbeMotions::Motion_35_RunLoop, "run_loop"},
    {eAbeMotions::Motion_36_DunnoBegin, "dunno_begin"},
    {eAbeMotions::Motion_37_DunnoMid, "dunno_mid"},
    {eAbeMotions::Motion_38_DunnoEnd, "dunno_end"},
    {eAbeMotions::Motion_39_CrouchTurn, "crouch_turn"},
    {eAbeMotions::Motion_40_RunToRoll, "run_to_roll"},
    {eAbeMotions::Motion_41_StandingToRun, "standing_to_run"},
    {eAbeMotions::Motion_42_SneakLoop, "sneak_loop"},
    {eAbeMotions::Motion_43_WalkToSneak, "walk_to_sneak"},
    {eAbeMotions::Motion_44_SneakToWalk, "sneak_to_walk"},
    {eAbeMotions::Motion_45_MidWalkToSneak, "mid_walk_to_sneak"},
    {eAbeMotions::Motion_46_MidSneakToWalk, "mid_sneak_to_walk"},
    {eAbeMotions::Motion_47_SneakBegin, "sneak_begin"},
    {eAbeMotions::Motion_48_SneakToIdle, "sneak_to_idle"},
    {eAbeMotions::Motion_49_MidSneakToIdle, "mid_sneak_to_idle"},
    {eAbeMotions::Motion_50_WalkToRun, "walk_to_run"},
    {eAbeMotions::Motion_51_MidWalkToRun, "mid_walk_to_run"},
    {eAbeMotions::Motion_52_RunToWalk, "run_to_walk"},
    {eAbeMotions::Motion_53_MidRunToWalk, "mid_run_to_walk"},
    {eAbeMotions::Motion_54_RunTurnToRun, "run_turn_to_run"},
    {eAbeMotions::Motion_55_RunTurnToWalk, "run_turn_to_walk"},
    {eAbeMotions::Motion_56_RunJumpLandRun, "run_jump_land_run"},
    {eAbeMotions::Motion_57_RunJumpLand_Walk, "run_jump_land_walk"},
    {eAbeMotions::Motion_58_ToSpeak, "to_speak"},
    {eAbeMotions::Motion_59_DeathDropFall, "death_drop_fall"},
    {eAbeMotions::Motion_60_Dead, "dead"},
    {eAbeMotions::Motion_61_Respawn, "respawn"},
    {eAbeMotions::Motion_62_LoadedSaveSpawn, "loaded_save_spawn"},
    {eAbeMotions::Motion_63_TurnToRun, "turn_to_run"},
    {eAbeMotions::Motion_64_LedgeAscend, "ledge_ascend"},
    {eAbeMotions::Motion_65_LedgeDescend, "ledge_descend"},
    {eAbeMotions::Motion_66_LedgeHang, "ledge_hang"},
    {eAbeMotions::Motion_67_ToOffScreenHoist, "to_off_screen_hoist"},
    {eAbeMotions::Motion_68_LedgeHangWobble, "ledge_hang_wobble"},
    {eAbeMotions::Motion_69_RingRopePullHang, "ring_rope_pull_hang"},
    {eAbeMotions::Motion_70_Knockback, "knockback"},
    {eAbeMotions::Motion_71_KnockbackGetUp, "knockback_get_up"},
    {eAbeMotions::Motion_72_PushWall, "push_wall"},
    {eAbeMotions::Motion_73_RollingKnockback, "rolling_knockback"},
    {eAbeMotions::Motion_74_JumpIntoWell, "jump_into_well"},
    {eAbeMotions::Motion_75_ToInsideOfAWellLocal, "to_inside_of_a_well_local"},
    {eAbeMotions::Motion_76_ToWellShotOut, "motion_76_to_well_shot_out"},
    {eAbeMotions::Motion_77_WellBegin, "well_begin"},
    {eAbeMotions::Motion_78_InsideWellLocal, "inside_well_local"},
    {eAbeMotions::Motion_79_WellShotOut, "well_shot_out"},
    {eAbeMotions::Motion_80_430EF0, "motion_80"},
    {eAbeMotions::Motion_81_InsideWellExpress, "inside_well_express"},
    {eAbeMotions::Motion_82_WellExpressShotOut, "well_express_shot_out"},
    {eAbeMotions::Motion_83_430F00, "motion_83"},
    {eAbeMotions::Motion_84_ToInsideWellLocal, "to_inside_well_local"},
    {eAbeMotions::Motion_85_ToWellShotOut, "motion_85_to_well_shot_out"},
    {eAbeMotions::Motion_86_FallLandDie, "fall_land_die"},
    {eAbeMotions::Motion_87_ToFall, "to_fall"},
    {eAbeMotions::Motion_88_HandstoneBegin, "handstone_begin"},
    {eAbeMotions::Motion_89_HandstoneEnd, "handstone_end"},
    {eAbeMotions::Motion_90_GrenadeMachineUse, "grenade_machine_use"},
    {eAbeMotions::Motion_91_FallingFromGrab, "falling_from_grab"},
    {eAbeMotions::Motion_92_ForceDownFromHoist, "force_down_from_hoist"},
    {eAbeMotions::Motion_93_WalkOffEdge, "walk_off_edge"},
    {eAbeMotions::Motion_94_RunOffEdge, "run_off_edge"},
    {eAbeMotions::Motion_95_SneakOffEdge, "sneak_off_edge"},
    {eAbeMotions::Motion_96_HopToFall, "hop_to_fall"},
    {eAbeMotions::Motion_97_RunJumpToFall, "run_jump_to_fall"},
    {eAbeMotions::Motion_98_LandSoft, "land_soft"},
    {eAbeMotions::Motion_99_HoistBeginLong, "hoist_begin_long"},
    {eAbeMotions::Motion_100_RollOffLedge, "roll_off_ledge"},
    {eAbeMotions::Motion_101_LeverUse, "lever_use"},
    {eAbeMotions::Motion_102_ElumWalkLoop, "elum_walk_loop"},
    {eAbeMotions::Motion_103_ElumIdle, "elum_idle"},
    {eAbeMotions::Motion_104_ElumRunSlideStop, "elum_run_slide_stop"},
    {eAbeMotions::Motion_105_ElumRunTurn, "elum_run_turn"},
    {eAbeMotions::Motion_106_Null, "motion_106_null"},
    {eAbeMotions::Motion_107_ElumHopBegin, "elum_hop_begin"},
    {eAbeMotions::Motion_108_ElumHopMid, "elum_hop_mid"},
    {eAbeMotions::Motion_109_ElumHopLand, "elum_hop_land"},
    {eAbeMotions::Motion_110_ElumRunJumpBegin, "elum_run_jump_begin"},
    {eAbeMotions::Motion_111_ElumRunJumpMid, "elum_run_jump_mid"},
    {eAbeMotions::Motion_112_ElumRunJumpLand, "elum_run_jump_land"},
    {eAbeMotions::Motion_113_ElumTurn, "elum_turn"},
    {eAbeMotions::Motion_114_ElumRunLoop, "elum_run_loop"},
    {eAbeMotions::Motion_115_ElumSpeak, "elum_speak"},
    {eAbeMotions::Motion_116_Null, "motion_116_null"},
    {eAbeMotions::Motion_117_ElumWalkBegin, "elum_walk_begin"},
    {eAbeMotions::Motion_118_ElumRunBegin, "elum_run_begin"},
    {eAbeMotions::Motion_119_Null, "motion_119_null"},
    {eAbeMotions::Motion_120_ElumRunToWalk, "elum_run_to_walk"},
    {eAbeMotions::Motion_121_ElumMidRunToWalk, "elum_mid_run_to_walk"},
    {eAbeMotions::Motion_122_ElumRunTurnToRun, "elum_run_turn_to_run"},
    {eAbeMotions::Motion_123_ElumRunTurnToWalk, "elum_run_turn_to_walk"},
    {eAbeMotions::Motion_124_ElumWalkEnd, "elum_walk_end"},
    {eAbeMotions::Motion_125_ElumMidWalkEnd, "elum_mid_walk_end"},
    {eAbeMotions::Motion_126_ElumBeesStruggling, "elum_bees_struggling"},
    {eAbeMotions::Motion_127_SlapBomb, "slap_bomb"},
    {eAbeMotions::Motion_128_KnockForward, "knock_forward"},
    {eAbeMotions::Motion_129_RollingKnockForward, "rolling_knock_forward"},
    {eAbeMotions::Motion_130_KnockForwardGetUp, "knock_forward_up"},
    {eAbeMotions::Motion_131_LiftUseUp, "lift_use_up"},
    {eAbeMotions::Motion_132_LiftUseDown, "lift_use_down"},
    {eAbeMotions::Motion_133_LiftGrabBegin, "lift_grab_begin"},
    {eAbeMotions::Motion_134_LiftGrabEnd, "lift_grab_end"},
    {eAbeMotions::Motion_135_LiftGrabIdle, "lift_grab_idle"},
    {eAbeMotions::Motion_136_ElumMountEnd, "elum_mount_end"},
    {eAbeMotions::Motion_137_ElumUnmountBegin, "elum_unmount_begin"},
    {eAbeMotions::Motion_138_ElumUnmountEnd, "elum_unmount_end"},
    {eAbeMotions::Motion_139_ElumMountBegin, "elum_mount_begin"},
    {eAbeMotions::Motion_140_BeesStruggling, "bees_struggling"},
    {eAbeMotions::Motion_141_BeesStrugglingOnLift, "bees_struggling_on_lift"},
    {eAbeMotions::Motion_142_RockThrowStandingHold, "rock_throw_standing_hold"},
    {eAbeMotions::Motion_143_RockThrowStandingThrow, "rock_throw_standing_throw"},
    {eAbeMotions::Motion_144_RockThrowStandingEnd, "rock_throw_standing_end"},
    {eAbeMotions::Motion_145_RockThrowCrouchingHold, "rock_throw_crouching_hold"},
    {eAbeMotions::Motion_146_RockThrowCrouchingThrow, "rock_throw_crouching_throw"},
    {eAbeMotions::Motion_147_ShotRolling, "shot_rolling"},
    {eAbeMotions::Motion_148_Shot, "shot"},
    {eAbeMotions::Motion_149_PickupItem, "pickup_item"},
    {eAbeMotions::Motion_150_Chant, "chant"},
    {eAbeMotions::Motion_151_ChantEnd, "chant_end"},
    {eAbeMotions::Motion_152_ElumFallOffEdge, "elum_fall_off_edge"},
    {eAbeMotions::Motion_153_ElumFall, "elum_fall"},
    {eAbeMotions::Motion_154_ElumLand, "elum_land"},
    {eAbeMotions::Motion_155_ElumJumpToFall, "elum_jump_to_fall"},
    {eAbeMotions::Motion_156_DoorEnter, "door_enter"},
    {eAbeMotions::Motion_157_DoorExit, "door_exit"},
    {eAbeMotions::Motion_158_ElumKnockback, "elum_knockback"},
    {eAbeMotions::Motion_159_Idle_RubEyes, "idle_rub_eyes"},
    {eAbeMotions::Motion_160_Idle_Stretch_Arms, "idle_stretch_arms"},
    {eAbeMotions::Motion_161_Idle_Yawn, "idle_yawn"},
    {eAbeMotions::Motion_162_ToShrykull, "to_shyrkull"},
    {eAbeMotions::Motion_163_ShrykullEnd, "shrykull_end"},
    {eAbeMotions::Motion_164_PoisonGasDeath, "poison_gas_death"}
})

NLOHMANN_JSON_SERIALIZE_ENUM(eElumMotions, {
    {eElumMotions::None_m1, "none"},
    {eElumMotions::Motion_0_Respawn, "respawn"},
    {eElumMotions::Motion_1_Idle, "idle"},
    {eElumMotions::Motion_2_Unknown, "motion_2_unknown"},
    {eElumMotions::Motion_3_WalkLoop, "walk_loop"},
    {eElumMotions::Motion_4_Turn, "turn"},
    {eElumMotions::Motion_5_WalkToIdle, "walk_to_idle"},
    {eElumMotions::Motion_6_MidWalkToIdle, "mid_walk_to_idle"},
    {eElumMotions::Motion_7_IdleToWalk1, "idle_to_walk1"},
    {eElumMotions::Motion_8_IdleToWalk2, "idle_to_walk2"},
    {eElumMotions::Motion_9_ToYell, "to_yell"},
    {eElumMotions::Motion_10_Yell, "yell"},
    {eElumMotions::Motion_11_Unknown, "motion_11_unknown"},
    {eElumMotions::Motion_12_RunTurn, "run_turn"},
    {eElumMotions::Motion_13_RunTurnToWalk, "run_turn_to_walk"},
    {eElumMotions::Motion_14_Speak, "motion_14_speak"},
    {eElumMotions::Motion_15_Speak, "motion_15_speak"},
    {eElumMotions::Motion_16_Speak, "motion_16_speak"},
    {eElumMotions::Motion_17_WalkToHop, "walk_top_hop"},
    {eElumMotions::Motion_18_MidWalkToHop, "mid_walk_top_hop"},
    {eElumMotions::Motion_19_Dead, "dead"},
    {eElumMotions::Motion_20_Fall, "fall"},
    {eElumMotions::Motion_21_Land, "land"},
    {eElumMotions::Motion_22_RunOffEdge, "run_off_edge"},
    {eElumMotions::Motion_23_WalkOffEdge, "walk_off_edge"},
    {eElumMotions::Motion_24_JumpToFall, "jump_to_fall"},
    {eElumMotions::Motion_25_LickingHoney, "licking_honey"},
    {eElumMotions::Motion_26_LickingToStruggling, "licking_to_struggling"},
    {eElumMotions::Motion_27_AbeMountingEnd, "abe_mounting_end"},
    {eElumMotions::Motion_28_AbeUnmountingEnd, "abe_unmounting_end"},
    {eElumMotions::Motion_29_BeesStruggling, "bees_struggling"},
    {eElumMotions::Motion_30_HopBegin, "hop_begin"},
    {eElumMotions::Motion_31_HopMid, "hop_mid"},
    {eElumMotions::Motion_32_HopLand, "hop_land"},
    {eElumMotions::Motion_33_RunJumpBegin, "run_jump_begin"},
    {eElumMotions::Motion_34_RunJumpMid, "run_jump_mid"},
    {eElumMotions::Motion_35_RunJumpLand, "run_jump_land"},
    {eElumMotions::Motion_36_RunLoop, "run_loop"},
    {eElumMotions::Motion_37_RunSlideStop, "run_slide_stop"},
    {eElumMotions::Motion_38_RunTurnToRun, "run_turn_to_run"},
    {eElumMotions::Motion_39_IdleToRun, "idle_to_run"},
    {eElumMotions::Motion_40_WalkToRun, "walk_to_run"},
    {eElumMotions::Motion_41_MidWalkToRun, "mid_walk_to_run"},
    {eElumMotions::Motion_42_RunToWalk, "run_to_walk"},
    {eElumMotions::Motion_43_MidRunToWalk, "mid_run_to_walk"},
    {eElumMotions::Motion_44_ScratchBegin, "scratch_begin"},
    {eElumMotions::Motion_45_ScratchLoop, "scratch_loop"},
    {eElumMotions::Motion_46_ScratchEnd, "scratch_end"},
    {eElumMotions::Motion_47_Unknown, "motion_47_unknown"},
    {eElumMotions::Motion_48_AbeMoutingBegin, "abe_mounting_begin"},
    {eElumMotions::Motion_49_AbeUnmountingBegin, "abe_unmounting_begin"},
    {eElumMotions::Motion_50_Knockback, "knockback"},
})

inline void to_json(nlohmann::json& j, const SaveData& p)
{
    j = nlohmann::json{
        {"continue_point_zone_number", p.mContinuePoint_ZoneNumber},
        {"continue_point_clear_from_id", p.mContinuePoint_ClearFromId},
        {"continue_point_clear_to_id", p.mContinuePoint_ClearToId},
        {"continue_point_top_left", p.mContinuePoint_TopLeft},
        {"continue_point_bottom_right", p.mContinuePoint_BottomRight},
        {"continue_point_level", p.mContinuePoint_Level},
        {"continue_point_path", p.mContinuePoint_Path},
        {"continue_point_camera", p.mContinuePoint_Camera},
        {"continue_point_sprite_scale", p.mContinuePoint_SpriteScale},
        {"saved_ring_timer", p.field_21C_saved_ring_timer},
        {"active_hero_saved_have_shrykull", p.mActiveHero_SavedHaveShrykull},
        {"active_hero_xpos", p.mActiveHero_XPos},
        {"active_hero_ypos", p.mActiveHero_YPos},
        {"active_hero_health", p.mActiveHero_Health},
        {"active_hero_sprite_scale", p.mActiveHero_SpriteScale},
        {"current_level", p.mCurrentLevel},
        {"current_path", p.mCurrentPath},
        {"current_camera", p.mCurrentCamera},
        {"mode_mask", p.field_23A_mode_mask},
        {"active_hero_flip_x", p.mActiveHero_FlipX},
        {"active_hero_current_motion", p.mActiveHero_CurrentMotion},
        {"active_hero_current_frame", p.mActiveHero_CurrentFrame},
        {"stone_state", p.field_244_stone_state},
        {"gnframe", p.field_248_gnFrame},
        {"field_24C_field_118", p.field_24C_field_118},
        {"throwable_count", p.field_250_throwable_count},
        {"active_hero_paramonia_done", p.mActiveHero_ParamoniaDone},
        {"active_hero_scrabania_done", p.mActiveHero_ScrabaniaDone},
        {"ring_pulse_timer", p.field_254_ring_pulse_timer},
        {"active_hero_have_shrykull", p.mActiveHero_HaveShrykull},
        {"elum_exists", p.mElumExists},
        {"controlling_elum", p.mControllingElum},
        {"elum_respawn_on_dead", p.mElum_RespawnOnDead},
        {"elum_current_level", p.mElum_CurrentLevel},
        {"elum_current_path", p.mElum_CurrentPath},
        {"infinite_grenades", p.mInfiniteGrenades},
        {"elum_xpos", p.mElum_XPos},
        {"elum_ypos", p.mElum_YPos},
        {"elum_line_type", p.mElum_LineType},
        {"elum_flip_x", p.mElum_FlipX},
        {"elum_current_motion", p.mElum_CurrentMotion},
        {"elum_dont_follow_abe", p.mElum_DontFollowAbe},
        {"elum_brain_idx", p.mElum_BrainIdx},
        {"elum_brain_sub_state", p.mElum_BrainSubState},
        {"elum_honey_xpos", p.mElum_HoneyXPos},
        {"elum_honey_camera", p.mElum_HoneyCamera},
        {"elum_struggling_with_bees", p.mElum_StrugglingWithBees},
        {"elum_stung_by_bees", p.mElum_StungByBees},
        {"elum_falling", p.mElum_Falling},
        {"elum_found_honey", p.mElum_FoundHoney},
        {"elum_continue_rect", p.mElum_ContinueRect},
        {"elum_previous_continue_zone_number", p.mElum_PreviousContinueZoneNumber},
        {"elum_abe_zone_number", p.mElum_AbeZoneNumber},
        {"elum_continue_path", p.mElum_ContinuePath},
        {"elum_continue_level", p.mElum_ContinueLevel},
        {"elum_continue_sprite_scale", p.mElum_ContinueSpriteScale},
        {"rescued_mudokons", p.mRescuedMudokons},
        {"killed_mudokons", p.mKilledMudokons},
        {"restart_rupture_farms_saved_mudokons", p.field_2A4_restartRuptureFarmsSavedMudokons},
        {"restart_rupture_farms_killed_muds", p.mRestartRuptureFarmsKilledMuds},
        {"gas_timer", p.field_2A8_gasTimer},
        {"use_alt_save_header", p.field_2AC_bUseAltSaveHeader},
        {"current_controller_idx", p.mCurrentControllerIdx},
        {"save_buffer", p.field_2B0_pSaveBuffer},
    };
}

inline void from_json(const nlohmann::json& j, SaveData& p)
{
    j.at("continue_point_zone_number").get_to(p.mContinuePoint_ZoneNumber);
    j.at("continue_point_clear_from_id").get_to(p.mContinuePoint_ClearFromId);
    j.at("continue_point_clear_to_id").get_to(p.mContinuePoint_ClearToId);
    j.at("continue_point_top_left").get_to(p.mContinuePoint_TopLeft);
    j.at("continue_point_bottom_right").get_to(p.mContinuePoint_BottomRight);
    j.at("continue_point_level").get_to(p.mContinuePoint_Level);
    j.at("continue_point_path").get_to(p.mContinuePoint_Path);
    j.at("continue_point_camera").get_to(p.mContinuePoint_Camera);
    j.at("continue_point_sprite_scale").get_to(p.mContinuePoint_SpriteScale);
    j.at("saved_ring_timer").get_to(p.field_21C_saved_ring_timer);
    j.at("active_hero_saved_have_shrykull").get_to(p.mActiveHero_SavedHaveShrykull);
    j.at("active_hero_xpos").get_to(p.mActiveHero_XPos);
    j.at("active_hero_ypos").get_to(p.mActiveHero_YPos);
    j.at("active_hero_health").get_to(p.mActiveHero_Health);
    j.at("active_hero_sprite_scale").get_to(p.mActiveHero_SpriteScale);
    j.at("current_level").get_to(p.mCurrentLevel);
    j.at("current_path").get_to(p.mCurrentPath);
    j.at("current_camera").get_to(p.mCurrentCamera);
    j.at("mode_mask").get_to(p.field_23A_mode_mask);
    j.at("active_hero_flip_x").get_to(p.mActiveHero_FlipX);
    j.at("active_hero_current_motion").get_to(p.mActiveHero_CurrentMotion);
    j.at("active_hero_current_frame").get_to(p.mActiveHero_CurrentFrame);
    j.at("stone_state").get_to(p.field_244_stone_state);
    j.at("gnframe").get_to(p.field_248_gnFrame);
    j.at("field_24C_field_118").get_to(p.field_24C_field_118);
    j.at("throwable_count").get_to(p.field_250_throwable_count);
    j.at("active_hero_paramonia_done").get_to(p.mActiveHero_ParamoniaDone);
    j.at("active_hero_scrabania_done").get_to(p.mActiveHero_ScrabaniaDone);
    j.at("ring_pulse_timer").get_to(p.field_254_ring_pulse_timer);
    j.at("active_hero_have_shrykull").get_to(p.mActiveHero_HaveShrykull);
    j.at("elum_exists").get_to(p.mElumExists);
    j.at("controlling_elum").get_to(p.mControllingElum);
    j.at("elum_respawn_on_dead").get_to(p.mElum_RespawnOnDead);
    j.at("elum_current_level").get_to(p.mElum_CurrentLevel);
    j.at("elum_current_path").get_to(p.mElum_CurrentPath);
    j.at("infinite_grenades").get_to(p.mInfiniteGrenades);
    j.at("elum_xpos").get_to(p.mElum_XPos);
    j.at("elum_ypos").get_to(p.mElum_YPos);
    j.at("elum_line_type").get_to(p.mElum_LineType);
    j.at("elum_flip_x").get_to(p.mElum_FlipX);
    j.at("elum_current_motion").get_to(p.mElum_CurrentMotion);
    j.at("elum_dont_follow_abe").get_to(p.mElum_DontFollowAbe);
    j.at("elum_brain_idx").get_to(p.mElum_BrainIdx);
    j.at("elum_brain_sub_state").get_to(p.mElum_BrainSubState);
    j.at("elum_honey_xpos").get_to(p.mElum_HoneyXPos);
    j.at("elum_honey_camera").get_to(p.mElum_HoneyCamera);
    j.at("elum_struggling_with_bees").get_to(p.mElum_StrugglingWithBees);
    j.at("elum_stung_by_bees").get_to(p.mElum_StungByBees);
    j.at("elum_falling").get_to(p.mElum_Falling);
    j.at("elum_found_honey").get_to(p.mElum_FoundHoney);
    j.at("elum_continue_rect").get_to(p.mElum_ContinueRect);
    j.at("elum_previous_continue_zone_number").get_to(p.mElum_PreviousContinueZoneNumber);
    j.at("elum_abe_zone_number").get_to(p.mElum_AbeZoneNumber);
    j.at("elum_continue_path").get_to(p.mElum_ContinuePath);
    j.at("elum_continue_level").get_to(p.mElum_ContinueLevel);
    j.at("elum_continue_sprite_scale").get_to(p.mElum_ContinueSpriteScale);
    j.at("rescued_mudokons").get_to(p.mRescuedMudokons);
    j.at("killed_mudokons").get_to(p.mKilledMudokons);
    j.at("restart_rupture_farms_saved_mudokons").get_to(p.field_2A4_restartRuptureFarmsSavedMudokons);
    j.at("restart_rupture_farms_killed_muds").get_to(p.mRestartRuptureFarmsKilledMuds);
    j.at("gas_timer").get_to(p.field_2A8_gasTimer);
    j.at("use_alt_save_header").get_to(p.field_2AC_bUseAltSaveHeader);
    j.at("current_controller_idx").get_to(p.mCurrentControllerIdx);
    j.at("save_buffer").get_to(p.field_2B0_pSaveBuffer);
}


} // namespace AO

