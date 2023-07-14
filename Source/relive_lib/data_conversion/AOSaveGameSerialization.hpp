#pragma once
#include "Types.hpp"
#include "../relive_lib/Psx.hpp"
#include "nlohmann/json.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAO/SaveGame.hpp"
#include "CommonTypesSerialization.hpp"

namespace AO {


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
            {"active_hero_flipx", p.mActiveHero_FlipX},
            {"active_hero_current_motion", p.mActiveHero_CurrentMotion},
            {"active_hero_current_frame", p.mActiveHero_CurrentFrame},
            {"stone_state", p.field_244_stone_state},
            {"gn_frame", p.field_248_gnFrame},
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
            {"elum_flipx", p.mElum_FlipX},
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
        j.at("active_hero_flipx").get_to(p.mActiveHero_FlipX);
        j.at("active_hero_current_motion").get_to(p.mActiveHero_CurrentMotion);
        j.at("active_hero_current_frame").get_to(p.mActiveHero_CurrentFrame);
        j.at("stone_state").get_to(p.field_244_stone_state);
        j.at("gn_frame").get_to(p.field_248_gnFrame);
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
        j.at("elum_flipx").get_to(p.mElum_FlipX);
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
        j.at("current_controller_idx").get_to(p.mCurrentControllerIdx);
        j.at("save_buffer").get_to(p.field_2B0_pSaveBuffer);
    }

    // Any enum/struct in the AOData namespace is related to OG data and can't ever be changed
    // otherwise interpreting the OG data will break.
    namespace AOData {

    struct Save_PSX_Header final
    {
        u8 field_0_frame_1_name[128];
        s8 field_80_frame_2_padding[128];
        s8 field_100_frame_3_padding[128];
        s8 field_180_frame_4_padding[128];
    };
    ALIVE_ASSERT_SIZEOF_ALWAYS(Save_PSX_Header, 0x200);

    struct SaveData final
    {
        Save_PSX_Header mSavePsxHeader;
        s32 mSaveHashValue;
        s16 mContinuePoint_ZoneNumber;
        s16 mContinuePoint_ClearFromId;
        s16 mContinuePoint_ClearToId;
        PSX_Point mContinuePoint_TopLeft;
        PSX_Point mContinuePoint_BottomRight;
        LevelIds mContinuePoint_Level;
        s16 mContinuePoint_Path;
        s16 mContinuePoint_Camera;
        FP mContinuePoint_SpriteScale;
        s32 field_21C_saved_ring_timer;
        s16 mActiveHero_SavedHaveShrykull;
        s16 field_222_pad;
        s32 mActiveHero_XPos;
        u32 mActiveHero_YPos;
        FP mActiveHero_Health;
        FP mActiveHero_SpriteScale;
        LevelIds mCurrentLevel;
        u16 mCurrentPath;
        s16 mCurrentCamera;
        s16 field_23A_mode_mask;
        s16 mActiveHero_FlipX;
        s16 mActiveHero_CurrentMotion;
        u16 mActiveHero_CurrentFrame;
        s16 field_242_pad;
        s32 field_244_stone_state; //TODO @ 100%  fix type length
        u32 field_248_gnFrame;
        s32 field_24C_field_118;
        s16 field_250_throwable_count;
        s8 mActiveHero_ParamoniaDone;
        s8 mActiveHero_ScrabaniaDone;
        s32 field_254_ring_pulse_timer;
        s16 mActiveHero_HaveShrykull;
        u16 mElumExists;
        s16 mControllingElum;
        u16 mElum_RespawnOnDead;
        LevelIds mElum_CurrentLevel;
        u16 mElum_CurrentPath;
        s16 mInfiniteGrenades;
        s16 field_266_pad;
        u32 mElum_XPos;
        u32 mElum_YPos;
        s16 mElum_LineType;
        s16 mElum_FlipX;
        s16 mElum_CurrentMotion;
        u16 mElum_DontFollowAbe;
        u16 mElum_BrainIdx;
        u16 mElum_BrainSubState;
        u32 mElum_HoneyXPos;
        u16 mElum_HoneyCamera;
        s16 field_282_pad;
        u32 field_284_unused;
        s8 mElum_StrugglingWithBees;
        s8 mElum_StungByBees;
        s8 mElum_Falling;
        s8 mElum_FoundHoney;
        PSX_RECT mElum_ContinueRect;
        u16 mElum_PreviousContinueZoneNumber;
        u16 mElum_AbeZoneNumber;
        u16 mElum_ContinuePath;
        LevelIds mElum_ContinueLevel;
        FP mElum_ContinueSpriteScale;
        u16 mRescuedMudokons;
        u16 mKilledMudokons;
        u16 field_2A4_restartRuptureFarmsSavedMudokons;
        u16 mRestartRuptureFarmsKilledMuds;
        s32 field_2A8_gasTimer;
        s16 field_2AC_bUseAltSaveHeader;
        s16 mCurrentControllerIdx;
        u8 field_2B0_pSaveBuffer[7501];
    };
    ALIVE_ASSERT_SIZEOF_ALWAYS(SaveData, 0x2000);

    }


} // namespace AO