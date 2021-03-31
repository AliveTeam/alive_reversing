#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/Switch.hpp"
#include "../AliveLibAE/ElectricWall.hpp"
#include "../AliveLibAE/Mudokon.hpp"
#include "../AliveLibAE/BirdPortal.hpp"
#include "../AliveLibAE/LCDStatusBoard.hpp"
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
#include "../AliveLibAE/Slog.hpp"
#include "../AliveLibAE/TrapDoor.hpp"
#include "../AliveLibAE/LiftMover.hpp"
#include "../AliveLibAE/RockSack.hpp"
#include "../AliveLibAE/TimerTrigger.hpp"
#include "../AliveLibAE/MotionDetector.hpp"
#include "../AliveLibAE/MineCar.hpp"
#include "../AliveLibAE/ExplosionSet.hpp"
#include "../AliveLibAE/ColourfulMeter.hpp"
#include "../AliveLibAE/SlapLock.hpp"
#include "../AliveLibAE/Slurg.hpp"
#include "../AliveLibAE/DoorBlocker.hpp"
#include "../AliveLibAE/Dove.hpp"
#include "../AliveLibAE/DoorFlame.hpp"
#include "../AliveLibAE/Greeter.hpp"
#include "../AliveLibAE/MovingBomb.hpp"
#include "../AliveLibAE/SecurityDoor.hpp"
#include "../AliveLibAE/NakedSlig.hpp"
#include "../AliveLibAE/NakedSligButton.hpp"
#include "../AliveLibAE/Glukkon.hpp"
#include "../AliveLibAE/GlukkonSwitch.hpp"
#include "../AliveLibAE/GasCountDown.hpp"
#include "../AliveLibAE/FallingItem.hpp"
#include "../AliveLibAE/Bone.hpp"
#include "../AliveLibAE/FootSwitch.hpp"
#include "../AliveLibAE/SlogHut.hpp"
#include "../AliveLibAE/SlogSpawner.hpp"
#include "../AliveLibAE/Scrab.hpp"
#include "../AliveLibAE/ScrabSpawner.hpp"
#include "../AliveLibAE/SlurgSpawner.hpp"
#include "../AliveLibAE/Paramite.hpp"
#include "../AliveLibAE/ParamiteWebLine.hpp"
#include "../AliveLibAE/Meat.hpp"
#include "../AliveLibAE/TorturedMudokon.hpp"

#define CTOR_AE(className, objectTypeName, tlvType) className() : TlvObjectBaseAE(tlvType, objectTypeName) {}  className(TypesCollection& globalTypes, Path_TLV* pTlv = nullptr) : TlvObjectBaseAE(tlvType, objectTypeName, pTlv)

struct Path_Null_63 : public Path_TLV
{
    // TODO: check
    __int16 padding1;
    __int16 padding2;
};

struct Path_MainMenuController : public Path_TLV
{
    // Empty
};

struct Path_SligGetWings : public Path_TLV
{
    Scale_short scale;
    __int16 state;
    __int16 hi_pause_time;
    __int16 patrol_pause_min;
    __int16 patrol_pause_max;
    __int16 direction;
    __int16 panic_delay;
    __int16 give_up_chase_delay;
    __int16 prechase_delay;
    __int16 slig_id;
    __int16 listen_time;
    __int16 trigger_id;
    __int16 grenade_delay;
    __int16 max_velocity;
    __int16 launch_id;
    __int16 persistant;
};

struct Path_SligGetPants : public Path_TLV
{
    // TODO: check
    Scale_short scale;
    __int16 unknown2;
    __int16 unknown3;
    __int16 unknown4;
    __int16 unknown5;
    __int16 unknown6;
    __int16 unknown7;
    __int16 unknown8;
    __int16 unknown9;
    __int16 unknown10;
    __int16 unknown11;
    __int16 unknown12;
    __int16 unknown13;
    __int16 unknown14;
    __int16 unknown15;
    __int16 unknown16;
    __int16 unknown17;
    __int16 unknown18;
    __int16 unknown19;
    __int16 unknown20;
    __int16 unknown21;
    __int16 unknown22;
    __int16 unknown23;
    __int16 unknown24;
    __int16 unknown25;
    __int16 unknown26;
    __int16 unknown27;
    __int16 unknown28;
    __int16 unknown29;
    __int16 unknown30;
    __int16 unknown31;
    __int16 unknown32;
};

struct Path_MovieHandstone : public Path_TLV
{
    __int16 padding;
    Scale_short scale;
    __int16 movie_number;
    __int16 id;
};

struct Path_CreditsController : public Path_TLV
{
    // Empty
};

struct Path_ScrabLeftBound : public Path_TLV
{
    // Empty
};

struct Path_ScrabRightBound : public Path_TLV
{
    // Empty
};

struct Path_DemoSpawnPoint : public Path_TLV
{
    // Empty
};

struct Path_PathTransition : public Path_TLV
{
    __int16 level;
    __int16 path;
    __int16 camera;
    __int16 movie;
    __int16 wipe;
    Scale_short scale;
};

struct Path_ZSligCover : public Path_TLV
{
    // Empty
};

struct Path_SligSpawner : public Path_TLV
{
    Scale_short scale;
    __int16 start_state;
    __int16 pause_time;
    __int16 pause_left_min;
    __int16 pause_left_max;
    __int16 pause_right_min;
    __int16 pause_right_max;
    __int16 chal_number;
    __int16 chal_timer;
    __int16 number_of_times_to_shoot;
    __int16 unknown;
    __int16 code_1;
    __int16 code_2;
    __int16 chase_abe;
    __int16 start_direction;
    __int16 panic_timeout;
    __int16 num_panic_sounds;
    __int16 panic_sound_timeout;
    __int16 stop_chase_delay;
    __int16 time_to_wait_before_chase;
    __int16 slig_id;
    __int16 listen_time;
    __int16 percent_say_what;
    __int16 percent_beat_mud;
    __int16 talk_to_abe;
    __int16 dont_shoot;
    __int16 z_shoot_delay;
    __int16 stay_awake;
    __int16 disable_resources;
    __int16 noise_wake_up_distance;
    __int16 id;
    __int16 spawn_many;
};

struct Path_DeathDrop : public Path_TLV
{
    __int16 field_10_animation;
    __int16 field_12_sound;
    __int16 field_14_id;
    __int16 field_16_action;
    __int16 field_18_unknown1;
    __int16 field_1A_unknown2;
};

struct Path_InvisibleZone : public Path_TLV
{
    // Empty
};

struct Path_Pulley : public Path_TLV
{
    __int16 field_10_unknown1;
    __int16 field_12_unknown2;
};

namespace AETlvs
{
    struct Path_ElectricWall : public TlvObjectBaseAE<::Path_ElectricWall>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::ElectricWallStartState>("Enum_ElectricWallStartState",
                {
                    {::ElectricWallStartState::eOff_0, "Off"},
                    {::ElectricWallStartState::eOn_1, "On"},
                });
        }

        CTOR_AE(Path_ElectricWall, "ElectricWall", TlvTypes::ElectricWall_38)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("ID", mTlv.field_12_id);
            ADD("Start State", mTlv.field_14_start_state);
        }
    };

    struct Path_Mudokon : public TlvObjectBaseAE<::Path_Mudokon>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Mud_TLV_Emotion>("Enum_Mud_TLV_Emotion",
                {
                    {::Mud_TLV_Emotion::eNormal_0, "Normal"},
                    {::Mud_TLV_Emotion::eAngry_1, "Angry"},
                    {::Mud_TLV_Emotion::eSad_2, "Sad"},
                    {::Mud_TLV_Emotion::eWired_3, "Wired"},
                    {::Mud_TLV_Emotion::eSick_4, "Sick"}
                });

            types.AddEnum<::Mud_State>("Enum_Mud_State",
                {
                     {::Mud_State::eChisle_0, "Chisle"},
                     {::Mud_State::eScrub_1, "Scrub"},
                     {::Mud_State::eAngryWorker_2, "Angry Worker"},
                     {::Mud_State::eDamageRingGiver_3, "Damage Ring Giver"},
                     {::Mud_State::eHealthRingGiver_4, "Health Ring Giver"},
                });
        }

        CTOR_AE(Path_Mudokon, "Mudokon", TlvTypes::Mudokon_49)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("State", mTlv.field_12_state);
            ADD("Direction", mTlv.field_14_direction);
            ADD("Voice Pitch", mTlv.field_16_voice_pitch);
            ADD("Rescue ID", mTlv.field_18_rescue_id);
            ADD("Deaf (Unused?)", mTlv.field_1A_bDeaf);
            ADD("Disabled Resources", mTlv.field_1C_disabled_resources);
            ADD("Reset Position On Screen Change", mTlv.field_1E_reset_pos_on_screen_change);
            ADD("Emotion", mTlv.field_20_emotion);
            ADD("Blind", mTlv.field_22_bBlind);
            ADD("Angry Trigger ID", mTlv.field_24_angry_trigger);
            ADD("Stop Trigger ID", mTlv.field_26_stop_turning_work_wheel);
            ADD("Gets Depressed", mTlv.field_28_bGets_depressed);
            ADD("Ring Timeout", mTlv.field_2A_ring_timeout); // TODO: also used for angry worker?
            ADD("Give Ring Without Password", mTlv.field_2C_bGive_ring_without_password);
        }
    };

    struct Path_BirdPortal : public TlvObjectBaseAE<::Path_BirdPortal>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::PortalType>("Enum_PortalType",
                {
                    {::PortalType::eAbe_0, "Abe"},
                    {::PortalType::eWorker_1, "Worker"},
                    {::PortalType::eShrykull_2, "Shrykull"},
                });

            types.AddEnum<::PortalSide>("Enum_PortalSide",
                {
                    {::PortalSide::eRight_0, "Right"},
                    {::PortalSide::eLeft_1, "Left"},
                });
        }

        CTOR_AE(Path_BirdPortal, "BirdPortal", TlvTypes::BirdPortal_28)
        {
            ADD("Side", mTlv.field_10_side);
            ADD("Level Destination", mTlv.field_12_dest_level);
            ADD("Path Destination", mTlv.field_14_dest_path);
            ADD("Camera Destination", mTlv.field_16_dest_camera);
            ADD("Scale", mTlv.field_18_scale);
            ADD("Movie ID", mTlv.field_1A_movie_id);
            ADD("Portal Type", mTlv.field_1C_portal_type);
            ADD("Mudokon Amount For Shrykull", mTlv.field_1E_mudokon_amount_for_shrykull);
            ADD("Portal Create ID (Unused?)", mTlv.field_20_create_id);
            ADD("Portal Delete ID", mTlv.field_22_delete_id);
        }
    };

    struct Path_LCDStatusBoard : public TlvObjectBaseAE<::Path_LCDStatusBoard>
    {
        CTOR_AE(Path_LCDStatusBoard, "LCDStatusBoard", TlvTypes::LCDStatusBoard_64)
        {
            ADD("Number Of Mudokons", mTlv.field_10_number_of_muds);
            ADD("Zulag Number", mTlv.field_12_zulag_number);
            ADD("Hidden", mTlv.field_14_hidden);
        }
    };
    // TODO: finish
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
                    {::SwitchSoundType::eNone, "None"},
                    {::SwitchSoundType::eWell_1, "Well"},
                    {::SwitchSoundType::eUnknown_2, "Unknown"},
                    {::SwitchSoundType::eDoor_3, "Door"},
                    {::SwitchSoundType::eElectricWall_4, "Electric Wall"},
                    {::SwitchSoundType::eSecurityOrb_5, "Security Orb"},
                    {::SwitchSoundType::eLift_6, "Lift"}
                });

            types.AddEnum<::SwitchSoundDirection>("Enum_SwitchSoundDirection",
                {
                    {::SwitchSoundDirection::eLeftAndRight_0, "Left And Right"},
                    {::SwitchSoundDirection::eLeft_1, "Left"},
                    {::SwitchSoundDirection::eRight_2, "Right"},
                });
        }

        CTOR_AE(Path_Switch, "Switch", TlvTypes::Switch_17)
        {
            ADD("Target Action", mTlv.field_10_target_action);
            ADD("Scale", mTlv.field_12_scale);
            ADD("On Sound", mTlv.field_14_on_sound);
            ADD("Off Sound", mTlv.field_16_off_sound);
            ADD("Sound Direction", mTlv.field_18_sound_direction);
            ADD("Trigger ID", mTlv.field_1A_trigger_id);
            ADD("Persist Offscreen", mTlv.field_1C_persist_offscreen);
        }
    };

    struct Path_Hoist : public TlvObjectBaseAE<::Path_Hoist>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_Hoist::Type>("Enum_HoistType",
                {
                    {::Path_Hoist::Type::eNextEdge, "Next Edge"},
                    {::Path_Hoist::Type::eNextFloor, "Next Floor"},
                    {::Path_Hoist::Type::eOffScreen, "Off Screen"},
                });

            types.AddEnum<::Path_Hoist::GrabDirection>("Enum_HoistGrabDirection",
                {
                    {::Path_Hoist::GrabDirection::eBoth, "Both"},
                    {::Path_Hoist::GrabDirection::eLeft, "Left"},
                    {::Path_Hoist::GrabDirection::eRight, "Right"},
                });
        }

        CTOR_AE(Path_Hoist, "Hoist", TlvTypes::Hoist_2)
        {
            ADD("Hoist Type", mTlv.field_10_type);
            ADD("Grab Direction", mTlv.field_12_grab_direction);
            ADD("ID", mTlv.field_14_id);
            ADD("Scale", mTlv.field_16_scale);
        }
    };

    struct Path_BoomMachine : public TlvObjectBaseAE<::Path_BoomMachine>
    {
        CTOR_AE(Path_BoomMachine, "BoomMachine", TlvTypes::BoomMachine_59)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Nozzle Side", mTlv.field_12_nozzle_side);
            ADD("Disabled Resources", mTlv.field_14_disabled_resources);
            ADD("Number Of Grenades", mTlv.field_16_number_of_grenades);
        }
    };

    struct Path_Slig : public TlvObjectBaseAE<::Path_Slig>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_Slig::StartState>("Enum_SligStartState",
                {
                    {::Path_Slig::StartState::Listening_0, "Listening"},
                    {::Path_Slig::StartState::Paused_1, "Paused"},
                    {::Path_Slig::StartState::Sleeping_2, "Sleeping"},
                    {::Path_Slig::StartState::Chase_3, "Chase"},
                    {::Path_Slig::StartState::RunOffScreen_4, "Run Off Screen"},
                    {::Path_Slig::StartState::GameEnder_5, "Game Ender"},
                    {::Path_Slig::StartState::ListeningToGlukkon_6, "Listen To Glukkon"},
                });
        }

        CTOR_AE(Path_Slig, "Slig", TlvTypes::Slig_15)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Start State", mTlv.field_12_start_state);
            ADD("Pause Time (Frames)", mTlv.field_14_pause_time);
            ADD("Pause Left Min (Frames)", mTlv.field_16_pause_left_min);
            ADD("Pause Left Max (Frames)", mTlv.field_18_pause_left_max);
            ADD("Pause Right Min (Frames)", mTlv.field_1A_pause_right_min);
            ADD("Pause Right Max (Frames)", mTlv.field_1C_pause_right_max);
            ADD("chal_number", mTlv.field_1E_chal_number);
            ADD("chal_timer", mTlv.field_20_chal_timer);
            ADD("Bullet Shoot Count", mTlv.field_22_num_times_to_shoot);
            ADD("unknown1", mTlv.field_24_padding);

            ADD("Code 1", mTlv.field_26_code1);
            ADD("Code 2", mTlv.field_28_code2);
            ADD("Chase Abe When Spotted", mTlv.field_2A_chase_abe_when_spotted);
            ADD("Start Direction", mTlv.field_2C_start_direction);
            ADD("Panic Timeout", mTlv.field_2E_panic_timeout);
            ADD("Amount Of Panic Sounds (Unused?)", mTlv.field_30_num_panic_sounds);
            ADD("Panic Sound Timeout (Unused?)", mTlv.field_32_panic_sound_timeout);
            ADD("Stop Chase Delay (Frames)", mTlv.field_34_stop_chase_delay);
            ADD("Time To Wait Before Chase (Frames)", mTlv.field_36_time_to_wait_before_chase);
            ADD("Slig ID", mTlv.field_38_slig_id);
            ADD("Listen Time (Frames)", mTlv.field_3A_listen_time);

            ADD("Percent Say What", mTlv.field_3C_percent_say_what);
            ADD("Percent Beat Mudokon", mTlv.field_3E_percent_beat_mud);
            ADD("Talk To Abe (Unused?)", mTlv.field_40_talk_to_abe);
            ADD("Don't Shoot (Unused?)", mTlv.field_42_dont_shoot);
            ADD("Z Shoot Delay (Frames)", mTlv.field_44_Z_shoot_delay);
            ADD("Stay Awake", mTlv.field_46_stay_awake);
            ADD("Disable Resources", mTlv.field_48_disable_resources);
            ADD("Noise Wake Up Distance (Grids)", mTlv.field_4A_noise_wake_up_distance);
            ADD("ID", mTlv.field_4C_id);
            ADD("unknown2", mTlv.field_4E_unknown);
        }
    };

    struct Path_Fleech : public TlvObjectBaseAE<::Path_Fleech>
    {
        CTOR_AE(Path_Fleech, "Fleech", TlvTypes::Fleech_83)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Direction", mTlv.field_12_direction);
            ADD("Asleep", mTlv.field_14_asleep);
            ADD("Wake Up (Unused?)", mTlv.field_16_wake_up);
            ADD("unknown1", mTlv.field_18_not_used);

            ADD("Attack Anger Increaser", mTlv.field_1A_attack_anger_increaser);
            ADD("Attack Delay (Unused?)", mTlv.field_1C_attack_delay);
            ADD("Wake Up ID 1", mTlv.field_1E_wake_up_id1);
            ADD("Hanging", mTlv.field_20_hanging);

            ADD("Lost Target Timeout", mTlv.field_22_lost_target_timeout);
            ADD("Goes To Sleep", mTlv.field_24_goes_to_sleep);
            ADD("Patrol Range (Grids)", mTlv.field_26_patrol_range_in_grids);
            ADD("unknown2", mTlv.field_28_unused);

            ADD("Wake Up ID 2", mTlv.field_2A_wake_up_id2);
            ADD("Persistant", mTlv.field_2C_persistant);
            ADD("unknown3", mTlv.field_2E_padding);

        }
    };

    struct Path_EnemyStopper : public TlvObjectBaseAE<::Path_EnemyStopper>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_EnemyStopper::StopDirection>("Enum_StopDirection",
                {
                    {::Path_EnemyStopper::StopDirection::Left_0, "Left"},
                    {::Path_EnemyStopper::StopDirection::Right_1, "Right"},
                    {::Path_EnemyStopper::StopDirection::Both_2, "Both"},
                });
        }

        CTOR_AE(Path_EnemyStopper, "EnemyStopper", TlvTypes::EnemyStopper_47)
        {
            ADD("Stop Direction", mTlv.field_10_stop_direction);
            ADD("ID", mTlv.field_12_id);
        }
    };

    struct Path_Teleporter : public TlvObjectBaseAE<::Path_Teleporter>
    {
        CTOR_AE(Path_Teleporter, "Teleporter", TlvTypes::Teleporter_88)
        {
            ADD("ID", mTlv.field_10_data.field_10_id);
            ADD("Target ID", mTlv.field_10_data.field_12_target_id);
            ADD("Camera", mTlv.field_10_data.field_14_camera);
            ADD("Path", mTlv.field_10_data.field_16_path);
            ADD("Level", mTlv.field_10_data.field_18_level);
            ADD("Trigger ID", mTlv.field_10_data.field_1A_trigger_id);
            ADD("Scale", mTlv.field_10_data.field_1C_scale);
            ADD("Camera Swap Effect", mTlv.field_10_data.field_1E_cam_swap_effect);
            ADD("Movie Number", mTlv.field_10_data.field_20_movie_number);
            ADD("Electric X", mTlv.field_10_data.field_22_eletric_x);
            ADD("Electric Y", mTlv.field_10_data.field_24_electric_y);
        }
    };

    struct Path_UXB : public TlvObjectBaseAE<::Path_UXB>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_UXB::StartState>("Enum_Path_UXB_StartState",
                {
                    {::Path_UXB::StartState::eOn_0, "On"},
                    {::Path_UXB::StartState::eOff_1, "Off"},
                });
        }

        CTOR_AE(Path_UXB, "UXB", TlvTypes::UXB_25)
        {
            ADD("Pattern Length (Max 4)", mTlv.field_10_pattern_length);
            ADD("Pattern", mTlv.field_12_pattern);
            ADD("Scale", mTlv.field_14_scale);
            ADD("Start State", mTlv.field_16_start_state);
            ADD("Disabled Resources", mTlv.field_18_disabled_resources);
        }
    };

    struct Path_LCDScreen : public TlvObjectBaseAE<::Path_LCDScreen>
    {
        CTOR_AE(Path_LCDScreen, "LCDScreen", TlvTypes::LCD_60)
        {
            ADD("Message 1 ID", mTlv.field_10_message_1_id);
            ADD("Random Message Min ID", mTlv.field_12_message_rand_min_id);
            ADD("Random Message Max ID", mTlv.field_14_message_rand_max_id);
            ADD("Message 2 ID", mTlv.field_16_message_2_id);
            ADD("Toggle Message Switch ID", mTlv.field_18_toggle_message_switch_id);
        }
    };

    struct Path_Edge : public TlvObjectBaseAE<::Path_Edge>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_Edge::GrabDirection>("Enum_EdgeGrabDirection",
                {
                    {::Path_Edge::GrabDirection::eLeft, "Left"},
                    {::Path_Edge::GrabDirection::eRight, "Right"},
                    {::Path_Edge::GrabDirection::eBoth, "Both"},
                });
        }

        CTOR_AE(Path_Edge, "Edge", TlvTypes::Edge_3)
        {
            ADD("Grab Direction", mTlv.field_10_grab_direction);
            ADD("Can Grab", mTlv.field_12_bCan_grab);
            ADD("Scale", mTlv.field_14_scale);
        }
    };

    struct Path_StatusLight : public TlvObjectBaseAE<::Path_StatusLight>
    {
        CTOR_AE(Path_StatusLight, "StatusLight", TlvTypes::StatusLight_97)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Scale", mTlv.field_12_scale);
            ADD("ID 1", mTlv.field_14_id1);
            ADD("ID 2", mTlv.field_16_id2);
            ADD("ID 3", mTlv.field_18_id3);
            ADD("ID 4", mTlv.field_1A_id4);
            ADD("ID 5", mTlv.field_1C_id5);
            ADD("Ignore Grid Snapping", mTlv.field_1E_bIgnore_grid_snapping);
        }
    };

    struct Path_ShadowZone : public TlvObjectBaseAE<::Path_ShadowZone>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::ShadowZoneScale>("Enum_ShadowZoneScale",
                {
                    {::ShadowZoneScale::eBoth_0, "Both"},
                    {::ShadowZoneScale::eHalf_1, "Half"},
                    {::ShadowZoneScale::eFull_2, "Full"},
                });
        }

        CTOR_AE(Path_ShadowZone, "ShadowZone", TlvTypes::ShadowZone_6)
        {
            ADD("Center W (Unused?)", mTlv.field_10_center_w);
            ADD("Center H (Unused?)", mTlv.field_12_center_h);
            ADD("R", mTlv.field_14_r);
            ADD("G", mTlv.field_16_g);
            ADD("B", mTlv.field_18_b);
            ADD("ID", mTlv.field_1A_id);
            ADD("Scale", mTlv.field_1C_scale);
        }
    };

    struct Path_WorkWheel : public TlvObjectBaseAE<::Path_WorkWheel>
    {
        CTOR_AE(Path_WorkWheel, "WorkWheel", TlvTypes::WorkWheel_79)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("ID", mTlv.field_12_id);
            ADD("Activation Time (Frames)", mTlv.field_14_activation_time);
            ADD("Off Time (Frames)", mTlv.field_16_off_time);
            ADD("Turn Off When Stopped", mTlv.field_18_turn_off_when_stopped);
        }
    };

    struct Path_MusicTrigger : public TlvObjectBaseAE<::Path_MusicTrigger>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::MusicTriggerMusicType>("Enum_MusicTriggerMusicType",
                {
                    {::MusicTriggerMusicType::eUnknown_0, "Unknown 0"},
                    {::MusicTriggerMusicType::eDeathShort_1, "Death Short"},
                    {::MusicTriggerMusicType::eSecretAreaLong_2, "Secret Area Long"},
                    {::MusicTriggerMusicType::eChase_3, "Chase"},
                    {::MusicTriggerMusicType::eSlogChase_4, "Slog Chase"},
                    {::MusicTriggerMusicType::eChime_5, "Chime"},
                    {::MusicTriggerMusicType::eSecretAreaShort_6, "Secret Area Short"},
                });

            types.AddEnum<::TriggeredBy>("Enum_MusicTriggeredBy",
                {
                    {::TriggeredBy::eTimer_0, "Timer"},
                    {::TriggeredBy::eTouching_1, "Touching"},
                });
        }

        CTOR_AE(Path_MusicTrigger, "MusicTrigger", TlvTypes::MusicTrigger_66)
        {
            ADD("Music Type", mTlv.field_10_music_type);
            ADD("Triggered By", mTlv.field_12_triggered_by);
            ADD("Music Delay (Frames)", mTlv.field_14_music_delay);
        }
    };

    struct Path_AbeStart : public TlvObjectBaseAE<::Path_AbeStart>
    {
        CTOR_AE(Path_AbeStart, "AbeStart", TlvTypes::AbeStart_22)
        {
            ADD("X Position", mTlv.field_10_xpos);
            ADD("Y Position", mTlv.field_12_ypos);
        }
    };

    struct Path_SoftLanding : public TlvObjectBaseAE<::Path_SoftLanding>
    {
        CTOR_AE(Path_SoftLanding, "SoftLanding", TlvTypes::SoftLanding_75)
        {
            ADD("ID", mTlv.field_10_id);
        }
    };

    struct Path_WellExpress : public TlvObjectBaseAE<::Path_WellExpress>
    {
        CTOR_AE(Path_WellExpress, "WellExpress", TlvTypes::WellExpress_23)
        {
            // Path_WellBase
            ADD("Scale", mTlv.field_0_scale);
            ADD("Trigger ID", mTlv.field_2_trigger_id);
            ADD("Well ID", mTlv.field_4_well_id);
            ADD("Resource ID", mTlv.field_6_res_id);

            // Path_WellExpress
            ADD("Exit X", mTlv.field_18_exit_x);
            ADD("Exit Y", mTlv.field_1A_exit_y);
            ADD("Disabled Well Level", mTlv.field_1C_disabled_well_level);
            ADD("Disabled Well Path", mTlv.field_1E_disabled_well_path);
            ADD("Disabled Well Camera", mTlv.field_20_disabled_well_camera);
            ADD("Disabled Well ID", mTlv.field_22_disabled_well_id);
            ADD("Enabled Well Level", mTlv.field_24_enabled_well_level);
            ADD("Enabled Well Path", mTlv.field_26_enabled_well_path);
            ADD("Enabled Well Camera", mTlv.field_28_enabled_well_camera);
            ADD("Enabled Well ID", mTlv.field_2A_enabled_well_id);
            ADD("Emit Leaves", mTlv.field_2C_bEmit_leaves);
            ADD("Leaf X", mTlv.field_2E_leaf_x);
            ADD("Leaf Y", mTlv.field_30_leaf_y);
            ADD("Movie ID", mTlv.field_32_movie_id);
        }
    };

    struct Path_SlamDoor : public TlvObjectBaseAE<::Path_SlamDoor>
    {
        CTOR_AE(Path_SlamDoor, "SlamDoor", TlvTypes::SlamDoor_85)
        {
            ADD("Start Shut", mTlv.field_10_bStart_closed);
            ADD("Scale", mTlv.field_12_scale);
            ADD("ID", mTlv.field_14_id);
            ADD("Start Inverted", mTlv.field_16_bStart_inverted);
            ADD("Delete", mTlv.field_18_bDelete);
        }
    };

    struct Path_HandStone : public TlvObjectBaseAE<::Path_HandStone>
    {
        CTOR_AE(Path_HandStone, "HandStone", TlvTypes::HandStone_61)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Camera ID 1", mTlv.field_12_camera_id1);
            ADD("Camera ID 2", mTlv.field_12_camera_id2);
            ADD("Camera ID 3", mTlv.field_12_camera_id3);
            ADD("Trigger ID", mTlv.field_18_trigger_id);
        }
    };

    struct Path_LaughingGas : public TlvObjectBaseAE<::Path_LaughingGas>
    {
        CTOR_AE(Path_LaughingGas, "LaughingGas", TlvTypes::LaughingGas_81)
        {
            ADD("Is Laughing Gas", mTlv.field_10_bLaughing_gas);
            ADD("Laughing Gas ID", mTlv.field_12_laughing_gas_id);
            ADD("Red %", mTlv.field_14_red_percent);
            ADD("Green %", mTlv.field_16_green_percent);
            ADD("Blue %", mTlv.field_18_blue_percent);
        }
    };

    struct Path_InvisibleSwitch : public TlvObjectBaseAE<::Path_InvisibleSwitch>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::InvisibleSwitchScale>("Enum_InvisibleSwitchScale",
                {
                    {::InvisibleSwitchScale::eHalf_0, "Half"},
                    {::InvisibleSwitchScale::eFull_1, "Full"},
                    {::InvisibleSwitchScale::eAny_2, "Any"},
                });
        }

        CTOR_AE(Path_InvisibleSwitch, "InvisibleSwitch", TlvTypes::InvisibleSwitch_48)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Action", mTlv.field_12_action);
            ADD("Delay (Frames)", mTlv.field_14_delay);
            ADD("Set Off Alarm", mTlv.field_16_set_off_alarm);
            ADD("Scale", mTlv.field_18_scale);
        }
    };

    struct Path_Water : public TlvObjectBaseAE<::Path_Water>
    {
        CTOR_AE(Path_Water, "Water", TlvTypes::Water_77)
        {
            ADD("Max Drops", mTlv.field_10_data.field_10_max_drops);
            ADD("ID", mTlv.field_10_data.field_12_id);
            ADD("Splash Time (Frames)", mTlv.field_10_data.field_14_splash_time);
            ADD("Splash X Velocity", mTlv.field_10_data.field_16_splash_x_velocity);
            ADD("Splash Y Velocity", mTlv.field_10_data.field_18_splash_y_velocity);
            ADD("Water Duration (Frames)", mTlv.field_10_data.field_1A_water_duration);
        }
    };

    struct Path_GasEmitter : public TlvObjectBaseAE<::Path_GasEmitter>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::GasColour>("Enum_GasColour",
                {
                    {::GasColour::Yellow_0, "Yellow"},
                    {::GasColour::Red_1, "Red"},
                    {::GasColour::Green_2, "Green"},
                    {::GasColour::Blue_3, "Blue"},
                    {::GasColour::White_4, "White"},
                });
        }

        CTOR_AE(Path_GasEmitter, "GasEmitter", TlvTypes::GasEmitter_71)
        {
            ADD("Switch ID", mTlv.field_10_switch_id);
            ADD("Colour", mTlv.field_12_colour);
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
                    {::Layer::eLayer_2, "Layer_2"},
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
                    {::Layer::eLayer_19, "Layer_19"},
                    {::Layer::eLayer_20, "Layer_20"},
                    {::Layer::eLayer_21, "Layer_21"},
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
            ADD("Resource ID", mTlv.field_10_res_id);
            ADD("Is Semi Trans", mTlv.field_12_is_semi_trans);
            ADD("Semi Trans Mode", mTlv.field_14_semi_trans_mode);
            ADD("Sound Effect (Unused?)", mTlv.field_16_sound_effect);
            ADD("ID", mTlv.field_18_id);
            ADD("Layer", mTlv.field_1A_layer);
        }
    };

    struct Path_LiftPoint : public TlvObjectBaseAE<::Path_LiftPoint>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::LiftPointStopType>("Enum_LiftPointStopType",
                {
                    {::LiftPointStopType::eTopFloor_0, "Top Floor"},
                    {::LiftPointStopType::eBottomFloor_1, "Bottom Floor"},
                    {::LiftPointStopType::eMiddleFloor_2, "Middle Floor"},
                    {::LiftPointStopType::eMiddleLockFloor_3, "Middle Lock Floor"},
                    {::LiftPointStopType::eStartPointOnly_4, "Start Point Only"},
                });
        }

        CTOR_AE(Path_LiftPoint, "LiftPoint", TlvTypes::LiftPoint_7)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Start Point", mTlv.field_12_bStart_point);
            ADD("Lift Type (Unused?)", mTlv.field_14_lift_type);
            ADD("Lift Point Stop Type", mTlv.field_16_lift_point_stop_type);
            ADD("Scale", mTlv.field_18_scale);
            ADD("Ignore Lift Mover", mTlv.field_1A_bIgnore_lift_mover);
        }
    };

    struct Path_PullRingRope : public TlvObjectBaseAE<::Path_PullRingRope>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::PullRingSwitchSound>("Enum_PullRingSwitchSound",
                {
                    {::PullRingSwitchSound::eNone_0, "None"},
                    {::PullRingSwitchSound::eWellExit_1, "Well Exit"},
                    {::PullRingSwitchSound::eRingUnknownTrigger_2, "Ring Unknown Trigger"},
                    {::PullRingSwitchSound::eDoorEffect_3, "Door Effect"},
                });

            types.AddEnum<::PullRingSoundDirection>("Enum_PullRingSoundDirection",
                {
                    {::PullRingSoundDirection::eLeftAndRight_0, "Left And Right"},
                    {::PullRingSoundDirection::eLeft_1, "Left"},
                    {::PullRingSoundDirection::eRight_2, "Right"},
                });
        }

        CTOR_AE(Path_PullRingRope, "PullRingRope", TlvTypes::PullRingRope_12)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Target Action", mTlv.field_12_target_action);
            ADD("Rope Length", mTlv.field_14_rope_length);
            ADD("Scale", mTlv.field_16_scale);
            ADD("On Sound", mTlv.field_18_on_sound);
            ADD("Off Sound", mTlv.field_1A_off_sound);
            ADD("Sound Direction", mTlv.field_1C_sound_direction);
        }
    };

    struct Path_MultiSwitchController : public TlvObjectBaseAE<::Path_MultiSwitchController>
    {
        CTOR_AE(Path_MultiSwitchController, "MultiSwitchController", TlvTypes::MultiSwitchController_96)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Operation", mTlv.field_12_operation);
            ADD("Delay", mTlv.field_14_delay);
            ADD("ID 1", mTlv.field_16_id1);
            ADD("ID 2", mTlv.field_18_id2);
            ADD("ID 3", mTlv.field_1A_id3);
            ADD("ID 4", mTlv.field_1C_id4);
            ADD("ID 5", mTlv.field_1E_id5);
            ADD("ID 6", mTlv.field_20_id6);
        }
    };

    struct Path_SecurityOrb : public TlvObjectBaseAE<::Path_SecurityOrb>
    {
        CTOR_AE(Path_SecurityOrb, "SecurityOrb", TlvTypes::SecurityOrb_19)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Disabled Resources", mTlv.field_12_disabled_resources);
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
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_ContinuePoint::Scale>("Enum_ContinuePointScale",
                {
                    {::Path_ContinuePoint::Scale::eNone_0, "None"},
                    {::Path_ContinuePoint::Scale::eHalf_1, "Half"},
                    {::Path_ContinuePoint::Scale::eFull_2, "Full"},
                });
        }

        CTOR_AE(Path_ContinuePoint, "ContinuePoint", TlvTypes::ContinuePoint_0)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Save File ID", mTlv.field_12_save_file_id);
        }
    };

    struct Path_WheelSyncer : public TlvObjectBaseAE<::Path_WheelSyncer>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::WheelSyncerAction>("Enum_WheelSyncerAction",
                {
                    {::WheelSyncerAction::eAllOn_0, "All On"},
                    {::WheelSyncerAction::eOff_1, "Off"},
                    {::WheelSyncerAction::eToggle_2, "Toggle"},
                    {::WheelSyncerAction::eOn_3, "On"},
                });
        }

        CTOR_AE(Path_WheelSyncer, "WheelSyncer", TlvTypes::WheelSyncer_65)
        {
            ADD("ID 1", mTlv.field_10_id1);
            ADD("ID 2", mTlv.field_12_id2);
            ADD("Trigger ID", mTlv.field_14_trigger_id);
            ADD("Action", mTlv.field_16_action);
            ADD("ID 3", mTlv.field_18_id3);
            ADD("ID 4", mTlv.field_1A_id4);
            ADD("ID 5", mTlv.field_1C_id5);
            ADD("ID 6", mTlv.field_1E_id6);
        }
    };

    struct Path_LevelLoader : public TlvObjectBaseAE<::Path_LevelLoader>
    {
        CTOR_AE(Path_LevelLoader, "LevelLoader", TlvTypes::LevelLoader_86)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Destination Level", mTlv.field_12_dest_level);
            ADD("Destination Path", mTlv.field_14_dest_path);
            ADD("Destination Camera", mTlv.field_16_dest_camera);
            ADD("Movie ID", mTlv.field_18_movie_id);
        }
    };
    // TODO: finish
    struct Path_Pulley : public TlvObjectBaseAE<::Path_Pulley>
    {
        CTOR_AE(Path_Pulley, "Pulley", TlvTypes::Pulley_21)
        {
            ADD("unknown1", mTlv.field_10_unknown1);
            ADD("unknown2", mTlv.field_12_unknown2);
        }
    };
    // TODO: finish
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
    // TODO: finish
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
    // TODO: finish
    struct Path_DeathDrop : public TlvObjectBaseAE<::Path_DeathDrop>
    {
        CTOR_AE(Path_DeathDrop, "DeathDrop", TlvTypes::DeathDrop_4)
        {
            ADD("not_used1", mTlv.field_10_animation);
            ADD("not_used2", mTlv.field_12_sound);
            ADD("not_used3", mTlv.field_14_id);
            ADD("not_used4", mTlv.field_16_action);
            ADD("not_used5", mTlv.field_18_unknown1);
            ADD("not_used6", mTlv.field_1A_unknown2);
        }
    };
    // TODO: finish
    struct Path_SligSpawner : public TlvObjectBaseAE<::Path_SligSpawner>
    {
        CTOR_AE(Path_SligSpawner, "SligSpawner", TlvTypes::SligSpawner_37)
        {
            ADD("scale", mTlv.scale);
            ADD("start_state", mTlv.start_state);
            ADD("pause_time", mTlv.pause_time);
            ADD("pause_left_min", mTlv.pause_left_min);
            ADD("pause_left_max", mTlv.pause_left_max);
            ADD("pause_right_min", mTlv.pause_right_min);
            ADD("pause_right_max", mTlv.pause_right_max);
            ADD("chal_number", mTlv.chal_number);
            ADD("chal_timer", mTlv.chal_timer);
            ADD("number_of_times_to_shoot", mTlv.number_of_times_to_shoot);
            ADD("unknown", mTlv.unknown);
            ADD("code_1", mTlv.code_1);
            ADD("code_2", mTlv.code_2);
            ADD("chase_abe", mTlv.chase_abe);
            ADD("start_direction", mTlv.start_direction);
            ADD("panic_timeout", mTlv.panic_timeout);
            ADD("num_panic_sounds", mTlv.num_panic_sounds);
            ADD("panic_sound_timeout", mTlv.panic_sound_timeout);
            ADD("stop_chase_delay", mTlv.stop_chase_delay);
            ADD("time_to_wait_before_chase", mTlv.time_to_wait_before_chase);
            ADD("slig_id", mTlv.slig_id);
            ADD("listen_time", mTlv.listen_time);
            ADD("percent_say_what", mTlv.percent_say_what);
            ADD("percent_beat_mud", mTlv.percent_beat_mud);
            ADD("talk_to_abe", mTlv.talk_to_abe);
            ADD("dont_shoot", mTlv.dont_shoot);
            ADD("z_shoot_delay", mTlv.z_shoot_delay);
            ADD("stay_awake", mTlv.stay_awake);
            ADD("disable_resources", mTlv.disable_resources);
            ADD("noise_wake_up_distance", mTlv.noise_wake_up_distance);
            ADD("id", mTlv.id);
            ADD("spawn_many", mTlv.spawn_many);
        }
    };

    struct Path_SligLeftBound : public TlvObjectBaseAE<::Path_SligLeftBound>
    {
        CTOR_AE(Path_SligLeftBound, "SligLeftBound", TlvTypes::SligBoundLeft_32)
        {
            ADD("Slig ID", mTlv.field_10_slig_id);
            ADD("Disabled Resources", mTlv.field_12_disabled_resources);
        }
    };

    struct Path_SligRightBound : public TlvObjectBaseAE<::Path_SligRightBound>
    {
        CTOR_AE(Path_SligRightBound, "SligRightBound", TlvTypes::SligBoundRight_45)
        {
            ADD("Slig ID", mTlv.field_10_slig_id);
            ADD("Disabled Resources", mTlv.field_12_disabled_resources);
        }
    };

    struct Path_SligPersist : public TlvObjectBaseAE<::Path_SligPersist>
    {
        CTOR_AE(Path_SligPersist, "SligPersist", TlvTypes::SligPersist_46)
        {
            ADD("Slig ID", mTlv.field_10_slig_id);
            ADD("Disabled Resources", mTlv.field_12_disabled_resources);
        }
    };

    struct Path_ZSligCover : public TlvObjectBaseAE<::Path_ZSligCover>
    {
        CTOR_AE(Path_ZSligCover, "ZSligCover", TlvTypes::ZSligCover_50)
        {
            // Empty
        }
    };
    // TODO: finish
    struct Path_WellLocal : public TlvObjectBaseAE<::Path_WellLocal>
    {
        CTOR_AE(Path_WellLocal, "WellLocal", TlvTypes::LocalWell_8)
        {
            // Path_WellBase
            ADD("scale", mTlv.field_0_scale);
            ADD("trigger_id", mTlv.field_2_trigger_id);
            ADD("well_id", mTlv.field_4_well_id);
            ADD("resource_id", mTlv.field_6_res_id);

            // Path_WellLocal
            ADD("off_dx", mTlv.field_18_off_dx);
            ADD("off_dy", mTlv.field_1A_off_dy);
            ADD("on_dx", mTlv.field_1C_on_dx);
            ADD("on_dy", mTlv.field_1E_on_dy);
            ADD("emit_leaves", mTlv.field_20_bEmit_leaves);
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
                    {::GrinderBehavior::eNotInteractable_0, "Not Interactable"},
                    {::GrinderBehavior::eToggle_1, "Toggle"},
                    {::GrinderBehavior::eUse_2, "Use"},
                });

            types.AddEnum<::GrinderDirection>("Enum_GrinderDirection",
                {
                    {::GrinderDirection::eDown_0, "Down"},
                    {::GrinderDirection::eRight_1, "Right"},
                    {::GrinderDirection::eLeft_2, "Left"},
                });
        }

        CTOR_AE(Path_Grinder, "Grinder", TlvTypes::Grinder_90)
        {
            ADD("Scale", mTlv.field_10_data.field_10_scale);
            ADD("Min Off Time", mTlv.field_10_data.field_12_min_off_time);
            ADD("Max Off Time", mTlv.field_10_data.field_14_max_off_time);
            ADD("ID", mTlv.field_10_data.field_16_id);
            ADD("Behavior", mTlv.field_10_data.field_18_behavior);
            ADD("Speed", mTlv.field_10_data.field_1A_speed);
            ADD("Start State On", mTlv.field_10_data.field_1C_bStart_state_on);
            ADD("Off Speed", mTlv.field_10_data.field_1E_off_speed);
            ADD("Min Off Time Speed Change", mTlv.field_10_data.field_20_min_off_time_speed_change);
            ADD("Max Off Time Speed Change", mTlv.field_10_data.field_22_max_off_time_speed_change);
            ADD("Start Position Bottom", mTlv.field_10_data.field_24_bStart_position_bottom);
            ADD("Direction", mTlv.field_10_data.field_26_direction);
        }
    };

    struct Path_Mine : public TlvObjectBaseAE<::Path_Mine>
    {
        CTOR_AE(Path_Mine, "Mine", TlvTypes::Mine_24)
        {
            ADD("Num Patterns (Unused?)", mTlv.field_10_num_patterns);
            ADD("Pattern (Unused?)", mTlv.field_12_pattern);
            ADD("Scale", mTlv.field_14_scale);
            ADD("Disabled Resources", mTlv.field_16_disabled_resources);
            ADD("Persist Offscreen", mTlv.field_18_persist_offscreen);
        }
    };
    // TODO: finish
    struct Path_Slog : public TlvObjectBaseAE<::Path_Slog>
    {
        CTOR_AE(Path_Slog, "Slog", TlvTypes::Slog_16)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("direction", mTlv.field_12_direction);
            ADD("asleep", mTlv.field_14_asleep);
            ADD("wake_up_anger", mTlv.field_16_wake_up_anger);
            ADD("bark_anger", mTlv.field_18_bark_anger);
            ADD("chase_anger", mTlv.field_1A_chase_anger);
            ADD("jump_delay", mTlv.field_1C_jump_delay);
            ADD("disabled_resources", mTlv.field_1E_disabled_resources);
            ADD("angry_id", mTlv.field_20_angry_id);
            ADD("bone_eating_time", mTlv.field_22_bone_eating_time);
        }
    };
    // TODO: finish
    struct Path_ResetSwitchRange : public TlvObjectBaseAE<::Path_ResetSwitchRange>
    {
        CTOR_AE(Path_ResetSwitchRange, "ResetSwitchRange", TlvTypes::ResetSwitchRange_76)
        {
             ADD("set_switches", mTlv.field_10_set_switches);
             ADD("start_id", mTlv.field_12_start_id);
             ADD("end_id", mTlv.field_14_end_id);
             ADD("skip_id", mTlv.field_16_skip_id);
             ADD("free_path_resource", mTlv.field_18_free_path_res);
             ADD("path_resource_to_free_id", mTlv.field_1A_path_to_free_id);
             ADD("enabled", mTlv.field_1C_bEnabled);
        }
    };

    struct Path_TrapDoor : public TlvObjectBaseAE<::Path_TrapDoor>
    {
        CTOR_AE(Path_TrapDoor, "TrapDoor", TlvTypes::TrapDoor_30)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Start State", mTlv.field_12_start_state); // TODO: enum
            ADD("Self Closing", mTlv.field_14_self_closing);
            ADD("Scale", mTlv.field_16_scale);
            ADD("Dest Level (Unused?)", mTlv.field_18_dest_level);
            ADD("Direction", mTlv.field_1A_direction);
            ADD("X Offset", mTlv.field_1C_xOff);
            ADD("stay_open_time", mTlv.field_1E_stay_open_time);
        }
    };

    struct Path_PathTransition : public TlvObjectBaseAE<::Path_PathTransition>
    {
        CTOR_AE(Path_PathTransition, "PathTransition", TlvTypes::PathTransition_1)
        {
            ADD("Level", mTlv.level);
            ADD("Path", mTlv.path);
            ADD("Camera", mTlv.camera);
            ADD("Movie", mTlv.movie);
            ADD("Wipe", mTlv.wipe);
            ADD("Scale", mTlv.scale);
        }
    };

    struct Path_LiftMover : public TlvObjectBaseAE<::Path_LiftMover>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_LiftMover::YDirection>("Enum_LiftMoverYDirection",
                {
                    {::Path_LiftMover::YDirection::eDown_0, "Down"},
                    {::Path_LiftMover::YDirection::eUp_1, "Up"},
                });
        }

        CTOR_AE(Path_LiftMover, "LiftMover", TlvTypes::LiftMover_39)
        {
            ADD("Switch ID", mTlv.field_10_switch_id);
            ADD("Lift ID", mTlv.field_12_lift_id);
            ADD("Direction", mTlv.field_14_direction);
        }
    };

    struct Path_RockSack : public TlvObjectBaseAE<::Path_RockSack>
    {
        CTOR_AE(Path_RockSack, "RockSack", TlvTypes::RockSack_10)
        {
            ADD("Rock Fall Direction", mTlv.field_10_fall_direction);
            ADD("X Velocity", mTlv.field_12_x_vel);
            ADD("Y Velocity", mTlv.field_14_y_vel);
            ADD("Scale", mTlv.field_16_scale);
            ADD("Rock Amount", mTlv.field_18_rock_amount);
        }
    };

    struct Path_TimerTrigger : public TlvObjectBaseAE<::Path_TimerTrigger>
    {
        CTOR_AE(Path_TimerTrigger, "TimeTrigger", TlvTypes::TimerTrigger_57)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Trigger Delay (Frames)", mTlv.field_12_trigger_delay);
            ADD("ID 1", mTlv.field_14_id1);
            ADD("ID 2", mTlv.field_16_id2);
            ADD("ID 3", mTlv.field_18_id3);
            ADD("ID 4", mTlv.field_1A_id4);
        }
    };

    struct Path_MotionDetector : public TlvObjectBaseAE<::Path_MotionDetector>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_MotionDetector::InitialMoveDirection>("Enum_InitialMoveDirection",
                {
                    {::Path_MotionDetector::InitialMoveDirection::eRight_0, "Right"},
                    {::Path_MotionDetector::InitialMoveDirection::eLeft_1, "Left"},
                });
        }

        CTOR_AE(Path_MotionDetector, "MotionDetector", TlvTypes::MotionDetector_36)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Device X", mTlv.field_12_device_x);
            ADD("Device Y", mTlv.field_14_device_y);
            ADD("Speed", mTlv.field_16_speed_x256);
            ADD("Initial Move Direction", mTlv.field_18_initial_move_direction);
            ADD("Draw Flare", mTlv.field_1A_draw_flare);
            ADD("Disabled ID", mTlv.field_1C_disable_id);
            ADD("Alarm ID", mTlv.field_1E_alarm_id);
            ADD("Alarm Duration (Frames)", mTlv.field_20_alarm_duration);
        }
    };

    struct Path_MineCar : public TlvObjectBaseAE<::Path_MineCar>
    {
        CTOR_AE(Path_MineCar, "MineCar", TlvTypes::MineCar_93)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Max Damage (Unused?)", mTlv.field_12_max_damage);
        }
    };

    struct Path_ExplosionSet : public TlvObjectBaseAE<::Path_ExplosionSet>
    {
        CTOR_AE(Path_ExplosionSet, "ExplosionSet", TlvTypes::ExplosionSet_95)
        {
            ADD("Start Enabled", mTlv.field_10_bStart_enabled);
            ADD("Switch ID", mTlv.field_12_switch_id);
            ADD("Big Rocks", mTlv.field_14_big_rocks);
            ADD("Start Delay", mTlv.field_16_start_delay);
            ADD("Direction", mTlv.field_18_direction);
            ADD("Start Delay (Frames)", mTlv.field_1A_start_delay);
            ADD("Grid Spacing", mTlv.field_1C_grid_spacing);
            ADD("Increasing Grid Spacing", mTlv.field_1E_increasing_grid_spacing);
            ADD("Scale", mTlv.field_20_scale);
        }
    };

    struct Path_ColourfulMeter : public TlvObjectBaseAE<::Path_ColourfulMeter>
    {
        CTOR_AE(Path_ColourfulMeter, "ColourfulMeter", TlvTypes::ColourfulMeter_91)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Number Of Meter Bars", mTlv.field_12_number_of_meter_bars);
            ADD("Mines Alarm Countdown (Seconds)", mTlv.field_14_mines_alarm_countdown);
            ADD("Start Filled", mTlv.field_16_bStartFilled);
        }
    };

    struct Path_Alarm : public TlvObjectBaseAE<::Path_Alarm>
    {
        CTOR_AE(Path_Alarm, "Alarm", TlvTypes::Alarm_100)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Duration (Frames)", mTlv.field_12_duration);
        }
    };

    struct Path_DemoSpawnPoint : public TlvObjectBaseAE<::Path_DemoSpawnPoint>
    {
        CTOR_AE(Path_DemoSpawnPoint, "DemoSpawnPoint", TlvTypes::DemoSpawnPoint_87)
        {
            // Empty
        }
    };

    struct Path_SlapLock : public TlvObjectBaseAE<::Path_SlapLock>
    {
        CTOR_AE(Path_SlapLock, "SlapLock", TlvTypes::SlapLock_98)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Target Tomb ID 1", mTlv.field_12_target_tomb_id1);
            ADD("Target Tomb ID 2", mTlv.field_14_target_tomb_id2);
            ADD("Persistant", mTlv.field_16_bPersistant);
            ADD("Has Ghost", mTlv.field_18_has_ghost);
            ADD("Give Invisibility Power-up", mTlv.field_1A_give_invisibility_powerup);
            ADD("Invisibility Power-up ID", mTlv.field_1C_invisibility_id);
            ADD("Option ID", mTlv.field_1E_option_id);
        }
    };

    struct Path_Slurg : public TlvObjectBaseAE<::Path_Slurg>
    {
        CTOR_AE(Path_Slurg, "Slurg", TlvTypes::Slurg_84)
        {
            ADD("Moving Time (Frames)", mTlv.field_10_slurg_data.field_0_moving_timer);
            ADD("Direction", mTlv.field_10_slurg_data.field_2_direction);
            ADD("Scale", mTlv.field_10_slurg_data.field_4_scale);
            ADD("ID", mTlv.field_10_slurg_data.field_6_id);
        }
    };

    struct Path_DoorBlocker : public TlvObjectBaseAE<::Path_DoorBlocker>
    {
        CTOR_AE(Path_DoorBlocker, "DoorBlocker", TlvTypes::DoorBlocker_109)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("ID", mTlv.field_12_id);
        }
    };

    struct Path_Dove : public TlvObjectBaseAE<::Path_Dove>
    {
        CTOR_AE(Path_Dove, "Dove", TlvTypes::Dove_9)
        {
            ADD("Dove Count", mTlv.field_10_dove_count);
            ADD("Pixel Perfect", mTlv.field_12_pixel_perfect);
            ADD("Scale", mTlv.field_14_scale);
        }
    };

    struct Path_BirdPortalExit : public TlvObjectBaseAE<::Path_BirdPortalExit>
    {
        CTOR_AE(Path_BirdPortalExit, "BirdPortalExit", TlvTypes::BirdPortalExit_29)
        {
            ADD("Side", mTlv.field_10_side);
            ADD("Scale", mTlv.field_12_scale);
        }
    };

    struct Path_DoorFlame : public TlvObjectBaseAE<::Path_DoorFlame>
    {
        CTOR_AE(Path_DoorFlame, "DoorFlame", TlvTypes::DoorFlame_51)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Scale", mTlv.field_12_scale);
            ADD("Colour", mTlv.field_14_colour);
        }
    };

    struct Path_TrainDoor : public TlvObjectBaseAE<::Path_TrainDoor>
    {
        CTOR_AE(Path_TrainDoor, "TrainDoor", TlvTypes::TrainDoor_111)
        {
            ADD("Direction", mTlv.field_10_direction);
        }
    };

    struct Path_Greeter : public TlvObjectBaseAE<::Path_Greeter>
    {
        CTOR_AE(Path_Greeter, "Greeter", TlvTypes::Greeter_106)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Motion Detector Speed", mTlv.field_12_motion_detector_speed);
            ADD("Direction", mTlv.field_14_direction);
        }
    };

    struct Path_ScrabLeftBound : public TlvObjectBaseAE<::Path_ScrabLeftBound>
    {
        CTOR_AE(Path_ScrabLeftBound, "ScrabLeftBound", TlvTypes::ScrabLeftBound_43)
        {
            // Empty
        }
    };

    struct Path_ScrabRightBound : public TlvObjectBaseAE<::Path_ScrabRightBound>
    {
        CTOR_AE(Path_ScrabRightBound, "ScrabRightBound", TlvTypes::ScrabRightBound_44)
        {
            // Empty
        }
    };

    struct Path_CreditsController : public TlvObjectBaseAE<::Path_CreditsController>
    {
        CTOR_AE(Path_CreditsController, "CreditsController", TlvTypes::CreditsController_62)
        {
            // Empty
        }
    };

    struct Path_MovieHandstone : public TlvObjectBaseAE<::Path_MovieHandstone>
    {
        CTOR_AE(Path_MovieHandstone, "MovieHandstone", TlvTypes::MovieHandStone_27)
        {
            ADD("Scale", mTlv.scale);
            ADD("Movie Number", mTlv.movie_number);
            ADD("ID", mTlv.id);
            ADD("Padding", mTlv.padding);
        }
    };

    struct Path_MovingBomb : public TlvObjectBaseAE<::Path_MovingBomb>
    {
        CTOR_AE(Path_MovingBomb, "MovingBomb", TlvTypes::MovingBomb_52)
        {
            ADD("Speed", mTlv.field_10_speed);
            ADD("ID", mTlv.field_12_id);
            ADD("Triggered By Alarm", mTlv.field_14_bStart_type_triggered_by_alarm);
            ADD("Scale", mTlv.field_16_scale);
            ADD("Max Rise", mTlv.field_18_max_rise);
            ADD("Disable Resources", mTlv.field_1A_disable_resources);
            ADD("Start Speed", mTlv.field_1C_start_speed);
            ADD("Persist Offscreen", mTlv.field_1E_persist_offscreen);
        }
    };

    struct Path_SecurityDoor : public TlvObjectBaseAE<::Path_SecurityDoor>
    {
        CTOR_AE(Path_SecurityDoor, "SecurityDoor", TlvTypes::SecurityDoor_58)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("ID", mTlv.field_12_id);
            ADD("Code 1", mTlv.field_14_code_1);
            ADD("Code 2", mTlv.field_16_code_2);
            ADD("X Position", mTlv.field_18_xpos);
            ADD("Y Position", mTlv.field_1A_ypos);
        }
    };

    struct Path_NakedSlig : public TlvObjectBaseAE<::Path_NakedSlig>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_NakedSlig::State>("Enum_NakedSligState",
                {
                    {::Path_NakedSlig::State::eSleeping_0, "Sleeping"},
                    {::Path_NakedSlig::State::eSleeping_1, "Sleeping2 (Duplicate)"},
                    {::Path_NakedSlig::State::eAwake_2, "Awake"},
                });

            types.AddEnum<::Path_NakedSlig::CrawlDirection>("Enum_CrawlDirection",
                {
                    {::Path_NakedSlig::CrawlDirection::eLeft_0, "Left"},
                    {::Path_NakedSlig::CrawlDirection::eRight_1, "Right"},
                    {::Path_NakedSlig::CrawlDirection::eRandom_2, "Random"},
                });
        }

        CTOR_AE(Path_NakedSlig, "NakedSlig", TlvTypes::NakedSlig_103)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Direction (Unused?)", mTlv.field_12_direction);
            ADD("State", mTlv.field_14_state);
            ADD("Crawl Direction", mTlv.field_16_crawl_direction);
            ADD("Panic ID", mTlv.field_18_panic_id);
            ADD("Reset On Death", mTlv.field_1A_reset_on_death);
        }
    };
    // TODO: finish
    struct Path_SligGetPants : public TlvObjectBaseAE<::Path_SligGetPants>
    {
        CTOR_AE(Path_SligGetPants, "SligGetPants", TlvTypes::SligGetPants_104)
        {
            ADD("Scale", mTlv.scale);
            ADD("unknown2", mTlv.unknown2);
            ADD("unknown3", mTlv.unknown3);
            ADD("unknown4", mTlv.unknown4);
            ADD("unknown5", mTlv.unknown5);
            ADD("unknown6", mTlv.unknown6);
            ADD("unknown7", mTlv.unknown7);
            ADD("unknown8", mTlv.unknown8);
            ADD("unknown9", mTlv.unknown9);
            ADD("unknown10", mTlv.unknown10);
            ADD("unknown11", mTlv.unknown11);
            ADD("unknown12", mTlv.unknown12);
            ADD("unknown13", mTlv.unknown13);
            ADD("unknown14", mTlv.unknown14);
            ADD("unknown15", mTlv.unknown15);
            ADD("unknown16", mTlv.unknown16);
            ADD("unknown17", mTlv.unknown17);
            ADD("unknown18", mTlv.unknown18);
            ADD("unknown19", mTlv.unknown19);
            ADD("unknown20", mTlv.unknown20);
            ADD("unknown21", mTlv.unknown21);
            ADD("unknown22", mTlv.unknown22);
            ADD("unknown23", mTlv.unknown23);
            ADD("unknown24", mTlv.unknown24);
            ADD("unknown25", mTlv.unknown25);
            ADD("unknown26", mTlv.unknown26);
            ADD("unknown27", mTlv.unknown27);
            ADD("unknown28", mTlv.unknown28);
            ADD("unknown29", mTlv.unknown29);
            ADD("unknown30", mTlv.unknown30);
            ADD("unknown31", mTlv.unknown31);
            ADD("unknown32", mTlv.unknown32);

        }
    };
    // TODO: finish
    struct Path_SligGetWings : public TlvObjectBaseAE<::Path_SligGetWings>
    {
        CTOR_AE(Path_SligGetWings, "SligGetWings", TlvTypes::SligGetWings_105)
        {
            ADD("Scale", mTlv.scale);
            ADD("state", mTlv.state);
            ADD("hi_pause_time", mTlv.hi_pause_time);
            ADD("patrol_pause_min", mTlv.patrol_pause_min);
            ADD("patrol_pause_max", mTlv.patrol_pause_max);
            ADD("direction", mTlv.direction);
            ADD("panic_delay", mTlv.panic_delay);
            ADD("give_up_chase_delay", mTlv.give_up_chase_delay);
            ADD("prechase_delay", mTlv.prechase_delay);
            ADD("slig_id", mTlv.slig_id);
            ADD("listen_time", mTlv.listen_time);
            ADD("trigger_id", mTlv.trigger_id);
            ADD("grenade_delay", mTlv.grenade_delay);
            ADD("max_velocity", mTlv.max_velocity);
            ADD("launch_id", mTlv.launch_id);
            ADD("persistant", mTlv.persistant);
        }
    };

    struct Path_NakedSligButton : public TlvObjectBaseAE<::Path_NakedSligButton>
    {
        CTOR_AE(Path_NakedSligButton, "NakedSligButton", TlvTypes::NakedSligButton_107)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("ID", mTlv.field_12_id);
            ADD("Action", mTlv.field_14_action);
            ADD("On Sound", mTlv.field_16_on_sound);
            ADD("Off Sound", mTlv.field_18_off_sound);
            ADD("Sound Direction", mTlv.field_1A_sound_direction); // TODO: enum
        }
    };

    struct Path_Glukkon : public TlvObjectBaseAE<::Path_Glukkon>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::GlukkonTypes>("Enum_GlukkonTypes",
                {
                    {::GlukkonTypes::Normal_0, "normal_0"},
                    {::GlukkonTypes::Aslik_1, "aslik"},
                    {::GlukkonTypes::Drpik_2, "drpik"},
                    {::GlukkonTypes::Phleg_3, "phleg"},
                    {::GlukkonTypes::Normal_4, "normal_4"},
                    {::GlukkonTypes::Normal_5, "normal_5"},
                });

            types.AddEnum<::Path_Glukkon::StartDirection>("Enum_GlukkonStartDirection",
                {
                    {::Path_Glukkon::StartDirection::eRight_0, "Right"},
                    {::Path_Glukkon::StartDirection::eLeft_1, "Left"},
                });
        }

        // TODO: finish
        CTOR_AE(Path_Glukkon, "Glukkon", TlvTypes::Glukkon_73)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Start Direction", mTlv.field_12_start_direction);
            ADD("default_behavior", mTlv.field_14_default_behaviour);
            ADD("pre_alarmed_delay", mTlv.field_16_pre_alarmed_delay);
            ADD("switch_id", mTlv.field_18_switch_id);
            ADD("post_alarm_delay", mTlv.field_1A_post_alarm_delay);
            ADD("spawn_id", mTlv.field_1C_spawn_id);
            ADD("spawn_direction", mTlv.field_1E_spawn_direction);
            ADD("spawn_delay", mTlv.field_20_spawn_delay);
            ADD("glukkon_type", mTlv.field_22_glukkon_type);
            ADD("start_gas_id", mTlv.field_24_start_gas_id);
            ADD("play_movie_id", mTlv.field_26_play_movie_id);
            ADD("movie_to_play", mTlv.field_28_movie_to_play_id);
        }
    };

    struct Path_GlukkonSwitch : public TlvObjectBaseAE<::Path_GlukkonSwitch>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_GlukkonSwitch::Scale>("Enum_GlukkonSwitchScale",
                {
                    {::Path_GlukkonSwitch::Scale::eHalf_0, "Half"},
                    {::Path_GlukkonSwitch::Scale::eFull_1 , "Full"},
                });
        };

        CTOR_AE(Path_GlukkonSwitch, "GlukkonSwitch", TlvTypes::GlukkonSwitch_108)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("OK ID", mTlv.field_12_ok_id);
            ADD("Fail ID", mTlv.field_14_fail_id);
            ADD("X Position", mTlv.field_16_xpos);
            ADD("Y Position", mTlv.field_18_ypos);
        }
    };

    struct Path_GasCountDown : public TlvObjectBaseAE<::Path_GasCountDown>
    {
        CTOR_AE(Path_GasCountDown, "GasCountDown", TlvTypes::GasCountdown_69)
        {
            ADD("Start Trigger ID", mTlv.field_10_start_trigger_id);
            ADD("Gas Countdown Time (Frames)", mTlv.field_12_gas_countdown_timer);
            ADD("Stop Trigger ID", mTlv.field_14_stop_trigger_id);
        }
    };

    struct Path_FallingItem : public TlvObjectBaseAE<::Path_FallingItem>
    {
        CTOR_AE(Path_FallingItem, "FallingItem", TlvTypes::FallingItem_11)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Scale", mTlv.field_12_scale);
            ADD("Fall Delay (Frames)", mTlv.field_14_fall_delay);
            ADD("Max Falling Items", mTlv.field_16_max_falling_items);
            ADD("Reset ID", mTlv.field_18_reset_id);
        }
    };

    struct Path_BoneBag : public TlvObjectBaseAE<::Path_BoneBag>
    {
        CTOR_AE(Path_BoneBag, "BoneBag", TlvTypes::BoneBag_94)
        {
            ADD("Bone Fall Direction", mTlv.field_10_bone_fall_direction);
            ADD("X Velocity", mTlv.field_12_x_vel);
            ADD("Y Velocity", mTlv.field_14_y_vel);
            ADD("Scale", mTlv.field_16_scale);
            ADD("Bone Amount", mTlv.field_18_bone_amount);
        }
    };

    struct Path_SecurityClaw : public TlvObjectBaseAE<::Path_SecurityClaw>
    {
        CTOR_AE(Path_SecurityClaw, "SecurityClaw", TlvTypes::SecurityClaw_35)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Disabled Resources", mTlv.field_12_disabled_resources);
            ADD("Unknown", mTlv.field_14_unknown);
        }
    };

    struct Path_FootSwitch : public TlvObjectBaseAE<::Path_FootSwitch>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
                {
                    {::FootSwitchTriggerBy::eOnlyAbe_0, "Only Abe"},
                    {::FootSwitchTriggerBy::eAnyone_1, "Anyone"},
                });
        }

        CTOR_AE(Path_FootSwitch, "FootSwitch", TlvTypes::FootSwitch_34)
        {
            ADD("ID", mTlv.field_10_id);
            ADD("Scale", mTlv.field_12_scale);
            ADD("Action", mTlv.field_14_action);
            ADD("Triggered By", mTlv.field_16_trigger_by);
        }
    };

    struct Path_SlogHut : public TlvObjectBaseAE<::Path_SlogHut>
    {
        CTOR_AE(Path_SlogHut, "SlogHut", TlvTypes::SlogHut_72)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Switch ID", mTlv.field_12_switch_id);
            ADD("Zzz Delay (Frames)", mTlv.field_14_Zzz_delay);
        }
    };

    struct Path_SlogSpawner : public TlvObjectBaseAE<::Path_SlogSpawner>
    {
        CTOR_AE(Path_SlogSpawner, "SlogSpawner", TlvTypes::SlogSpawner_68)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Number Of Slogs", mTlv.field_12_number_of_slogs);
            ADD("Max Slogs At A Time", mTlv.field_14_max_slogs_at_a_time);
            ADD("Direction", mTlv.field_16_direction);
            ADD("Spawn Delay (Frames)", mTlv.field_18_slog_spawn_delay);
            ADD("ID", mTlv.field_1A_id);
            ADD("Listen To Sligs", mTlv.field_1C_listen_to_sligs);
            ADD("Chase Delay (Frames)", mTlv.field_1E_chase_delay);
        }
    };

    struct Path_MainMenuController : public TlvObjectBaseAE<::Path_MainMenuController>
    {
        CTOR_AE(Path_MainMenuController, "MainMenuController", TlvTypes::MainMenuController_54)
        {
            // Empty
        }
    };

    struct Path_Null_63 : public TlvObjectBaseAE<::Path_Null_63>
    {
        CTOR_AE(Path_Null_63, "Null_63", TlvTypes::Null_63)
        {
            ADD("padding1", mTlv.padding1);
            ADD("padding2", mTlv.padding2);
        }
    };

    struct Path_Scrab : public TlvObjectBaseAE<::Path_Scrab>
    {
        CTOR_AE(Path_Scrab, "Scrab", TlvTypes::Scrab_41)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Attack Delay (Frames)", mTlv.field_12_attack_delay);
            ADD("Patrol Type Run Or Walk Chance (6 Max)", mTlv.field_14_patrol_type_run_or_walk_chance);
            ADD("Left Min Delay (Frames)", mTlv.field_16_left_min_delay);
            ADD("Left Max Delay (Frames)", mTlv.field_18_left_max_delay);
            ADD("Right Min Delay (Frames)", mTlv.field_1A_right_min_delay);
            ADD("Right Max Delay (Frames)", mTlv.field_1C_right_max_delay);
            ADD("Spotting Abe Delay (Frames)", mTlv.field_1E_spotting_abe_delay);
            ADD("Disabled Resources", mTlv.field_20_disabled_resources);
            ADD("Roar Randomly", mTlv.field_22_roar_randomly);
            ADD("Persistant", mTlv.field_24_persistant);
            ADD("Whirl Attack Duration (Frames)", mTlv.field_26_whirl_attack_duration);
            ADD("Unused", mTlv.field_28_unused);
            ADD("Kill Enemy", mTlv.field_2A_bKill_enemy);
        }
    };

    struct Path_ScrabSpawner : public TlvObjectBaseAE<::Path_ScrabSpawner>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::ScrabSpawnDirection>("Enum_ScrabSpawnDirection",
                {
                    {::ScrabSpawnDirection::eNone_0, "None"},
                    {::ScrabSpawnDirection::eLeft_1, "Left"},
                    {::ScrabSpawnDirection::eRight_2, "Right"},
                });
        }

        CTOR_AE(Path_ScrabSpawner, "ScrabSpawner", TlvTypes::ScrabSpawner_102)
        {
            // Scrab properties
            ADD("Scrab Scale", mTlv.field_10_scale);
            ADD("Scrab Attack Delay (Frames)", mTlv.field_12_attack_delay);
            ADD("Scrab Patrol Type Run Or Walk Chance (6 Max)", mTlv.field_14_patrol_type_run_or_walk_chance);
            ADD("Scrab Left Min Delay (Frames)", mTlv.field_16_left_min_delay);
            ADD("Scrab Left Max Delay (Frames)", mTlv.field_18_left_max_delay);
            ADD("Scrab Right Min Delay (Frames)", mTlv.field_1A_right_min_delay);
            ADD("Scrab Right Max Delay (Frames)", mTlv.field_1C_right_max_delay);
            ADD("Scrab Spotting Abe Delay (Frames)", mTlv.field_1E_spotting_abe_delay);
            ADD("Scrab Disabled Resources", mTlv.field_20_disabled_resources);
            ADD("Scrab Roar Randamly", mTlv.field_22_roar_randomly);
            ADD("Scrab Persistant", mTlv.field_24_persistant);
            ADD("Scrab Whirl Attack Duration (Frames)", mTlv.field_26_whirl_attack_duration);
            ADD("Scrab Unused", mTlv.field_28_unused);
            ADD("Scrab Kill Enemy", mTlv.field_2A_bKill_enemy);

            // Spawner properties
            ADD("Spawner Switch ID", mTlv.field_2C_switch_id);
            ADD("Scrab Spawn Direction", mTlv.field_2E_spawn_direction);
        }
    };

    struct Path_SlurgSpawner : public TlvObjectBaseAE<::Path_SlurgSpawner>
    {
        CTOR_AE(Path_SlurgSpawner, "SlurgSpawner", TlvTypes::SlurgSpawner_89)
        {
            ADD("Slurg Pause Delay", mTlv.field_10_slurg_data.field_0_moving_timer);
            ADD("Slurg Direction", mTlv.field_10_slurg_data.field_2_direction);
            ADD("Slurg Scale", mTlv.field_10_slurg_data.field_4_scale);
            ADD("Slurg ID", mTlv.field_10_slurg_data.field_6_id);

            ADD("Spawn Delay Between Slurgs", mTlv.field_18_spawner_data.field_8_spawn_delay_between_slurgs);
            ADD("Max Slurgs", mTlv.field_18_spawner_data.field_A_max_slurgs);
            ADD("Spawner Switch ID", mTlv.field_18_spawner_data.field_C_switch_id);
        }
    };

    struct Path_Paramite : public TlvObjectBaseAE<::Path_Paramite>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::Path_Paramite::EntranceType>("Enum_ParamiteEntranceType",
                {
                    {::Path_Paramite::EntranceType::eNormalPatrol_0, "Normal Patrol"},
                    {::Path_Paramite::EntranceType::eNormalSurpriseWeb_1, "Normal Surprise Web"},
                    {::Path_Paramite::EntranceType::eSlightlyHigherSpawnSurpriseWeb_2, "Slightly Higher Spawn Surprise Web"},
                    {::Path_Paramite::EntranceType::eUnused_ScaleToLeftGridSize_3, "Unused Scale To Left Grid Size"},
                    {::Path_Paramite::EntranceType::eUnused_ScaleToRightGridSize_4, "Unused Scale To Right Grid Size"},
                });
        }

        CTOR_AE(Path_Paramite, "Paramite", TlvTypes::Paramite_26)
        {
            ADD("Scale", mTlv.field_10_scale);
            ADD("Entrance Type", mTlv.field_12_entrace_type);
            ADD("Alone Wait Before Chase (Frames)", mTlv.field_14_alone_wait_before_chase_timer);
            ADD("Surprise Web Delay (Frames)", mTlv.field_16_surprise_web_delay_timer);
            ADD("Meat Eating Time (Frames)", mTlv.field_18_meat_eating_time);
            ADD("Group Wait Before Chase (Frames)", mTlv.field_1A_group_wait_before_chase_timer);
            ADD("Disabled Resources", mTlv.field_1C_disabled_resources);
            ADD("ID", mTlv.field_1E_id);
            ADD("Hiss Before Attacking", mTlv.field_20_hiss_before_attack);
            ADD("Delete When Far Away", mTlv.field_22_delete_when_far_away);
            ADD("Can Attack Fleeches", mTlv.field_24_bAttack_fleeches);
        }
    };

    struct Path_ParamiteWebLine : public TlvObjectBaseAE<::Path_ParamiteWebLine>
    {
        CTOR_AE(Path_ParamiteWebLine, "ParamiteWebLine", TlvTypes::ParamiteWebLine_99)
        {
            ADD("Scale", mTlv.field_10_scale);
        }
    };

    struct Path_MeatSack : public TlvObjectBaseAE<::Path_MeatSack>
    {
        CTOR_AE(Path_MeatSack, "MeatSack", TlvTypes::MeatSack_40)
        {
            ADD("Meat Fall Direction", mTlv.field_10_meat_fall_direction);
            ADD("X Velocity", mTlv.field_12_xVel);
            ADD("Y Velocity", mTlv.field_14_yVel);
            ADD("Scale", mTlv.field_16_scale);
            ADD("Meat Amount", mTlv.field_18_meat_amount);
        }
    };

    struct Path_TorturedMudokon : public TlvObjectBaseAE<::Path_TorturedMudokon>
    {
        CTOR_AE(Path_TorturedMudokon, "TorturedMudokon", TlvTypes::TorturedMudokon_110)
        {
            ADD("speed_id", mTlv.field_10_speed_id);
            ADD("release_id", mTlv.field_12_release_id);
        }
    };
}
