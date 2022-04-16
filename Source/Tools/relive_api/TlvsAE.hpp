#pragma once

#include "TlvObjectBaseAE.hpp"
#include "TlvObjectBaseMacros.hpp"

#include "../../AliveLibAE/Path.hpp"
#include "../../AliveLibAE/Lever.hpp"
#include "../../AliveLibAE/ElectricWall.hpp"
#include "../../AliveLibAE/Mudokon.hpp"
#include "../../AliveLibAE/BirdPortal.hpp"
#include "../../AliveLibAE/LCDStatusBoard.hpp"
#include "../../AliveLibAE/Door.hpp"
#include "../../AliveLibAE/BoomMachine.hpp"
#include "../../AliveLibAE/Slig.hpp"
#include "../../AliveLibAE/Fleech.hpp"
#include "../../AliveLibAE/UXB.hpp"
#include "../../AliveLibAE/LCDScreen.hpp"
#include "../../AliveLibAE/StatusLight.hpp"
#include "../../AliveLibAE/ShadowZone.hpp"
#include "../../AliveLibAE/WorkWheel.hpp"
#include "../../AliveLibAE/MusicTrigger.hpp"
#include "../../AliveLibAE/SlamDoor.hpp"
#include "../../AliveLibAE/LaughingGas.hpp"
#include "../../AliveLibAE/InvisibleSwitch.hpp"
#include "../../AliveLibAE/Water.hpp"
#include "../../AliveLibAE/GasEmitter.hpp"
#include "../../AliveLibAE/BackgroundAnimation.hpp"
#include "../../AliveLibAE/LiftPoint.hpp"
#include "../../AliveLibAE/PullRingRope.hpp"
#include "../../AliveLibAE/MultiSwitchController.hpp"
#include "../../AliveLibAE/SecurityOrb.hpp"
#include "../../AliveLibAE/Abe.hpp"
#include "../../AliveLibAE/WheelSyncer.hpp"
#include "../../AliveLibAE/LevelLoader.hpp"
#include "../../AliveLibAE/FlyingSligSpawner.hpp"
#include "../../AliveLibAE/BrewMachine.hpp"
#include "../../AliveLibAE/Drill.hpp"
#include "../../AliveLibAE/Mine.hpp"
#include "../../AliveLibAE/Slog.hpp"
#include "../../AliveLibAE/TrapDoor.hpp"
#include "../../AliveLibAE/LiftMover.hpp"
#include "../../AliveLibAE/RockSack.hpp"
#include "../../AliveLibAE/TimerTrigger.hpp"
#include "../../AliveLibAE/MotionDetector.hpp"
#include "../../AliveLibAE/MineCar.hpp"
#include "../../AliveLibAE/ExplosionSet.hpp"
#include "../../AliveLibAE/ColourfulMeter.hpp"
#include "../../AliveLibAE/SlapLock.hpp"
#include "../../AliveLibAE/Slurg.hpp"
#include "../../AliveLibAE/DoorBlocker.hpp"
#include "../../AliveLibAE/Dove.hpp"
#include "../../AliveLibAE/DoorFlame.hpp"
#include "../../AliveLibAE/Greeter.hpp"
#include "../../AliveLibAE/MovingBomb.hpp"
#include "../../AliveLibAE/SecurityDoor.hpp"
#include "../../AliveLibAE/CrawlingSlig.hpp"
#include "../../AliveLibAE/CrawlingSligButton.hpp"
#include "../../AliveLibAE/Glukkon.hpp"
#include "../../AliveLibAE/GlukkonSwitch.hpp"
#include "../../AliveLibAE/GasCountDown.hpp"
#include "../../AliveLibAE/FallingItem.hpp"
#include "../../AliveLibAE/Bone.hpp"
#include "../../AliveLibAE/FootSwitch.hpp"
#include "../../AliveLibAE/SlogHut.hpp"
#include "../../AliveLibAE/SlogSpawner.hpp"
#include "../../AliveLibAE/Scrab.hpp"
#include "../../AliveLibAE/ScrabSpawner.hpp"
#include "../../AliveLibAE/SlurgSpawner.hpp"
#include "../../AliveLibAE/Paramite.hpp"
#include "../../AliveLibAE/ParamiteWebLine.hpp"
#include "../../AliveLibAE/Meat.hpp"
#include "../../AliveLibAE/TorturedMudokon.hpp"

#define CTOR_AE(className, objectTypeName, tlvEnumType)\
    className() : TlvObjectBaseAE(sizeof(::className), tlvEnumType, objectTypeName, &mTlv)\
    {\
    }\
    \
    className(ReliveAPI::TypesCollectionBase& globalTypes, const Path_TLV* pTlvSrc = nullptr) \
        : TlvObjectBaseAE(sizeof(::className), globalTypes, tlvEnumType, objectTypeName, &mTlv)\
    {\
        if (pTlvSrc)\
        {\
            mTlv = *static_cast<const ::className*>(pTlvSrc);\
            ConvertXYPos(); \
        }\
        else\
        {\
            mPSelfTlv->field_2_length = static_cast<s16>(mSizeOfT);\
            mPSelfTlv->field_4_type.mType = mType;\
        }\
        AddProperties(globalTypes);\
    }\
    ::className mTlv = {};\
    void AddProperties(ReliveAPI::TypesCollectionBase& globalTypes)

#define EMPTY_CTOR_AE() (void) globalTypes

struct Path_Null_63 final : public Path_TLV
{
    // TODO: check
    s16 padding1;
    s16 padding2;
};

struct Path_MainMenuController final : public Path_TLV
{
    // Empty
};

struct Path_SligGetWings final : public Path_TLV
{
    Scale_short scale;
    Path_FlyingSlig_Data::SpawnDelayStates state;
    s16 spawn_move_delay;
    s16 patrol_pause_min;
    s16 patrol_pause_max;
    XDirection_short start_direction;
    s16 panic_delay;
    s16 give_up_chase_delay;
    s16 prechase_delay;
    s16 slig_bound_id;
    s16 alerted_listen_time;
    s16 spawner_switch_id;
    s16 grenade_delay;
    s16 max_velocity;
    s16 launch_switch_id;
    Choice_short persistant;
};

struct Path_SligGetPants final : public Path_TLV
{
    Scale_short Scale;
    Path_Slig::StartState start_state;
    s16 pause_time;
    s16 pause_left_min;
    s16 pause_left_max;
    s16 pause_right_min;
    s16 pause_right_max;
    Path_Slig::ShootPossessedSligs shoot_possessed_sligs;
    s16 shoot_on_sight_delay;
    s16 num_times_to_shoot;
    s16 padding;
    s16 code1;
    s16 code2;
    Choice_short chase_abe_when_spotted;
    XDirection_short start_direction;
    s16 panic_timeout;
    s16 amount_of_panic_sounds;
    s16 panic_sounds_timeout;
    s16 stop_chase_delay;
    s16 time_to_wait_before_chase;
    s16 slig_bound_id;
    s16 alerted_listen_time;
    s16 percent_say_what;
    s16 percent_beat_mud;
    s16 talk_to_abe;
    s16 dont_shoot;
    s16 z_shoot_delay;
    Choice_short stay_awake;
    s16 disable_resources;
    s16 noise_wake_up_distance;
    s16 slig_spawner_switch_id;
    Choice_short unlimited_spawns;
};

struct Path_MovieHandstone final : public Path_TLV
{
    s16 padding;
    Scale_short scale;
    s16 movie_number;
    s16 trigger_switch_id;
};

struct Path_CreditsController final : public Path_TLV
{
    // Empty
};

struct Path_ScrabLeftBound final : public Path_TLV
{
    // Empty
};

struct Path_ScrabRightBound final : public Path_TLV
{
    // Empty
};

struct Path_DemoSpawnPoint final : public Path_TLV
{
    // Empty
};

struct Path_PathTransition final : public Path_TLV
{
    s16 level;
    s16 path;
    s16 camera;
    s16 movie;
    s16 wipe;
    Scale_short scale;
};

struct Path_ZSligCover final : public Path_TLV
{
    // Empty
};

struct Path_SligSpawner final : public Path_TLV
{
    Scale_short scale;
    Path_Slig::StartState start_state;
    s16 pause_time;
    s16 pause_left_min;
    s16 pause_left_max;
    s16 pause_right_min;
    s16 pause_right_max;
    Path_Slig::ShootPossessedSligs shoot_possessed_sligs;
    s16 shoot_on_sight_delay; 
    s16 number_of_times_to_shoot;
    s16 unknown; // maybe number of times to shoot after spawning?
    s16 code_1;
    s16 code_2;
    XDirection_short chase_abe_when_spotted;
    XDirection_short start_direction;
    s16 panic_timeout;
    s16 num_panic_sounds;
    s16 panic_sound_timeout;
    s16 stop_chase_delay;
    s16 time_to_wait_before_chase;
    s16 slig_bound_id;
    s16 alerted_listen_time;
    s16 percent_say_what;
    s16 percent_beat_mud;
    s16 talk_to_abe;
    s16 dont_shoot;
    s16 z_shoot_delay;
    s16 stay_awake;
    s16 disable_resources;
    s16 noise_wake_up_distance;
    s16 id;
    Choice_short unlimited_spawns;
};

struct Path_DeathDrop final : public Path_TLV
{
    s16 field_10_animation;
    s16 field_12_sound;
    s16 field_14_id;
    s16 field_16_action;
    s16 field_18_unknown1;
    s16 field_1A_unknown2;
};

struct Path_InvisibleZone final : public Path_TLV
{
    // Empty
};

struct Path_Pulley final : public Path_TLV
{
    s16 field_10_unused1;
    s16 field_12_unused2;
};

namespace AETlvs {
struct Path_ElectricWall final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Switch ID", mTlv.field_12_switch_id);
        ADD("Start State", mTlv.field_14_start_state);
    }
};
struct Path_Mudokon final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Start Direction", mTlv.field_14_direction);
        ADD("Voice Pitch", mTlv.field_16_voice_pitch);
        ADD("Rescue Switch ID", mTlv.field_18_rescue_switch_id);
        ADD_HIDDEN("Deaf (Unused?)", mTlv.field_1A_bDeaf); // logic for this only exists in AO
        ADD("Disabled Resources", mTlv.field_1C_disabled_resources);
        ADD("Persist & Reset Offscreen", mTlv.field_1E_persist_and_reset_offscreen);
        ADD("Emotion", mTlv.field_20_emotion);
        ADD("Blind", mTlv.field_22_bBlind);
        ADD("Angry Switch ID", mTlv.field_24_angry_switch_id);
        ADD("Work After Turning Wheel", mTlv.field_26_work_after_turning_wheel);
        ADD("Gets Depressed", mTlv.field_28_bGets_depressed);
        ADD("Ring Pulse Interval", mTlv.field_2A_ring_pulse_interval);
        ADD("Give Ring Without Password", mTlv.field_2C_bGive_ring_without_password);
    }
};

struct Path_BirdPortal final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Enter Side", mTlv.field_10_side);
        ADD("BirdPortalExit Level", mTlv.field_12_dest_level);
        ADD("BirdPortalExit Path", mTlv.field_14_dest_path);
        ADD("BirdPortalExit Camera", mTlv.field_16_dest_camera);
        ADD("Scale", mTlv.field_18_scale);
        ADD("Movie ID", mTlv.field_1A_movie_id);
        ADD("Portal Type", mTlv.field_1C_portal_type);
        ADD("Mudokon Amount For Shrykull", mTlv.field_1E_mudokon_amount_for_shrykull);
        ADD("Create Portal Switch ID", mTlv.field_20_create_portal_switch_id);
        ADD("Delete Portal Switch ID", mTlv.field_22_delete_portal_switch_id);
    }
};

struct Path_LCDStatusBoard final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_LCDStatusBoard, "LCDStatusBoard", TlvTypes::LCDStatusBoard_64)
    {
        ADD("Number Of Mudokons", mTlv.field_10_number_of_muds);
        ADD("Zulag Number", mTlv.field_12_zulag_number);
        ADD("Hide Board", mTlv.field_14_hidden);
    }
};

struct Path_Door final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::DoorStates>("Enum_DoorStates",
        {
            {::DoorStates::eOpen_0, "Open"},
            {::DoorStates::eClosed_1, "Closed"},
            {::DoorStates::eOpening_2, "Opening"},
            {::DoorStates::eClosing_3, "Closing"}
        });

        types.AddEnum<::DoorTypes>("Enum_DoorTypes",
        {
            {::DoorTypes::eBasicDoor_0, "Basic Door"},
            {::DoorTypes::eDoorPadding_1, "Padding (ignore)"},
            {::DoorTypes::eTasksDoorWithSecretMusic_2, "Tasks Door With Secret Music"},
            {::DoorTypes::eTasksDoor_3, "Tasks Door"}
        });
    }

    CTOR_AE(Path_Door, "Door", TlvTypes::Door_5)
    {
        ADD("Level", mTlv.field_10_level);
        ADD("Path", mTlv.field_12_path);
        ADD("Camera", mTlv.field_14_camera);
        ADD("Scale", mTlv.field_16_scale);
        ADD("Door Number", mTlv.field_18_door_number);
        ADD("Switch ID", mTlv.field_1A_switch_id);
        ADD("Target Door ID", mTlv.field_1C_target_door_id);
        ADD("Door Type", mTlv.field_1E_type);
        ADD("Start State", mTlv.field_20_start_state);
        ADD("Hub 1 ID", mTlv.field_22_hub1);
        ADD("Hub 2 ID", mTlv.field_22_hub2);
        ADD("Hub 3 ID", mTlv.field_22_hub3);
        ADD("Hub 4 ID", mTlv.field_22_hub4);
        ADD("Hub 5 ID", mTlv.field_22_hub5);
        ADD("Hub 6 ID", mTlv.field_22_hub6);
        ADD("Hub 7 ID", mTlv.field_22_hub7);
        ADD("Hub 8 ID", mTlv.field_22_hub8);
        ADD("Wipe Effect", mTlv.field_32_wipe_effect);
        ADD("Movie Number", mTlv.field_34_movie_number);
        ADD("X Offset", mTlv.field_36_x_offset);
        ADD("Y Offset", mTlv.field_38_y_offset);
        ADD("Wipe X Org (unused?)", mTlv.field_3A_wipe_x_org);
        ADD("Wipe Y Org (unused?)", mTlv.field_3C_wipe_y_org);
        ADD("Abe Direction On Exit", mTlv.field_3E_abe_direction);
        ADD("Close On Exit", mTlv.field_40_close_on_exit);
        ADD("Clear Throwables", mTlv.field_42_clear_throwables);
    }
};

struct Path_Lever final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::LeverSoundType>("Enum_LeverSoundType",
        {
            {::LeverSoundType::eNone, "None"},
            {::LeverSoundType::eWell_1, "Well"},
            {::LeverSoundType::eUnknown_2, "Unknown"},
            {::LeverSoundType::eDoor_3, "Door"},
            {::LeverSoundType::eElectricWall_4, "Electric Wall"},
            {::LeverSoundType::eSecurityOrb_5, "Security Orb"},
            {::LeverSoundType::eLift_6, "Lift"}
        });

        types.AddEnum<::LeverSoundDirection>("Enum_LeverSoundDirection",
        {
            {::LeverSoundDirection::eLeftAndRight_0, "Left And Right"},
            {::LeverSoundDirection::eLeft_1, "Left"},
            {::LeverSoundDirection::eRight_2, "Right"},
        });
    }

    CTOR_AE(Path_Lever, "Lever", TlvTypes::Lever_17)
    {
        ADD("Action", mTlv.field_10_action);
        ADD("Scale", mTlv.field_12_scale);
        ADD("On Sound", mTlv.field_14_on_sound);
        ADD("Off Sound", mTlv.field_16_off_sound);
        ADD("Sound Direction", mTlv.field_18_sound_direction);
        ADD("Switch ID", mTlv.field_1A_switch_id);
        ADD("Persist Offscreen", mTlv.field_1C_persist_offscreen);
    }
};

struct Path_Hoist final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Hoist::Type>("Enum_HoistType",
        {
            {::Path_Hoist::Type::eNextEdge, "Next Edge"},
            {::Path_Hoist::Type::eNextFloor, "Next Floor"},
            {::Path_Hoist::Type::eOffScreen, "Off Screen"},
        });

        types.AddEnum<::Path_Hoist::GrabDirection>("Enum_HoistGrabDirection",
        {
            {::Path_Hoist::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
            {::Path_Hoist::GrabDirection::eFacingLeft, "Facing Left"},
            {::Path_Hoist::GrabDirection::eFacingRight, "Facing Right"},
        });
    }

    CTOR_AE(Path_Hoist, "Hoist", TlvTypes::Hoist_2)
    {
        ADD("Hoist Type", mTlv.field_10_type);
        ADD("Grab Direction", mTlv.field_12_grab_direction);
        ADD("Switch ID (Unused?)", mTlv.field_14_switch_id);
        ADD("Scale", mTlv.field_16_scale);
    }
};

struct Path_BoomMachine final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_BoomMachine, "BoomMachine", TlvTypes::BoomMachine_59)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Nozzle Side", mTlv.field_12_nozzle_side);
        ADD("Disabled Resources", mTlv.field_14_disabled_resources);
        ADD("Number Of Grenades", mTlv.field_16_number_of_grenades);
    }
};

// TODO: figure out if some of these fields are unused and sync changes with Path_SligGetPants/Path_SligSpawner
struct Path_Slig final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Slig, "Slig", TlvTypes::Slig_15)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Start State", mTlv.field_12_start_state);
        ADD("Pause Time", mTlv.field_14_pause_time);
        ADD("Pause Left Min", mTlv.field_16_pause_left_min);
        ADD("Pause Left Max", mTlv.field_18_pause_left_max);
        ADD("Pause Right Min", mTlv.field_1A_pause_right_min);
        ADD("Pause Right Max", mTlv.field_1C_pause_right_max);
        ADD("Shoot Possessed Sligs", mTlv.field_1E_shoot_possessed_sligs);
        ADD("Shoot On Sight Delay", mTlv.field_20_shoot_on_sight_delay);
        ADD("Bullet Shoot Count", mTlv.field_22_num_times_to_shoot);
        ADD("unknown1", mTlv.field_24_padding);

        ADD("Code 1", mTlv.field_26_code1);
        ADD("Code 2", mTlv.field_28_code2);
        ADD("Chase Abe When Spotted", mTlv.field_2A_chase_abe_when_spotted);
        ADD("Start Direction", mTlv.field_2C_start_direction);
        ADD("Panic Timeout", mTlv.field_2E_panic_timeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.field_30_num_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.field_32_panic_sound_timeout);
        ADD("Stop Chase Delay", mTlv.field_34_stop_chase_delay);
        ADD("Time To Wait Before Chase", mTlv.field_36_time_to_wait_before_chase);
        ADD("Slig Bound/Persist ID", mTlv.field_38_slig_bound_id);
        ADD("Alerted Listen Time", mTlv.field_3A_alerted_listen_time);

        ADD("Percent Say What", mTlv.field_3C_percent_say_what);
        ADD("Percent Beat Mudokon", mTlv.field_3E_percent_beat_mud);
        ADD_HIDDEN("Talk To Abe (Unused?)", mTlv.field_40_talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.field_42_dont_shoot);
        ADD("Z Shoot Delay", mTlv.field_44_Z_shoot_delay);
        ADD("Stay Awake", mTlv.field_46_stay_awake);
        ADD("Disable Resources", mTlv.field_48_disable_resources);
        ADD("Noise Wake Up Distance (Grids)", mTlv.field_4A_noise_wake_up_distance);
        ADD("Slig Spawner Switch ID", mTlv.field_4C_slig_spawner_switch_id);
        ADD_HIDDEN("Unlimited Spawns", mTlv.field_4E_unlimited_spawns);
    }
};

struct Path_Fleech final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Fleech, "Fleech", TlvTypes::Fleech_83)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Start Direction", mTlv.field_12_direction);
        ADD("Asleep", mTlv.field_14_asleep);
        ADD_HIDDEN("Wake Up (Unused?)", mTlv.field_16_wake_up);
        ADD_HIDDEN("Unused1", mTlv.field_18_not_used);

        ADD("Attack Anger Increaser", mTlv.field_1A_attack_anger_increaser);
        ADD_HIDDEN("Attack Delay (Unused?)", mTlv.field_1C_attack_delay);
        ADD("Wake Up Switch ID", mTlv.field_1E_wake_up_switch_id);
        ADD("Hanging", mTlv.field_20_hanging);

        ADD("Lost Target Timeout", mTlv.field_22_lost_target_timeout);
        ADD("Goes To Sleep", mTlv.field_24_goes_to_sleep);
        ADD("Patrol Range (Grids)", mTlv.field_26_patrol_range_in_grids);
        ADD("Wake Up Switch Anger Value", mTlv.field_28_wake_up_switch_anger_value);

        ADD("Can Wake Up Switch ID", mTlv.field_2A_can_wake_up_switch_id);
        ADD("Persistant", mTlv.field_2C_persistant);
        ADD_HIDDEN("Unused2(?)", mTlv.field_2E_padding);
    }
};

struct Path_EnemyStopper final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Switch ID", mTlv.field_12_switch_id);
    }
};

struct Path_Teleporter final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Teleporter, "Teleporter", TlvTypes::Teleporter_88)
    {
        ADD("Teleporter Switch ID", mTlv.field_10_data.field_10_teleporter_switch_id);
        ADD("Other Teleporter Switch ID", mTlv.field_10_data.field_12_other_teleporter_switch_id);
        ADD("Camera", mTlv.field_10_data.field_14_camera);
        ADD("Path", mTlv.field_10_data.field_16_path);
        ADD("Level", mTlv.field_10_data.field_18_level);
        ADD("Switch ID", mTlv.field_10_data.field_1A_switch_id);
        ADD("Scale", mTlv.field_10_data.field_1C_scale);
        ADD("Camera Swap Effect", mTlv.field_10_data.field_1E_cam_swap_effect);
        ADD("Movie Number", mTlv.field_10_data.field_20_movie_number);
        ADD("Electric X", mTlv.field_10_data.field_22_eletric_x);
        ADD("Electric Y", mTlv.field_10_data.field_24_electric_y);
    }
};

struct Path_UXB final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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

struct Path_LCDScreen final : public ReliveAPI::TlvObjectBaseAE
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

struct Path_Edge final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_Edge::GrabDirection>("Enum_EdgeGrabDirection",
        {
            {::Path_Edge::GrabDirection::eFacingLeft, "Facing Left"},
            {::Path_Edge::GrabDirection::eFacingRight, "Facing Right"},
            {::Path_Edge::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
        });
    }

    CTOR_AE(Path_Edge, "Edge", TlvTypes::Edge_3)
    {
        ADD("Grab Direction", mTlv.field_10_grab_direction);
        ADD("Can Grab", mTlv.field_12_bCan_grab);
        ADD("Scale", mTlv.field_14_scale);
    }
};

struct Path_StatusLight final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_StatusLight, "StatusLight", TlvTypes::StatusLight_97)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Scale", mTlv.field_12_scale);
        ADD("ID 1", mTlv.field_14_id1);
        ADD("ID 2", mTlv.field_16_id2);
        ADD("ID 3", mTlv.field_18_id3);
        ADD("ID 4", mTlv.field_1A_id4);
        ADD("ID 5", mTlv.field_1C_id5);
        ADD("Ignore Grid Snapping", mTlv.field_1E_bIgnore_grid_snapping);
    }
};

struct Path_ShadowZone final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD_HIDDEN("Center W (Unused?)", mTlv.field_10_center_w);
        ADD_HIDDEN("Center H (Unused?)", mTlv.field_12_center_h);
        ADD("R", mTlv.field_14_r);
        ADD("G", mTlv.field_16_g);
        ADD("B", mTlv.field_18_b);
        ADD("Switch ID (Unused?)", mTlv.field_1A_switch_id);
        ADD("Scale", mTlv.field_1C_scale);
    }
};

struct Path_WorkWheel final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_WorkWheel, "WorkWheel", TlvTypes::WorkWheel_79)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Switch ID", mTlv.field_12_switch_id);
        ADD("Activation Time", mTlv.field_14_activation_time);
        ADD("Off Time", mTlv.field_16_off_time);
        ADD("Turn Off When Stopped", mTlv.field_18_turn_off_when_stopped);
    }
};

struct Path_MusicTrigger final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::MusicTriggerMusicType>("Enum_MusicTriggerMusicType",
        {
            {::MusicTriggerMusicType::eDrumAmbience_0, "Drum Ambience"},
            {::MusicTriggerMusicType::eDeathDrumShort_1, "Death Drum Short"},
            {::MusicTriggerMusicType::eSecretAreaLong_2, "Secret Area Long"},
            {::MusicTriggerMusicType::eSoftChase_3, "Soft Chase"},
            {::MusicTriggerMusicType::eIntenseChase_4, "Intense Chase"},
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
        ADD("Music Delay", mTlv.field_14_music_delay);
    }
};

struct Path_AbeStart final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_AbeStart, "AbeStart", TlvTypes::AbeStart_22)
    {
        ADD("X Position", mTlv.field_10_xpos);
        ADD("Y Position", mTlv.field_12_ypos);
    }
};

struct Path_SoftLanding final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SoftLanding, "SoftLanding", TlvTypes::SoftLanding_75)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
    }
};

struct Path_WellExpress final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_WellExpress, "WellExpress", TlvTypes::WellExpress_23)
    {
        // Path_WellBase
        ADD("Scale", mTlv.field_0_scale);
        ADD("Switch ID", mTlv.field_2_switch_id);
        ADD("Other Well ID", mTlv.field_4_other_well_id);
        ADD_HIDDEN("Animation ID (Unused?)", mTlv.field_6_anim_id); // leftover from AO well FG hack

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

struct Path_SlamDoor final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SlamDoor, "SlamDoor", TlvTypes::SlamDoor_85)
    {
        ADD("Start Shut", mTlv.field_10_bStart_closed);
        ADD("Scale", mTlv.field_12_scale);
        ADD("Switch ID", mTlv.field_14_switch_id);
        ADD("Flip On Y Axis", mTlv.field_16_bStart_inverted);
        ADD("Delete", mTlv.field_18_bDelete);
    }
};

struct Path_HandStone final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_HandStone, "HandStone", TlvTypes::HandStone_61)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Camera ID 1", mTlv.field_12_camera_id1);
        ADD("Camera ID 2", mTlv.field_12_camera_id2);
        ADD("Camera ID 3", mTlv.field_12_camera_id3);
        ADD("Trigger Switch ID", mTlv.field_18_trigger_switch_id);
    }
};

struct Path_LaughingGas final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_LaughingGas, "LaughingGas", TlvTypes::LaughingGas_81)
    {
        ADD("Is Laughing Gas", mTlv.field_10_bLaughing_gas);
        ADD("Laughing Gas Switch ID", mTlv.field_12_laughing_gas_switch_id);
        ADD("Red %", mTlv.field_14_red_percent);
        ADD("Green %", mTlv.field_16_green_percent);
        ADD("Blue %", mTlv.field_18_blue_percent);
    }
};

struct Path_InvisibleSwitch final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Action", mTlv.field_12_action);
        ADD("Delay", mTlv.field_14_delay);
        ADD("Set Off Alarm", mTlv.field_16_set_off_alarm);
        ADD("Scale", mTlv.field_18_scale);
    }
};

struct Path_Water final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Water, "Water", TlvTypes::Water_77)
    {
        ADD("Max Drops", mTlv.field_10_data.field_10_max_drops);
        ADD("Switch ID", mTlv.field_10_data.field_12_switch_id);
        ADD("Splash Time", mTlv.field_10_data.field_14_splash_time);
        ADD("Splash X Velocity", mTlv.field_10_data.field_16_splash_x_velocity);
        ADD("Splash Y Velocity", mTlv.field_10_data.field_18_splash_y_velocity);
        ADD("Water Duration", mTlv.field_10_data.field_1A_water_duration);
    }
};

struct Path_GasEmitter final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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

struct Path_BackgroundAnimation final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
            {::Layer::eLayer_NotUsed_2, "Layer_2"},
            {::Layer::eLayer_BeforeWell_Half_3, "Layer_3"},
            {::Layer::eLayer_Well_Half_4, "Layer_4"},
            {::Layer::eLayer_RopeWebDrill_Half_5, "Layer_5"},
            {::Layer::eLayer_BeforeShadow_Half_6, "Layer_6"},
            {::Layer::eLayer_Shadow_Half_7, "Layer_7"},
            {::Layer::eLayer_8, "Layer_8"},
            {::Layer::eLayer_ZapLinesMuds_Half_9, "Layer_9"},
            {::Layer::eLayer_BirdPortal_Half_10, "Layer_10"},
            {::Layer::eLayer_InBirdPortal_Half_11, "Layer_11"},
            {::Layer::eLayer_FallingItemPortalClip_Half_12, "Layer_12"},
            {::Layer::eLayer_AbeMenu_Half_13, "Layer_13"},
            {::Layer::eLayer_SligGreeterFarts_Half_14, "Layer_14"},
            {::Layer::eLayer_SlogFleech_Half_15, "Layer_15"},
            {::Layer::eLayer_BombMineCar_Half_16, "Layer_16"},
            {::Layer::eLayer_Foreground_Half_17, "Layer_17"},
            {::Layer::eLayer_FG1_Half_18, "Layer_18"},
            {::Layer::eLayer_19, "Layer_19"},
            {::Layer::eLayer_Above_FG1_Half_20, "Layer_20"},
            {::Layer::eLayer_NotUsed_21, "Layer_21"},
            {::Layer::eLayer_BeforeWell_22, "Layer_22"},
            {::Layer::eLayer_Well_23, "Layer_23"},
            {::Layer::eLayer_RopeWebDrill_24, "Layer_24"},
            {::Layer::eLayer_BeforeShadow_25, "Layer_25"},
            {::Layer::eLayer_Shadow_26, "Layer_26"},
            {::Layer::eLayer_27, "Layer_27"},
            {::Layer::eLayer_ZapLinesMuds_28, "Layer_28"},
            {::Layer::eLayer_BirdPortal_29, "Layer_29"},
            {::Layer::eLayer_InBirdPortal_30, "Layer_30"},
            {::Layer::eLayer_FallingItemPortalClip_31, "Layer_31"},
            {::Layer::eLayer_AbeMenu_32, "Layer_32"},
            {::Layer::eLayer_SligGreeterFarts_33, "Layer_33"},
            {::Layer::eLayer_SlogFleech_34, "Layer_34"},
            {::Layer::eLayer_BombMineCar_35, "Layer_35"},
            {::Layer::eLayer_Foreground_36, "Layer_36"},
            {::Layer::eLayer_FG1_37, "Layer_37"},
            {::Layer::eLayer_MainMenuButton_38, "Layer_38"},
            {::Layer::eLayer_Above_FG1_39, "Layer_39"},
            {::Layer::eLayer_FadeFlash_40, "Layer_40"},
            {::Layer::eLayer_Menu_41, "Layer_41"},
            {::Layer::eLayer_Text_42, "Layer_42"},
        });
    }

    CTOR_AE(Path_BackgroundAnimation, "BackgroundAnimation", TlvTypes::BackgroundAnimation_13)
    {
        ADD("Animation ID", mTlv.field_10_anim_id);
        ADD("Is Semi Trans", mTlv.field_12_is_semi_trans);
        ADD("Semi Trans Mode", mTlv.field_14_semi_trans_mode);
        ADD_HIDDEN("Sound Effect (Unused)", mTlv.field_16_sound_effect); // was used for campfires in AO but AE doesn't have them
        ADD("ID", mTlv.field_18_id);
        ADD("Layer", mTlv.field_1A_layer);
    }
};

struct Path_LiftPoint final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Lift Point ID", mTlv.field_10_lift_point_id);
        ADD("Start Point", mTlv.field_12_bStart_point);
        ADD("Lift Type (Unused?)", mTlv.field_14_lift_type);
        ADD("Lift Point Stop Type", mTlv.field_16_lift_point_stop_type);
        ADD("Scale", mTlv.field_18_scale);
        ADD("Ignore Lift Mover", mTlv.field_1A_bIgnore_lift_mover);
    }
};

struct Path_PullRingRope final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Action", mTlv.field_12_action);
        ADD("Rope Length", mTlv.field_14_rope_length);
        ADD("Scale", mTlv.field_16_scale);
        ADD("On Sound", mTlv.field_18_on_sound);
        ADD("Off Sound", mTlv.field_1A_off_sound);
        ADD("Sound Direction", mTlv.field_1C_sound_direction);
    }
};

struct Path_MultiSwitchController final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MultiSwitchController, "MultiSwitchController", TlvTypes::MultiSwitchController_96)
    {
        ADD("Output Switch ID", mTlv.field_10_output_switch_id);
        ADD("Action", mTlv.field_12_action);
        ADD("On/Off Delay", mTlv.field_14_on_off_delay);
        ADD("Input Switch ID 1", mTlv.field_16_input_switch_id1);
        ADD("Input Switch ID 2", mTlv.field_18_input_switch_id2);
        ADD("Input Switch ID 3", mTlv.field_1A_input_switch_id3);
        ADD("Input Switch ID 4", mTlv.field_1C_input_switch_id4);
        ADD("Input Switch ID 5", mTlv.field_1E_input_switch_id5);
        ADD("Input Switch ID 6", mTlv.field_20_input_switch_id6);
    }
};

struct Path_SecurityOrb final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SecurityOrb, "SecurityOrb", TlvTypes::SecurityOrb_19)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Disabled Resources", mTlv.field_12_disabled_resources);
    }
};

struct Path_InvisibleZone final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_InvisibleZone, "InvisibleZone", TlvTypes::InvisibleZone_33)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_ContinuePoint final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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

struct Path_WheelSyncer final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::WheelSyncerOutputRequirement>("Enum_WheelSyncerOutputRequirement",
        {
            {::WheelSyncerOutputRequirement::eAllOn_0, "All On"},
            {::WheelSyncerOutputRequirement::e1OnAnd2Off_1, "1 On And 2 Off"},
            {::WheelSyncerOutputRequirement::e1Or2On_2, "1 Or 2 On"},
            {::WheelSyncerOutputRequirement::e1OnOr2Off_3, "1 On Or 2 Off"},
        });
    }

    CTOR_AE(Path_WheelSyncer, "WheelSyncer", TlvTypes::WheelSyncer_65)
    {
        ADD("Input Switch ID 1", mTlv.field_10_input_switch_id1);
        ADD("Input Switch ID 2", mTlv.field_12_input_switch_id2);
        ADD("Output Switch ID", mTlv.field_14_output_switch_id);
        ADD("Output Requirement", mTlv.field_16_output_requirement);
        ADD("Input Switch ID 3", mTlv.field_18_input_switch_id3);
        ADD("Input Switch ID 4", mTlv.field_1A_input_switch_id4);
        ADD("Input Switch ID 5", mTlv.field_1C_input_switch_id5);
        ADD("Input Switch ID 6", mTlv.field_1E_input_switch_id6);
    }
};

struct Path_LevelLoader final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_LevelLoader, "LevelLoader", TlvTypes::LevelLoader_86)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Destination Level", mTlv.field_12_dest_level);
        ADD("Destination Path", mTlv.field_14_dest_path);
        ADD("Destination Camera", mTlv.field_16_dest_camera);
        ADD("Movie ID", mTlv.field_18_movie_id);
    }
};

struct Path_Pulley final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Pulley, "Pulley", TlvTypes::Pulley_21)
    {
        ADD("Unused1", mTlv.field_10_unused1);
        ADD("Unused2", mTlv.field_12_unused2);
    }
};

struct Path_FlyingSlig final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_FlyingSlig, "FlyingSlig", TlvTypes::FlyingSlig_82)
    {
        ADD("Scale", mTlv.field_10_data.field_0_scale);
        ADD("Spawn Delay State", mTlv.field_10_data.field_2_state);
        ADD("Spawn Move Delay", mTlv.field_10_data.field_4_spawn_move_delay);
        ADD("Patrol Pause Min", mTlv.field_10_data.field_6_patrol_pause_min);
        ADD("Patrol Pause Max", mTlv.field_10_data.field_8_patrol_pause_max);
        ADD("Start Direction", mTlv.field_10_data.field_A_direction);
        ADD("Panic Delay", mTlv.field_10_data.field_C_panic_delay);
        ADD("Give Up Chase Delay", mTlv.field_10_data.field_E_give_up_chase_delay);
        ADD("Pre-chase Delay", mTlv.field_10_data.field_10_prechase_delay);
        ADD("Slig Bound/Persist ID", mTlv.field_10_data.field_12_slig_bound_id);
        ADD("Alerted Listen Time", mTlv.field_10_data.field_14_alerted_listen_time);
        ADD("Spawner Switch ID", mTlv.field_10_data.field_16_spawner_switch_id);
        ADD("Grenade Delay", mTlv.field_10_data.field_18_grenade_delay);
        ADD("Max Velocity", mTlv.field_10_data.field_1A_max_velocity);
        ADD("Launch Grenade Switch ID", mTlv.field_10_data.field_1C_launch_switch_id);
        ADD("Persistant", mTlv.field_10_data.field_1E_persistant);
    }
};

struct Path_FlyingSligSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_FlyingSligSpawner, "FlyingSligSpawner", TlvTypes::FlyingSligSpawner_92)
    {
        ADD("Scale", mTlv.field_10.field_0_scale);
        ADD("Spawn Delay State", mTlv.field_10.field_2_state);
        ADD("Spawn Move Delay", mTlv.field_10.field_4_spawn_move_delay);
        ADD("Patrol Pause Min", mTlv.field_10.field_6_patrol_pause_min);
        ADD("Patrol Pause Max", mTlv.field_10.field_8_patrol_pause_max);
        ADD("Start Direction", mTlv.field_10.field_A_direction);
        ADD("Panic Delay", mTlv.field_10.field_C_panic_delay);
        ADD("Give Up Chase Delay", mTlv.field_10.field_E_give_up_chase_delay);
        ADD("Pre-Chase Delay", mTlv.field_10.field_10_prechase_delay);
        ADD("Slig Bound/Persist ID", mTlv.field_10.field_12_slig_bound_id);
        ADD("Listen Time", mTlv.field_10.field_14_alerted_listen_time);
        ADD("Spawner Switch ID", mTlv.field_10.field_16_spawner_switch_id);
        ADD("Grenade Delay", mTlv.field_10.field_18_grenade_delay);
        ADD("Max Velocity", mTlv.field_10.field_1A_max_velocity);
        ADD("Launch Grenade Switch ID", mTlv.field_10.field_1C_launch_switch_id);
        ADD("Persistant", mTlv.field_10.field_1E_persistant);
    }
};

struct Path_DeathDrop final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_DeathDrop, "DeathDrop", TlvTypes::DeathDrop_4)
    {
        ADD_HIDDEN("Unused1", mTlv.field_10_animation);
        ADD_HIDDEN("Unused2", mTlv.field_12_sound);
        ADD_HIDDEN("Unused3", mTlv.field_14_id);
        ADD_HIDDEN("Unused4", mTlv.field_16_action);
        ADD_HIDDEN("Unused5", mTlv.field_18_unknown1);
        ADD_HIDDEN("Unused6", mTlv.field_1A_unknown2);
    }
};

struct Path_SligSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligSpawner, "SligSpawner", TlvTypes::SligSpawner_37)
    {
        ADD("Scale", mTlv.scale);
        ADD("Start State", mTlv.start_state);
        ADD("Pause Time", mTlv.pause_time);
        ADD("Pause Left Min", mTlv.pause_left_min);
        ADD("Pause Left Max", mTlv.pause_left_max);
        ADD("Pause Right Min", mTlv.pause_right_min);
        ADD("Pause Right Max", mTlv.pause_right_max);
        ADD("Shoot Possessed Sligs", mTlv.shoot_possessed_sligs);
        ADD("Shoot On Sight Delay", mTlv.shoot_on_sight_delay);
        ADD("Bullet Shoot Count", mTlv.number_of_times_to_shoot);
        ADD("unknown", mTlv.unknown);
        ADD("code_1", mTlv.code_1);
        ADD("code_2", mTlv.code_2);
        ADD("Chase Abe When Spotted", mTlv.chase_abe_when_spotted);
        ADD("Start Direction", mTlv.start_direction);
        ADD("Panic Timeout", mTlv.panic_timeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.num_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.panic_sound_timeout);
        ADD("Stop Chase Delay", mTlv.stop_chase_delay);
        ADD("Time To Wait Before Chase", mTlv.time_to_wait_before_chase);
        ADD("Slig Bound/Persist ID", mTlv.slig_bound_id);
        ADD("Alerted Listen Time", mTlv.alerted_listen_time);
        ADD("Percent Say What", mTlv.percent_say_what);
        ADD("Percent Beat Mudokon", mTlv.percent_beat_mud);
        ADD("Talk To Abe (Unused?)", mTlv.talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.dont_shoot);
        ADD("Z Shoot Delay", mTlv.z_shoot_delay);
        ADD("Stay Awake", mTlv.stay_awake);
        ADD("Disable Resources", mTlv.disable_resources);
        ADD("Noise Wake Up Distance (Grids)", mTlv.noise_wake_up_distance);
        ADD("ID", mTlv.id);
        ADD("Unlimited Spawns", mTlv.unlimited_spawns);
    }
};

struct Path_SligLeftBound final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligLeftBound, "SligLeftBound", TlvTypes::SligBoundLeft_32)
    {
        ADD("Slig Bound/Persist ID", mTlv.field_10_slig_bound_id);
        ADD("Disabled Resources", mTlv.field_12_disabled_resources);
    }
};

struct Path_SligRightBound final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligRightBound, "SligRightBound", TlvTypes::SligBoundRight_45)
    {
        ADD("Slig Bound/Persist ID", mTlv.field_10_slig_bound_id);
        ADD("Disabled Resources", mTlv.field_12_disabled_resources);
    }
};

struct Path_SligPersist final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligPersist, "SligPersist", TlvTypes::SligPersist_46)
    {
        ADD("Slig Bound/Persist ID", mTlv.field_10_slig_bound_id);
        ADD("Disabled Resources", mTlv.field_12_disabled_resources);
    }
};

struct Path_ZSligCover final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ZSligCover, "ZSligCover", TlvTypes::ZSligCover_50)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_WellLocal final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_WellLocal, "WellLocal", TlvTypes::LocalWell_8)
    {
        // Path_WellBase
        ADD("Scale", mTlv.field_0_scale);
        ADD("Switch ID", mTlv.field_2_switch_id);
        ADD_HIDDEN("Other Well ID", mTlv.field_4_other_well_id); // hidden because this is only used by the well express
        ADD_HIDDEN("Animation ID (Unused?)", mTlv.field_6_anim_id); // leftover from AO well FG hack

        // Path_WellLocal
        ADD("Disabled XPos", mTlv.field_18_off_dx);
        ADD("Disabled YPos", mTlv.field_1A_off_dy);
        ADD("Enabled XPos", mTlv.field_1C_on_dx);
        ADD("Enabled YPos", mTlv.field_1E_on_dy);
        ADD("Emit Leaves", mTlv.field_20_bEmit_leaves);
        ADD("Leaf XPos", mTlv.field_22_leaf_x);
        ADD("Leaf YPos", mTlv.field_24_leaf_y);
    }
};

struct Path_BrewMachine final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_BrewMachine, "BrewMachine", TlvTypes::BrewMachine_101)
    {
        ADD("Brew Count", mTlv.field_10_brew_count);
    }
};

struct Path_Drill final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::DrillBehavior>("Enum_DrillBehavior",
        {
            {::DrillBehavior::eNotInteractable_0, "Not Interactable"},
            {::DrillBehavior::eToggle_1, "Toggle"},
            {::DrillBehavior::eUse_2, "Use"},
        });

        types.AddEnum<::DrillDirection>("Enum_DrillDirection",
        {
            {::DrillDirection::eDown_0, "Down"},
            {::DrillDirection::eRight_1, "Right"},
            {::DrillDirection::eLeft_2, "Left"},
        });
    }

    CTOR_AE(Path_Drill, "Drill", TlvTypes::Drill_90)
    {
        ADD("Scale", mTlv.field_10_data.field_10_scale);
        ADD("Min Off Time", mTlv.field_10_data.field_12_min_off_time);
        ADD("Max Off Time", mTlv.field_10_data.field_14_max_off_time);
        ADD("Switch ID", mTlv.field_10_data.field_16_switch_id);
        ADD("Behavior", mTlv.field_10_data.field_18_behavior);
        ADD("Speed", mTlv.field_10_data.field_1A_speed);
        ADD("Start State On", mTlv.field_10_data.field_1C_bStart_state_on);
        ADD("Off Speed", mTlv.field_10_data.field_1E_off_speed);
        ADD("Min Off Time Speed Change", mTlv.field_10_data.field_20_min_off_time_speed_change);
        ADD("Max Off Time Speed Change", mTlv.field_10_data.field_22_max_off_time_speed_change);
        ADD("Start Position Bottom", mTlv.field_10_data.field_24_bStart_position_bottom);
        ADD("Start Direction", mTlv.field_10_data.field_26_direction);
    }
};

struct Path_Mine final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Mine, "Mine", TlvTypes::Mine_24)
    {
        ADD_HIDDEN("Num Patterns (Unused?)", mTlv.field_10_num_patterns);
        ADD_HIDDEN("Pattern (Unused?)", mTlv.field_12_pattern);
        ADD("Scale", mTlv.field_14_scale);
        ADD("Disabled Resources", mTlv.field_16_disabled_resources);
        ADD("Persist Offscreen", mTlv.field_18_persist_offscreen);
    }
};

struct Path_Slog final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Slog, "Slog", TlvTypes::Slog_16)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Start Direction", mTlv.field_12_direction);
        ADD("Asleep", mTlv.field_14_asleep);
        ADD("Wake Up Anger", mTlv.field_16_wake_up_anger);
        ADD("Bark Anger", mTlv.field_18_bark_anger);
        ADD("Chase Anger", mTlv.field_1A_chase_anger);
        ADD("Chase Delay", mTlv.field_1C_chase_delay);
        ADD("Disabled Resources", mTlv.field_1E_disabled_resources);
        ADD("Anger Switch ID", mTlv.field_20_anger_switch_id);
        ADD("Bone Eating Time", mTlv.field_22_bone_eating_time);
    }
};

struct Path_ResetSwitchRange final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ResetSwitchRange, "ResetSwitchRange", TlvTypes::ResetSwitchRange_76)
    {
        ADD("Reset Switches", mTlv.field_10_reset_switch_ids);
        ADD("Start Switch ID", mTlv.field_12_start_switch_id);
        ADD("End Switch ID", mTlv.field_14_end_switch_id);
        ADD("Skip Switch ID", mTlv.field_16_skip_switch_id);
        ADD("Free Path Resources", mTlv.field_18_free_path_res);
        ADD("Path ID To Free Resources", mTlv.field_1A_path_to_free_id);
        ADD("Enabled", mTlv.field_1C_bEnabled);
    }
};

struct Path_TrapDoor final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_TrapDoor::StartState>("Enum_TrapDoorStartState",
        {
            {::Path_TrapDoor::StartState::eOpen_0, "Open"},
            {::Path_TrapDoor::StartState::eClosed_1, "Closed"}
        });
    }

    CTOR_AE(Path_TrapDoor, "TrapDoor", TlvTypes::TrapDoor_30)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Start State", mTlv.field_12_start_state);
        ADD("Self Closing", mTlv.field_14_self_closing);
        ADD("Scale", mTlv.field_16_scale);
        ADD("Dest Level (Unused?)", mTlv.field_18_dest_level);
        ADD("Direction", mTlv.field_1A_direction);
        ADD("X Offset", mTlv.field_1C_xOff);
        ADD("Stay Open Time", mTlv.field_1E_stay_open_time);
    }
};

struct Path_PathTransition final : public ReliveAPI::TlvObjectBaseAE
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

struct Path_LiftMover final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_LiftMover::YDirection>("Enum_LiftMoverYDirection",
        {
            {::Path_LiftMover::YDirection::eDown_0, "Down"},
            {::Path_LiftMover::YDirection::eUp_1, "Up"},
        });
    }

    CTOR_AE(Path_LiftMover, "LiftMover", TlvTypes::LiftMover_39)
    {
        ADD("Lift Mover Switch ID", mTlv.field_10_lift_mover_switch_id);
        ADD("Target Lift Point ID", mTlv.field_12_target_lift_point_id);
        ADD("Move Direction", mTlv.field_14_move_direction);
    }
};

struct Path_RockSack final : public ReliveAPI::TlvObjectBaseAE
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

struct Path_TimerTrigger final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_TimerTrigger, "TimerTrigger", TlvTypes::TimerTrigger_57)
    {
        ADD("Input Switch ID", mTlv.field_10_input_switch_id);
        ADD("Trigger Interval", mTlv.field_12_trigger_interval);
        ADD("Output Switch ID 1", mTlv.field_14_output_switch_id1);
        ADD("Output Switch ID 2", mTlv.field_16_output_switch_id2);
        ADD("Output Switch ID 3", mTlv.field_18_output_switch_id3);
        ADD("Output Switch ID 4", mTlv.field_1A_output_switch_id4);
    }
};

struct Path_MotionDetector final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Disable Switch ID", mTlv.field_1C_disable_switch_id);
        ADD("Alarm Switch ID", mTlv.field_1E_alarm_switch_id);
        ADD("Alarm Duration", mTlv.field_20_alarm_duration);
    }
};

struct Path_MineCar final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MineCar, "MineCar", TlvTypes::MineCar_93)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Max Damage (Unused?)", mTlv.field_12_max_damage);
    }
};

struct Path_ExplosionSet final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ExplosionSet, "ExplosionSet", TlvTypes::ExplosionSet_95)
    {
        ADD("Start Enabled", mTlv.field_10_bStart_enabled);
        ADD("Switch ID", mTlv.field_12_switch_id);
        ADD("Big Rocks", mTlv.field_14_big_rocks);
        ADD("Start Delay", mTlv.field_16_start_delay);
        ADD("Start Direction", mTlv.field_18_start_direction);
        ADD("Explosion Interval", mTlv.field_1A_explosion_interval);
        ADD("Grid Spacing", mTlv.field_1C_grid_spacing);
        ADD("Increasing Grid Spacing", mTlv.field_1E_increasing_grid_spacing);
        ADD("Scale", mTlv.field_20_scale);
    }
};

struct Path_ColourfulMeter final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ColourfulMeter, "ColourfulMeter", TlvTypes::ColourfulMeter_91)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Number Of Meter Bars", mTlv.field_12_number_of_meter_bars);
        ADD("Mines Alarm Countdown (Seconds)", mTlv.field_14_mines_alarm_countdown);
        ADD("Start Filled", mTlv.field_16_bStartFilled);
    }
};

struct Path_Alarm final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Alarm, "Alarm", TlvTypes::Alarm_100)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Duration", mTlv.field_12_duration);
    }
};

struct Path_DemoSpawnPoint final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_DemoSpawnPoint, "DemoSpawnPoint", TlvTypes::DemoSpawnPoint_87)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_SlapLock final : public ReliveAPI::TlvObjectBaseAE
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
        ADD("Toggle Switch ID", mTlv.field_1E_toggle_switch_id);
    }
};

struct Path_Slurg final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Slurg, "Slurg", TlvTypes::Slurg_84)
    {
        ADD("Time Until Turning Around", mTlv.field_10_slurg_data.field_0_moving_timer);
        ADD("Start Direction", mTlv.field_10_slurg_data.field_2_start_direction);
        ADD("Scale", mTlv.field_10_slurg_data.field_4_scale);
        ADD("Switch ID (increment by 1 on death)", mTlv.field_10_slurg_data.field_6_switch_id);
    }
};

struct Path_DoorBlocker final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_DoorBlocker, "DoorBlocker", TlvTypes::DoorBlocker_109)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Switch ID", mTlv.field_12_switch_id);
    }
};

struct Path_Dove final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Dove, "Dove", TlvTypes::Dove_9)
    {
        ADD("Dove Count", mTlv.field_10_dove_count);
        ADD("Pixel Perfect", mTlv.field_12_pixel_perfect);
        ADD("Scale", mTlv.field_14_scale);
    }
};

struct Path_BirdPortalExit final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_BirdPortalExit, "BirdPortalExit", TlvTypes::BirdPortalExit_29)
    {
        ADD("Exit Direction", mTlv.field_10_side);
        ADD("Scale", mTlv.field_12_scale);
    }
};

struct Path_DoorFlame final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_DoorFlame, "DoorFlame", TlvTypes::DoorFlame_51)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Scale", mTlv.field_12_scale);
        ADD("Colour", mTlv.field_14_colour);
    }
};

struct Path_TrainDoor final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_TrainDoor, "TrainDoor", TlvTypes::TrainDoor_111)
    {
        ADD("Direction", mTlv.field_10_direction);
    }
};

struct Path_Greeter final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Greeter, "Greeter", TlvTypes::Greeter_106)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Motion Detector Speed", mTlv.field_12_motion_detector_speed);
        ADD("Start Direction", mTlv.field_14_start_direction);
    }
};

struct Path_ScrabLeftBound final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ScrabLeftBound, "ScrabLeftBound", TlvTypes::ScrabLeftBound_43)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_ScrabRightBound final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ScrabRightBound, "ScrabRightBound", TlvTypes::ScrabRightBound_44)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_CreditsController final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_CreditsController, "CreditsController", TlvTypes::CreditsController_62)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_MovieHandstone final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MovieHandstone, "MovieHandstone", TlvTypes::MovieHandStone_27)
    {
        ADD("Scale", mTlv.scale);
        ADD("Movie Number", mTlv.movie_number);
        ADD("Trigger Switch ID", mTlv.trigger_switch_id);
        ADD("Padding", mTlv.padding);
    }
};

struct Path_MovingBomb final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MovingBomb, "MovingBomb", TlvTypes::MovingBomb_52)
    {
        ADD("Speed", mTlv.field_10_speed);
        ADD("Start Moving Switch ID", mTlv.field_12_start_moving_switch_id);
        ADD("Triggered By Alarm", mTlv.field_14_bTriggered_by_alarm);
        ADD("Scale", mTlv.field_16_scale);
        ADD("Max Rise", mTlv.field_18_max_rise);
        ADD("Disabled Resources", mTlv.field_1A_disabled_resources);
        ADD("Start Speed", mTlv.field_1C_start_speed);
        ADD("Persist Offscreen", mTlv.field_1E_persist_offscreen);
    }
};

struct Path_SecurityDoor final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SecurityDoor, "SecurityDoor", TlvTypes::SecurityDoor_58)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Switch ID", mTlv.field_12_switch_id);
        ADD("Code 1", mTlv.field_14_code_1);
        ADD("Code 2", mTlv.field_16_code_2);
        ADD("X Position", mTlv.field_18_xpos);
        ADD("Y Position", mTlv.field_1A_ypos);
    }
};

struct Path_CrawlingSlig final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_CrawlingSlig::State>("Enum_CrawlingSligState",
        {
            {::Path_CrawlingSlig::State::eSleeping_0, "Sleeping"},
            {::Path_CrawlingSlig::State::eSleeping_1, "Sleeping2 (Duplicate)"},
            {::Path_CrawlingSlig::State::eAwake_2, "Awake"},
        });

        types.AddEnum<::Path_CrawlingSlig::CrawlDirection>("Enum_CrawlDirection",
        {
            {::Path_CrawlingSlig::CrawlDirection::eLeft_0, "Left"},
            {::Path_CrawlingSlig::CrawlDirection::eRight_1, "Right"},
            {::Path_CrawlingSlig::CrawlDirection::eRandom_2, "Random"},
        });
    }

    CTOR_AE(Path_CrawlingSlig, "CrawlingSlig", TlvTypes::CrawlingSlig_103)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Start Direction", mTlv.field_12_direction);
        ADD("State", mTlv.field_14_state);
        ADD("Crawl Direction", mTlv.field_16_crawl_direction);
        ADD("Panic Switch ID", mTlv.field_18_panic_switch_id);
        ADD("Respawn On Death", mTlv.field_1A_respawn_on_death);
    }
};

struct Path_SligGetPants final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligGetPants, "SligGetPants", TlvTypes::SligGetPants_104)
    {
        ADD("Scale", mTlv.Scale);
        ADD("Start State", mTlv.start_state);
        ADD("Pause Time", mTlv.pause_time);
        ADD("Pause Left Min", mTlv.pause_left_min);
        ADD("Pause Left Max", mTlv.pause_left_max);
        ADD("Pause Right Min", mTlv.pause_right_min);
        ADD("Pause Right Max", mTlv.pause_right_max);
        ADD("Shoot Possessed Sligs", mTlv.shoot_possessed_sligs);
        ADD("Shoot On Sight Delay", mTlv.shoot_on_sight_delay);
        ADD("Bullet Shoot Count", mTlv.num_times_to_shoot);
        ADD("unknown1", mTlv.padding);
        ADD("Code 1", mTlv.code1);
        ADD("Code 2", mTlv.code2);
        ADD("Chase Abe When Spotted", mTlv.chase_abe_when_spotted);
        ADD("Start Direction", mTlv.start_direction);
        ADD("Panic Timeout", mTlv.panic_timeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.amount_of_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.panic_sounds_timeout);
        ADD("Stop Chase Delay", mTlv.stop_chase_delay);
        ADD("Time To Wait Before Chase", mTlv.time_to_wait_before_chase);
        ADD("Slig Bound/Persist ID", mTlv.slig_bound_id);
        ADD("Alerted Listen Time", mTlv.alerted_listen_time);
        ADD("Percent Say What", mTlv.percent_say_what);
        ADD("Percent Beat Mudokon", mTlv.percent_beat_mud);
        ADD("Talk To Abe (Unused?)", mTlv.talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.dont_shoot);
        ADD("Z Shoot Delay", mTlv.z_shoot_delay);
        ADD("Stay Awake", mTlv.stay_awake);
        ADD("Disable Resources", mTlv.disable_resources);
        ADD("Noise Wake Up Distance (Grids)", mTlv.noise_wake_up_distance);
        ADD("Slig Spawner Switch ID", mTlv.slig_spawner_switch_id);
        ADD("Unlimited Spawns", mTlv.unlimited_spawns);
    }
};

struct Path_SligGetWings final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SligGetWings, "SligGetWings", TlvTypes::SligGetWings_105)
    {
        ADD("Scale", mTlv.scale);
        ADD("Spawn Delay State", mTlv.state);
        ADD("Spawn Move Delay", mTlv.spawn_move_delay);
        ADD("Patrol Pause Min", mTlv.patrol_pause_min);
        ADD("Patrol Pause Max", mTlv.patrol_pause_max);
        ADD("Start Direction", mTlv.start_direction);
        ADD("Panic Delay", mTlv.panic_delay);
        ADD("Give Up Chase Delay", mTlv.give_up_chase_delay);
        ADD("Pre-chase Delay", mTlv.prechase_delay);
        ADD("Slig Bound/Persist ID", mTlv.slig_bound_id);
        ADD("Alerted Listen Time", mTlv.alerted_listen_time);
        ADD("Spawner Switch ID", mTlv.spawner_switch_id);
        ADD("Grenade Delay", mTlv.grenade_delay);
        ADD("Max Velocity", mTlv.max_velocity);
        ADD("Launch Grenade Switch ID", mTlv.launch_switch_id);
        ADD("Persistant", mTlv.persistant);
    }
};

struct Path_CrawlingSligButton final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        // this probably shouldn't be here since the sound/sound direction implementation is extremely scuffed
        // but maybe someone wants to use it anyway
        types.AddEnum<::CrawlingSligButtonSounds>("Enum_CrawlingSligButtonSounds",
        {
            {::CrawlingSligButtonSounds::None_0, "None"},
            {::CrawlingSligButtonSounds::SackHit_1, "Sack Hit"},
            {::CrawlingSligButtonSounds::FallingItemPresence2_2, "Falling Item Presence 2"},
            {::CrawlingSligButtonSounds::SecurityOrb_3, "Security Orb"},
            {::CrawlingSligButtonSounds::SackHit_4, "Sack Hit (duplicate)"},
            {::CrawlingSligButtonSounds::Bullet1_5, "Bullet"},
            {::CrawlingSligButtonSounds::AbeGenericMovement_6, "Abe Generic Movement"},
        });
    }
    CTOR_AE(Path_CrawlingSligButton, "CrawlingSligButton", TlvTypes::CrawlingSligButton_107)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Switch ID", mTlv.field_12_switch_id);
        ADD("Action", mTlv.field_14_action);
        ADD("On Sound", mTlv.field_16_on_sound);
        ADD("Off Sound", mTlv.field_18_off_sound);
        ADD_HIDDEN("Sound Direction", mTlv.field_1A_sound_direction);
    }
};

struct Path_Glukkon final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::GlukkonTypes>("Enum_GlukkonTypes",
        {
            {::GlukkonTypes::Normal_0, "Normal"},
            {::GlukkonTypes::StoryAslik_1, "Story Aslik"},
            {::GlukkonTypes::StoryDripik_2, "Story Dripik"},
            {::GlukkonTypes::StoryPhleg_3, "Story Phleg"},
            {::GlukkonTypes::Normal_4, "Normal 4"},
            {::GlukkonTypes::Normal_5, "Normal 5"},
        });

        types.AddEnum<::Path_Glukkon::StartDirection>("Enum_GlukkonStartDirection",
        {
            {::Path_Glukkon::StartDirection::eRight_0, "Right"},
            {::Path_Glukkon::StartDirection::eLeft_1, "Left"},
        });

        types.AddEnum<::Path_Glukkon::SpawnType>("Enum_GlukkonSpawnType",
        {
            {::Path_Glukkon::SpawnType::eRegularSpawn_0, "Regular Spawn"},
            {::Path_Glukkon::SpawnType::eFacingLeft_1, "Facing Left"},
            {::Path_Glukkon::SpawnType::eFacingRight_2, "Facing Right"},
            {::Path_Glukkon::SpawnType::eFullSpawnEffects_3, "Full Spawn Effects"},
        });

        types.AddEnum<::Path_Glukkon::Behavior>("Enum_GlukkonBehavior",
        {
            {::Path_Glukkon::Behavior::eIgnoreWalls_0, "Ignore Walls"},
            {::Path_Glukkon::Behavior::eCheckForWalls_1, "Check For Walls"},
        });
    }

    CTOR_AE(Path_Glukkon, "Glukkon", TlvTypes::Glukkon_73)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Start Direction", mTlv.field_12_start_direction);
        ADD("Behavior", mTlv.field_14_behaviour);
        ADD("Scream Help Delay", mTlv.field_16_scream_help_delay);
        ADD("Help Switch ID", mTlv.field_18_help_switch_id);
        ADD("To Calm Delay", mTlv.field_1A_to_calm_delay);
        ADD("Spawn Switch ID", mTlv.field_1C_spawn_switch_id);
        ADD("Spawn Type", mTlv.field_1E_spawn_type);
        ADD("Spawn Delay", mTlv.field_20_spawn_delay);
        ADD("Glukkon Type", mTlv.field_22_glukkon_type);
        ADD("Death Switch ID", mTlv.field_24_death_switch_id);
        ADD("Play Movie Switch ID", mTlv.field_26_play_movie_switch_id);
        ADD("Movie To Play (fmv ID)", mTlv.field_28_movie_to_play_fmvID);
    }
};

struct Path_GlukkonSwitch final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::Path_GlukkonSwitch::Scale>("Enum_GlukkonSwitchScale",
        {
            {::Path_GlukkonSwitch::Scale::eHalf_0, "Half"},
            {::Path_GlukkonSwitch::Scale::eFull_1, "Full"},
        });
    };

    CTOR_AE(Path_GlukkonSwitch, "GlukkonSwitch", TlvTypes::GlukkonSwitch_108)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("OK Switch ID", mTlv.field_12_ok_switch_id);
        ADD("Fail Switch ID", mTlv.field_14_fail_switch_id);
        ADD("X Position", mTlv.field_16_xpos);
        ADD("Y Position", mTlv.field_18_ypos);
    }
};

struct Path_GasCountDown final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_GasCountDown, "GasCountDown", TlvTypes::GasCountdown_69)
    {
        ADD("Start Timer Switch ID", mTlv.field_10_start_timer_switch_id);
        ADD("Gas Countdown Time", mTlv.field_12_gas_countdown_timer);
        ADD("Stop Timer Switch ID", mTlv.field_14_stop_timer_switch_id);
    }
};

struct Path_FallingItem final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_FallingItem, "FallingItem", TlvTypes::FallingItem_11)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Scale", mTlv.field_12_scale);
        ADD("Fall Interval", mTlv.field_14_fall_interval);
        ADD("Max Falling Items", mTlv.field_16_max_falling_items);
        ADD("Reset Switch ID After Use", mTlv.field_18_reset_switch_id_after_use);
    }
};

struct Path_BoneBag final : public ReliveAPI::TlvObjectBaseAE
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

// TODO: unknown field
struct Path_SecurityClaw final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SecurityClaw, "SecurityClaw", TlvTypes::SecurityClaw_35)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Disabled Resources", mTlv.field_12_disabled_resources);
        ADD("Unknown", mTlv.field_14_unknown);
    }
};

struct Path_FootSwitch final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
        {
            {::FootSwitchTriggerBy::eAbe_0, "Abe"},
            {::FootSwitchTriggerBy::eAnyone_1, "Anyone"},
        });
    }

    CTOR_AE(Path_FootSwitch, "FootSwitch", TlvTypes::FootSwitch_34)
    {
        ADD("Switch ID", mTlv.field_10_switch_id);
        ADD("Scale", mTlv.field_12_scale);
        ADD("Action", mTlv.field_14_action);
        ADD("Triggered By", mTlv.field_16_trigger_by);
    }
};

struct Path_SlogHut final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SlogHut, "SlogHut", TlvTypes::SlogHut_72)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Switch ID", mTlv.field_12_switch_id);
        ADD("Zzz Interval", mTlv.field_14_Zzz_interval);
    }
};

struct Path_SlogSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SlogSpawner, "SlogSpawner", TlvTypes::SlogSpawner_68)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Max Slogs", mTlv.field_12_max_slogs);
        ADD("Max Slogs At A Time", mTlv.field_14_max_slogs_at_a_time);
        ADD("Start Direction", mTlv.field_16_start_direction);
        ADD("Slog Spawn Interval", mTlv.field_18_slog_spawn_interval);
        ADD("Spawner Switch ID", mTlv.field_1A_spawner_switch_id);
        ADD("Listen To Sligs", mTlv.field_1C_listen_to_sligs);
        ADD("Chase Delay", mTlv.field_1E_chase_delay);
    }
};

struct Path_MainMenuController final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MainMenuController, "MainMenuController", TlvTypes::MainMenuController_54)
    {
        EMPTY_CTOR_AE();
    }
};

struct Path_Null_63 final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Null_63, "Null_63", TlvTypes::Null_63)
    {
        ADD("padding1", mTlv.padding1);
        ADD("padding2", mTlv.padding2);
    }
};

struct Path_Scrab final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_Scrab, "Scrab", TlvTypes::Scrab_41)
    {
        ADD("Scale", mTlv.field_10_scale);
        ADD("Attack Delay", mTlv.field_12_attack_delay);
        ADD("Patrol Type Run Or Walk Chance (6 Max)", mTlv.field_14_patrol_type_run_or_walk_chance);
        ADD("Left Min Delay", mTlv.field_16_left_min_delay);
        ADD("Left Max Delay", mTlv.field_18_left_max_delay);
        ADD("Right Min Delay", mTlv.field_1A_right_min_delay);
        ADD("Right Max Delay", mTlv.field_1C_right_max_delay);
        ADD("Pause After Chase Delay", mTlv.field_1E_pause_after_chase_delay);
        ADD("Disabled Resources", mTlv.field_20_disabled_resources);
        ADD("Roar Randomly", mTlv.field_22_roar_randomly);
        ADD("Persistant", mTlv.field_24_persistant);
        ADD("Possessed Max Whirl Attack Duration", mTlv.field_26_possessed_max_whirl_attack_duration);
        ADD_HIDDEN("Unused", mTlv.field_28_unused);
        ADD("Kill Enemy", mTlv.field_2A_bKill_enemy);
    }
};

struct Path_ScrabSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Scrab Attack Delay", mTlv.field_12_attack_delay);
        ADD("Scrab Patrol Type Run Or Walk Chance (6 Max)", mTlv.field_14_patrol_type_run_or_walk_chance);
        ADD("Scrab Left Min Delay", mTlv.field_16_left_min_delay);
        ADD("Scrab Left Max Delay", mTlv.field_18_left_max_delay);
        ADD("Scrab Right Min Delay", mTlv.field_1A_right_min_delay);
        ADD("Scrab Right Max Delay", mTlv.field_1C_right_max_delay);
        ADD("Scrab Pause After Chase Delay", mTlv.field_1E_pause_after_chase_delay);
        ADD("Scrab Disabled Resources", mTlv.field_20_disabled_resources);
        ADD("Scrab Roar Randamly", mTlv.field_22_roar_randomly);
        ADD("Scrab Persistant", mTlv.field_24_persistant);
        ADD("Scrab Whirl Attack Duration", mTlv.field_26_possessed_max_whirl_attack_duration);
        ADD("Scrab Unused", mTlv.field_28_unused);
        ADD("Scrab Kill Enemy", mTlv.field_2A_bKill_enemy);

        // Spawner properties
        ADD("Spawner Switch ID", mTlv.field_2C_spawner_switch_id);
        ADD("Scrab Spawn Direction", mTlv.field_2E_spawn_direction);
    }
};

struct Path_SlurgSpawner final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_SlurgSpawner, "SlurgSpawner", TlvTypes::SlurgSpawner_89)
    {
        ADD("Time Until Turning Around", mTlv.field_10_slurg_data.field_0_moving_timer);
        ADD("Start Direction", mTlv.field_10_slurg_data.field_2_start_direction);
        ADD("Scale", mTlv.field_10_slurg_data.field_4_scale);
        ADD("Switch ID (increment by 1 on death)", mTlv.field_10_slurg_data.field_6_switch_id);

        ADD("Spawn Interval", mTlv.field_18_spawner_data.field_8_spawn_delay_between_slurgs);
        ADD("Max Slurgs", mTlv.field_18_spawner_data.field_A_max_slurgs);
        ADD("Spawner Switch ID", mTlv.field_18_spawner_data.field_C_switch_id);
    }
};

struct Path_Paramite final : public ReliveAPI::TlvObjectBaseAE
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
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
        ADD("Alone - Chase Delay", mTlv.field_14_alone_chase_delay);
        ADD("Surprise Web Delay", mTlv.field_16_surprise_web_delay_timer);
        ADD("Meat Eating Time", mTlv.field_18_meat_eating_time);
        ADD("Group - Chase Delay", mTlv.field_1A_group_chase_delay);
        ADD("Disabled Resources", mTlv.field_1C_disabled_resources);
        ADD("Surprise Web Switch ID", mTlv.field_1E_id);
        ADD("Hiss Before Attacking", mTlv.field_20_hiss_before_attack);
        ADD("Delete When Out Of Sight", mTlv.field_22_delete_when_out_of_sight);
        ADD("Attack Fleeches", mTlv.field_24_bAttack_fleeches);
    }
};

struct Path_ParamiteWebLine final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_ParamiteWebLine, "ParamiteWebLine", TlvTypes::ParamiteWebLine_99)
    {
        ADD("Scale", mTlv.field_10_scale);
    }
};

struct Path_MeatSack final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_MeatSack, "MeatSack", TlvTypes::MeatSack_40)
    {
        ADD("Meat Fall Direction", mTlv.field_10_meat_fall_direction);
        ADD("X Velocity", mTlv.field_12_xVel);
        ADD("Y Velocity", mTlv.field_14_yVel);
        ADD("Scale", mTlv.field_16_scale);
        ADD("Amount Of Meat", mTlv.field_18_amount_of_meat);
    }
};

struct Path_TorturedMudokon final : public ReliveAPI::TlvObjectBaseAE
{
    CTOR_AE(Path_TorturedMudokon, "TorturedMudokon", TlvTypes::TorturedMudokon_110)
    {
        ADD("Kill Switch ID", mTlv.field_10_kill_switch_id);
        ADD("Release Switch ID", mTlv.field_12_release_switch_id);
    }
};

} // namespace AETlvs

#undef EMPTY_CTOR_AE
#undef CTOR_AE
