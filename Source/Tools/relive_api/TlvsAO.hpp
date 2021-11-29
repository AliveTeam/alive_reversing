#pragma once

#include "TlvObjectBaseAO.hpp"
#include "TlvObjectBaseMacros.hpp"

#include "../../AliveLibAO/HoistRocksEffect.hpp"
#include "../../AliveLibAO/Abe.hpp"
#include "../../AliveLibAO/Door.hpp"
#include "../../AliveLibAO/Switch.hpp"
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
#include "../../AliveLibAO/IdSplitter.hpp"
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
#include "../../AliveLibAO/SlogHut.hpp"
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
            mPSelfTlv->field_2_length = static_cast<s16>(mSizeOfT);\
            mPSelfTlv->field_4_type.mType = mType;\
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
    s16 id;
    s16 state;
    s32 scale;
};

struct Path_Bees final : public Path_TLV
{
    s16 id;
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
    s32 scale;
};

struct Path_MudokonPathTrans final : public Path_TLV
{
    LevelIds level;
    s16 path;
    s32 camera;
};

struct Path_Pulley final : public Path_TLV
{
    s32 scale;
};

struct Path_Preloader final : public Path_TLV
{
    s32 unload_cams_ASAP;
};

struct Path_SligSpawner final : public Path_TLV
{
    enum class StartState : s16
    {
        Listening_0 = 0,
        Paused_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        GameEnder_4 = 4,
        Paused_5 = 5,
    };
    s16 field_18_scale;
    StartState field_1A_start_state;
    s16 field_1C_pause_time;
    s16 field_1E_pause_left_min;
    s16 field_20_pause_left_max;
    s16 field_22_pause_right_min;
    s16 field_24_pause_right_max;
    s16 field_26_chal_type;
    s16 field_28_chal_time;
    s16 field_2A_number_of_times_to_shoot;
    s16 field_2C_unknown; // TODO: Part of above field, check me?
    s16 field_2E_code1;
    s16 field_30_code2;
    s16 field_32_chase_abe;
    s16 field_34_start_direction;
    s16 field_36_panic_timeout;
    s16 field_38_num_panic_sounds;
    s16 field_3A_panic_sound_timeout;
    s16 field_3C_stop_chase_delay;
    s16 field_3E_time_to_wait_before_chase;
    s16 field_40_slig_id;
    s16 field_42_listen_time;
    s16 field_44_percent_say_what;
    s16 field_46_percent_beat_mud;
    s16 field_48_talk_to_abe;
    s16 field_4A_dont_shoot;
    s16 field_4C_z_shoot_delay;
    s16 field_4E_stay_awake;
    s16 field_50_disable_resources;
    s16 field_52_noise_wake_up_distance;
    s32 field_54_id;
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
        types.AddEnum<AO::Path_DoorFlame::Colour>("Enum_DoorFlame_Colour",
            {
                {AO::Path_DoorFlame::Colour::unknown_0, "unknown"},
                {AO::Path_DoorFlame::Colour::red_1, "red"},
                {AO::Path_DoorFlame::Colour::green_2, "green"},
                {AO::Path_DoorFlame::Colour::blue_3, "blue"}
            });
    }

    CTOR_AO(Path_DoorFlame, "DoorFlame", AO::TlvTypes::DoorFlame_84)
    {
        ADD("id", mTlv.field_18_id);
        ADD("scale", mTlv.field_1A_scale);
        ADD("colour", mTlv.field_1C_colour);
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
        ADD("scale_percent", mTlv.field_18_scale_percent);
        ADD("pal_id", mTlv.field_1A_pal_id);
        ADD("target_id", mTlv.field_1C_target_id);
        ADD("voice_adjust", mTlv.field_1E_voice_adjust);
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
        ADD("start_id", mTlv.field_18_start_id);
    }
};

struct Path_MovieStone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovieStone, "MovieStone", AO::TlvTypes::MovieStone_51)
    {
        ADD("fmv_id", mTlv.mData.fmvId);
        ADD("scale", mTlv.mData.scale);
    }
};

struct Path_BellsongStone final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::BellsongTypes>("Enum_BellsongTypes",
                                         {
                                             {AO::BellsongTypes::eWhistle, "whistle"},
                                             {AO::BellsongTypes::eChimes, "chimes"},
                                         });
    }

    CTOR_AO(Path_BellsongStone, "BellSongStone", AO::TlvTypes::BellSongStone_54)
    {
        ADD("scale", mTlv.mData.scale);
        ADD("type", mTlv.mData.type);
        ADD("code1", mTlv.mData.code1);
        ADD("code2", mTlv.mData.code2);
        ADD("id", mTlv.mData.id);
    }
};

struct Path_HandStone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HandStone, "HandStone", AO::TlvTypes::HandStone_100)
    {
        ADD("scale", mTlv.mData.scale);

        ADD("camera1_level", mTlv.mData.camera1.level);
        ADD("camera1_path", mTlv.mData.camera1.path);
        ADD("camera1_camera", mTlv.mData.camera1.camera);

        ADD("camera2_level", mTlv.mData.camera2.level);
        ADD("camera2_path", mTlv.mData.camera2.path);
        ADD("camera2_camera", mTlv.mData.camera2.camera);

        ADD("camera3_level", mTlv.mData.camera3.level);
        ADD("camera3_path", mTlv.mData.camera3.path);
        ADD("camera3_camera", mTlv.mData.camera3.camera);
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
                                          {AO::DoorStates::eOpening_2, "Opening"},
                                          {AO::DoorStates::eClosing_3, "Closing"},
                                      });
    }

    CTOR_AO(Path_Door, "Door", AO::TlvTypes::Door_6)
    {
        ADD("Level", mTlv.field_18_level);
        ADD("Path", mTlv.field_1A_path);
        ADD("Camera", mTlv.field_1C_camera);
        ADD("Scale", mTlv.field_1E_scale);
        ADD("DoorNumber", mTlv.field_20_door_number);
        ADD("Id", mTlv.field_22_id);
        ADD("TargetDoorNumber", mTlv.field_24_target_door_number);
        ADD("StartState", mTlv.field_26_start_state);
        ADD("DoorClosed", mTlv.field_28_door_closed);
        ADD("Hub1", mTlv.field_2A_hub1);
        ADD("Hub2", mTlv.field_2A_hub2);
        ADD("Hub3", mTlv.field_2A_hub3);
        ADD("Hub4", mTlv.field_2A_hub4);
        ADD("Hub5", mTlv.field_2A_hub5);
        ADD("Hub6", mTlv.field_2A_hub6);
        ADD("Hub7", mTlv.field_2A_hub7);
        ADD("Hub8", mTlv.field_2A_hub8);
        ADD("WipeEffect", mTlv.field_3A_wipe_effect);
        ADD("MovieNumber", mTlv.field_3C_movie_number);
        ADD("XOffset", mTlv.field_3E_x_offset);
        ADD("YOffset", mTlv.field_40_y_offset);
        ADD("WipeXOrg", mTlv.field_42_wipe_x_org);
        ADD("WipeYOrg", mTlv.field_44_wipe_y_org);
        ADD("AbeDirection", mTlv.field_46_abe_direction);
    }
};

struct Path_ContinuePoint final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ContinuePoint, "ContinuePoint", AO::TlvTypes::ContinuePoint_0)
    {
        ADD("ZoneNumber", mTlv.field_18_zone_number);
        ADD("ClearFromId", mTlv.field_1A_clear_from_id);
        ADD("ClearToId", mTlv.field_1C_clear_to_id);
        ADD("ElumRestarts", mTlv.field_1E_elum_restarts);
        ADD("AbeSpawnDirection", mTlv.field_20_abe_direction);
    }
};

struct Path_Hoist final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Hoist::Type>("Enum_HoistType",
                                            {
                                                {AO::Path_Hoist::Type::eNextEdge, "next_edge"},
                                                {AO::Path_Hoist::Type::eNextFloor, "next_floor"},
                                                {AO::Path_Hoist::Type::eOffScreen, "off_screen"},
                                            });

        types.AddEnum<AO::Path_Hoist::EdgeType>("Enum_HoistEdgeType",
                                                {
                                                    {AO::Path_Hoist::EdgeType::eBoth, "both"},
                                                    {AO::Path_Hoist::EdgeType::eLeft, "left"},
                                                    {AO::Path_Hoist::EdgeType::eRight, "right"},
                                                });
    }

    CTOR_AO(Path_Hoist, "Hoist", AO::TlvTypes::Hoist_3)
    {
        ADD("HoistType", mTlv.field_18_hoist_type);
        ADD("HoistEdgeType", mTlv.field_1A_edge_type);
        ADD("Id", mTlv.field_1C_id);
    }
};

struct Path_Change final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Change, "PathTransition", AO::TlvTypes::PathTransition_1)
    {
        ADD("Level", mTlv.field_18_level);
        ADD("HoistEdgeType", mTlv.field_1A_path);
        ADD("Camera", mTlv.field_1C_camera);
        ADD("Movie", mTlv.field_1E_movie); // TODO: Enum
        ADD("Wipe", mTlv.field_20_wipe);   // TODO: Enum
        ADD("Scale", mTlv.field_22_scale); // TODO: Enum
    }
};

struct Path_Switch final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Switch, "Switch", AO::TlvTypes::Switch_26)
    {
        ADD("TriggerObject", mTlv.field_18_trigger_object);
        ADD("TriggerAction", mTlv.field_1A_trigger_object_action);
        ADD("Scale", mTlv.field_1C_scale);
        ADD("OnSound", mTlv.field_1E_on_sound);               // TODO: Enum
        ADD("OffSound", mTlv.field_20_off_sound);             // TODO: Enum
        ADD("SoundDirection", mTlv.field_22_sound_direction); // TODO: Enum
    }
};

struct Path_LightEffect final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_LightEffect::Type>("Enum_LightType",
                                                  {
                                                      {AO::Path_LightEffect::Type::Star_0, "Star"},
                                                      {AO::Path_LightEffect::Type::GoldGlow_1, "GoldGlow"},
                                                      {AO::Path_LightEffect::Type::GreenGlow_2, "GreenGlow"},
                                                      {AO::Path_LightEffect::Type::FlintGlow_3, "FlintGlow"},
                                                      {AO::Path_LightEffect::Type::Switchable_RedGreenDoorLights_4, "RedGreenDoorLight"},
                                                      {AO::Path_LightEffect::Type::Switchable_RedGreenHubLight_5, "RedGreenHubLight"},
                                                  });
    }

    CTOR_AO(Path_LightEffect, "LightEffect", AO::TlvTypes::LightEffect_106)
    {
        ADD("Type", mTlv.field_18_type);
        ADD("Size", mTlv.field_1A_size);
        ADD("Id", mTlv.field_1C_id);
        ADD("FlipX", mTlv.field_1E_flip_x);
    }
};

struct Path_ElectricWall final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::StartState>("Enum_StartState",
                                                  {
                                                      {AO::StartState::eOff_0, "off"},
                                                      {AO::StartState::eOn_1, "on"},
                                                  });
    }

    CTOR_AO(Path_ElectricWall, "ElectricWall", AO::TlvTypes::ElectricWall_67)
    {
        ADD("Scale", mTlv.field_18_scale);
        ADD("Id", mTlv.field_1A_id);
        ADD("State", mTlv.field_1C_start_state); // TODO: Enum
    }
};

struct Path_ContinueZone final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ContinueZone, "ContinueZone", AO::TlvTypes::ContinueZone_2)
    {
        ADD("ZoneNumber", mTlv.field_10_zone_number);
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
        types.AddEnum<AO::Path_Edge::Type>("Enum_EdgeType",
                                           {
                                               {AO::Path_Edge::Type::eLeft, "left"},
                                               {AO::Path_Edge::Type::eRight, "right"},
                                               {AO::Path_Edge::Type::eBoth, "both"},
                                           });
    }

    CTOR_AO(Path_Edge, "Edge", AO::TlvTypes::Edge_4)
    {
        ADD("type", mTlv.field_18_type);
        ADD("can_grab", mTlv.field_1A_can_grab);
    }
};

struct Path_WellLocal final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_WellLocal, "WellLocal", AO::TlvTypes::WellLocal_11)
    {
        // Well base
        ADD("scale", mTlv.field_18_scale);
        ADD("trigger_id", mTlv.field_1A_trigger_id);
        ADD("well_id", mTlv.field_1C_well_id);
        ADD("resource_id", mTlv.field_1E_res_id);
        ADD("exit_x", mTlv.field_20_exit_x);
        ADD("exit_y", mTlv.field_22_exit_y);
        ADD("dx", mTlv.field_24_off_level_or_dx.dx);
        ADD("dy", mTlv.field_26_off_path_or_dy);

        // Well local
        ADD("on_dx", mTlv.field_28_on_dx);
        ADD("on_dy", mTlv.field_2A_on_dy);
        ADD("emit_leaves", mTlv.field_2C_bEmit_leaves);
        ADD("leaf_x", mTlv.field_2E_leaf_x);
        ADD("leaf_y", mTlv.field_30_leaf_y);
    }
};

struct Path_WellExpress final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_WellExpress, "WellExpress", AO::TlvTypes::WellExpress_45)
    {
        // Well base
        ADD("scale", mTlv.field_18_scale);
        ADD("trigger_id", mTlv.field_1A_trigger_id);
        ADD("well_id", mTlv.field_1C_well_id);
        ADD("resource_id", mTlv.field_1E_res_id);
        ADD("exit_x", mTlv.field_20_exit_x);
        ADD("exit_y", mTlv.field_22_exit_y);
        ADD("off_level", mTlv.field_24_off_level_or_dx.level);
        ADD("off_path", mTlv.field_26_off_path_or_dy);

        // Well express
        ADD("off_camera", mTlv.field_28_off_camera);
        ADD("off_well_id", mTlv.field_2A_off_well_id);
        ADD("on_level", mTlv.field_2C_on_level);
        ADD("on_path", mTlv.field_2E_on_path);
        ADD("on_camera", mTlv.field_30_on_camera);
        ADD("on_well_id", mTlv.field_32_on_well_id);
        ADD("emit_leaves", mTlv.field_34_emit_leaves);
        ADD("leaf_x", mTlv.field_36_leaf_x);
        ADD("leaf_y", mTlv.field_38_leaf_y);
        ADD("movie_id", mTlv.field_3A_movie_id);
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
                                                                {AO::Path_EnemyStopper::StopDirection::Left_0, "left"},
                                                                {AO::Path_EnemyStopper::StopDirection::Right_1, "right"},
                                                                {AO::Path_EnemyStopper::StopDirection::Both_2, "both"},
                                                            });
    }

    CTOR_AO(Path_EnemyStopper, "EnemyStopper", AO::TlvTypes::EnemyStopper_79)
    {
        ADD("direction", mTlv.field_18_direction);
        ADD("id", mTlv.field_1A_id);
    }
};

struct Path_Slig final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_Slig::StartState>("Enum_SligStartState",
                                                 {
                                                     {AO::Path_Slig::StartState::Listening_0, "listening"},
                                                     {AO::Path_Slig::StartState::Paused_1, "paused1"},
                                                     {AO::Path_Slig::StartState::Sleeping_2, "sleeping"},
                                                     {AO::Path_Slig::StartState::Chase_3, "chase"},
                                                     {AO::Path_Slig::StartState::GameEnder_4, "game_ender"},
                                                     {AO::Path_Slig::StartState::Paused_5, "paused2"},
                                                 });
    }

    CTOR_AO(Path_Slig, "Slig", AO::TlvTypes::Slig_24)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("start_state", mTlv.field_1A_start_state);
        ADD("pause_time", mTlv.field_1C_pause_time);
        ADD("pause_left_min", mTlv.field_1E_pause_left_min);
        ADD("pause_left_max", mTlv.field_20_pause_left_max);
        ADD("pause_right_min", mTlv.field_22_pause_right_min);
        ADD("pause_right_max", mTlv.field_24_pause_right_max);
        ADD("chal_type", mTlv.field_26_chal_type);
        ADD("chal_time", mTlv.field_28_chal_time);
        ADD("number_of_times_to_shoot", mTlv.field_2A_number_of_times_to_shoot);
        ADD("unknown", mTlv.field_2C_unknown);
        ADD("code1", mTlv.field_2E_code1);
        ADD("code2", mTlv.field_30_code2);
        ADD("chase_abe", mTlv.field_32_chase_abe);
        ADD("start_direction", mTlv.field_34_start_direction);
        ADD("panic_timeout", mTlv.field_36_panic_timeout);
        ADD("num_panic_sounds", mTlv.field_38_num_panic_sounds);
        ADD("panic_sound_timeout", mTlv.field_3A_panic_sound_timeout);
        ADD("stop_chase_delay", mTlv.field_3C_stop_chase_delay);
        ADD("time_to_wait_before_chase", mTlv.field_3E_time_to_wait_before_chase);
        ADD("slig_id", mTlv.field_40_slig_id);
        ADD("listen_time", mTlv.field_42_listen_time);
        ADD("percent_say_what", mTlv.field_44_percent_say_what);
        ADD("percent_beat_mud", mTlv.field_46_percent_beat_mud);
        ADD("talk_to_abe", mTlv.field_48_talk_to_abe);
        ADD("dont_shoot", mTlv.field_4A_dont_shoot);
        ADD("z_shoot_delay", mTlv.field_4C_z_shoot_delay);
        ADD("stay_awake", mTlv.field_4E_stay_awake);
        ADD("disable_resources", mTlv.field_50_disable_resources.Raw().all);
        ADD("noise_wake_up_distance", mTlv.field_52_noise_wake_up_distance);
        ADD("id", mTlv.field_54_id);
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
        ADD("id", mTlv.field_18_slig_id);
        ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all); // ??
    }
};

struct Path_SligRightBound final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligRightBound, "SligRightBound", AO::TlvTypes::eSligBoundRight_76)
    {
        ADD("id", mTlv.field_18_slig_id);
        ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all);
    }
};

struct Path_SligPersist final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SligPersist, "SligPersist", AO::TlvTypes::eSligPersist_77)
    {
        ADD("id", mTlv.field_18_slig_id);
        ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all);
    }
};

struct Path_SecurityOrb final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityOrb, "SecurityOrb", AO::TlvTypes::SecurityOrb_29)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("disabled_resources", mTlv.field_1A_disable_resources);
    }
};

struct Path_FallingItem final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_FallingItem, "FallingItem", AO::TlvTypes::FallingItem_15)
    {
        ADD("id", mTlv.field_18_id);
        ADD("scale", mTlv.field_1A_scale);
        ADD("delay_time", mTlv.field_1C_delay_time);
        ADD("number_of_items", mTlv.field_1E_number_of_items);
        ADD("reset_id", mTlv.field_20_reset_id);
    }
};

struct Path_Mine final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Mine, "Mine", AO::TlvTypes::Mine_46)
    {
        ADD("num_patterns", mTlv.field_18_num_patterns);
        ADD("pattern", mTlv.field_1A_pattern);
        ADD("scale", mTlv.field_1C_scale);
        ADD("disabled_resources", mTlv.field_1E_disabled_resources);
        ADD("persists_offscreen", mTlv.field_20_persists_offscreen);
    }
};

struct Path_Dove final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Dove, "Dove", AO::TlvTypes::Dove_12)
    {
        ADD("dove_count", mTlv.field_18_dove_count);
        ADD("pixel_perfect", mTlv.field_1A_pixel_perfect);
        ADD("scale", mTlv.field_1C_scale);
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
        ADD("num_patterns", mTlv.field_18_num_patterns);
        ADD("pattern", mTlv.field_1A_pattern);
        ADD("scale", mTlv.field_1C_scale);
        ADD("state", mTlv.field_1E_state);
        ADD("disabled_resources", mTlv.field_20_disabled_resources);
    }
};

struct Path_HintFly final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HintFly, "HintFly", AO::TlvTypes::HintFly_92)
    {
        ADD("message_id", mTlv.field_18_message_id);
    }
};

struct Path_Bat final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Bat, "Bat", AO::TlvTypes::Bat_49)
    {
        ADD("ticks_before_moving", mTlv.field_18_ticks_before_moving);
        ADD("speed", mTlv.field_1A_speed);
        ADD("scale", mTlv.field_1C_scale);
        ADD("attack_duration", mTlv.field_1E_attack_duration);
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
        ADD("centre_w", mTlv.field_18_centre_w);
        ADD("centre_h", mTlv.field_1A_centre_h);
        ADD("r", mTlv.field_1C_r);
        ADD("g", mTlv.field_1E_g);
        ADD("b", mTlv.field_20_b);
        ADD("id", mTlv.field_22_id);
        ADD("scale", mTlv.field_24_scale);
    }
};

struct Path_BellHammer final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BellHammer, "BellHammer", AO::TlvTypes::BellHammer_27)
    {
        ADD("id", mTlv.field_18_id);
        ADD("action", mTlv.field_1A_action);
        ADD("scale", mTlv.field_1C_scale);
        ADD("direction", mTlv.field_1E_direction);
    }
};

struct Path_IdSplitter final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_IdSplitter, "IdSplitter", AO::TlvTypes::IdSplitter_94)
    {
        ADD("source_id", mTlv.field_18_source_id);
        ADD("delay", mTlv.field_1A_delay);
        ADD("id_1", mTlv.field_1C_id1);
        ADD("id_2", mTlv.field_1C_id2);
        ADD("id_3", mTlv.field_1C_id3);
        ADD("id_4", mTlv.field_1C_id4);
    }
};

struct Path_PullRingRope final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_PullRingRope, "PullRingRope", AO::TlvTypes::PullRingRope_18)
    {
        ADD("id", mTlv.field_18_id);
        ADD("action", mTlv.field_1A_action);
        ADD("rope_length", mTlv.field_1C_rope_length);
        ADD("scale", mTlv.field_1E_scale);
        ADD("on_sound", mTlv.field_20_on_sound);
        ADD("off_sound", mTlv.field_22_off_sound);
        ADD("sound_direction", mTlv.field_24_sound_direction);
    }
};

struct Path_MusicTrigger final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MusicTrigger, "MusicTrigger", AO::TlvTypes::MusicTrigger_105)
    {
        ADD("type", mTlv.field_18_type);
        ADD("enabled_by", mTlv.field_1A_enabled_by);
        ADD("id", mTlv.field_1C_id);
        ADD("timer", mTlv.field_1E_timer);
    }
};

struct Path_ElumPathTrans final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ElumPathTrans, "ElumPathTrans", AO::TlvTypes::ElumPathTrans_99)
    {
        ADD("level", mTlv.field_18_level);
        ADD("path", mTlv.field_1A_path);
        ADD("camera", mTlv.field_1C_camera);
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
    CTOR_AO(Path_LiftPoint, "LiftPoint", AO::TlvTypes::LiftPoint_8)
    {
        ADD("id", mTlv.field_18_id);
        ADD("is_start_point", mTlv.field_1A_bstart_point);
        ADD("lift_type", mTlv.field_1C_lift_type);
        ADD("lift_point_stop_type", mTlv.field_1E_lift_point_stop_type);
        ADD("scale", mTlv.field_20_scale);
        ADD("ignore_lift_mover", mTlv.field_22_bIgnore_lift_mover);
    }
};

struct Path_MovingBomb final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovingBomb, "MovingBomb", AO::TlvTypes::MovingBomb_86)
    {
        ADD("speed", mTlv.field_18_speed);
        ADD("id", mTlv.field_1A_id);
        ADD("start_type_triggered_by_alarm", mTlv.field_1C_bStart_type_triggered_by_alarm);
        ADD("scale", mTlv.field_1E_scale);
        ADD("max_rise", mTlv.field_20_max_rise);
        ADD("disabled_resources", mTlv.field_22_disabled_resources);
        ADD("start_speed", mTlv.field_24_start_speed);
        ADD("persist_offscreen", mTlv.field_26_persist_offscreen);
    }
};

struct Path_MovingBombStopper final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MovingBombStopper, "MovingBombStopper", AO::TlvTypes::MovingBombStopper_87)
    {
        ADD("min_delay", mTlv.field_18_min_delay);
        ADD("max_delay", mTlv.field_1A_max_delay);
    }
};

struct Path_RingMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RingMudokon, "RingMudokon", AO::TlvTypes::RingMudokon_50)
    {
        ADD("facing", mTlv.field_18_facing);
        ADD("abe_must_be_same_direction", mTlv.field_1A_abe_must_be_same_direction);
        ADD("scale", mTlv.field_1C_scale);
        ADD("silent", mTlv.field_1E_silent);
        ADD("code1", mTlv.field_20_code1);
        ADD("code2", mTlv.field_22_code2);
        ADD("action", mTlv.field_24_action);
        ADD("ring_timeout", mTlv.field_26_ring_timeout);
        ADD("instant_powerup", mTlv.field_28_instant_powerup);
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
    CTOR_AO(Path_MeatSaw, "MeatSaw", AO::TlvTypes::MeatSaw_88)
    {
        ADD("scale", mTlv.field_18_scale_background);
        ADD("min_time_off1", mTlv.field_1A_min_time_off1);
        ADD("max_time_off1", mTlv.field_1C_max_time_off1);
        ADD("max_rise_time", mTlv.field_1E_max_rise_time);
        ADD("id", mTlv.field_20_id);
        ADD("type", mTlv.field_22_type);
        ADD("speed", mTlv.field_24_speed);
        ADD("start_state", mTlv.field_26_start_state);
        ADD("off_speed", mTlv.field_28_off_speed);
        ADD("min_time_off2", mTlv.field_2A_min_time_off2);
        ADD("max_time_off2", mTlv.field_2C_max_time_off2);
        ADD("initial_position", mTlv.field_2E_inital_position);
    }
};

struct Path_LCDScreen final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_LCDScreen, "LCDScreen", AO::TlvTypes::LCDScreen_98)
    {
        ADD("message_1_id", mTlv.field_18_message_1_id);
        ADD("message_rand_min", mTlv.field_1A_message_rand_min);
        ADD("message_rand_max", mTlv.field_1C_message_rand_max);
    }
};

struct Path_InvisibleSwitch final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_InvisibleSwitch, "InvisibleSwitch", AO::TlvTypes::InvisibleSwitch_81)
    {
        ADD("id", mTlv.field_18_id);
        ADD("action", mTlv.field_1A_action);
        ADD("delay", mTlv.field_1C_delay);
        ADD("set_off_alarm", mTlv.field_1E_set_off_alarm);
        ADD("scale", mTlv.field_20_scale);
    }
};

struct Path_TrapDoor final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_TrapDoor, "TrapDoor", AO::TlvTypes::TrapDoor_55)
    {
        ADD("id", mTlv.field_18_id);
        ADD("start_state", mTlv.field_1A_start_state);
        ADD("self_closing", mTlv.field_1C_self_closing);
        ADD("scale", mTlv.field_1E_scale);
        ADD("dest_level", mTlv.field_20_dest_level);
        ADD("direction", mTlv.field_22_direction);
        ADD("anim_offset", mTlv.field_24_anim_offset);
    }
};

struct Path_BirdPortal final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::PortalSide>("Enum_PortalSide",
                                      {
                                          {AO::PortalSide::eRight_0, "right"},
                                          {AO::PortalSide::eLeft_1, "left"},
                                      });

        types.AddEnum<AO::PortalType>("Enum_PortalType",
                                      {
                                          {AO::PortalType::eAbe_0, "abe"},
                                          {AO::PortalType::eWorker_1, "worker"},
                                          {AO::PortalType::eShrykull_2, "shrykull"},
                                          {AO::PortalType::eMudTeleport_3, "mud_teleport"},
                                      });
    }

    CTOR_AO(Path_BirdPortal, "BirdPortal", AO::TlvTypes::BirdPortal_52)
    {
        ADD("side", mTlv.field_18_side);
        ADD("dest_level", mTlv.field_1A_dest_level);
        ADD("dest_path", mTlv.field_1C_dest_path);
        ADD("dest_camera", mTlv.field_1E_dest_camera);
        ADD("scale", mTlv.field_20_scale);
        ADD("movie_id", mTlv.field_22_movie_id);
        ADD("portal_type", mTlv.field_24_portal_type);
        ADD("num_muds_for_shrykull", mTlv.field_26_num_muds_for_shrykul);
    }
};

struct Path_BoomMachine final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BoomMachine, "BoomMachine", AO::TlvTypes::BoomMachine_97)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("nozzle_side", mTlv.field_1A_nozzle_side);
        ADD("disabled_resources", mTlv.field_1C_disabled_resources);
        ADD("number_of_grenades", mTlv.field_1E_number_of_grenades);
    }
};

struct Path_Mudokon final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Mudokon, "Mudokon", AO::TlvTypes::Mudokon_82)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("job", mTlv.field_1A_job);
        ADD("direction", mTlv.field_1C_direction);
        ADD("voice_adjust", mTlv.field_1E_voice_adjust);
        ADD("rescue_id", mTlv.field_20_rescue_id);
        ADD("deaf", mTlv.field_22_deaf);
        ADD("disabled_resources", mTlv.field_24_disabled_resources);
        ADD("persist", mTlv.field_26_persist);
    }
};

struct Path_BirdPortalExit final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_BirdPortalExit, "BirdPortalExit", AO::TlvTypes::BirdPortalExit_53)
    {
        ADD("side", mTlv.field_18_side);
        ADD("scale", mTlv.field_1A_scale);
    }
};

struct Path_Slog final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Slog, "Slog", AO::TlvTypes::Slog_25)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("direction", mTlv.field_1A_direction);
        ADD("wakeup_anger", mTlv.field_1C_wakeup_anger);
        ADD("bark_anger", mTlv.field_1E_bark_anger);
        ADD("sleeps", mTlv.field_20_sleeps);
        ADD("chase_anger", mTlv.field_22_chase_anger);
        ADD("jump_attack_delay", mTlv.field_24_jump_attack_delay);
        ADD("disabled_resources", mTlv.field_26_disabled_resources);
        ADD("anger_trigger_id", mTlv.field_28_anger_trigger_id);
    }
};

struct Path_ChimeLock final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_ChimeLock, "ChimeLock", AO::TlvTypes::ChimeLock_69)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("solve_id", mTlv.field_1A_solve_id);
        ADD("code1", mTlv.field_1C_code1);
        ADD("code2", mTlv.field_1E_code2);
        ADD("id", mTlv.field_20_id);
    }
};

struct Path_FlintLockFire final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_FlintLockFire, "FlintLockFire", AO::TlvTypes::FlintLockFire_73)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("id", mTlv.field_1A_id);
    }
};

struct Path_LiftMover final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_LiftMover, "LiftMover", AO::TlvTypes::LiftMover_68)
    {
        ADD("switch_id", mTlv.field_18_switch_id);
        ADD("lift_id", mTlv.field_1A_lift_id);
        ADD("direction", mTlv.field_1C_direction);
    }
};

struct Path_Scrab final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Scrab, "Scrab", AO::TlvTypes::Scrab_72)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("attack_delay", mTlv.field_1A_attack_delay);
        ADD("patrol_type", mTlv.field_1C_patrol_type);
        ADD("left_min_delay", mTlv.field_1E_left_min_delay);
        ADD("left_max_delay", mTlv.field_20_left_max_delay);
        ADD("right_min_delay", mTlv.field_22_right_min_delay);
        ADD("right_max_delay", mTlv.field_24_right_max_delay);
        ADD("attack_duration", mTlv.field_26_attack_duration);
        ADD("disable_resources", mTlv.field_28_disable_resources);
        ADD("roar_randomly", mTlv.field_2A_roar_randomly);
    }
};

struct Path_SlogSpawner final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SlogSpawner, "SlogSpawner", AO::TlvTypes::SlogSpawner_107)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("num_slogs", mTlv.field_1A_num_slogs);
        ADD("num_at_a_time", mTlv.field_1C_num_at_a_time);
        ADD("direction", mTlv.field_1E_direction);
        ADD("ticks_between_slogs", mTlv.field_20_ticks_between_slogs);
        ADD("start_id", mTlv.field_22_start_id);
    }
};

struct Path_RockSack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RockSack, "RockSack", AO::TlvTypes::RockSack_13)
    {
        ADD("side", mTlv.field_18_side);
        ADD("x_vel", mTlv.field_1A_x_vel);
        ADD("y_vel", mTlv.field_1C_y_vel);
        ADD("scale", mTlv.field_1E_scale);
        ADD("num_rocks", mTlv.field_20_num_rocks);
    }
};

struct Path_SlogHut final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SlogHut, "SlogHut", AO::TlvTypes::SlogHut_111)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("switch_id", mTlv.field_1A_switch_id);
        ADD("z_delay", mTlv.field_1C_z_delay);
    }
};

struct Path_SecurityClaw final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityClaw, "SecurityClaw", AO::TlvTypes::SecurityClaw_61)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("alarm_id", mTlv.field_1A_alarm_id);
        ADD("alarm_time", mTlv.field_1C_alarm_time);
        ADD("disabled_resources", mTlv.field_1E_disabled_resources);
    }
};

struct Path_SecurityDoor final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SecurityDoor, "SecurityDoor", AO::TlvTypes::SecurityDoor_95)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("id", mTlv.field_1A_id);
        ADD("code_1", mTlv.field_1C_code_1);
        ADD("code_2", mTlv.field_1E_code2);
        ADD("door_xpos", mTlv.field_20_xpos);
        ADD("door_ypos", mTlv.field_22_ypos);
    }
};

struct Path_TimedMine final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_TimedMine, "TimedMine", AO::TlvTypes::TimedMine_22)
    {
        ADD("id", mTlv.field_18_id);
        ADD("state", mTlv.field_1A_state);
        ADD("scale", mTlv.field_1C_scale);
        ADD("ticks_before_explode", mTlv.field_1E_ticks_before_explode);
        ADD("disable_resources", mTlv.field_20_disable_resources);
    }
};

struct Path_SligSpawner final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_SligSpawner::StartState>("Enum_SligSpawnerStartState",
                                                        {
                                                            {AO::Path_SligSpawner::StartState::Listening_0, "listening"},
                                                            {AO::Path_SligSpawner::StartState::Paused_1, "paused1"},
                                                            {AO::Path_SligSpawner::StartState::Sleeping_2, "sleeping"},
                                                            {AO::Path_SligSpawner::StartState::Chase_3, "chase"},
                                                            {AO::Path_SligSpawner::StartState::GameEnder_4, "game_ender"},
                                                            {AO::Path_SligSpawner::StartState::Paused_5, "paused2"},
                                                        });
    }

    CTOR_AO(Path_SligSpawner, "SligSpawner", AO::TlvTypes::SligSpawner_66)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("start_state", mTlv.field_1A_start_state);
        ADD("pause_time", mTlv.field_1C_pause_time);
        ADD("pause_left_min", mTlv.field_1E_pause_left_min);
        ADD("pause_left_max", mTlv.field_20_pause_left_max);
        ADD("pause_right_min", mTlv.field_22_pause_right_min);
        ADD("pause_right_max", mTlv.field_24_pause_right_max);
        ADD("chal_type", mTlv.field_26_chal_type);
        ADD("chal_time", mTlv.field_28_chal_time);
        ADD("number_of_times_to_shoot", mTlv.field_2A_number_of_times_to_shoot);
        ADD("unknown", mTlv.field_2C_unknown);
        ADD("code1", mTlv.field_2E_code1);
        ADD("code2", mTlv.field_30_code2);
        ADD("chase_abe", mTlv.field_32_chase_abe);
        ADD("start_direction", mTlv.field_34_start_direction);
        ADD("panic_timeout", mTlv.field_36_panic_timeout);
        ADD("num_panic_sounds", mTlv.field_38_num_panic_sounds);
        ADD("panic_sound_timeout", mTlv.field_3A_panic_sound_timeout);
        ADD("stop_chase_delay", mTlv.field_3C_stop_chase_delay);
        ADD("time_to_wait_before_chase", mTlv.field_3E_time_to_wait_before_chase);
        ADD("slig_id", mTlv.field_40_slig_id);
        ADD("listen_time", mTlv.field_42_listen_time);
        ADD("percent_say_what", mTlv.field_44_percent_say_what);
        ADD("percent_beat_mud", mTlv.field_46_percent_beat_mud);
        ADD("talk_to_abe", mTlv.field_48_talk_to_abe);
        ADD("dont_shoot", mTlv.field_4A_dont_shoot);
        ADD("z_shoot_delay", mTlv.field_4C_z_shoot_delay);
        ADD("stay_awake", mTlv.field_4E_stay_awake);
        ADD("disable_resources", mTlv.field_50_disable_resources);
        ADD("noise_wake_up_distance", mTlv.field_52_noise_wake_up_distance);
        ADD("id", mTlv.field_54_id);
    }
};

struct Path_MotionDetector final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_MotionDetector::StartMoveDirection>("Enum_MotionDetectorStartMoveDirection",
                                                                   {
                                                                       {AO::Path_MotionDetector::StartMoveDirection::eRight_0, "right"},
                                                                       {AO::Path_MotionDetector::StartMoveDirection::eLeft_1, "left"},
                                                                   });
    }

    CTOR_AO(Path_MotionDetector, "MotionDetector", AO::TlvTypes::MotionDetector_62)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("device_x", mTlv.field_1A_device_x);
        ADD("device_y", mTlv.field_1C_device_y);
        ADD("speed_x256", mTlv.field_1E_speed_x256);
        ADD("start_move_direction", mTlv.field_20_start_move_direction);
        ADD("draw_flare", mTlv.field_22_draw_flare);
        ADD("disable_id", mTlv.field_24_disable_id);
        ADD("alarm_id", mTlv.field_26_alarm_id);
        ADD("alarm_ticks", mTlv.field_28_alarm_ticks);
    }
};

struct Path_BackgroundAnimation final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::TPageAbr>("Enum_TPageAbr",
                                    {
                                        {AO::TPageAbr::eBlend_1, "blend_1"},
                                        {AO::TPageAbr::eBlend_2, "blend_2"},
                                        {AO::TPageAbr::eBlend_3, "blend_3"},
                                        {AO::TPageAbr::eBlend_0, "blend_0"},
                                    });
    }

    CTOR_AO(Path_BackgroundAnimation, "BackgroundAnimation", AO::TlvTypes::BackgroundAnimation_19)
    {
        ADD("animation_id", mTlv.field_18_animation_id);
        ADD("is_semi_trans", mTlv.field_1A_is_semi_trans);
        ADD("semi_trans_mode", mTlv.field_1C_semi_trans_mode);
        ADD("sound_effect", mTlv.field_1E_sound_effect);
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
        ADD("unload_cams_asap", mTlv.unload_cams_ASAP);
    }
};

struct Path_Pulley final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Pulley, "Pulley", AO::TlvTypes::Pulley_35)
    {
        ADD("scale", mTlv.scale);
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
        ADD("level", mTlv.level);
        ADD("path", mTlv.path);
        ADD("camera", mTlv.camera);
    }
};

struct Path_AbeStart final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_AbeStart, "AbeStart", AO::TlvTypes::AbeStart_37)
    {
        ADD("scale", mTlv.scale);
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

struct Path_LiftMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_LiftMudokon, "LiftMudokon", AO::TlvTypes::LiftMudokon_32)
    {
        ADD("how_far_to_walk", mTlv.field_18_how_far_to_walk);
        ADD("lift_id", mTlv.field_1A_lift_id);
        ADD("direction", mTlv.field_1C_direction);
        ADD("silent", mTlv.field_1E_silent);
        ADD("scale", mTlv.field_20_scale);
        ADD("code1", mTlv.field_22_code1);
        ADD("code2", mTlv.field_24_code2);
    }
};

struct Path_HoneySack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_HoneySack, "HoneySack", AO::TlvTypes::HoneySack_36)
    {
        ADD("chase_ticks", mTlv.field_18_chase_ticks);
        ADD("scale", mTlv.field_1A_scale);
    }
};

struct Path_SlingMudokon final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_SlingMudokon, "SlingMudokon", AO::TlvTypes::SlingMudokon_41)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("silent", mTlv.field_1A_bDontWhistlePassword);
        ADD("code1", mTlv.field_1C_code_1);
        ADD("code2", mTlv.field_1E_code_2);
    }
};

struct Path_BeeSwarmHole final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_BeeSwarmHole::MovementType>("Enum_BeeSwarmHoleMovementType",
                                                           {
                                                               {AO::Path_BeeSwarmHole::MovementType::eHover_0, "hover"},
                                                               {AO::Path_BeeSwarmHole::MovementType::eAttack_1, "attack"},
                                                               {AO::Path_BeeSwarmHole::MovementType::eFollowPath_2, "follow_path"},
                                                           });
    }

    CTOR_AO(Path_BeeSwarmHole, "BeeSwarmHole", AO::TlvTypes::BeeSwarmHole_34)
    {
        ADD("what_to_spawn", mTlv.field_18_what_to_spawn);
        ADD("interval", mTlv.field_1A_interval);
        ADD("id", mTlv.field_1C_id);
        ADD("movement_type", mTlv.field_1E_movement_type);
        ADD("size", mTlv.field_20_size);
        ADD("chase_time", mTlv.field_22_chase_time);
        ADD("speed", mTlv.field_24_speed);
        ADD("scale", mTlv.field_26_scale);
    }
};

struct Path_MeatSack final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_MeatSack, "MeatSack", AO::TlvTypes::MeatSack_71)
    {
        ADD("side", mTlv.field_18_side);
        ADD("x_vel", mTlv.field_1A_x_vel);
        ADD("y_vel", mTlv.field_1C_y_vel);
        ADD("scale", mTlv.field_1E_scale);
        ADD("amount_of_meat", mTlv.field_20_amount_of_meat);
    }
};

struct Path_RollingBall final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RollingBall, "RollingBall", AO::TlvTypes::RollingBall_56)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("roll_direction", mTlv.field_1A_roll_direction);
        ADD("release", mTlv.field_1C_release);
        ADD("speed", mTlv.field_1E_speed);
        ADD("acceleration", mTlv.field_20_acceleration);
    }
};

struct Path_RollingBallStopper final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_RollingBallStopper, "RollingBallStopper", AO::TlvTypes::RollingBallStopper_59)
    {
        ADD("id_on", mTlv.field_18_id_on);
        ADD("scale", mTlv.field_1A_scale);
        ADD("id_off", mTlv.field_1C_id_off);
        ADD("direction", mTlv.field_1E_direction);
    }
};

struct Path_Bees final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Bees, "Bees", AO::TlvTypes::Bees_43)
    {
        ADD("id", mTlv.id);
        ADD("swarm_size", mTlv.swarm_size);
        ADD("chase_time", mTlv.chase_time);
        ADD("speed", mTlv.speed);
        ADD("disable_resources", mTlv.disable_resources);
        ADD("num_bees", mTlv.num_bees);
    }
};

struct Path_ZBall final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::Path_ZBall::StartPos>("Enum_ZBallStartPos",
                                                {
                                                    {AO::Path_ZBall::StartPos::eCenter_0, "center"},
                                                    {AO::Path_ZBall::StartPos::eOut_1, "out"},
                                                    {AO::Path_ZBall::StartPos::eIn_2, "in"},
                                                });

        types.AddEnum<AO::Path_ZBall::Speed>("Enum_ZBallSpeed",
                                             {
                                                 {AO::Path_ZBall::Speed::eNormal_0, "normal"},
                                                 {AO::Path_ZBall::Speed::eFast_1, "fast"},
                                                 {AO::Path_ZBall::Speed::eSlow_2, "slow"},
                                             });
    }

    CTOR_AO(Path_ZBall, "ZBall", AO::TlvTypes::ZBall_14)
    {
        ADD("start_pos", mTlv.field_18_start_pos);
        ADD("scale", mTlv.field_1A_scale);
        ADD("speed", mTlv.field_1C_speed);
    }
};

struct Path_FootSwitch final : public ReliveAPI::TlvObjectBaseAO
{
    void AddTypes(ReliveAPI::TypesCollectionBase& types) override
    {
        types.AddEnum<AO::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
                                               {
                                                   {AO::FootSwitchTriggerBy::eOnlyAbe_0, "only_abe"},
                                                   {AO::FootSwitchTriggerBy::eAnyone_1, "anyone"},
                                               });
    }

    CTOR_AO(Path_FootSwitch, "FootSwitch", AO::TlvTypes::FootSwitch_60)
    {
        ADD("id", mTlv.field_18_id);
        ADD("scale", mTlv.field_1A_scale);
        ADD("action", mTlv.field_1C_action);
        ADD("triggered_by", mTlv.field_1E_trigger_by);
    }
};

struct Path_Paramite final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Paramite, "Paramite", AO::TlvTypes::Paramite_48)
    {
        ADD("scale", mTlv.field_18_scale);
        ADD("enter_from_web", mTlv.field_1A_bEnter_from_web);
        ADD("attack_delay", mTlv.field_1C_attack_delay);
        ADD("drop_in_timer", mTlv.field_1E_drop_in_timer);
        ADD("meat_eating_time", mTlv.field_20_meat_eating_time);
        ADD("attack_duration", mTlv.field_22_attack_duration);
        ADD("disabled_resources", mTlv.field_24_disabled_resources);
        ADD("id", mTlv.field_26_id);
        ADD("hiss_before_attack", mTlv.field_28_hiss_before_attack);
        ADD("delete_when_far_away", mTlv.field_2A_delete_when_far_away);
    }
};

struct Path_Honey final : public ReliveAPI::TlvObjectBaseAO
{
    CTOR_AO(Path_Honey, "Honey", AO::TlvTypes::Honey_20)
    {
        ADD("id", mTlv.id);
        ADD("state", mTlv.state);
        ADD("scale", mTlv.scale);
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
