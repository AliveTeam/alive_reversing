#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/Switch.hpp"
#include "../AliveLibAE/ElectricWall.hpp"
#include "../AliveLibAE/Mudokon.hpp"
#include "../AliveLibAE/BirdPortal.hpp"
#include "../AliveLibAE/StatsSign.hpp"
#include "../AliveLibAE/Door.hpp"
#include "../AliveLibAE/BoomMachine.hpp"
#include "../AliveLibAE/Slig.hpp"
#include "../AliveLibAE/Fleech.hpp"
#include "../AliveLibAE/UXB.hpp"
#include "../AliveLibAE/LCDScreen.hpp"
#include "../AliveLibAE/StatusLight.hpp"
#include "../AliveLibAE/ShadowZone.hpp"
#include "../AliveLibAE/WorkWheel.hpp"
#include "../AliveLibAE/MusicTrigger.hpp"
#include "../AliveLibAE/SlamDoor.hpp"
#include "../AliveLibAE/LaughingGas.hpp"
#include "../AliveLibAE/InvisibleSwitch.hpp"
#include "../AliveLibAE/Water.hpp"
#include "../AliveLibAE/GasEmitter.hpp"
#include "../AliveLibAE/BackgroundAnimation.hpp"
#include "../AliveLibAE/LiftPoint.hpp"
#include "../AliveLibAE/PullRingRope.hpp"
#include "../AliveLibAE/MultiSwitchController.hpp"
#include "../AliveLibAE/SecurityOrb.hpp"
#include "../AliveLibAE/Abe.hpp"
#include "../AliveLibAE/WheelSyncer.hpp"
#include "../AliveLibAE/LevelLoader.hpp"
#include "../AliveLibAE/FlyingSligSpawner.hpp"
#include "../AliveLibAE/FartMachine.hpp"
#include "../AliveLibAE/Grinder.hpp"
#include "../AliveLibAE/Mine.hpp"

#define CTOR_AE(className, objectTypeName, tlvType)  className(TypesCollection& globalTypes, Path_TLV* pTlv = nullptr) : TlvObjectBaseAE(tlvType, objectTypeName, pTlv)

struct Path_ZSligCover : public Path_TLV
{
    // Empty
};

struct Path_SligSpawner : public Path_TLV
{
    // Empty
};

struct Path_DeathDrop : public Path_TLV
{
    // Empty
};

struct Path_InvisibleZone : public Path_TLV
{
    // Empty
};

struct Path_Pulley : public Path_TLV
{
    // Empty
};

namespace AETlvs
{
    struct Path_ElectricWall : public TlvObjectBaseAE<::Path_ElectricWall>
    {
        CTOR_AE(Path_ElectricWall, "ElectricWall", TlvTypes::ElectricWall_38)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("id", mTlv.field_12_id);
            ADD("start_state", mTlv.field_14_start_state);
        }
    };

    struct Path_Mudokon : public TlvObjectBaseAE<::Path_Mudokon>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Mud_TLV_Emotion>("Enum_Mud_TLV_Emotion",
                {
                    {::Mud_TLV_Emotion::eNormal_0, "normal"},
                    {::Mud_TLV_Emotion::eAngry_1, "angry"},
                    {::Mud_TLV_Emotion::eSad_2, "sad"},
                    {::Mud_TLV_Emotion::eWired_3, "wired"},
                    {::Mud_TLV_Emotion::eSick_4, "sick"}
                });

            types.AddEnum<::TLV_Scale>("Enum_TLV_Scale",
                {
                    {::TLV_Scale::Full_0, "full"},
                    {::TLV_Scale::Half_1, "half"}
                });

            types.AddEnum<::Mud_State>("Enum_Mud_State",
                {
                     {::Mud_State::eChisle_0, "chisle"},
                     {::Mud_State::eScrub_1, "scrub"},
                     {::Mud_State::eAngryWorker_2, "angry_worker"},
                     {::Mud_State::eDamageRingGiver_3, "damage_ring_giver"},
                     {::Mud_State::eHealthRingGiver_4, "health_ring_giver"},
                });

            types.AddEnum<::Mud_Direction>("Enum_Mud_Direction",
                {
                    {::Mud_Direction::eLeft_0, "left"},
                    {::Mud_Direction::eRight_1, "right"}
                });
        }

        CTOR_AE(Path_Mudokon, "Mudokon", TlvTypes::Mudokon_49)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("state", mTlv.field_12_state);
            ADD("direction", mTlv.field_14_direction);
            ADD("voice_pitch", mTlv.field_16_voice_pitch);
            ADD("rescue_id", mTlv.field_18_rescue_id);
            ADD("is_deaf", mTlv.field_1A_bDeaf);
            ADD("disable_resources", mTlv.field_1C_disabled_resources);
            ADD("save_state", mTlv.field_1E_save_state);
            ADD("emotion", mTlv.field_20_emotion);
            ADD("is_blind", mTlv.field_22_bBlind);
            ADD("angry_trigger_id", mTlv.field_24_angry_trigger);
            ADD("stop_trigger_id", mTlv.field_26_stop_trigger);
            ADD("gets_depressed", mTlv.field_28_bGets_depressed);
            ADD("ring_timeout", mTlv.field_2A_ring_timeout);
            ADD("is_instant_powerup", mTlv.field_2C_bInstant_power_up);
        }
    };

    struct Path_BirdPortal : public TlvObjectBaseAE<::Path_BirdPortal>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::PortalType>("Enum_PortalType",
                {
                    {::PortalType::eAbe_0, "abe"},
                    {::PortalType::eWorker_1, "worker"},
                    {::PortalType::eShrykull_2, "shrykull"},
                    {::PortalType::eMudTeleport_3, "mud_teleport"}
                });

            types.AddEnum<::PortalSide>("Enum_PortalSide",
                {
                    {::PortalSide::eRight_0, "right"},
                    {::PortalSide::eLeft_1, "left"},
                });
        }

        CTOR_AE(Path_BirdPortal, "BirdPortal", TlvTypes::BirdPortal_28)
        {
            ADD("side", mTlv.field_10_side);
            ADD("dest_level", mTlv.field_12_dest_level);
            ADD("dest_path", mTlv.field_14_dest_path);
            ADD("dest_camera", mTlv.field_16_dest_camera);
            ADD("scale", mTlv.field_18_scale);
            ADD("movie_id", mTlv.field_1A_moive_id);
            ADD("portal_type", mTlv.field_1C_portal_type);
            ADD("num_muds_for_shrykull", mTlv.field_1E_num_muds_for_shrykul);
            ADD("create_id", mTlv.field_20_create_id);
            ADD("delete_id", mTlv.field_22_delete_id);
        }
    };

    struct Path_StatsSign : public TlvObjectBaseAE<::Path_StatsSign>
    {
        CTOR_AE(Path_StatsSign, "StatsSign", TlvTypes::StatusBoard_64)
        {
            ADD("number_of_muds", mTlv.field_10_number_of_muds);
            ADD("zulag_number", mTlv.field_12_zulag_number);
            ADD("is_hidden", mTlv.field_14_hidden);
        }
    };

    struct Path_Door : public TlvObjectBaseAE<::Path_Door>
    {
        CTOR_AE(Path_Door, "Door", TlvTypes::Door_5)
        {
            ADD("level", mTlv.field_10_level);
            ADD("path", mTlv.field_12_path);
            ADD("camera", mTlv.field_14_camera);
            ADD("scale", mTlv.field_16_scale);
            ADD("door_number", mTlv.field_18_door_number);
            ADD("id", mTlv.field_1A_id);
            ADD("target_door_number", mTlv.field_1C_target_door_number);
            ADD("type", mTlv.field_1E_type);
            ADD("start_state", mTlv.field_20_start_state);
            ADD("hub1", mTlv.field_22_hub1);
            ADD("hub2", mTlv.field_22_hub2);
            ADD("hub3", mTlv.field_22_hub3);
            ADD("hub4", mTlv.field_22_hub4);
            ADD("hub5", mTlv.field_22_hub5);
            ADD("hub6", mTlv.field_22_hub6);
            ADD("hub7", mTlv.field_22_hub7);
            ADD("hub8", mTlv.field_22_hub8);
            ADD("wipe_effect", mTlv.field_32_wipe_effect);
            ADD("movie_number", mTlv.field_34_movie_number);
            ADD("x_offset", mTlv.field_36_x_offset);
            ADD("y_offset", mTlv.field_38_y_offset);
            ADD("wipe_x_org", mTlv.field_3A_wipe_x_org);
            ADD("wipe_y_org", mTlv.field_3C_wipe_y_org);
            ADD("abe_direction", mTlv.field_3E_abe_direction);
            ADD("close_after_use", mTlv.field_40_close_after_use);
            ADD("cancel_throwables", mTlv.field_42_cancel_throwables);
        }
    };

    struct Path_Switch : public TlvObjectBaseAE<::Path_Switch>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::SwitchSoundType>("Enum_SwitchSoundType",
                {
                    {::SwitchSoundType::eNone, "none"},
                    {::SwitchSoundType::eWell_1, "well"},
                    {::SwitchSoundType::eUnknown_2, "unknown"},
                    {::SwitchSoundType::eDoor_3, "door"},
                    {::SwitchSoundType::eElectricWall_4, "electric_wall"},
                    {::SwitchSoundType::eSecurityOrb_5, "security_orb"},
                    {::SwitchSoundType::eLift_6, "lift"}
                });
        }

        CTOR_AE(Path_Switch, "Switch", TlvTypes::Switch_17)
        {
            ADD("target_action", mTlv.field_10_target_action);
            ADD("scale", mTlv.field_12_scale);
            ADD("on_sound", mTlv.field_14_on_sound);
            ADD("off_sound", mTlv.field_16_off_sound);
            ADD("sound_direction", mTlv.field_18_sound_direction);
            ADD("trigger_id", mTlv.field_1A_trigger_id);
            ADD("persist_offscreen", mTlv.field_1C_persist_offscreen);
        }
    };

    struct Path_Hoist : public TlvObjectBaseAE<::Path_Hoist>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_Hoist::Type>("Enum_HoistType",
                {
                    {::Path_Hoist::Type::eNextEdge, "next_edge"},
                    {::Path_Hoist::Type::eNextFloor, "next_floor"},
                    {::Path_Hoist::Type::eOffScreen, "off_screen"},
                });

            types.AddEnum<::Path_Hoist::EdgeType>("Enum_HoistEdgeType",
                {
                    {::Path_Hoist::EdgeType::eBoth, "both"},
                    {::Path_Hoist::EdgeType::eLeft, "left"},
                    {::Path_Hoist::EdgeType::eRight, "right"},
                });

            types.AddEnum<::Path_Hoist::Scale>("Enum_HoistScale",
                {
                    {::Path_Hoist::Scale::eFull, "full"},
                    {::Path_Hoist::Scale::eHalf, "half"}
                });
        }

        CTOR_AE(Path_Hoist, "Hoist", TlvTypes::Hoist_2)
        {
            ADD("HoistType", mTlv.field_10_type);
            ADD("HoistEdgeType", mTlv.field_12_edge_type);
            ADD("Id", mTlv.field_14_id);
            ADD("Scale", mTlv.field_16_scale);
        }
    };

    struct Path_BoomMachine : public TlvObjectBaseAE<::Path_BoomMachine>
    {
        CTOR_AE(Path_BoomMachine, "BoomMachine", TlvTypes::BoomMachine_59)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("nozzle_side", mTlv.field_12_nozzle_side);
            ADD("disabled_resources", mTlv.field_14_disabled_resources);
            ADD("number_of_grenades", mTlv.field_16_number_of_grenades);
        }
    };

    struct Path_Slig : public TlvObjectBaseAE<::Path_Slig>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_Slig::StartState>("Enum_StartState",
                {
                    {::Path_Slig::StartState::Listening_0, "listening"},
                    {::Path_Slig::StartState::Paused_1, "paused"},
                    {::Path_Slig::StartState::Sleeping_2, "sleeping"},
                    {::Path_Slig::StartState::Chase_3, "chase"},
                    {::Path_Slig::StartState::RunOffScreen_4, "run_off_screen"},
                    {::Path_Slig::StartState::GameEnder_5, "game_ender"},
                    {::Path_Slig::StartState::ListeningToGlukkon_6, "listening_to_glukkon"},
                });
        }

        CTOR_AE(Path_Slig, "Slig", TlvTypes::Slig_15)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("pause_time", mTlv.field_14_pause_time);
            ADD("pause_left_min", mTlv.field_16_pause_left_min);
            ADD("pause_left_max", mTlv.field_18_pause_left_max);
            ADD("pause_right_min", mTlv.field_1A_pause_right_min);
            ADD("pause_right_max", mTlv.field_1C_pause_right_max);
            ADD("chal_number", mTlv.field_1E_chal_number);
            ADD("chal_timer", mTlv.field_20_chal_timer);
            ADD("num_times_to_shot", mTlv.field_22_num_times_to_shoot);
            ADD("code_1", mTlv.field_26_code1);
            ADD("code_2", mTlv.field_28_code2);
            ADD("chase_abe", mTlv.field_2A_chase_abe);
            ADD("start_direction", mTlv.field_2C_start_direction);
            ADD("panic_timeout", mTlv.field_2E_panic_timeout);
            ADD("num_panic_sounds", mTlv.field_30_num_panic_sounds);
            ADD("panic_sound_timeout", mTlv.field_32_panic_sound_timeout);
            ADD("stop_chase_delay", mTlv.field_34_stop_chase_delay);
            ADD("time_to_wait_before_chase", mTlv.field_36_time_to_wait_before_chase);
            ADD("slig_id", mTlv.field_38_slig_id);
            ADD("percent_say_what", mTlv.field_3C_percent_say_what);
            ADD("percent_beat_mud", mTlv.field_3E_percent_beat_mud);
            ADD("talk_to_abe", mTlv.field_40_talk_to_abe);
            ADD("dont_shoot", mTlv.field_42_dont_shoot);
            ADD("z_shoot_delay", mTlv.field_44_Z_shoot_delay);
            ADD("stay_awake", mTlv.field_46_stay_awake);
            ADD("disable_resources", mTlv.field_48_disable_resources);
            ADD("noise_wake_up_distance", mTlv.field_4A_noise_wake_up_distance);
            ADD("id", mTlv.field_4C_id);
            ADD("unknown", mTlv.field_4E_unknown);
        }
    };

    struct Path_Fleech : public TlvObjectBaseAE<::Path_Fleech>
    {
        CTOR_AE(Path_Fleech, "Fleech", TlvTypes::Fleech_83)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("direction", mTlv.field_12_direction);
            ADD("asleep", mTlv.field_14_asleep);
            ADD("attack_anger", mTlv.field_1A_attack_anger);
            ADD("attack_delay", mTlv.field_1C_attack_delay);
            ADD("wake_up_id", mTlv.field_1E_wake_up_id);
            ADD("lost_target_timeout", mTlv.field_22_lost_target_timeout);
            ADD("goes_to_sleep", mTlv.field_24_goes_to_sleep);
            ADD("patrol_range_in_grids", mTlv.field_26_patrol_range_in_grids);
            ADD("allow_wake_up_id", mTlv.field_2A_allow_wake_up_id);
            ADD("persistant", mTlv.field_2C_persistant);
        }
    };

    struct Path_EnemyStopper : public TlvObjectBaseAE<::Path_EnemyStopper>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_EnemyStopper::StopDirection>("Enum_StopDirection",
                {
                    {::Path_EnemyStopper::StopDirection::Left_0, "left"},
                    {::Path_EnemyStopper::StopDirection::Right_1, "right"},
                    {::Path_EnemyStopper::StopDirection::Both_2, "both"},
                });
        }

        CTOR_AE(Path_EnemyStopper, "EnemyStopper", TlvTypes::EnemyStopper_47)
        {
            ADD("stop_direction", mTlv.field_10_stop_direction);
            ADD("id", mTlv.field_12_id);
        }
    };

    struct Path_Teleporter : public TlvObjectBaseAE<::Path_Teleporter>
    {
        CTOR_AE(Path_Teleporter, "Teleporter", TlvTypes::Teleporter_88)
        {
            ADD("id", mTlv.field_10_data.field_10_id);
            ADD("target_id", mTlv.field_10_data.field_12_target_id);
            ADD("camera", mTlv.field_10_data.field_14_camera);
            ADD("path", mTlv.field_10_data.field_16_path);
            ADD("level", mTlv.field_10_data.field_18_level);
            ADD("trigger_id", mTlv.field_10_data.field_1A_trigger_id);
            ADD("scale", mTlv.field_10_data.field_1C_scale);
            ADD("wipe", mTlv.field_10_data.field_1E_wipe);
            ADD("movie_number", mTlv.field_10_data.field_20_movie_number);
            ADD("electric_x", mTlv.field_10_data.field_22_eletric_x);
            ADD("electric_y", mTlv.field_10_data.field_24_electric_y);
        }
    };

    struct Path_UXB : public TlvObjectBaseAE<::Path_UXB>
    {
        CTOR_AE(Path_UXB, "UXB", TlvTypes::UXB_25)
        {
            ADD("num_patterns", mTlv.field_10_num_patterns);
            ADD("pattern", mTlv.field_12_pattern);
            ADD("scale", mTlv.field_14_scale);
            ADD("state", mTlv.field_16_state);
            ADD("disabled_resources", mTlv.field_18_disabled_resources);
        }
    };

    struct Path_LCDScreen : public TlvObjectBaseAE<::Path_LCDScreen>
    {
        CTOR_AE(Path_LCDScreen, "LCDScreen", TlvTypes::LCD_60)
        {
            ADD("message_1_id", mTlv.field_10_message_1_id);
            ADD("message_rand_min", mTlv.field_12_message_rand_min);
            ADD("message_rand_max", mTlv.field_14_message_rand_max);
            ADD("message_2_id", mTlv.field_16_message_2_id);
            ADD("swap_message_sets_switch_id", mTlv.field_18_swap_message_sets_switch_id);
        }
    };

    struct Path_Edge : public TlvObjectBaseAE<::Path_Edge>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_Edge::Type>("Enum_Type",
                {
                    {::Path_Edge::Type::eLeft, "left"},
                    {::Path_Edge::Type::eRight, "right"},
                    {::Path_Edge::Type::eBoth, "both"},
                });

            types.AddEnum<::Path_Edge::Scale>("Enum_Scale",
                {
                    {::Path_Edge::Scale::eFull, "full"},
                    {::Path_Edge::Scale::eHalf, "half"},
                });
        }

        CTOR_AE(Path_Edge, "Edge", TlvTypes::Edge_3)
        {
            ADD("type", mTlv.field_10_type);
            ADD("can_grab", mTlv.field_12_can_grab);
            ADD("scale", mTlv.field_14_scale);
        }
    };

    struct Path_StatusLight : public TlvObjectBaseAE<::Path_StatusLight>
    {
        CTOR_AE(Path_StatusLight, "StatusLight", TlvTypes::StatusLight_97)
        {
            ADD("id", mTlv.field_10_id);
            ADD("scale", mTlv.field_12_scale);
            ADD("id_1", mTlv.field_14_id1);
            ADD("id_2", mTlv.field_16_id2);
            ADD("id_3", mTlv.field_18_id3);
            ADD("id_4", mTlv.field_1A_id4);
            ADD("id_5", mTlv.field_1C_id5);
            ADD("snap_to_grid", mTlv.field_1E_snap_to_grid);
        }
    };

    struct Path_ShadowZone : public TlvObjectBaseAE<::Path_ShadowZone>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::ShadowZoneScale>("Enum_ShadowZoneScale",
                {
                    {::ShadowZoneScale::eBoth_0, "both"},
                    {::ShadowZoneScale::eHalf_1, "half"},
                    {::ShadowZoneScale::eFull_2, "full"},
                });
        }

        CTOR_AE(Path_ShadowZone, "ShadowZone", TlvTypes::ShadowZone_6)
        {
            ADD("centre_w", mTlv.field_10_centre_w);
            ADD("center_h", mTlv.field_12_center_h);
            ADD("r", mTlv.field_14_r);
            ADD("g", mTlv.field_16_g);
            ADD("b", mTlv.field_18_b);
            ADD("id", mTlv.field_1A_id);
            ADD("scale", mTlv.field_1C_scale);
        }
    };

    struct Path_WorkWheel : public TlvObjectBaseAE<::Path_WorkWheel>
    {
        CTOR_AE(Path_WorkWheel, "WorkWheel", TlvTypes::WorkWheel_79)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("id", mTlv.field_12_id);
            ADD("duration", mTlv.field_14_duration);
            ADD("off_time", mTlv.field_16_off_time);
            ADD("off_when_stopped", mTlv.field_18_off_when_stopped);
        }
    };

    struct Path_MusicTrigger : public TlvObjectBaseAE<::Path_MusicTrigger>
    {
        CTOR_AE(Path_MusicTrigger, "MusicTrigger", TlvTypes::MusicTrigger_66)
        {
            ADD("type", mTlv.field_10_type);
            ADD("enabled_by", mTlv.field_12_enabled_by);
            ADD("timer", mTlv.field_14_timer);
        }
    };

    struct Path_AbeStart : public TlvObjectBaseAE<::Path_AbeStart>
    {
        CTOR_AE(Path_AbeStart, "AbeStart", TlvTypes::AbeStart_22)
        {
            ADD("xpos", mTlv.field_10_xpos);
            ADD("ypos", mTlv.field_12_ypos);
        }
    };

    struct Path_SoftLanding : public TlvObjectBaseAE<::Path_SoftLanding>
    {
        CTOR_AE(Path_SoftLanding, "SoftLanding", TlvTypes::SoftLanding_75)
        {
            ADD("id", mTlv.field_10_id);
        }
    };

    struct Path_WellExpress : public TlvObjectBaseAE<::Path_WellExpress>
    {
        CTOR_AE(Path_WellExpress, "WellExpress", TlvTypes::WellExpress_23)
        {
            ADD("exit_x", mTlv.field_18_exit_x);
            ADD("exit_y", mTlv.field_1A_exit_y);
            ADD("off_level", mTlv.field_1C_off_level);
            ADD("off_path", mTlv.field_1E_off_path);
            ADD("off_camera", mTlv.field_20_off_camera);
            ADD("off_well_id", mTlv.field_22_off_well_id);
            ADD("on_level", mTlv.field_24_on_level);
            ADD("on_path", mTlv.field_26_on_path);
            ADD("on_camera", mTlv.field_28_on_camera);
            ADD("on_well_id", mTlv.field_2A_on_well_id);
            ADD("emit_leaves", mTlv.field_2C_emit_leaves);
            ADD("leaf_x", mTlv.field_2E_leaf_x);
            ADD("leaf_y", mTlv.field_30_leaf_y);
            ADD("movie_id", mTlv.field_32_movie_id);
        }
    };

    struct Path_SlamDoor : public TlvObjectBaseAE<::Path_SlamDoor>
    {
        CTOR_AE(Path_SlamDoor, "SlamDoor", TlvTypes::SlamDoor_85)
        {
            ADD("starts_shut", mTlv.field_10_starts_shut);
            ADD("half_scale", mTlv.field_12_half_scale);
            ADD("id", mTlv.field_14_id);
            ADD("inverted", mTlv.field_16_inverted);
            ADD("delete", mTlv.field_18_delete);
        }
    };

    struct Path_HandStone : public TlvObjectBaseAE<::Path_HandStone>
    {
        CTOR_AE(Path_HandStone, "HandStone", TlvTypes::HandStone_61)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("camera_id_1", mTlv.field_12_camera_id1);
            ADD("camera_id_2", mTlv.field_12_camera_id2);
            ADD("camera_id_3", mTlv.field_12_camera_id3);
            ADD("trigger_id", mTlv.field_18_trigger_id);
        }
    };

    struct Path_LaughingGas : public TlvObjectBaseAE<::Path_LaughingGas>
    {
        CTOR_AE(Path_LaughingGas, "LaughingGas", TlvTypes::LaughingGas_81)
        {
            ADD("is_laughing_gas", mTlv.field_10_is_laughing_gas);
            ADD("gas_id", mTlv.field_12_gas_id);
            ADD("red_percent", mTlv.field_14_red_percent);
            ADD("green_percent", mTlv.field_16_green_percent);
            ADD("blue_percent", mTlv.field_18_blue_percent);
        }
    };

    struct Path_InvisibleSwitch : public TlvObjectBaseAE<::Path_InvisibleSwitch>
    {
        CTOR_AE(Path_InvisibleSwitch, "InvisibleSwitch", TlvTypes::InvisibleSwitch_48)
        {
            ADD("id", mTlv.field_10_id);
            ADD("action", mTlv.field_12_action);
            ADD("delay", mTlv.field_14_delay);
            ADD("set_off_alarm", mTlv.field_16_set_off_alarm);
            ADD("scale", mTlv.field_18_scale);
        }
    };

    struct Path_Water : public TlvObjectBaseAE<::Path_Water>
    {
        CTOR_AE(Path_Water, "Water", TlvTypes::Water_77)
        {
            ADD("max_drops", mTlv.field_10_data.field_10_max_drops);
            ADD("id", mTlv.field_10_data.field_12_id);
            ADD("splash_time", mTlv.field_10_data.field_14_splash_time);
            ADD("splash_x_velocity", mTlv.field_10_data.field_16_splash_x_velocity);
            ADD("splash_y_velocity", mTlv.field_10_data.field_18_splash_y_velocity);
            ADD("timeout", mTlv.field_10_data.field_1A_timeout);
        }
    };

    struct Path_GasEmitter : public TlvObjectBaseAE<::Path_GasEmitter>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::GasColour>("Enum_GasColour",
                {
                    {::GasColour::Yellow_0, "yellow"},
                    {::GasColour::Red_1, "red"},
                    {::GasColour::Green_2, "green"},
                    {::GasColour::Blue_3, "blue"},
                    {::GasColour::White_4, "white"},
                });
        }

        CTOR_AE(Path_GasEmitter, "GasEmitter", TlvTypes::GasEmitter_71)
        {
            ADD("port_id", mTlv.field_10_port_id);
            ADD("colour", mTlv.field_12_colour);
        }
    };

    struct Path_BackgroundAnimation : public TlvObjectBaseAE<::Path_BackgroundAnimation>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::TPageAbr>("Enum_TPageAbr",
                {
                    {::TPageAbr::eBlend_0, "blend_0"},
                    {::TPageAbr::eBlend_1, "blend_1"},
                    {::TPageAbr::eBlend_2, "blend_2"},
                    {::TPageAbr::eBlend_3, "blend_3"},
                });

            types.AddEnum<::Layer>("Enum_Layer",
                {
                    {::Layer::eLayer_0, "Layer_0"},
                    {::Layer::eLayer_1, "Layer_1"},
                    {::Layer::eLayer_3, "Layer_3"},
                    {::Layer::eLayer_4, "Layer_4"},
                    {::Layer::eLayer_5, "Layer_5"},
                    {::Layer::eLayer_6, "Layer_6"},
                    {::Layer::eLayer_7, "Layer_7"},
                    {::Layer::eLayer_8, "Layer_8"},
                    {::Layer::eLayer_9, "Layer_9"},
                    {::Layer::eLayer_10, "Layer_10"},
                    {::Layer::eLayer_11, "Layer_11"},
                    {::Layer::eLayer_12, "Layer_12"},
                    {::Layer::eLayer_13, "Layer_13"},
                    {::Layer::eLayer_14, "Layer_14"},
                    {::Layer::eLayer_15, "Layer_15"},
                    {::Layer::eLayer_16, "Layer_16"},
                    {::Layer::eLayer_17, "Layer_17"},
                    {::Layer::eLayer_18, "Layer_18"},
                    {::Layer::eLayer_20, "Layer_20"},
                    {::Layer::eLayer_22, "Layer_22"},
                    {::Layer::eLayer_23, "Layer_23"},
                    {::Layer::eLayer_24, "Layer_24"},
                    {::Layer::eLayer_25, "Layer_25"},
                    {::Layer::eLayer_26, "Layer_26"},
                    {::Layer::eLayer_27, "Layer_27"},
                    {::Layer::eLayer_28, "Layer_28"},
                    {::Layer::eLayer_29, "Layer_29"},
                    {::Layer::eLayer_30, "Layer_30"},
                    {::Layer::eLayer_31, "Layer_31"},
                    {::Layer::eLayer_32, "Layer_32"},
                    {::Layer::eLayer_33, "Layer_33"},
                    {::Layer::eLayer_34, "Layer_34"},
                    {::Layer::eLayer_35, "Layer_35"},
                    {::Layer::eLayer_36, "Layer_36"},
                    {::Layer::eLayer_37, "Layer_37"},
                    {::Layer::eLayer_38, "Layer_38"},
                    {::Layer::eLayer_39, "Layer_39"},
                    {::Layer::eLayer_40, "Layer_40"},
                    {::Layer::eLayer_41, "Layer_41"},
                    {::Layer::eLayer_42, "Layer_42"},

                });
        }


        CTOR_AE(Path_BackgroundAnimation, "BackgroundAnimation", TlvTypes::BackgroundAnimation_13)
        {
            ADD("res_id", mTlv.field_10_res_id);
            ADD("is_semi_trans", mTlv.field_12_is_semi_trans);
            ADD("semi_trans_mode", mTlv.field_14_semi_trans_mode);
            ADD("sound_effect", mTlv.field_16_sound_effect);
            ADD("id", mTlv.field_18_id);
            ADD("layer", mTlv.field_1A_layer);
        }
    };

    struct Path_LiftPoint : public TlvObjectBaseAE<::Path_LiftPoint>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::LiftPointStopType>("Enum_LiftPointStopType",
                {
                    {::LiftPointStopType::eTopFloor_0, "top_floor"},
                    {::LiftPointStopType::eBottomFloor_1, "bottom_floor"},
                    {::LiftPointStopType::eMiddleFloor_2, "middle_floor"},
                    {::LiftPointStopType::eMiddleLockFloor_3, "middle_lock_floor"},
                    {::LiftPointStopType::eStartPointOnly_4, "start_point_only"},
                });
        }

        CTOR_AE(Path_LiftPoint, "LiftPoint", TlvTypes::LiftPoint_7)
        {
            ADD("id", mTlv.field_10_id);
            ADD("is_start_point", mTlv.field_12_bstart_point);
            ADD("lift_point_stop_type", mTlv.field_16_lift_point_stop_type);
            ADD("scale", mTlv.field_18_scale);
            ADD("ignore_lift_mover", mTlv.field_1A_bIgnore_lift_mover);
        }
    };

    struct Path_PullRingRope : public TlvObjectBaseAE<::Path_PullRingRope>
    {
        CTOR_AE(Path_PullRingRope, "PullRingRope", TlvTypes::PullRingRope_12)
        {
            ADD("id", mTlv.field_10_id);
            ADD("target_action", mTlv.field_12_target_action);
            ADD("length_of_rope", mTlv.field_14_length_of_rope);
            ADD("scale", mTlv.field_16_scale);
            ADD("on_sound", mTlv.field_18_on_sound);
            ADD("off_sound", mTlv.field_1A_off_sound);
            ADD("sound_direction", mTlv.field_1C_sound_direction);
        }
    };

    struct Path_MultiSwitchController : public TlvObjectBaseAE<::Path_MultiSwitchController>
    {
        CTOR_AE(Path_MultiSwitchController, "MultiSwitchController", TlvTypes::MultiSwitchController_96)
        {
            ADD("id", mTlv.field_10_id);
            ADD("operation", mTlv.field_12_operation);
            ADD("delay", mTlv.field_14_delay);
            ADD("id_1", mTlv.field_16_id1);
            ADD("id_2", mTlv.field_18_id2);
            ADD("id_3", mTlv.field_1A_id3);
            ADD("id_4", mTlv.field_1C_id4);
            ADD("id_5", mTlv.field_1E_id5);
            ADD("id_6", mTlv.field_20_id6);
        }
    };

    struct Path_SecurityOrb : public TlvObjectBaseAE<::Path_SecurityOrb>
    {
        CTOR_AE(Path_SecurityOrb, "SecurityOrb", TlvTypes::SecurityOrb_19)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("disabled_resources", mTlv.field_12_disabled_resources);
        }
    };

    struct Path_InvisibleZone : public TlvObjectBaseAE<::Path_InvisibleZone>
    {
        CTOR_AE(Path_InvisibleZone, "InvisibleZone", TlvTypes::InvisibleZone_33)
        {
            // Empty
        }
    };

    struct Path_ContinuePoint : public TlvObjectBaseAE<::Path_ContinuePoint>
    {
        CTOR_AE(Path_ContinuePoint, "ContinuePoint", TlvTypes::ContinuePoint_0)
        {
            ADD("scale", mTlv.field_10_scale);
            ADD("save_file_id", mTlv.field_12_save_file_id);
        }
    };

    struct Path_WheelSyncer : public TlvObjectBaseAE<::Path_WheelSyncer>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::WheelSyncerAction>("Enum_WheelSyncerAction",
                {
                    {::WheelSyncerAction::eAllOn_0, "all_on"},
                    {::WheelSyncerAction::eOff_1, "off"},
                    {::WheelSyncerAction::eToggle_2, "toggle"},
                    {::WheelSyncerAction::eOn_3, "on"},
                });
        }

        CTOR_AE(Path_WheelSyncer, "WheelSyncer", TlvTypes::WheelSyncer_65)
        {
            ADD("id_1", mTlv.field_10_id1);
            ADD("id_2", mTlv.field_12_id2);
            ADD("trigger_id", mTlv.field_14_trigger_id);
            ADD("action", mTlv.field_16_action);
            ADD("id_3", mTlv.field_18_id3);
            ADD("id_4", mTlv.field_1A_id4);
            ADD("id_5", mTlv.field_1C_id5);
            ADD("id_6", mTlv.field_1E_id6);
        }
    };

    struct Path_LevelLoader : public TlvObjectBaseAE<::Path_LevelLoader>
    {
        CTOR_AE(Path_LevelLoader, "LevelLoader", TlvTypes::LevelLoader_86)
        {
            ADD("id", mTlv.field_10_id);
            ADD("dest_level", mTlv.field_12_dest_level);
            ADD("dest_path", mTlv.field_14_dest_path);
            ADD("dest_camera", mTlv.field_16_dest_camera);
            ADD("movie", mTlv.field_18_movie);
        }
    };

    struct Path_Pulley : public TlvObjectBaseAE<::Path_Pulley>
    {
        CTOR_AE(Path_Pulley, "Pulley", TlvTypes::Pulley_21)
        {
            // Empty
        }
    };

    struct Path_FlyingSlig : public TlvObjectBaseAE<::Path_FlyingSlig>
    {
        CTOR_AE(Path_FlyingSlig, "FlyingSlig", TlvTypes::FlyingSlig_82)
        {
            ADD("scale", mTlv.field_10_data.field_0_scale);
            ADD("state", mTlv.field_10_data.field_2_state);
            ADD("hi_pause_time", mTlv.field_10_data.field_4_hi_pause_time);
            ADD("patrol_pause_min", mTlv.field_10_data.field_6_patrol_pause_min);
            ADD("patrol_pause_max", mTlv.field_10_data.field_8_patrol_pause_max);
            ADD("direction", mTlv.field_10_data.field_A_direction);
            ADD("panic_delay", mTlv.field_10_data.field_C_panic_delay);
            ADD("give_up_chase_delay", mTlv.field_10_data.field_E_give_up_chase_delay);
            ADD("prechase_delay", mTlv.field_10_data.field_10_prechase_delay);
            ADD("slig_id", mTlv.field_10_data.field_12_slig_id);
            ADD("listen_time", mTlv.field_10_data.field_14_listen_time);
            ADD("trigger_id", mTlv.field_10_data.field_16_trigger_id);
            ADD("grenade_delay", mTlv.field_10_data.field_18_grenade_delay);
            ADD("max_velocity", mTlv.field_10_data.field_1A_max_velocity);
            ADD("launch_id", mTlv.field_10_data.field_1C_launch_id);
            ADD("persistant", mTlv.field_10_data.field_1E_persistant);
        }
    };

    struct Path_FlyingSligSpawner : public TlvObjectBaseAE<::Path_FlyingSligSpawner>
    {
        CTOR_AE(Path_FlyingSligSpawner, "FlyingSligSpawner", TlvTypes::FlyingSligSpawner_92)
        {
            ADD("scale", mTlv.field_10.field_0_scale);
            ADD("state", mTlv.field_10.field_2_state);
            ADD("hi_pause_time", mTlv.field_10.field_4_hi_pause_time);
            ADD("patrol_pause_min", mTlv.field_10.field_6_patrol_pause_min);
            ADD("patrol_pause_max", mTlv.field_10.field_8_patrol_pause_max);
            ADD("direction", mTlv.field_10.field_A_direction);
            ADD("panic_delay", mTlv.field_10.field_C_panic_delay);
            ADD("give_up_chase_delay", mTlv.field_10.field_E_give_up_chase_delay);
            ADD("prechase_delay", mTlv.field_10.field_10_prechase_delay);
            ADD("slig_id", mTlv.field_10.field_12_slig_id);
            ADD("listen_time", mTlv.field_10.field_14_listen_time);
            ADD("trigger_id", mTlv.field_10.field_16_trigger_id);
            ADD("grenade_delay", mTlv.field_10.field_18_grenade_delay);
            ADD("max_velocity", mTlv.field_10.field_1A_max_velocity);
            ADD("launch_id", mTlv.field_10.field_1C_launch_id);
            ADD("persistant", mTlv.field_10.field_1E_persistant);
        }
    };

    struct Path_DeathDrop : public TlvObjectBaseAE<::Path_DeathDrop>
    {
        CTOR_AE(Path_DeathDrop, "DeathDrop", TlvTypes::DeathDrop_4)
        {
            // Empty
        }
    };

    struct Path_SligSpawner : public TlvObjectBaseAE<::Path_SligSpawner>
    {
        CTOR_AE(Path_SligSpawner, "SligSpawner", TlvTypes::SligSpawner_37)
        {
            // Empty
        }
    };

    struct Path_Slig_LeftBound : public TlvObjectBaseAE<::Path_Slig_LeftBound>
    {
        CTOR_AE(Path_Slig_LeftBound, "SligLeftBound", TlvTypes::SligBoundLeft_32)
        {
            ADD("slig_id", mTlv.field_10_slig_id);
            ADD("disabled_resources", mTlv.field_12_disabled_resources);
        }
    };

    struct Path_Slig_RightBound : public TlvObjectBaseAE<::Path_Slig_RightBound>
    {
        CTOR_AE(Path_Slig_RightBound, "SligRightBound", TlvTypes::SligBoundRight_45)
        {
            ADD("slig_id", mTlv.field_10_slig_id);
            ADD("disabled_resources", mTlv.field_12_disabled_resources);
        }
    };

    struct Path_Slig_Persist : public TlvObjectBaseAE<::Path_Slig_Persist>
    {
        CTOR_AE(Path_Slig_Persist, "SligPersist", TlvTypes::SligPersist_46)
        {
            ADD("slig_id", mTlv.field_10_slig_id);
            ADD("disabled_resources", mTlv.field_12_disabled_resources);
        }
    };

    struct Path_ZSligCover : public TlvObjectBaseAE<::Path_ZSligCover>
    {
        CTOR_AE(Path_ZSligCover, "ZSligCover", TlvTypes::ZSligCover_50)
        {
            // Empty
        }
    };

    struct Path_WellLocal : public TlvObjectBaseAE<::Path_WellLocal>
    {
        CTOR_AE(Path_WellLocal, "WellLocal", TlvTypes::LocalWell_8)
        {
            ADD("off_dx", mTlv.field_18_off_dx);
            ADD("off_dy", mTlv.field_1A_off_dy);
            ADD("on_dx", mTlv.field_1C_on_dx);
            ADD("on_dy", mTlv.field_1E_on_dy);
            ADD("emit_leaves", mTlv.field_20_emit_leaves);
            ADD("leaf_x", mTlv.field_22_leaf_x);
            ADD("leaf_y", mTlv.field_24_leaf_y);
        }
    };

    struct Path_FartMachine : public TlvObjectBaseAE<::Path_FartMachine>
    {
        CTOR_AE(Path_FartMachine, "FartMachine", TlvTypes::FartMachine_101)
        {
            ADD("num_brews", mTlv.field_10_num_brews);
        }
    };

    struct Path_Grinder : public TlvObjectBaseAE<::Path_Grinder>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::GrinderBehavior>("Enum_GrinderBehavior",
                {
                    {::GrinderBehavior::eUnknown_0, "unknown"},
                    {::GrinderBehavior::eToggle_1, "toggle"},
                    {::GrinderBehavior::eUse_2, "use"},
                });

            types.AddEnum<::GrinderDirection>("Enum_GrinderDirection",
                {
                    {::GrinderDirection::eDown_0, "down"},
                    {::GrinderDirection::eRight_1, "right"},
                    {::GrinderDirection::eLeft_2, "left"},
                });
        }

        CTOR_AE(Path_Grinder, "Grinder", TlvTypes::Grinder_90)
        {
            ADD("scale_background", mTlv.field_10_data.field_10_scale_background);
            ADD("min_off_time", mTlv.field_10_data.field_12_min_off_time);
            ADD("max_off_time", mTlv.field_10_data.field_14_max_off_time);
            ADD("id", mTlv.field_10_data.field_16_id);
            ADD("behavior", mTlv.field_10_data.field_18_behavior);
            ADD("speed", mTlv.field_10_data.field_1A_speed);
            ADD("start_state_on", mTlv.field_10_data.field_1C_start_state_on);
            ADD("off_speed", mTlv.field_10_data.field_1E_off_speed);
            ADD("min_off_time2", mTlv.field_10_data.field_20_min_off_time2);
            ADD("max_off_time2", mTlv.field_10_data.field_22_max_off_time2);
            ADD("start_position", mTlv.field_10_data.field_24_start_position);
            ADD("direction", mTlv.field_10_data.field_26_direction);
        }
    };

    struct Path_Mine : public TlvObjectBaseAE<::Path_Mine>
    {
        CTOR_AE(Path_Mine, "Mine", TlvTypes::Mine_24)
        {
            ADD("num_patterns", mTlv.field_10_num_patterns);
            ADD("pattern", mTlv.field_12_pattern);
            ADD("scale", mTlv.field_14_scale);
            ADD("disabled_resources", mTlv.field_16_disabled_resources);
            ADD("persists_offscreen", mTlv.field_18_persists_offscreen);
        }
    };
}
