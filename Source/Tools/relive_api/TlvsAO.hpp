#pragma once

#include "TlvObjectBaseAO.hpp"
#include "TlvObjectBaseMacros.hpp"

#include "../../AliveLibAO/HoistRocksEffect.hpp"
#include "../../AliveLibAO/Abe.hpp"
#include "../../AliveLibAO/Door.hpp"
#include "../../AliveLibAO/Lever.hpp"
#include "../../AliveLibAO/DoorLight.hpp"
#include "../../AliveLibAO/ElectricWall.hpp"
#include "../../AliveLibAO/Well.hpp"
#include "../../AliveLibAO/Slig.hpp"
#include "../../AliveLibAO/SecurityOrb.hpp"
#include "../../AliveLibAO/FallingItem.hpp"
#include "../../AliveLibAO/Mine.hpp"
#include "../../AliveLibAO/Dove.hpp"
#include "../../AliveLibAO/UXB.hpp"
#include "../../AliveLibAO/HintFly.hpp"
#include "../../AliveLibAO/Bat.hpp"
#include "../../AliveLibAO/ShadowZone.hpp"
#include "../../AliveLibAO/BellHammer.hpp"
#include "../../AliveLibAO/TimerTrigger.hpp"
#include "../../AliveLibAO/PullRingRope.hpp"
#include "../../AliveLibAO/MusicTrigger.hpp"
#include "../../AliveLibAO/Elum.hpp"
#include "../../AliveLibAO/LiftPoint.hpp"
#include "../../AliveLibAO/MovingBomb.hpp"
#include "../../AliveLibAO/Mudokon.hpp"
#include "../../AliveLibAO/MeatSaw.hpp"
#include "../../AliveLibAO/LCDScreen.hpp"
#include "../../AliveLibAO/InvisibleSwitch.hpp"
#include "../../AliveLibAO/TrapDoor.hpp"
#include "../../AliveLibAO/BirdPortal.hpp"
#include "../../AliveLibAO/BoomMachine.hpp"
#include "../../AliveLibAO/Slog.hpp"
#include "../../AliveLibAO/ChimeLock.hpp"
#include "../../AliveLibAO/FlintLockFire.hpp"
#include "../../AliveLibAO/LiftMover.hpp"
#include "../../AliveLibAO/Scrab.hpp"
#include "../../AliveLibAO/SlogSpawner.hpp"
#include "../../AliveLibAO/Rock.hpp"
#include "../../AliveLibAO/RockSack.hpp"
#include "../../AliveLibAO/ZzzSpawner.hpp"
#include "../../AliveLibAO/SecurityClaw.hpp"
#include "../../AliveLibAO/SecurityDoor.hpp"
#include "../../AliveLibAO/TimedMine.hpp"
#include "../../AliveLibAO/MotionDetector.hpp"
#include "../../AliveLibAO/BackgroundAnimation.hpp"
#include "../../AliveLibAO/LCDStatusBoard.hpp"
#include "../../AliveLibAO/HoneySack.hpp"
#include "../../AliveLibAO/SlingMudokon.hpp"
#include "../../AliveLibAO/BeeSwarmHole.hpp"
#include "../../AliveLibAO/Meat.hpp"
#include "../../AliveLibAO/RollingBall.hpp"
#include "../../AliveLibAO/RollingBallStopper.hpp"
#include "../../AliveLibAO/ZBall.hpp"
#include "../../AliveLibAO/FootSwitch.hpp"
#include "../../AliveLibAO/Paramite.hpp"
#include "../../AliveLibAO/Map.hpp"
#include "../../AliveLibAO/GasCountDown.hpp"
#include "../../AliveLibAO/GasEmitter.hpp"
#include "../../AliveLibAO/BackgroundGlukkon.hpp"
#include "../../AliveLibAO/CreditsController.hpp"
#include "../../AliveLibAO/DoorFlame.hpp"

#define CTOR_AO(className, objectTypeName, tlvEnumType)\
    className() : TlvObjectBaseAO(sizeof(AO::className), tlvEnumType, objectTypeName, &mTlv)\
    {\
    }\
    \
    className(ReliveAPI::TypesCollectionBase& globalTypes, const AO::Path_TLV* pTlvSrc = nullptr) \
        : TlvObjectBaseAO(sizeof(AO::className), globalTypes, tlvEnumType, objectTypeName, &mTlv)\
    {\
        if (pTlvSrc)\
        {\
            mTlv = *static_cast<const AO::className*>(pTlvSrc);\
            ConvertXYPos(); \
        }\
        else\
        {\
            mPSelfTlv->mLength = static_cast<s16>(mSizeOfT);\
            mPSelfTlv->mTlvType32.mType = mType;\
        }\
        AddProperties(globalTypes);\
    }\
    AO::className mTlv = {};\
    void AddProperties(ReliveAPI::TypesCollectionBase& globalTypes)

#define EMPTY_CTOR_AO() (void) globalTypes

namespace AO {
struct Path_HoneyDripTarget final : public Path_TLV
{
    // No fields
};

struct Path_Honey final : public Path_TLV
{
    s16 switch_id;
    s16 state;
    Scale_int scale;
};

struct Path_Bees final : public Path_TLV
{
    s16 switch_id;
    s16 swarm_size;
    s16 chase_time;
    s16 speed;
    s16 disable_resources;
    s16 num_bees;
};

struct Path_ScrabNoFall final : public Path_TLV
{
    // No fields
};

struct Path_ScrabLeftBound final : public Path_TLV
{
    // No fields
};

struct Path_ScrabRightBound final : public Path_TLV
{
    // No fields
};

struct Path_ZSligCover final : public Path_TLV
{
    // No fields
};

struct Path_AbeStart final : public Path_TLV
{
    Scale_int scale;
};

struct Path_MudokonPathTrans final : public Path_TLV
{
    LevelIds level;
    s16 path;
    s32 camera;
};

struct Path_Pulley final : public Path_TLV
{
    Scale_int scale;
};

struct Path_Preloader final : public Path_TLV
{
    s32 unload_cams_ASAP;
};

struct Path_SligSpawner final : public Path_TLV
{
    Scale_short field_18_scale;
    Path_Slig::StartState field_1A_start_state;
    s16 field_1C_pause_time;
    s16 field_1E_pause_left_min;
    s16 field_20_pause_left_max;
    s16 field_22_pause_right_min;
    s16 field_24_pause_right_max;
    Path_Slig::ShootPossessedSligs field_26_shoot_possessed_sligs;
    s16 field_28_shoot_on_sight_delay;
    s16 field_2A_num_times_to_shoot;
    s16 field_2C_unknown; // TODO: Part of above field, check me?
    s16 field_2E_code1;
    s16 field_30_code2;
    Choice_short field_32_chase_abe;
    XDirection_short field_34_start_direction;
    s16 field_36_panic_timeout;
    s16 field_38_num_panic_sounds;
    s16 field_3A_panic_sound_timeout;
    s16 field_3C_stop_chase_delay;
    s16 field_3E_time_to_wait_before_chase;
    s16 field_40_slig_bound_id;
    s16 field_42_listen_time;
    s16 field_44_percent_say_what;
    s16 field_46_percent_beat_mud;
    s16 field_48_talk_to_abe;
    s16 field_4A_dont_shoot;
    s16 field_4C_z_shoot_delay;
    Choice_short field_4E_stay_awake;
    s16 field_50_disable_resources;
    s16 field_52_noise_wake_up_distance;
    s32 field_54_slig_spawner_switch_id;
};

struct Path_ContinueZone final : public Path_TLV
{
    s32 field_10_zone_number;
};

struct Path_StartController final : public Path_TLV
{
    // No fields
};

struct Path_InvisibleZone final : public Path_TLV
{
    // No fields
};

struct Path_KillUnsavedMuds final : public Path_TLV
{
    // No fields
};

struct Path_MenuController final : public Path_TLV
{
    // No fields
};

struct Path_DeathDrop final : public Path_TLV
{
    s16 animation;
    s16 sound;
    s16 id;
    s16 action;
    s32 set_value;
};

struct Path_ElumStart final : public Path_TLV
{
    // No fields
};

struct Path_ElumWall final : public Path_TLV
{
    // No fields
};

struct Path_HandStone final : public Path_TLV
{
    Path_Handstone_data mData;
};

struct Path_BellsongStone final : public Path_TLV
{
    Path_BellsongStone_data mData;
};

struct Path_MovieStone final : public Path_TLV
{
    Path_Moviestone_data mData;
};
} // namespace AO

namespace AOTlvs {

struct Path_MenuController final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MenuController, "MenuController", AO::TlvTypes::MenuController_90)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_KillUnsavedMuds final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_KillUnsavedMuds, "KillUnsavedMuds", AO::TlvTypes::KillUnsavedMuds_113)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_DoorFlame final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_DoorFlame::Scale>("Enum_DoorFlame_Scale",
            {
                {AO::Path_DoorFlame::Scale::eFull_0, "Full"},
                {AO::Path_DoorFlame::Scale::eHalf_1, "Half"},
                {AO::Path_DoorFlame::Scale::eHalf_2, "Half (ignore)"},
            });

        types.AddEnum<AO::Path_DoorFlame::Colour>("Enum_DoorFlame_Colour",
            {
                {AO::Path_DoorFlame::Colour::unknown_0, "Unknown"},
                {AO::Path_DoorFlame::Colour::red_1, "Red"},
                {AO::Path_DoorFlame::Colour::green_2, "Green"},
                {AO::Path_DoorFlame::Colour::blue_3, "Blue"}
            });
    }

    CTOR_AO(Path_DoorFlame, "DoorFlame", AO::TlvTypes::DoorFlame_84)
    {
        ADD("Switch ID", mTlv.field_18_switch_id);
        ADD("Scale", mTlv.field_1A_scale);
        ADD("Colour", mTlv.field_1C_colour);
    }
};

struct Path_ResetPath final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ResetPath, "ResetPath", AO::TlvTypes::ResetPath_115)
    {
        ADD("clear_ids", mTlv.field_18_clearIds);
        ADD("from", mTlv.field_1A_from);
        ADD("to", mTlv.field_1C_to);
        ADD("exclude", mTlv.field_1E_exclude);
        ADD("clear_objects", mTlv.field_20_clearObjects);
        ADD("path", mTlv.field_22_path);
    }
};

struct Path_CreditsController final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_CreditsController, "CreditsController", AO::TlvTypes::CreditsController_101)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_BackgroundGlukkon final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BackgroundGlukkon, "BackgroundGlukkon", AO::TlvTypes::BackgroundGlukkon_112)
    {
        ADD("Scale Percent", mTlv.field_18_scale_percent);
        ADD("pal_id", mTlv.field_1A_pal_id);
        ADD("target_id", mTlv.field_1C_target_id);
        ADD("voice_adjust", mTlv.field_1E_voice_adjust);

        ADD_RESOURCE(AnimId::Background_Glukkon_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Background_Glukkon_KillHim1, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Background_Glukkon_KillHim2, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Background_Glukkon_Laugh, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_GasEmitter final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_GasEmitter, "GasEmitter", AO::TlvTypes::GasEmitter_110)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_GasCountDown final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_GasCountDown, "GasCountDown", AO::TlvTypes::GasCountDown_108)
    {
        ADD("Start Switch ID", mTlv.field_18_start_switch_id);
    }
};

struct Path_MovieStone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovieStone, "MovieStone", AO::TlvTypes::MovieStone_51)
    {
        ADD("FMV ID", mTlv.mData.fmvId);
        ADD("Scale", mTlv.mData.scale);
    }
};

struct Path_BellsongStone final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::BellsongTypes>("Enum_BellsongTypes",
        {
            {AO::BellsongTypes::eWhistle, "Whistle"},
            {AO::BellsongTypes::eChimes, "Chimes"},
        });
    }

    CTOR_AO(Path_BellsongStone, "BellSongStone", AO::TlvTypes::BellSongStone_54)
    {
        ADD("Scale", mTlv.mData.scale);
        ADD("Type", mTlv.mData.type);
        ADD("Code 1", mTlv.mData.code1);
        ADD("Code 2", mTlv.mData.code2);
        ADD("Switch ID", mTlv.mData.switch_id);
    }
};

struct Path_HandStone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HandStone, "HandStone", AO::TlvTypes::HandStone_100)
    {
        ADD("Scale", mTlv.mData.scale);

        ADD("Camera 1 Level", mTlv.mData.camera1.level);
        ADD("Camera 1 Path", mTlv.mData.camera1.path);
        ADD("Camera 1 Camera", mTlv.mData.camera1.camera);

        ADD("Camera 2 Level", mTlv.mData.camera2.level);
        ADD("Camera 2 Path", mTlv.mData.camera2.path);
        ADD("Camera 2 Camera", mTlv.mData.camera2.camera);

        ADD("Camera 3 Level", mTlv.mData.camera3.level);
        ADD("Camera 3 Path", mTlv.mData.camera3.path);
        ADD("Camera 3 Camera", mTlv.mData.camera3.camera);
    }
};

struct Path_Door final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::DoorStates>("Enum_DoorStates",
        {
            {AO::DoorStates::eOpen_0, "Open"},
            {AO::DoorStates::eClosed_1, "Closed"},
            {AO::DoorStates::eHubDoorClosed_2, "Hub Door Closed"},
            {AO::DoorStates::eClosing_3, "Closing (Internal Only)"},
        });
    }

    CTOR_AO(Path_Door, "Door", AO::TlvTypes::Door_6)
    {
        ADD("Level", mTlv.field_18_level);
        ADD("Path", mTlv.field_1A_path);
        ADD("Camera", mTlv.field_1C_camera);
        ADD("Scale", mTlv.field_1E_scale);
        ADD("Door Number", mTlv.field_20_door_number);
        ADD("Switch ID", mTlv.field_22_switch_id);
        ADD("Target Door Number", mTlv.field_24_target_door_number);
        ADD("Start State", mTlv.field_26_start_state);
        ADD("Door Closed", mTlv.field_28_door_closed);
        ADD("Hub 1 ID", mTlv.field_2A_hub1);
        ADD("Hub 2 ID", mTlv.field_2A_hub2);
        ADD("Hub 3 ID", mTlv.field_2A_hub3);
        ADD("Hub 4 ID", mTlv.field_2A_hub4);
        ADD("Hub 5 ID", mTlv.field_2A_hub5);
        ADD("Hub 6 ID", mTlv.field_2A_hub6);
        ADD("Hub 7 ID", mTlv.field_2A_hub7);
        ADD("Hub 8 ID", mTlv.field_2A_hub8);
        ADD("Wipe Effect", mTlv.field_3A_wipe_effect); // TODO: enum
        ADD("Movie Number", mTlv.field_3C_movie_number);
        ADD("X Offset", mTlv.field_3E_x_offset);
        ADD("Y Offset", mTlv.field_40_y_offset);
        ADD("Wipe X Org (Unused?)", mTlv.field_42_wipe_x_org);
        ADD("Wipe Y Org (Unused?)", mTlv.field_44_wipe_y_org);
        ADD("Abe Direction On Exit", mTlv.field_46_abe_direction);
    }
};

struct Path_ContinuePoint final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_ContinuePoint::spawnDirection>("Enum_ContinuePointSpawnDirection",
            {
                {AO::Path_ContinuePoint::spawnDirection::eRight_0, "Right"},
                {AO::Path_ContinuePoint::spawnDirection::eLeft_1, "Left"},
            });
    }

    CTOR_AO(Path_ContinuePoint, "ContinuePoint", AO::TlvTypes::ContinuePoint_0)
    {
        ADD("Zone Number", mTlv.field_18_zone_number);
        ADD("Clear From Id", mTlv.field_1A_clear_from_id);
        ADD("Clear To Id", mTlv.field_1C_clear_to_id);
        ADD("Elum Restarts", mTlv.field_1E_elum_restarts);
        ADD("Abe Spawn Direction", mTlv.field_20_abe_direction);
    }
};

struct Path_Hoist final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Hoist::Type>("Enum_HoistType",
        {
            {AO::Path_Hoist::Type::eNextEdge, "Next Edge"},
            {AO::Path_Hoist::Type::eNextFloor, "Next Floor"},
            {AO::Path_Hoist::Type::eOffScreen, "Off Screen"},
        });

        types.AddEnum<AO::Path_Hoist::GrabDirection>("Enum_HoistGrabDirection",
        {
            {AO::Path_Hoist::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
            {AO::Path_Hoist::GrabDirection::eFacingLeft, "Facing Left"},
            {AO::Path_Hoist::GrabDirection::eFacingRight, "Facing Right"},
        });
    }

    CTOR_AO(Path_Hoist, "Hoist", AO::TlvTypes::Hoist_3)
    {
        ADD("Hoist Type", mTlv.field_18_hoist_type);
        ADD("Grab Direction", mTlv.field_1A_grab_direction);
        ADD("Switch ID (Unused?)", mTlv.field_1C_switch_id);
    }
};

struct Path_Change final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Change, "PathTransition", AO::TlvTypes::PathTransition_1)
    {
        ADD("Level", mTlv.field_18_level);
        ADD("Path", mTlv.field_1A_path);
        ADD("Camera", mTlv.field_1C_camera);
        ADD("Movie", mTlv.field_1E_movie); // TODO: Enum
        ADD("Wipe", mTlv.field_20_wipe);   // TODO: Enum
        ADD("Next Path Scale", mTlv.field_22_next_path_scale);
    }
};

struct Path_Lever final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::LeverSoundType>("Enum_LeverSoundType",
        {
            {AO::LeverSoundType::eNone, "None"},
            {AO::LeverSoundType::eWell_1, "Well"},
            {AO::LeverSoundType::eSwitchBellHammer_2, "Switch Bell Hammer"},
            {AO::LeverSoundType::eDoor_3, "Door"},
            {AO::LeverSoundType::eElectricWall_4, "Electric Wall"},
            {AO::LeverSoundType::eSecurityOrb_5, "Security Orb"},
        });

        types.AddEnum<AO::LeverSoundDirection>("Enum_LeverSoundDirection",
        {
            {AO::LeverSoundDirection::eLeftAndRight_0, "Left And Right"},
            {AO::LeverSoundDirection::eLeft_1, "Left"},
            {AO::LeverSoundDirection::eRight_2, "Right"},
        });
    }

    CTOR_AO(Path_Lever, "Lever", AO::TlvTypes::Lever_26)
    {
        ADD("Switch ID", mTlv.field_18_switch_id);
        ADD("Action", mTlv.field_1A_action);
        ADD("Scale", mTlv.field_1C_scale);
        ADD("On Sound", mTlv.field_1E_on_sound);
        ADD("Off Sound", mTlv.field_20_off_sound);
        ADD("Sound Direction", mTlv.field_22_sound_direction);
    }
};

struct Path_LightEffect final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_LightEffect::Type>("Enum_LightType",
        {
            {AO::Path_LightEffect::Type::Star_0, "Star"},
            {AO::Path_LightEffect::Type::GoldGlow_1, "Gold Glow"},
            {AO::Path_LightEffect::Type::GreenGlow_2, "Green Glow"},
            {AO::Path_LightEffect::Type::FlintGlow_3, "Flint Glow"},
            {AO::Path_LightEffect::Type::Switchable_RedGreenDoorLights_4, "RedGreen Door Light"},
            {AO::Path_LightEffect::Type::Switchable_RedGreenHubLight_5, "RedGreen Hub Light"},
        });
    }

    CTOR_AO(Path_LightEffect, "LightEffect", AO::TlvTypes::LightEffect_106)
    {
        ADD("Type", mTlv.field_18_type);
        ADD("Size", mTlv.field_1A_size);
        ADD("Switch ID", mTlv.field_1C_switch_id);
        ADD("Direction", mTlv.field_1E_direction);
    }
};

struct Path_ElectricWall final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::ElectricWallStartState>("Enum_ElectricWallStartState",
        {
            {AO::ElectricWallStartState::eOff_0, "Off"},
            {AO::ElectricWallStartState::eOn_1, "On"},
        });
    }

    CTOR_AO(Path_ElectricWall, "ElectricWall", AO::TlvTypes::ElectricWall_67)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Switch ID", mTlv.field_1A_switch_id);
        ADD("Start State", mTlv.field_1C_start_state);

        ADD_RESOURCE(AnimId::Electric_Wall, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_ContinueZone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ContinueZone, "ContinueZone", AO::TlvTypes::ContinueZone_2)
    {
        ADD("Zone Number", mTlv.field_10_zone_number);
    }
};

struct Path_StartController final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_StartController, "StartController", AO::TlvTypes::StartController_28)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_Edge final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Edge::GrabDirection>("Enum_EdgeGrabDirection",
        {
            {AO::Path_Edge::GrabDirection::eFacingLeft, "Facing Left"},
            {AO::Path_Edge::GrabDirection::eFacingRight, "Facing Right"},
            {AO::Path_Edge::GrabDirection::eFacingAnyDirection, "Facing Any Direction"},
        });
    }

    CTOR_AO(Path_Edge, "Edge", AO::TlvTypes::Edge_4)
    {
        ADD("Grab Direction", mTlv.field_18_grab_direction);
        ADD("Can Grab", mTlv.field_1A_can_grab);
    }
};

struct Path_WellLocal final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_WellLocal, "WellLocal", AO::TlvTypes::WellLocal_11)
    {
        // Well base
        ADD("Scale", mTlv.field_18_scale);
        ADD("Switch ID", mTlv.field_1A_switch_id);
        ADD_HIDDEN("Other Well ID", mTlv.field_1C_other_well_id); // hidden because this is only used by the well express
        ADD("Animation ID", mTlv.field_1E_anim_id);
        ADD("Exit X", mTlv.field_20_exit_x);
        ADD("Exit Y", mTlv.field_22_exit_y);
        ADD("Disabled XPos", mTlv.field_24_off_level_or_dx.dx);
        ADD("Disabled YPos", mTlv.field_26_off_path_or_dy);

        // Well local
        ADD("Enabled XPos", mTlv.field_28_on_dx);
        ADD("Enabled YPos", mTlv.field_2A_on_dy);
        ADD("Emit Leaves", mTlv.field_2C_bEmit_leaves);
        ADD("Leaf X", mTlv.field_2E_leaf_x);
        ADD("Leaf Y", mTlv.field_30_leaf_y);
    }
};

struct Path_WellExpress final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_WellExpress, "WellExpress", AO::TlvTypes::WellExpress_45)
    {
        // Well base
        ADD("Scale", mTlv.field_18_scale);
        ADD("Switch ID", mTlv.field_1A_switch_id);
        ADD("Other Well ID", mTlv.field_1C_other_well_id);
        ADD("Animation ID", mTlv.field_1E_anim_id);
        ADD("Exit X", mTlv.field_20_exit_x);
        ADD("Exit Y", mTlv.field_22_exit_y);
        ADD("Off Level", mTlv.field_24_off_level_or_dx.level);
        ADD("Off Path", mTlv.field_26_off_path_or_dy);

        // Well express
        ADD("Off Camera", mTlv.field_28_off_camera);
        ADD("Off Other Well ID", mTlv.field_2A_off_well_id);
        ADD("On Level", mTlv.field_2C_on_level);
        ADD("On Path", mTlv.field_2E_on_path);
        ADD("On Camera", mTlv.field_30_on_camera);
        ADD("On Other Well ID", mTlv.field_32_on_well_id);
        ADD("Emit Leaves", mTlv.field_34_emit_leaves);
        ADD("Leaf X", mTlv.field_36_leaf_x);
        ADD("Leaf Y", mTlv.field_38_leaf_y);
        ADD("Movie ID", mTlv.field_3A_movie_id);
    }
};

struct Path_InvisibleZone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_InvisibleZone, "InvisibleZone", AO::TlvTypes::InvisibleZone_58)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_EnemyStopper final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_EnemyStopper::StopDirection>("Enum_StopDirection",
        {
            {AO::Path_EnemyStopper::StopDirection::Left_0, "Left"},
            {AO::Path_EnemyStopper::StopDirection::Right_1, "Right"},
            {AO::Path_EnemyStopper::StopDirection::Both_2, "Both"},
        });
    }

    CTOR_AO(Path_EnemyStopper, "EnemyStopper", AO::TlvTypes::EnemyStopper_79)
    {
        ADD("Stop Direction", mTlv.field_18_direction);
        ADD("Switch ID", mTlv.field_1A_switch_id);
    }
};

struct Path_Slig final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Slig, "Slig", AO::TlvTypes::Slig_24)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Start State", mTlv.field_1A_start_state);
        ADD("Pause Time", mTlv.field_1C_pause_time);
        ADD("Pause Left Min", mTlv.field_1E_pause_left_min);
        ADD("Pause Left Max", mTlv.field_20_pause_left_max);
        ADD("Pause Right Min", mTlv.field_22_pause_right_min);
        ADD("Pause Right Max", mTlv.field_24_pause_right_max);
        ADD("Shoot Possessed Sligs", mTlv.field_26_shoot_possessed_sligs);
        ADD("Shoot On Sight Delay", mTlv.field_28_shoot_on_sight_delay);
        ADD("Bullet Shoot Count", mTlv.field_2A_num_times_to_shoot);
        ADD("unknown", mTlv.field_2C_unknown);
        ADD("Code 1", mTlv.field_2E_code1);
        ADD("Code 2", mTlv.field_30_code2);
        ADD("Chase Abe When Spotted", mTlv.field_32_chase_abe);
        ADD("Start Direction", mTlv.field_34_start_direction);
        ADD("Panic Timeout", mTlv.field_36_panic_timeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.field_38_num_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.field_3A_panic_sound_timeout);
        ADD("Stop Chase Delay", mTlv.field_3C_stop_chase_delay);
        ADD("Time To Wait Before Chase", mTlv.field_3E_time_to_wait_before_chase);
        ADD("Slig Bound/Persist ID", mTlv.field_40_slig_bound_id);
        ADD("Alerted Listen Time", mTlv.field_42_listen_time);
        ADD("Percent Say What", mTlv.field_44_percent_say_what);
        ADD("Percent Beat Mudokon", mTlv.field_46_percent_beat_mud);
        ADD_HIDDEN("Talk To Abe (Unused?)", mTlv.field_48_talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.field_4A_dont_shoot);
        ADD("Z Shoot Delay", mTlv.field_4C_z_shoot_delay);
        ADD("Stay Awake", mTlv.field_4E_stay_awake);
        ADD("Disabled Resources", mTlv.field_50_disable_resources.Raw().all);
        ADD("Noise Wake Up Distance (Grids)", mTlv.field_52_noise_wake_up_distance);
        ADD("Slig Spawner Switch ID", mTlv.field_54_slig_spawner_switch_id);
    }
};

struct Path_DeathDrop final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_DeathDrop, "DeathDrop", AO::TlvTypes::DeathDrop_5)
    {
        ADD_HIDDEN("animation", mTlv.animation);
        ADD_HIDDEN("sound", mTlv.sound);
        ADD_HIDDEN("id", mTlv.id);
        ADD_HIDDEN("action", mTlv.action);
        ADD_HIDDEN("set_value", mTlv.set_value);
    }
};

struct Path_SligLeftBound final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligLeftBound, "SligLeftBound", AO::TlvTypes::eSligBoundLeft_57)
    {
        ADD("Slig ID", mTlv.field_18_slig_id);
        ADD("Disabled Resources", mTlv.field_1A_disabled_resources.Raw().all);
    }
};

struct Path_SligRightBound final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligRightBound, "SligRightBound", AO::TlvTypes::eSligBoundRight_76)
    {
        ADD("Slig ID", mTlv.field_18_slig_id);
        ADD("Disabled Resources", mTlv.field_1A_disabled_resources.Raw().all);
    }
};

struct Path_SligPersist final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligPersist, "SligPersist", AO::TlvTypes::eSligPersist_77)
    {
        ADD("Slig ID", mTlv.field_18_slig_id);
        ADD("Disabled Resources", mTlv.field_1A_disabled_resources.Raw().all);
    }
};

struct Path_SecurityOrb final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityOrb, "SecurityOrb", AO::TlvTypes::SecurityOrb_29)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Disabled Resources", mTlv.field_1A_disable_resources);

        ADD_RESOURCE(AnimId::Abe_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Abe_Arm_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Abe_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Arm_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Body_Gib, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_FallingItem final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_FallingItem, "FallingItem", AO::TlvTypes::FallingItem_15)
    {
        ADD("Switch ID", mTlv.field_18_switch_id);
        ADD("Scale", mTlv.field_1A_scale);
        ADD("Fall Interval", mTlv.field_1C_fall_interval);
        ADD("Max Falling Items", mTlv.field_1E_max_falling_items);
        ADD("Reset Switch ID After Use", mTlv.field_20_reset_switch_id_after_use);
    }
};

struct Path_Mine final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Mine, "Mine", AO::TlvTypes::Mine_46)
    {
        ADD_HIDDEN("Num Patterns (Unused?)", mTlv.field_18_num_patterns);
        ADD_HIDDEN("Pattern (Unused?)", mTlv.field_1A_pattern);
        ADD("Scale", mTlv.field_1C_scale);
        ADD("Disabled Resources", mTlv.field_1E_disabled_resources);
        ADD("Persist Offscreen", mTlv.field_20_persists_offscreen);
    }
};

struct Path_Dove final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Dove, "Dove", AO::TlvTypes::Dove_12)
    {
        ADD("Dove Count", mTlv.mDoveCount);
        ADD("Pixel Perfect", mTlv.mPixelPerfect);
        ADD("Scale", mTlv.mScale);
    }
};

struct Path_UXB final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::UXBStartState>("UXBStartState",
        {
            {AO::UXBStartState::eOn_0, "On"},
            {AO::UXBStartState::eOff_1, "Off"},
        });
    }

    CTOR_AO(Path_UXB, "UXB", AO::TlvTypes::UXB_47)
    {
        ADD("Pattern Length (Max 4)", mTlv.field_18_pattern_length);
        ADD("Pattern", mTlv.field_1A_pattern);
        ADD("Scale", mTlv.field_1C_scale);
        ADD("Start State", mTlv.field_1E_state);
        ADD("Disabled Resources", mTlv.field_20_disabled_resources);
    }
};

struct Path_HintFly final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HintFly, "HintFly", AO::TlvTypes::HintFly_92)
    {
        ADD("Message ID", mTlv.field_18_message_id);

        ADD_RESOURCE(AnimId::HintFly, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_Bat final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Bat, "Bat", AO::TlvTypes::Bat_49)
    {
        ADD("Ticks Before Moving", mTlv.mTicksBeforeMoving);
        ADD("Speed", mTlv.mSpeed);
        ADD("Scale", mTlv.mScale);
        ADD("Attack Duration", mTlv.mAttackDuration);

        //ADD_RESOURCE(AnimId::Bat, ReliveAPI::AddResourceTo::CameraBlock);
        //ADD_RESOURCE(AnimId::Bat_Flying, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_ShadowZone final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::ShadowZoneScale>("Enum_ShadowZoneScale",
        {
            {AO::ShadowZoneScale::eBoth_0, "both"},
            {AO::ShadowZoneScale::eHalf_1, "half"},
            {AO::ShadowZoneScale::eFull_2, "full"},
        });
    }

    CTOR_AO(Path_ShadowZone, "ShadowZone", AO::TlvTypes::ShadowZone_7)
    {
        ADD("Centre W", mTlv.field_18_centre_w);
        ADD("Centre H", mTlv.field_1A_centre_h);
        ADD_LINKED("R", mTlv.field_1C_r, "RGB_R_Half_FP");
        ADD_LINKED("G", mTlv.field_1E_g, "RGB_G_Half_FP");
        ADD_LINKED("B", mTlv.field_20_b, "RGB_B_Half_FP");
        ADD("ID (Unused?)", mTlv.field_22_id);
        ADD("Scale", mTlv.field_24_scale);
    }
};

struct Path_BellHammer final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BellHammer, "BellHammer", AO::TlvTypes::BellHammer_27)
    {
        ADD("Switch ID", mTlv.field_18_switch_id);
        ADD("Action", mTlv.field_1A_action);
        ADD("Scale", mTlv.field_1C_scale);
        ADD("Direction", mTlv.field_1E_direction);

        ADD_RESOURCE(AnimId::BellHammer_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BellHammer_Smashing, ReliveAPI::AddResourceTo::CameraBlock);

        ADD_RESOURCE(AnimId::Elum_Land, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Idle, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_TimerTrigger final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_TimerTrigger, "TimerTrigger", AO::TlvTypes::TimerTrigger_94)
    {
        ADD("Source Switch ID", mTlv.field_18_source_switch_id);
        ADD("Trigger Interval", mTlv.field_1A_trigger_interval);
        ADD("ID 1", mTlv.field_1C_id1);
        ADD("ID 2", mTlv.field_1C_id2);
        ADD("ID 3", mTlv.field_1C_id3);
        ADD("ID 4", mTlv.field_1C_id4);
    }
};

struct Path_PullRingRope final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        
        types.AddEnum<AO::PullRingSwitchSound>("Enum_PullRingSwitchSound",
        {
            {AO::PullRingSwitchSound::eNone_0, "None"},
            {AO::PullRingSwitchSound::eWellExit_1, "WellExit"},
            {AO::PullRingSwitchSound::RingBellHammer_2, "RingBellHammer"},
            {AO::PullRingSwitchSound::eDoorEffect_3, "DoorEffect"},
        });

        types.AddEnum<AO::PullRingSoundDirection>("Enum_PullRingSoundDirection",
        {
            {AO::PullRingSoundDirection::eBoth_0, "Both"},
            {AO::PullRingSoundDirection::eLeft_1, "Left"},
            {AO::PullRingSoundDirection::eRight_2, "Right"},
        });
    }

    CTOR_AO(Path_PullRingRope, "PullRingRope", AO::TlvTypes::PullRingRope_18)
    {
        ADD("Switch ID", mTlv.field_18_switch_id);
        ADD("Action", mTlv.field_1A_action);
        ADD("Rope Length", mTlv.field_1C_rope_length);
        ADD("Scale", mTlv.field_1E_scale);
        ADD("On Sound", mTlv.field_20_on_sound);
        ADD("Off Sound", mTlv.field_22_off_sound);
        ADD("Sound Direction", mTlv.field_24_sound_direction);
    }
};

struct Path_MusicTrigger final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::TriggeredBy>("Enum_MusicTriggeredBy",
        {
            {AO::TriggeredBy::eTimer_0, "Timer"},
            {AO::TriggeredBy::eTouching_1, "Touching"},
            {AO::TriggeredBy::eSwitchID_2, "Switch ID"},
            {AO::TriggeredBy::eUnknown_3, "Unknown"},
        });

        types.AddEnum<AO::MusicTriggerMusicType>("Enum_MusicTriggeredMusicType",
        {
            {AO::MusicTriggerMusicType::eDrumAmbience_0, "Drum Ambience"},
            {AO::MusicTriggerMusicType::eDeathDrumShort_1, "Death Drum Short"},
            {AO::MusicTriggerMusicType::eSecretAreaLong_2, "Secret Area Long"},
            {AO::MusicTriggerMusicType::eSoftChase_3, "Soft Chase"},
            {AO::MusicTriggerMusicType::eIntenseChase_4, "Intense Chase"},
            {AO::MusicTriggerMusicType::eChime_5, "Chime"},
            {AO::MusicTriggerMusicType::eSecretAreaShort_6, "Secret Area Short"},
        });
    }

    CTOR_AO(Path_MusicTrigger, "MusicTrigger", AO::TlvTypes::MusicTrigger_105)
    {
        ADD("Music Type", mTlv.field_18_music_type);
        ADD("Triggered By", mTlv.field_1A_triggered_by);
        ADD("Switch ID", mTlv.field_1C_switch_id);
        ADD("Music Delay", mTlv.field_1E_music_delay);
    }
};

struct Path_ElumPathTrans final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ElumPathTrans, "ElumPathTrans", AO::TlvTypes::ElumPathTrans_99)
    {
        ADD("Level", mTlv.field_18_level);
        ADD("Path", mTlv.field_1A_path);
        ADD("Camera", mTlv.field_1C_camera);
    }
};

struct Path_ElumStart final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ElumStart, "ElumStart", AO::TlvTypes::ElumStart_38)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_ElumWall final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ElumWall, "ElumWall", AO::TlvTypes::ElumWall_40)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_LiftPoint final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::LiftPointStopType>("Enum_LiftPointStopType",
        {
            {AO::LiftPointStopType::eTopFloor_0, "Top Floor"},
            {AO::LiftPointStopType::eBottomFloor_1, "Bottom Floor"},
            {AO::LiftPointStopType::eMiddleFloor_2, "Middle Floor"},
            {AO::LiftPointStopType::eMiddleLockFloor_3, "Middle Lock Floor"},
            {AO::LiftPointStopType::eStartPointOnly_4, "Start Point Only"},
        });
    }

    CTOR_AO(Path_LiftPoint, "LiftPoint", AO::TlvTypes::LiftPoint_8)
    {
        ADD("Lift Point ID", mTlv.field_18_lift_point_id);
        ADD("Start Point", mTlv.field_1A_bstart_point);
        ADD("Lift Type (Unused?)", mTlv.field_1C_lift_type);
        ADD("Lift Point Stop Type", mTlv.field_1E_lift_point_stop_type);
        ADD("Scale", mTlv.field_20_scale);
        ADD("Ignore Lift Mover", mTlv.field_22_bIgnore_lift_mover);
    }
};

struct Path_MovingBomb final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovingBomb, "MovingBomb", AO::TlvTypes::MovingBomb_86)
    {
        ADD("Speed", mTlv.field_18_speed);
        ADD("Switch ID", mTlv.field_1A_switch_id);
        ADD("Triggered By Alarm", mTlv.field_1C_bTriggered_by_alarm);
        ADD("Scale", mTlv.field_1E_scale);
        ADD("Max Rise", mTlv.field_20_max_rise);
        ADD("Disabled Resources", mTlv.field_22_disabled_resources);
        ADD("Start Speed", mTlv.field_24_start_speed);
        ADD("Persist Offscreen", mTlv.field_26_persist_offscreen);
    }
};

struct Path_MovingBombStopper final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovingBombStopper, "MovingBombStopper", AO::TlvTypes::MovingBombStopper_87)
    {
        ADD("Min Delay", mTlv.field_18_min_delay);
        ADD("Max Delay", mTlv.field_1A_max_delay);
    }
};

struct Path_RingMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_RingMudokon::MustFaceMud>("Enum_RingMudokon_MustFaceMud",
        {
            {AO::Path_RingMudokon::MustFaceMud::eYes_0, "Yes"},
            {AO::Path_RingMudokon::MustFaceMud::eNo_1, "No"},
        });
    }

    CTOR_AO(Path_RingMudokon, "RingMudokon", AO::TlvTypes::RingMudokon_50)
    {
        ADD("Start Direction", mTlv.field_18_direction);
        ADD("Abe Must Face Mudokon", mTlv.field_1A_abe_must_face_mud);
        ADD("Scale", mTlv.field_1C_scale);
        ADD("Give Password", mTlv.field_1E_give_password);
        ADD("Code 1", mTlv.field_20_code1);
        ADD("Code 2", mTlv.field_22_code2);
        ADD("Action", mTlv.field_24_action);
        ADD("Ring And Angry Mudokon Timeout", mTlv.field_26_ring_timeout);
        ADD("Give Ring Without Password", mTlv.field_28_give_ring_without_password);
    }
};

struct Path_RingCancel final : public ReliveAPI::TlvObjectBaseAO // TODO: correct size is 24 not 28
{
    CTOR_AO(Path_RingCancel, "RingCancel", AO::TlvTypes::RingCancel_109)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_MeatSaw final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_MeatSaw::Type>("Enum_MeatSawType",
        {
            {AO::Path_MeatSaw::Type::eAutomaticPersistOffscreen_0, "Automatic Persist Offscreen"},
            {AO::Path_MeatSaw::Type::eAutomatic_1, "Automatic"},
            {AO::Path_MeatSaw::Type::eSwitchId_2, "Switch ID"},
        });

        types.AddEnum<AO::Path_MeatSaw::StartState>("Enum_MeatSawStartState",
        {
            {AO::Path_MeatSaw::StartState::eOff_0, "Off"},
            {AO::Path_MeatSaw::StartState::eOn_1, "On"},
        });
    }

    CTOR_AO(Path_MeatSaw, "MeatSaw", AO::TlvTypes::MeatSaw_88)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Switch Min Time Off", mTlv.field_1A_switch_min_time_off);
        ADD("Switch Max Time Off", mTlv.field_1C_switch_max_time_off);
        ADD("Max Rise Time", mTlv.field_1E_max_rise_time);
        ADD("Switch ID", mTlv.field_20_switch_id);
        ADD("Type", mTlv.field_22_type);
        ADD("Speed", mTlv.field_24_speed);
        ADD("Start State", mTlv.field_26_start_state);
        ADD("Off Speed", mTlv.field_28_off_speed);
        ADD("Automatic Min Time Off", mTlv.field_2A_automatic_min_time_off);
        ADD("Automatic Max Time Off", mTlv.field_2C_automatic_max_time_off);
        ADD("Initial Position", mTlv.field_2E_inital_position);

        ADD_RESOURCE(AnimId::MeatSawMotor, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::MeatSaw_Idle, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::MeatSaw_Moving, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_LCDScreen final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_LCDScreen, "LCDScreen", AO::TlvTypes::LCDScreen_98)
    {
        ADD("Message 1 ID", mTlv.field_18_message_1_id);
        ADD("Random Message Min ID", mTlv.field_1A_message_rand_min);
        ADD("Random Message Max ID", mTlv.field_1C_message_rand_max);
    }
};

struct Path_InvisibleSwitch final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::InvisibleSwitchScale>("Enum_InvisibleSwitchScale",
        {
            {AO::InvisibleSwitchScale::eHalf_0, "Half"},
            {AO::InvisibleSwitchScale::eFull_1, "Full"},
            {AO::InvisibleSwitchScale::eAny_2, "Any"},
        });
    }

    CTOR_AO(Path_InvisibleSwitch, "InvisibleSwitch", AO::TlvTypes::InvisibleSwitch_81)
    {
        ADD("Switch ID", mTlv.field_18_switch_id);
        ADD("Action", mTlv.field_1A_action);
        ADD("Delay", mTlv.field_1C_delay);
        ADD("Set Off Alarm", mTlv.field_1E_set_off_alarm);
        ADD("Scale", mTlv.field_20_scale);
    }
};

struct Path_TrapDoor final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_TrapDoor::StartState>("Enum_TrapDoorStartState",
        {
            {AO::Path_TrapDoor::StartState::eOpen_0, "Open"},
            {AO::Path_TrapDoor::StartState::eClosed_1, "Closed"}
        });
    }

    CTOR_AO(Path_TrapDoor, "TrapDoor", AO::TlvTypes::TrapDoor_55)
    {
        ADD("Switch ID", mTlv.field_18_switch_id);
        ADD("Start State", mTlv.field_1A_start_state);
        ADD("Self Closing", mTlv.field_1C_self_closing);
        ADD("Scale", mTlv.field_1E_scale);
        ADD("Dest Level (Unused?)", mTlv.field_20_dest_level);
        ADD("Direction", mTlv.field_22_direction);
        ADD("X Offset", mTlv.field_24_xOff);
    }
};

struct Path_BirdPortal final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::PortalSide>("Enum_PortalSide",
        {
            {AO::PortalSide::eRight_0, "Right"},
            {AO::PortalSide::eLeft_1, "Left"},
        });

        types.AddEnum<AO::PortalType>("Enum_PortalType",
        {
            {AO::PortalType::eAbe_0, "Abe"},
            {AO::PortalType::eWorker_1, "Worker"},
            {AO::PortalType::eShrykull_2, "Shrykull"},
            {AO::PortalType::eMudTeleport_3, "Mudokon Teleport"},
        });
    }

    CTOR_AO(Path_BirdPortal, "BirdPortal", AO::TlvTypes::BirdPortal_52)
    {
        ADD("Enter Side", mTlv.field_18_side);
        ADD("Level Destination", mTlv.field_1A_dest_level);
        ADD("Path Destination", mTlv.field_1C_dest_path);
        ADD("Camera Destination", mTlv.field_1E_dest_camera);
        ADD("Scale", mTlv.field_20_scale);
        ADD("Movie ID", mTlv.field_22_movie_id);
        ADD("Portal Type", mTlv.field_24_portal_type);
        ADD("Mudokon Amount For Shrykull", mTlv.field_26_num_muds_for_shrykul);
    }
};

struct Path_BoomMachine final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_BoomMachine::NozzleSide>("Enum_BoomMachineNozzleSide",
            {
                {AO::Path_BoomMachine::NozzleSide::eRight_0, "Right"},
                {AO::Path_BoomMachine::NozzleSide::eLeft_1, "Left"},
            });
    }

    CTOR_AO(Path_BoomMachine, "BoomMachine", AO::TlvTypes::BoomMachine_97)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Nozzle Side", mTlv.field_1A_nozzle_side);
        ADD("Disabled Resources", mTlv.field_1C_disabled_resources);
        ADD("Number Of Grenades", mTlv.field_1E_number_of_grenades);

        ADD_RESOURCE(AnimId::Mudokon_GrenadeMachineUse, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_HandstoneBegin, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_HandstoneEnd, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::BoomMachine_Button_Off, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Button_On, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Nozzle_DropGrenade, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BoomMachine_Nozzle_Idle, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_Mudokon final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Mudokon::MudJobs>("Enum_MudJobs",
        {
            {AO::Path_Mudokon::MudJobs::eStandScrub_0, "Stand Scrub"},
            {AO::Path_Mudokon::MudJobs::eSitScrub_1, "Sit Scrub"},
            {AO::Path_Mudokon::MudJobs::eSitChant_2, "Sit Chant"}
        });
    }

    CTOR_AO(Path_Mudokon, "Mudokon", AO::TlvTypes::Mudokon_82)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Job", mTlv.field_1A_job);
        ADD("Start Direction", mTlv.field_1C_direction);
        ADD("Voice Pitch", mTlv.field_1E_voice_pitch);
        ADD("Rescue Switch ID", mTlv.field_20_rescue_switch_id);
        ADD("Deaf", mTlv.field_22_deaf);
        ADD("Disabled Resources", mTlv.field_24_disabled_resources);
        ADD("Persist Offscreen", mTlv.field_26_persist);
    }
};

struct Path_BirdPortalExit final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BirdPortalExit, "BirdPortalExit", AO::TlvTypes::BirdPortalExit_53)
    {
        ADD("Exit Direction", mTlv.field_18_side);
        ADD("Scale", mTlv.field_1A_scale);
    }
};

struct Path_Slog final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Slog, "Slog", AO::TlvTypes::Slog_25)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Start Direction", mTlv.field_1A_start_direction);
        ADD("Asleep", mTlv.field_1C_asleep);
        ADD("Wake Up Anger", mTlv.field_1E_wake_up_anger);
        ADD("Bark Anger", mTlv.field_20_bark_anger);
        ADD("Chase Anger", mTlv.field_22_chase_anger);
        ADD("Chase Delay", mTlv.field_24_chase_delay);
        ADD("Disabled Resources", mTlv.field_26_disabled_resources);
        ADD("Anger Switch ID", mTlv.field_28_anger_switch_id);
        
        ADD_RESOURCE(AnimId::Slog_AngryBark, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Dying, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_ChimeLock final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ChimeLock, "ChimeLock", AO::TlvTypes::ChimeLock_69)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Solve Switch ID", mTlv.field_1A_solve_switch_id);
        ADD("Code 1", mTlv.field_1C_code1);
        ADD("Code 2", mTlv.field_1E_code2);
        ADD("Password Switch ID", mTlv.field_20_password_switch_id);

        ADD_RESOURCE(AnimId::Chime_Ball, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::BigChime, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_FlintLockFire final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_FlintLockFire, "FlintLockFire", AO::TlvTypes::FlintLockFire_73)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Switch ID", mTlv.field_1A_switch_id);

        //ADD_RESOURCE(AnimId::Fire, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::FlintLock_Hammers_Activating, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::FlintLock_Hammers_Disabled, ReliveAPI::AddResourceTo::File);
        //ADD_RESOURCE(AnimId::FlintLock_Gourd, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_LiftMover final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_LiftMover::YDirection>("Enum_LiftMoverYDirection",
        {
            {AO::Path_LiftMover::YDirection::eDown_0, "Down"},
            {AO::Path_LiftMover::YDirection::eUp_1, "Up"},
        });
    }

    CTOR_AO(Path_LiftMover, "LiftMover", AO::TlvTypes::LiftMover_68)
    {
        ADD("Lift Mover Switch ID", mTlv.field_18_lift_mover_switch_id);
        ADD("Target Lift Point ID", mTlv.field_1A_target_lift_point_id);
        ADD("Move Direction", mTlv.field_1C_direction);
    }
};

struct Path_Scrab final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::ScrabPatrolType>("Enum_ScrabPatrolType",
            {
                {AO::ScrabPatrolType::eWalk_0, "Walk"},
                {AO::ScrabPatrolType::eRunOrWalk192_1, "Run Or Walk RNG 192"},
                {AO::ScrabPatrolType::eRunOrWalk128_2, "Run OR Walk RNG 128"},
                {AO::ScrabPatrolType::eRunOrWalk64_3, "Run Or Walk RNG 64"},
                {AO::ScrabPatrolType::eRun_4, "Run"},
            });
    }

    CTOR_AO(Path_Scrab, "Scrab", AO::TlvTypes::Scrab_72)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Attack Delay", mTlv.field_1A_attack_delay);
        ADD("Patrol Type", mTlv.field_1C_patrol_type);
        ADD("Left Min Delay", mTlv.field_1E_left_min_delay);
        ADD("Left Max Delay", mTlv.field_20_left_max_delay);
        ADD("Right Min Delay", mTlv.field_22_right_min_delay);
        ADD("Right Max Delay", mTlv.field_24_right_max_delay);
        ADD("Spotting Abe Delay", mTlv.field_26_spotting_abe_delay);
        ADD("Disable Resources", mTlv.field_28_disable_resources);
        ADD("Roar Randomly", mTlv.field_2A_roar_randomly);

        ADD_RESOURCE(AnimId::Scrab_AttackLunge, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SlogSpawner final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::StartDirection>("Enum_SlogSpawnerStartDirection",
            {
                {AO::StartDirection::eRight_0, "Right"},
                {AO::StartDirection::eLeft_1, "Left"},
            });
    }

    CTOR_AO(Path_SlogSpawner, "SlogSpawner", AO::TlvTypes::SlogSpawner_107)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Max Slogs", mTlv.field_1A_max_slogs);
        ADD("Max Slogs At A Time", mTlv.field_1C_max_slogs_at_a_time);
        ADD("Start Direction", mTlv.field_1E_start_direction);
        ADD("Slog Spawn Interval", mTlv.field_20_slog_spawn_interval);
        ADD("Spawner Switch ID", mTlv.field_22_spawner_switch_id);

        ADD_RESOURCE(AnimId::Slog_AngryBark, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Dying, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_RockSack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RockSack, "RockSack", AO::TlvTypes::RockSack_13)
    {
        ADD("Rock Fall Direction", mTlv.field_18_fall_direction);
        ADD("X Velocity", mTlv.field_1A_x_vel);
        ADD("Y Velocity", mTlv.field_1C_y_vel);
        ADD("Scale", mTlv.field_1E_scale);
        ADD("Rock Amount", mTlv.field_20_rock_amount);

        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::RockSack_HardHit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::RockSack_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::RockSack_SoftHit, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_ZzzSpawner final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ZzzSpawner, "ZzzSpawner", AO::TlvTypes::ZzzSpawner_111)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Switch ID", mTlv.field_1A_switch_id);
        ADD("Zzz Delay", mTlv.field_1C_Zzz_delay);
    }
};

struct Path_SecurityClaw final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityClaw, "SecurityClaw", AO::TlvTypes::SecurityClaw_61)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Alarm Switch ID", mTlv.field_1A_alarm_switch_id);
        ADD("Alarm Duration", mTlv.field_1C_alarm_duration);
        ADD("Disabled Resources", mTlv.field_1E_disabled_resources);

        ADD_RESOURCE(AnimId::Abe_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Abe_Arm_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Abe_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Slog_Body_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Head_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Arm_Gib, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Elum_Body_Gib, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SecurityDoor final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityDoor, "SecurityDoor", AO::TlvTypes::SecurityDoor_95)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Switch ID", mTlv.field_1A_switch_id);
        ADD("Code 1", mTlv.field_1C_code_1);
        ADD("Code 2", mTlv.field_1E_code2);
        ADD("X Position", mTlv.field_20_xpos);
        ADD("Y Position", mTlv.field_22_ypos);

        ADD_RESOURCE(AnimId::Security_Door, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_TimedMine final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_TimedMine, "TimedMine", AO::TlvTypes::TimedMine_22)
    {
        ADD("Switch ID(Unused?)", mTlv.field_18_switch_id);
        ADD("State", mTlv.field_1A_state);
        ADD("Scale", mTlv.field_1C_scale);
        ADD("Ticks Before Explosion", mTlv.mTicksUntilExplosion);
        ADD("Disabled Resources", mTlv.mDisabledResources);
    }
};

struct Path_SligSpawner final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligSpawner, "SligSpawner", AO::TlvTypes::SligSpawner_66)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Start State", mTlv.field_1A_start_state);
        ADD("Pause Time", mTlv.field_1C_pause_time);
        ADD("Pause Left Min", mTlv.field_1E_pause_left_min);
        ADD("Pause Left Max", mTlv.field_20_pause_left_max);
        ADD("Pause Right Min", mTlv.field_22_pause_right_min);
        ADD("Pause Right Max", mTlv.field_24_pause_right_max);
        ADD("Shoot Possessed Sligs", mTlv.field_26_shoot_possessed_sligs);
        ADD("Shoot On Sight Delay", mTlv.field_28_shoot_on_sight_delay);
        ADD("Bullet Shoot Count", mTlv.field_2A_num_times_to_shoot);
        ADD("unknown", mTlv.field_2C_unknown);
        ADD("Code 1", mTlv.field_2E_code1);
        ADD("Code 2", mTlv.field_30_code2);
        ADD("Chase Abe When Spotted", mTlv.field_32_chase_abe);
        ADD("Start Direction", mTlv.field_34_start_direction);
        ADD("Panic Timeout", mTlv.field_36_panic_timeout);
        ADD("Amount Of Panic Sounds (Unused?)", mTlv.field_38_num_panic_sounds);
        ADD("Panic Sound Timeout (Unused?)", mTlv.field_3A_panic_sound_timeout);
        ADD("Stop Chase Delay", mTlv.field_3C_stop_chase_delay);
        ADD("Time To Wait Before Chase", mTlv.field_3E_time_to_wait_before_chase);
        ADD("Slig Bound/Persist ID", mTlv.field_40_slig_bound_id);
        ADD("Alerted Listen Time", mTlv.field_42_listen_time);
        ADD("Percent Say What", mTlv.field_44_percent_say_what);
        ADD("Percent Beat Mudokon", mTlv.field_46_percent_beat_mud);
        ADD_HIDDEN("Talk To Abe (Unused?)", mTlv.field_48_talk_to_abe);
        ADD("Don't Shoot (Unused?)", mTlv.field_4A_dont_shoot);
        ADD("Z Shoot Delay", mTlv.field_4C_z_shoot_delay);
        ADD("Stay Awake", mTlv.field_4E_stay_awake);
        ADD("Disabled Resources", mTlv.field_50_disable_resources);
        ADD("Noise Wake Up Distance (Grids)", mTlv.field_52_noise_wake_up_distance);
        ADD("Slig Spawner Switch ID", mTlv.field_54_slig_spawner_switch_id);
    }
};

struct Path_MotionDetector final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_MotionDetector::InitialMoveDirection>("Enum_MotionDetectorInitialMoveDirection",
        {
            {AO::Path_MotionDetector::InitialMoveDirection::eRight_0, "right"},
            {AO::Path_MotionDetector::InitialMoveDirection::eLeft_1, "left"},
        });
    }

    CTOR_AO(Path_MotionDetector, "MotionDetector", AO::TlvTypes::MotionDetector_62)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Device X", mTlv.field_1A_device_x);
        ADD("Device Y", mTlv.field_1C_device_y);
        ADD("Speed", mTlv.field_1E_speed_x256);
        ADD("Initial Move Direction", mTlv.field_20_initial_move_direction);
        ADD("Draw Flare", mTlv.field_22_draw_flare);
        ADD("Disable Switch ID", mTlv.field_24_disable_switch_id);
        ADD("Alarm Switch ID", mTlv.field_26_alarm_switch_id);
        ADD("Alarm Duration", mTlv.field_28_alarm_duration);

        ADD_RESOURCE(AnimId::MotionDetector_Flare, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::MotionDetector_Laser, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_BackgroundAnimation final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<TPageAbr>("Enum_TPageAbr",
        {
            {TPageAbr::eBlend_1, "blend_1"},
            {TPageAbr::eBlend_2, "blend_2"},
            {TPageAbr::eBlend_3, "blend_3"},
            {TPageAbr::eBlend_0, "blend_0"},
        });

        types.AddEnum<AO::BgAnimSounds>("Enum_BgAnimSounds",
        {
            {AO::BgAnimSounds::eNone_m1, "None -1"},
            {AO::BgAnimSounds::eNone_0, "None 0"},
            {AO::BgAnimSounds::eFire_1, "Fire"},
            {AO::BgAnimSounds::eFireIdx_40, "Fire (ignore)"},
        });
    }

    CTOR_AO(Path_BackgroundAnimation, "BackgroundAnimation", AO::TlvTypes::BackgroundAnimation_19)
    {
        ADD("Animation ID", mTlv.field_18_animation_id);
        ADD("Is Semi Trans", mTlv.field_1A_is_semi_trans);
        ADD("Semi Trans Mode", mTlv.field_1C_semi_trans_mode);
        ADD("Sound Effect", mTlv.field_1E_sound_effect);
    }
};

struct Path_LCDStatusBoard final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_LCDStatusBoard, "LCDStatusBoard", AO::TlvTypes::LCDStatusBoard_103)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_Preloader final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Preloader, "Preloader", AO::TlvTypes::Preloader_102)
    {
        ADD("Unload Cams ASAP", mTlv.unload_cams_ASAP);
    }
};

struct Path_Pulley final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Pulley, "Pulley", AO::TlvTypes::Pulley_35)
    {
        ADD("Scale", mTlv.scale);
    }
};

struct Path_SoftLanding final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SoftLanding, "SoftLanding", AO::TlvTypes::SoftLanding_114)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_MudokonPathTrans final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MudokonPathTrans, "MudokonPathTrans", AO::TlvTypes::MudokonPathTrans_89)
    {
        ADD("Level", mTlv.level);
        ADD("Path", mTlv.path);
        ADD("Camera", mTlv.camera);
    }
};

struct Path_AbeStart final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_AbeStart, "AbeStart", AO::TlvTypes::AbeStart_37)
    {
        ADD("Scale", mTlv.scale);
    }
};

struct Path_ZSligCover final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ZSligCover, "ZSligCover", AO::TlvTypes::ZSligCover_83)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_ScrabLeftBound final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ScrabLeftBound, "ScrabLeftBound", AO::TlvTypes::ScrabLeftBound_74)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_ScrabRightBound final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ScrabRightBound, "ScrabRightBound", AO::TlvTypes::ScrabRightBound_75)
    {
        EMPTY_CTOR_AO();
    }
};

struct Path_ScrabNoFall final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ScrabNoFall, "ScrabNoFall", AO::TlvTypes::ScrabNoFall_93)
    {
        EMPTY_CTOR_AO();
    }
};

// TODO: lift mudokon might not be the best name because this mud can also be a password giver
struct Path_LiftMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_LiftMudokon::Direction>("Enum_LiftMudokonDirection",
            {
                {AO::Path_LiftMudokon::Direction::eRight_0, "Right"},
                {AO::Path_LiftMudokon::Direction::eLeft_1, "Left"},
            });
    }

    CTOR_AO(Path_LiftMudokon, "LiftMudokon", AO::TlvTypes::LiftMudokon_32)
    {
        ADD("How Far To Walk", mTlv.field_18_how_far_to_walk);
        ADD("Lift Switch ID", mTlv.field_1A_lift_switch_id);
        ADD("Start Direction", mTlv.field_1C_direction);
        ADD("Give Password", mTlv.field_1E_give_password);
        ADD("Scale", mTlv.field_20_scale);
        ADD("Code 1", mTlv.field_22_code1);
        ADD("Code 2", mTlv.field_24_code2);
    }
};

struct Path_HoneySack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HoneySack, "HoneySack", AO::TlvTypes::HoneySack_36)
    {
        ADD("Chase Time", mTlv.field_18_chase_ticks);
        ADD("Scale", mTlv.field_1A_scale);

        ADD_RESOURCE(AnimId::HoneySack_Falling, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::HoneySack_FallingToSmashed, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::HoneySack_Hanging, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::HoneySack_OnGround, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Elum_BeesStruggling, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_SlingMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SlingMudokon, "SlingMudokon", AO::TlvTypes::SlingMudokon_41)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Don't Whistle Password", mTlv.field_1A_bDontWhistlePassword);
        ADD("Code 1", mTlv.field_1C_code_1);
        ADD("Code 2", mTlv.field_1E_code_2);

        ADD_RESOURCE(AnimId::Mudokon_Sling_Angry, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_AngryToIdle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_Idle, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_ShootEnd, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_ShootStart, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_Sling_Speak, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_BeeSwarmHole final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_BeeSwarmHole::MovementType>("Enum_BeeSwarmHoleMovementType",
        {
            {AO::Path_BeeSwarmHole::MovementType::eHover_0, "Hover"},
            {AO::Path_BeeSwarmHole::MovementType::eAttack_1, "Attack"},
            {AO::Path_BeeSwarmHole::MovementType::eFollowPath_2, "Follow Path"},
        });
    }

    CTOR_AO(Path_BeeSwarmHole, "BeeSwarmHole", AO::TlvTypes::BeeSwarmHole_34)
    {
        ADD("what_to_spawn", mTlv.field_18_what_to_spawn);
        ADD("Start Interval", mTlv.field_1A_interval);
        ADD("Fall Switch ID", mTlv.field_1C_fall_switch_id);
        ADD("Movement Type", mTlv.field_1E_movement_type);
        ADD("Bees Amount", mTlv.field_20_bees_amount);
        ADD("Chase Time", mTlv.field_22_chase_time);
        ADD("Speed", mTlv.field_24_speed);
        ADD("Scale (Unused?)", mTlv.field_26_scale);
    }
};

struct Path_MeatSack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MeatSack, "MeatSack", AO::TlvTypes::MeatSack_71)
    {
        ADD("Meat Fall Direction", mTlv.field_18_meat_fall_direction);
        ADD("X Velocity", mTlv.field_1A_x_vel);
        ADD("Y Velocity", mTlv.field_1C_y_vel);
        ADD("Scale", mTlv.field_1E_scale);
        ADD("Amount Of Meat", mTlv.field_20_amount_of_meat);

        ADD_RESOURCE(AnimId::Rock, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Grenade, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::Meat, ReliveAPI::AddResourceTo::File);
        ADD_RESOURCE(AnimId::MeatSack_Hit, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::MeatSack_Idle, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_RollingBall final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RollingBall, "RollingBall", AO::TlvTypes::RollingBall_56)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Roll Direction", mTlv.field_1A_roll_direction);
        ADD("Release Switch ID", mTlv.field_1C_release_switch_id);
        ADD("Speed", mTlv.field_1E_speed);
        ADD("Acceleration", mTlv.field_20_acceleration);
    }
};

struct Path_RollingBallStopper final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RollingBallStopper, "RollingBallStopper", AO::TlvTypes::RollingBallStopper_59)
    {
        ADD("Stopper Switch ID", mTlv.field_18_stopper_switch_id);
        ADD("Scale", mTlv.field_1A_scale);
        ADD("Ball Switch ID", mTlv.field_1C_ball_switch_id);
        ADD("Direction", mTlv.field_1E_direction);
    }
};

struct Path_Bees final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Bees, "Bees", AO::TlvTypes::Bees_43)
    {
        ADD("Switch ID", mTlv.switch_id);
        ADD("Swarm Size", mTlv.swarm_size);
        ADD("Chase Time", mTlv.chase_time);
        ADD("Speed", mTlv.speed);
        ADD("Disable Resources", mTlv.disable_resources);
        ADD("Amount Of Bees", mTlv.num_bees);

        ADD_RESOURCE(AnimId::Elum_BeesStruggling, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_ZBall final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_ZBall::StartPos>("Enum_ZBallStartPos",
        {
            {AO::Path_ZBall::StartPos::eCenter_0, "Center"},
            {AO::Path_ZBall::StartPos::eOut_1, "Out"},
            {AO::Path_ZBall::StartPos::eIn_2, "In"},
        });

        types.AddEnum<AO::Path_ZBall::Speed>("Enum_ZBallSpeed",
        {
            {AO::Path_ZBall::Speed::eNormal_0, "Normal"},
            {AO::Path_ZBall::Speed::eFast_1, "Fast"},
            {AO::Path_ZBall::Speed::eSlow_2, "Slow"},
        });
    }

    CTOR_AO(Path_ZBall, "ZBall", AO::TlvTypes::ZBall_14)
    {
        ADD("Start Position", mTlv.field_18_start_pos);
        ADD("Scale", mTlv.field_1A_scale);
        ADD("Speed", mTlv.field_1C_speed);

        ADD_RESOURCE(AnimId::Swinging_Ball_Fast, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Swinging_Ball_Normal, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Swinging_Ball_Slow, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_ZShot, ReliveAPI::AddResourceTo::CameraBlock);
        ADD_RESOURCE(AnimId::Mudokon_ZShotRolling, ReliveAPI::AddResourceTo::CameraBlock);
    }
};

struct Path_FootSwitch final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
        {
            {AO::FootSwitchTriggerBy::eAbe_0, "Abe"},
            {AO::FootSwitchTriggerBy::eAnyone_1, "Anyone"},
        });
    }

    CTOR_AO(Path_FootSwitch, "FootSwitch", AO::TlvTypes::FootSwitch_60)
    {
        ADD("Switch ID", mTlv.field_18_switch_id);
        ADD("Scale", mTlv.field_1A_scale);
        ADD("Action", mTlv.field_1C_action);
        ADD("Triggered By", mTlv.field_1E_trigger_by);
    }
};

struct Path_Paramite final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Paramite, "Paramite", AO::TlvTypes::Paramite_48)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Enter From Web", mTlv.field_1A_bEnter_from_web);
        ADD("Alone - Chase Delay", mTlv.field_1C_alone_chase_delay);
        ADD("Surprise Web Delay", mTlv.field_1E_surprise_web_delay_timer);
        ADD("Meat Eating Time", mTlv.field_20_meat_eating_time);
        ADD("Group - Chase Delay", mTlv.field_22_group_chase_delay);
        ADD("Disabled Resources", mTlv.field_24_disabled_resources);
        ADD("Surprise Web Switch ID", mTlv.field_26_surprise_web_switch_id);
        ADD("Hiss Before Attack", mTlv.field_28_hiss_before_attack);
        ADD("Delete When Far Away", mTlv.field_2A_delete_when_far_away);

        ADD_RESOURCE(AnimId::Paramite_Death, ReliveAPI::AddResourceTo::File);
    }
};

struct Path_Honey final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Honey, "Honey", AO::TlvTypes::Honey_20)
    {
        ADD("Switch ID", mTlv.switch_id);
        ADD("State", mTlv.state);
        ADD("Scale", mTlv.scale);
    }
};

struct Path_HoneyDripTarget final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HoneyDripTarget, "HoneyDripTarget", AO::TlvTypes::HoneyDripTarget_42)
    {
        EMPTY_CTOR_AO();
    }
   
};
} // namespace AOTlvs

#undef EMPTY_CTOR_AO
#undef CTOR_AO
