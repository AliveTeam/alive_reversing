#pragma once

#include "TlvObjectBaseAO.hpp"
#include "TlvObjectBaseMacros.hpp"

#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../AliveLibAO/Door.hpp"
#include "../AliveLibAO/Switch.hpp"
#include "../AliveLibAO/DoorLight.hpp"
#include "../AliveLibAO/ElectricWall.hpp"
#include "../AliveLibAO/Well.hpp"
#include "../AliveLibAO/Slig.hpp"
#include "../AliveLibAO/SecurityOrb.hpp"
#include "../AliveLibAO/FallingItem.hpp"
#include "../AliveLibAO/Mine.hpp"
#include "../AliveLibAO/Dove.hpp"
#include "../AliveLibAO/UXB.hpp"
#include "../AliveLibAO/HintFly.hpp"
#include "../AliveLibAO/Bat.hpp"
#include "../AliveLibAO/ShadowZone.hpp"
#include "../AliveLibAO/BellHammer.hpp"
#include "../AliveLibAO/IdSplitter.hpp"
#include "../AliveLibAO/PullRingRope.hpp"
#include "../AliveLibAO/MusicTrigger.hpp"
#include "../AliveLibAO/Elum.hpp"
#include "../AliveLibAO/LiftPoint.hpp"
#include "../AliveLibAO/MovingBomb.hpp"
#include "../AliveLibAO/Mudokon.hpp"
#include "../AliveLibAO/MeatSaw.hpp"
#include "../AliveLibAO/LCDScreen.hpp"
#include "../AliveLibAO/InvisibleSwitch.hpp"
#include "../AliveLibAO/TrapDoor.hpp"
#include "../AliveLibAO/BirdPortal.hpp"
#include "../AliveLibAO/BoomMachine.hpp"
#include "../AliveLibAO/Slog.hpp"
#include "../AliveLibAO/ChimeLock.hpp"
#include "../AliveLibAO/FlintLockFire.hpp"
#include "../AliveLibAO/LiftMover.hpp"
#include "../AliveLibAO/Scrab.hpp"
#include "../AliveLibAO/SlogSpawner.hpp"
#include "../AliveLibAO/Rock.hpp"
#include "../AliveLibAO/SlogHut.hpp"
#include "../AliveLibAO/SecurityClaw.hpp"
#include "../AliveLibAO/SecurityDoor.hpp"
#include "../AliveLibAO/TimedMine.hpp"
#include "../AliveLibAO/MotionDetector.hpp"
#include "../AliveLibAO/BackgroundAnimation.hpp"
#include "../AliveLibAO/LCDStatusBoard.hpp"
#include "../AliveLibAO/HoneySack.hpp"
#include "../AliveLibAO/SlingMudokon.hpp"
#include "../AliveLibAO/BeeSwarmHole.hpp"
#include "../AliveLibAO/Meat.hpp"
#include "../AliveLibAO/RollingBall.hpp"
#include "../AliveLibAO/RollingBallStopper.hpp"
#include "../AliveLibAO/ZBall.hpp"
#include "../AliveLibAO/FootSwitch.hpp"
#include "../AliveLibAO/Paramite.hpp"

#define CTOR_AO(className, objectTypeName, tlvType) className() : TlvObjectBaseAO(tlvType, objectTypeName) {}  className(TypesCollectionBase& globalTypes, AO::Path_TLV* pTlv = nullptr) : TlvObjectBaseAO(globalTypes, tlvType, objectTypeName, pTlv)
#define EMPTY_CTOR_AO() (void)globalTypes

namespace AO
{
    struct Path_HoneyDripTarget : public Path_TLV
    {
        // No fields
    };

    struct Path_Honey : public Path_TLV
    {
        s16 id;
        s16 state;
        s32 scale;
    };

    struct Path_Bees : public Path_TLV
    {
        s16 id;
        s16 swarm_size;
        s16 chase_time;
        s16 speed;
        s16 disable_resources;
        s16 num_bees;
    };

    struct Path_ScrabNoFall : public Path_TLV
    {
        // No fields
    };

    struct Path_ScrabLeftBound : public Path_TLV
    {
        // No fields
    };

    struct Path_ScrabRightBound : public Path_TLV
    {
        // No fields
    };

    struct Path_ZSligCover : public Path_TLV
    {
        // No fields
    };

    struct Path_AbeStart : public Path_TLV
    {
        s32 scale;
    };

    struct Path_MudokonPathTrans : public Path_TLV
    {
        LevelIds level;
        s16 path;
        s32 camera;
    };

    struct Path_Pulley : public Path_TLV
    {
        s32 scale;
    };

    struct Path_Preloader : public Path_TLV
    {
        s32 unload_cams_ASAP;
    };

    struct Path_SligSpawner : public Path_TLV
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

    struct Path_ContinueZone : public Path_TLV
    {
        s32 field_10_zone_number;
    };

    struct Path_StartController : public Path_TLV
    {
        // No fields
    };

    struct Path_InvisibleZone : public Path_TLV
    {
        // No fields
    };

    struct Path_DeathDrop : public Path_TLV
    {
        s16 animation;
        s16 sound;
        s16 id;
        s16 action;
        s32 set_value;
    };

    struct Path_ElumStart : public Path_TLV
    {
        // No fields
    };

    struct Path_ElumWall : public Path_TLV
    {
        // No fields
    };

    struct Path_HandStone : public Path_TLV
    {
        Path_Handstone_data mData;
    };

    struct Path_BellsongStone : public Path_TLV
    {
        Path_BellsongStone_data mData;
    };

    struct Path_MovieStone : public Path_TLV
    {
        Path_Moviestone_data mData;
    };
}

namespace AOTlvs
{
    struct Path_MovieStone : public TlvObjectBaseAO<AO::Path_MovieStone>
    {
        CTOR_AO(Path_MovieStone, "MovieStone", AO::TlvTypes::MovieStone_51)
        {
            ADD("fmv_id", tlv().mData.fmvId);
            ADD("scale", tlv().mData.scale);
        }
    };

    struct Path_BellsongStone : public TlvObjectBaseAO<AO::Path_BellsongStone>
    {
        void AddTypes(TypesCollectionBase& types) override
        {
            types.AddEnum<AO::BellsongTypes>("Enum_BellsongTypes",
                {
                    {AO::BellsongTypes::eWhistle, "whistle"},
                    {AO::BellsongTypes::eChimes, "chimes"},
                });
        }

        CTOR_AO(Path_BellsongStone, "BellSongStone", AO::TlvTypes::BellSongStone_54)
        {
            ADD("scale", tlv().mData.scale);
            ADD("type", tlv().mData.type);
            ADD("code1", tlv().mData.code1);
            ADD("code2", tlv().mData.code2);
            ADD("id", tlv().mData.id);
        }
    };

    struct Path_HandStone : public TlvObjectBaseAO<AO::Path_HandStone>
    {
        CTOR_AO(Path_HandStone, "HandStone", AO::TlvTypes::HandStone_100)
        {
            ADD("scale", tlv().mData.scale);

            ADD("camera1_level", tlv().mData.camera1.level);
            ADD("camera1_path", tlv().mData.camera1.path);
            ADD("camera1_camera", tlv().mData.camera1.camera);

            ADD("camera2_level", tlv().mData.camera2.level);
            ADD("camera2_path", tlv().mData.camera2.path);
            ADD("camera2_camera", tlv().mData.camera2.camera);

            ADD("camera3_level", tlv().mData.camera3.level);
            ADD("camera3_path", tlv().mData.camera3.path);
            ADD("camera3_camera", tlv().mData.camera3.camera);
        }
    };

    struct Path_Door : public TlvObjectBaseAO<AO::Path_Door>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("Level", tlv().field_18_level);
            ADD("Path", tlv().field_1A_path);
            ADD("Camera", tlv().field_1C_camera);
            ADD("Scale", tlv().field_1E_scale);
            ADD("DoorNumber", tlv().field_20_door_number);
            ADD("Id", tlv().field_22_id);
            ADD("TargetDoorNumber", tlv().field_24_target_door_number);
            ADD("StartState", tlv().field_26_start_state);
            ADD("DoorClosed", tlv().field_28_door_closed);
            ADD("Hub1", tlv().field_2A_hub1);
            ADD("Hub2", tlv().field_2A_hub2);
            ADD("Hub3", tlv().field_2A_hub3);
            ADD("Hub4", tlv().field_2A_hub4);
            ADD("Hub5", tlv().field_2A_hub5);
            ADD("Hub6", tlv().field_2A_hub6);
            ADD("Hub7", tlv().field_2A_hub7);
            ADD("Hub8", tlv().field_2A_hub8);
            ADD("WipeEffect", tlv().field_3A_wipe_effect);
            ADD("MovieNumber", tlv().field_3C_movie_number);
            ADD("XOffset", tlv().field_3E_x_offset);
            ADD("YOffset", tlv().field_40_y_offset);
            ADD("WipeXOrg", tlv().field_42_wipe_x_org);
            ADD("WipeYOrg", tlv().field_44_wipe_y_org);
            ADD("AbeDirection", tlv().field_46_abe_direction);
        }
    };

    struct Path_ContinuePoint : public TlvObjectBaseAO<AO::Path_ContinuePoint>
    {
        CTOR_AO(Path_ContinuePoint, "ContinuePoint",  AO::TlvTypes::ContinuePoint_0)
        {
            ADD("ZoneNumber", tlv().field_18_zone_number);
            ADD("ClearFromId", tlv().field_1A_clear_from_id);
            ADD("ClearToId", tlv().field_1C_clear_to_id);
            ADD("ElumRestarts", tlv().field_1E_elum_restarts);
            ADD("AbeSpawnDirection", tlv().field_20_abe_direction);
        }
    };

    struct Path_Hoist : public TlvObjectBaseAO<AO::Path_Hoist>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("HoistType", tlv().field_18_hoist_type);
            ADD("HoistEdgeType", tlv().field_1A_edge_type);
            ADD("Id", tlv().field_1C_id);
        }
    };

    struct Path_Change : public TlvObjectBaseAO<AO::Path_ChangeTLV>
    {
        CTOR_AO(Path_Change, "PathTransition", AO::TlvTypes::PathTransition_1)
        {
            ADD("Level", tlv().field_18_level);
            ADD("HoistEdgeType", tlv().field_1A_path);
            ADD("Camera", tlv().field_1C_camera);
            ADD("Movie", tlv().field_1E_movie); // TODO: Enum
            ADD("Wipe", tlv().field_20_wipe); // TODO: Enum
            ADD("Scale", tlv().field_22_scale); // TODO: Enum
        }
    };

    struct Path_Switch : public TlvObjectBaseAO<AO::Path_Switch>
    {
        CTOR_AO(Path_Switch, "Switch", AO::TlvTypes::Switch_26)
        {
            ADD("TriggerObject", tlv().field_18_trigger_object);
            ADD("TriggerAction", tlv().field_1A_trigger_object_action);
            ADD("Scale", tlv().field_1C_scale);
            ADD("OnSound", tlv().field_1E_on_sound); // TODO: Enum
            ADD("OffSound", tlv().field_20_off_sound); // TODO: Enum
            ADD("SoundDirection", tlv().field_22_sound_direction); // TODO: Enum
        }
    };

    struct Path_LightEffect : public TlvObjectBaseAO<AO::Path_LightEffect>
    {
        void AddTypes(TypesCollectionBase& types) override
        {
            types.AddEnum<AO::Path_LightEffect::Type>("Enum_LightType",
                {
                    {AO::Path_LightEffect::Type::Star_0, "Star"},
                    {AO::Path_LightEffect::Type::RedGlow_1, "RedGlow"},
                    {AO::Path_LightEffect::Type::GreenGlow_2, "GreenGlow"},
                    {AO::Path_LightEffect::Type::FlintGlow_3, "FlintGlow"},
                    {AO::Path_LightEffect::Type::Switchable_RedGreenDoorLights_4, "RedGreenDoorLight"},
                    {AO::Path_LightEffect::Type::Switchable_RedGreenHubLight_5, "RedGreenHubLight"},
                });
        }

        CTOR_AO(Path_LightEffect, "LightEffect", AO::TlvTypes::LightEffect_106)
        {
            ADD("Type", tlv().field_18_type);
            ADD("Size", tlv().field_1A_size);
            ADD("Id", tlv().field_1C_id);
            ADD("FlipX", tlv().field_1E_flip_x);
        }
    };

    struct Path_ElectricWall : public TlvObjectBaseAO<AO::Path_ElectricWall>
    {
        CTOR_AO(Path_ElectricWall, "ElectricWall", AO::TlvTypes::ElectricWall_67)
        {
            ADD("Scale", tlv().field_18_scale);
            ADD("Id", tlv().field_1A_id);
            ADD("State", tlv().field_1C_start_state); // TODO: Enum
        }
    };

    struct Path_ContinueZone : public TlvObjectBaseAO<AO::Path_ContinueZone>
    {
        CTOR_AO(Path_ContinueZone, "ContinueZone", AO::TlvTypes::ContinueZone_2)
        {
            ADD("ZoneNumber", tlv().field_10_zone_number);
        }
    };

    struct Path_StartController : public TlvObjectBaseAO<AO::Path_StartController>
    {
        CTOR_AO(Path_StartController, "StartController", AO::TlvTypes::StartController_28)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_Edge : public TlvObjectBaseAO<AO::Path_Edge>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("type", tlv().field_18_type);
            ADD("can_grab", tlv().field_1A_can_grab);
        }
    };

    struct Path_WellLocal : public TlvObjectBaseAO<AO::Path_WellLocal>
    {
        CTOR_AO(Path_WellLocal, "WellLocal", AO::TlvTypes::WellLocal_11)
        {
            // Well base
            ADD("scale", tlv().field_18_scale);
            ADD("trigger_id", tlv().field_1A_trigger_id);
            ADD("well_id", tlv().field_1C_well_id);
            ADD("resource_id", tlv().field_1E_res_id);
            ADD("exit_x", tlv().field_20_exit_x);
            ADD("exit_y", tlv().field_22_exit_y);
            ADD("dx", tlv().field_24_off_level_or_dx.dx);
            ADD("dy", tlv().field_26_off_path_or_dy);

            // Well local
            ADD("on_dx", tlv().field_28_on_dx);
            ADD("on_dy", tlv().field_2A_on_dy);
            ADD("emit_leaves", tlv().field_2C_bEmit_leaves);
            ADD("leaf_x", tlv().field_2E_leaf_x);
            ADD("leaf_y", tlv().field_30_leaf_y);
        }
    };

    struct Path_WellExpress : public TlvObjectBaseAO<AO::Path_WellExpress>
    {
        CTOR_AO(Path_WellExpress, "WellExpress", AO::TlvTypes::WellExpress_45)
        {
            // Well base
            ADD("scale", tlv().field_18_scale);
            ADD("trigger_id", tlv().field_1A_trigger_id);
            ADD("well_id", tlv().field_1C_well_id);
            ADD("resource_id", tlv().field_1E_res_id);
            ADD("exit_x", tlv().field_20_exit_x);
            ADD("exit_y", tlv().field_22_exit_y);
            ADD("off_level", tlv().field_24_off_level_or_dx.level);
            ADD("off_path", tlv().field_26_off_path_or_dy);

            // Well express
            ADD("off_camera", tlv().field_28_off_camera);
            ADD("off_well_id", tlv().field_2A_off_well_id);
            ADD("on_level", tlv().field_2C_on_level);
            ADD("on_path", tlv().field_2E_on_path);
            ADD("on_camera", tlv().field_30_on_camera);
            ADD("on_well_id", tlv().field_32_on_well_id);
            ADD("emit_leaves", tlv().field_34_emit_leaves);
            ADD("leaf_x", tlv().field_36_leaf_x);
            ADD("leaf_y", tlv().field_38_leaf_y);
            ADD("movie_id", tlv().field_3A_movie_id);
        }
    };

    struct Path_InvisibleZone : public TlvObjectBaseAO<AO::Path_InvisibleZone>
    {
        CTOR_AO(Path_InvisibleZone, "InvisibleZone", AO::TlvTypes::InvisibleZone_58)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_EnemyStopper : public TlvObjectBaseAO<AO::Path_EnemyStopper>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("direction", tlv().field_18_direction);
            ADD("id", tlv().field_1A_id);
        }
    };

    struct Path_Slig : public TlvObjectBaseAO<AO::Path_Slig>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("scale", tlv().field_18_scale);
            ADD("start_state", tlv().field_1A_start_state);
            ADD("pause_time", tlv().field_1C_pause_time);
            ADD("pause_left_min", tlv().field_1E_pause_left_min);
            ADD("pause_left_max", tlv().field_20_pause_left_max);
            ADD("pause_right_min", tlv().field_22_pause_right_min);
            ADD("pause_right_max", tlv().field_24_pause_right_max);
            ADD("chal_type", tlv().field_26_chal_type);
            ADD("chal_time", tlv().field_28_chal_time);
            ADD("number_of_times_to_shoot", tlv().field_2A_number_of_times_to_shoot);
            ADD("unknown", tlv().field_2C_unknown);
            ADD("code1", tlv().field_2E_code1);
            ADD("code2", tlv().field_30_code2);
            ADD("chase_abe", tlv().field_32_chase_abe);
            ADD("start_direction", tlv().field_34_start_direction);
            ADD("panic_timeout", tlv().field_36_panic_timeout);
            ADD("num_panic_sounds", tlv().field_38_num_panic_sounds);
            ADD("panic_sound_timeout", tlv().field_3A_panic_sound_timeout);
            ADD("stop_chase_delay", tlv().field_3C_stop_chase_delay);
            ADD("time_to_wait_before_chase", tlv().field_3E_time_to_wait_before_chase);
            ADD("slig_id", tlv().field_40_slig_id);
            ADD("listen_time", tlv().field_42_listen_time);
            ADD("percent_say_what", tlv().field_44_percent_say_what);
            ADD("percent_beat_mud", tlv().field_46_percent_beat_mud);
            ADD("talk_to_abe", tlv().field_48_talk_to_abe);
            ADD("dont_shoot", tlv().field_4A_dont_shoot);
            ADD("z_shoot_delay", tlv().field_4C_z_shoot_delay);
            ADD("stay_awake", tlv().field_4E_stay_awake);
            ADD("disable_resources", tlv().field_50_disable_resources.Raw().all);
            ADD("noise_wake_up_distance", tlv().field_52_noise_wake_up_distance);
            ADD("id", tlv().field_54_id);
        }
    };

    struct Path_DeathDrop : public TlvObjectBaseAO<AO::Path_DeathDrop>
    {
        CTOR_AO(Path_DeathDrop, "DeathDrop", AO::TlvTypes::DeathDrop_5)
        {
            ADD_HIDDEN("animation", tlv().animation);
            ADD_HIDDEN("sound", tlv().sound);
            ADD_HIDDEN("id", tlv().id);
            ADD_HIDDEN("action", tlv().action);
            ADD_HIDDEN("set_value", tlv().set_value);
        }
    };

    struct Path_SligLeftBound : public TlvObjectBaseAO<AO::Path_SligLeftBound>
    {
        CTOR_AO(Path_SligLeftBound, "SligLeftBound", AO::TlvTypes::eSligBoundLeft_57)
        {
            ADD("id", tlv().field_18_slig_id);
            ADD("disabled_resources", tlv().field_1A_disabled_resources.Raw().all);
        }
    };

    struct Path_SligRightBound : public TlvObjectBaseAO<AO::Path_SligRightBound>
    {
        CTOR_AO(Path_SligRightBound, "SligRightBound", AO::TlvTypes::eSligBoundRight_76)
        {
            ADD("id", tlv().field_18_slig_id);
            ADD("disabled_resources", tlv().field_1A_disabled_resources.Raw().all);
        }
    };

    struct Path_SligPersist : public TlvObjectBaseAO<AO::Path_SligPersist>
    {
        CTOR_AO(Path_SligPersist, "SligPersist", AO::TlvTypes::eSligPersist_77)
        {
            ADD("id", tlv().field_18_slig_id);
            ADD("disabled_resources", tlv().field_1A_disabled_resources.Raw().all);
        }
    };

    struct Path_SecurityOrb : public TlvObjectBaseAO<AO::Path_SecurityOrb>
    {
        CTOR_AO(Path_SecurityOrb, "SecurityOrb", AO::TlvTypes::SecurityOrb_29)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("disabled_resources", tlv().field_1A_disable_resources);
        }
    };

    struct Path_FallingItem : public TlvObjectBaseAO<AO::Path_FallingItem>
    {
        CTOR_AO(Path_FallingItem, "FallingItem", AO::TlvTypes::FallingItem_15)
        {
            ADD("id", tlv().field_18_id);
            ADD("scale", tlv().field_1A_scale);
            ADD("delay_time", tlv().field_1C_delay_time);
            ADD("number_of_items", tlv().field_1E_number_of_items);
            ADD("reset_id", tlv().field_20_reset_id);
        }
    };

    struct Path_Mine : public TlvObjectBaseAO<AO::Path_Mine>
    {
        CTOR_AO(Path_Mine, "Mine", AO::TlvTypes::Mine_46)
        {
            ADD("num_patterns", tlv().field_18_num_patterns);
            ADD("pattern", tlv().field_1A_pattern);
            ADD("scale", tlv().field_1C_scale);
            ADD("disabled_resources", tlv().field_1E_disabled_resources);
            ADD("persists_offscreen", tlv().field_20_persists_offscreen);
        }
    };

    struct Path_Dove : public TlvObjectBaseAO<AO::Path_Dove>
    {
        CTOR_AO(Path_Dove, "Dove", AO::TlvTypes::Dove_12)
        {
            ADD("dove_count", tlv().field_18_dove_count);
            ADD("pixel_perfect", tlv().field_1A_pixel_perfect);
            ADD("scale", tlv().field_1C_scale);
        }
    };

    struct Path_UXB : public TlvObjectBaseAO<AO::Path_UXB>
    {
        void AddTypes(TypesCollectionBase& types) override
        {
            types.AddEnum<AO::UXB_State>("UXB_State",
                {
                    {AO::UXB_State::eArmed_0, "Armed"},
                    {AO::UXB_State::eDisarmed_1, "Disarmed"},
                });
        }

        CTOR_AO(Path_UXB, "UXB", AO::TlvTypes::UXB_47)
        {
            ADD("num_patterns", tlv().field_18_num_patterns);
            ADD("pattern", tlv().field_1A_pattern);
            ADD("scale", tlv().field_1C_scale);
            ADD("state", tlv().field_1E_state);
            ADD("disabled_resources", tlv().field_20_disabled_resources);
        }
    };

    struct Path_HintFly : public TlvObjectBaseAO<AO::Path_HintFly>
    {
        CTOR_AO(Path_HintFly, "HintFly", AO::TlvTypes::HintFly_92)
        {
            ADD("message_id", tlv().field_18_message_id);
        }
    };

    struct Path_Bat : public TlvObjectBaseAO<AO::Path_Bat>
    {
        CTOR_AO(Path_Bat, "Bat", AO::TlvTypes::Bat_49)
        {
            ADD("ticks_before_moving", tlv().field_18_ticks_before_moving);
            ADD("speed", tlv().field_1A_speed);
            ADD("scale", tlv().field_1C_scale);
            ADD("attack_duration", tlv().field_1E_attack_duration);
        }
    };

    struct Path_ShadowZone : public TlvObjectBaseAO<AO::Path_ShadowZone>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("centre_w", tlv().field_18_centre_w);
            ADD("centre_h", tlv().field_1A_centre_h);
            ADD("r", tlv().field_1C_r);
            ADD("g", tlv().field_1E_g);
            ADD("b", tlv().field_20_b);
            ADD("id", tlv().field_22_id);
            ADD("scale", tlv().field_24_scale);
        }
    };

    struct Path_BellHammer : public TlvObjectBaseAO<AO::Path_BellHammer>
    {
        CTOR_AO(Path_BellHammer, "BellHammer", AO::TlvTypes::BellHammer_27)
        {
            ADD("id", tlv().field_18_id);
            ADD("action", tlv().field_1A_action);
            ADD("scale", tlv().field_1C_scale);
            ADD("direction", tlv().field_1E_direction);
        }
    };

    struct Path_IdSplitter : public TlvObjectBaseAO<AO::Path_IdSplitter>
    {
        CTOR_AO(Path_IdSplitter, "IdSplitter", AO::TlvTypes::IdSplitter_94)
        {
            ADD("source_id", tlv().field_18_source_id);
            ADD("delay", tlv().field_1A_delay);
            ADD("id_1", tlv().field_1C_id1);
            ADD("id_2", tlv().field_1C_id2);
            ADD("id_3", tlv().field_1C_id3);
            ADD("id_4", tlv().field_1C_id4);
        }
    };

    struct Path_PullRingRope : public TlvObjectBaseAO<AO::Path_PullRingRope>
    {
        CTOR_AO(Path_PullRingRope, "PullRingRope", AO::TlvTypes::PullRingRope_18)
        {
            ADD("id", tlv().field_18_id);
            ADD("action", tlv().field_1A_action);
            ADD("rope_length", tlv().field_1C_rope_length);
            ADD("scale", tlv().field_1E_scale);
            ADD("on_sound", tlv().field_20_on_sound);
            ADD("off_sound", tlv().field_22_off_sound);
            ADD("sound_direction", tlv().field_24_sound_direction);
        }
    };

    struct Path_MusicTrigger : public TlvObjectBaseAO<AO::Path_MusicTrigger>
    {
        CTOR_AO(Path_MusicTrigger, "MusicTrigger", AO::TlvTypes::MusicTrigger_105)
        {
            ADD("type", tlv().field_18_type);
            ADD("enabled_by", tlv().field_1A_enabled_by);
            ADD("id", tlv().field_1C_id);
            ADD("timer", tlv().field_1E_timer);
        }
    };

    struct Path_ElumPathTrans : public TlvObjectBaseAO<AO::Path_ElumPathTrans>
    {
        CTOR_AO(Path_ElumPathTrans, "ElumPathTrans", AO::TlvTypes::ElumPathTrans_99)
        {
            ADD("level", tlv().field_18_level);
            ADD("path", tlv().field_1A_path);
            ADD("camera", tlv().field_1C_camera);
        }
    };

    struct Path_ElumStart : public TlvObjectBaseAO<AO::Path_ElumStart>
    {
        CTOR_AO(Path_ElumStart, "ElumStart", AO::TlvTypes::ElumStart_38)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_ElumWall : public TlvObjectBaseAO<AO::Path_ElumWall>
    {
        CTOR_AO(Path_ElumWall, "ElumWall", AO::TlvTypes::ElumWall_40)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_LiftPoint : public TlvObjectBaseAO<AO::Path_LiftPoint>
    {
        CTOR_AO(Path_LiftPoint, "LiftPoint", AO::TlvTypes::LiftPoint_8)
        {
            ADD("id", tlv().field_18_id);
            ADD("is_start_point", tlv().field_1A_bstart_point);
            ADD("lift_type", tlv().field_1C_lift_type);
            ADD("lift_point_stop_type", tlv().field_1E_lift_point_stop_type);
            ADD("scale", tlv().field_20_scale);
            ADD("ignore_lift_mover", tlv().field_22_bIgnore_lift_mover);
        }
    };

    struct Path_MovingBomb : public TlvObjectBaseAO<AO::Path_MovingBomb>
    {
        CTOR_AO(Path_MovingBomb, "MovingBomb", AO::TlvTypes::MovingBomb_86)
        {
            ADD("speed", tlv().field_18_speed);
            ADD("id", tlv().field_1A_id);
            ADD("start_type_triggered_by_alarm", tlv().field_1C_bStart_type_triggered_by_alarm);
            ADD("scale", tlv().field_1E_scale);
            ADD("max_rise", tlv().field_20_max_rise);
            ADD("disabled_resources", tlv().field_22_disabled_resources);
            ADD("start_speed", tlv().field_24_start_speed);
            ADD("persist_offscreen", tlv().field_26_persist_offscreen);
        }
    };

    struct Path_MovingBombStopper : public TlvObjectBaseAO<AO::Path_MovingBombStopper>
    {
        CTOR_AO(Path_MovingBombStopper, "MovingBombStopper", AO::TlvTypes::MovingBombStopper_87)
        {
            ADD("min_delay", tlv().field_18_min_delay);
            ADD("max_delay", tlv().field_1A_max_delay);
        }
    };

    struct Path_RingMudokon : public TlvObjectBaseAO<AO::Path_RingMudokon>
    {
        CTOR_AO(Path_RingMudokon, "RingMudokon", AO::TlvTypes::RingMudokon_50)
        {
            ADD("facing", tlv().field_18_facing);
            ADD("abe_must_be_same_direction", tlv().field_1A_abe_must_be_same_direction);
            ADD("scale", tlv().field_1C_scale);
            ADD("silent", tlv().field_1E_silent);
            ADD("code1", tlv().field_20_code1);
            ADD("code2", tlv().field_22_code2);
            ADD("action", tlv().field_24_action);
            ADD("ring_timeout", tlv().field_26_ring_timeout);
            ADD("instant_powerup", tlv().field_28_instant_powerup);
        }
    };

    struct Path_RingCancel : public TlvObjectBaseAO<AO::Path_RingCancel> // TODO: correct size is 24 not 28
    {
        CTOR_AO(Path_RingCancel, "RingCancel", AO::TlvTypes::RingCancel_109)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_MeatSaw : public TlvObjectBaseAO<AO::Path_MeatSaw>
    {
        CTOR_AO(Path_MeatSaw, "MeatSaw", AO::TlvTypes::MeatSaw_88)
        {
            ADD("scale", tlv().field_18_scale_background);
            ADD("min_time_off1", tlv().field_1A_min_time_off1);
            ADD("max_time_off1", tlv().field_1C_max_time_off1);
            ADD("max_rise_time", tlv().field_1E_max_rise_time);
            ADD("id", tlv().field_20_id);
            ADD("type", tlv().field_22_type);
            ADD("speed", tlv().field_24_speed);
            ADD("start_state", tlv().field_26_start_state);
            ADD("off_speed", tlv().field_28_off_speed);
            ADD("min_time_off2", tlv().field_2A_min_time_off2);
            ADD("max_time_off2", tlv().field_2C_max_time_off2);
            ADD("initial_position", tlv().field_2E_inital_position);
        }
    };

    struct Path_LCDScreen : public TlvObjectBaseAO<AO::Path_LCDScreen>
    {
        CTOR_AO(Path_LCDScreen, "LCDScreen", AO::TlvTypes::LCDScreen_98)
        {
            ADD("message_1_id", tlv().field_18_message_1_id);
            ADD("message_rand_min", tlv().field_1A_message_rand_min);
            ADD("message_rand_max", tlv().field_1C_message_rand_max);
        }
    };

    struct Path_InvisibleSwitch : public TlvObjectBaseAO<AO::Path_InvisibleSwitch>
    {
        CTOR_AO(Path_InvisibleSwitch, "InvisibleSwitch", AO::TlvTypes::InvisibleSwitch_81)
        {
            ADD("id", tlv().field_18_id);
            ADD("action", tlv().field_1A_action);
            ADD("delay", tlv().field_1C_delay);
            ADD("set_off_alarm", tlv().field_1E_set_off_alarm);
            ADD("scale", tlv().field_20_scale);
        }
    };

    struct Path_TrapDoor : public TlvObjectBaseAO<AO::Path_TrapDoor>
    {
        CTOR_AO(Path_TrapDoor, "TrapDoor", AO::TlvTypes::TrapDoor_55)
        {
            ADD("id", tlv().field_18_id);
            ADD("start_state", tlv().field_1A_start_state);
            ADD("self_closing", tlv().field_1C_self_closing);
            ADD("scale", tlv().field_1E_scale);
            ADD("dest_level", tlv().field_20_dest_level);
            ADD("direction", tlv().field_22_direction);
            ADD("anim_offset", tlv().field_24_anim_offset);
        }
    };

    struct Path_BirdPortal : public TlvObjectBaseAO<AO::Path_BirdPortal>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("side", tlv().field_18_side);
            ADD("dest_level", tlv().field_1A_dest_level);
            ADD("dest_path", tlv().field_1C_dest_path);
            ADD("dest_camera", tlv().field_1E_dest_camera);
            ADD("scale", tlv().field_20_scale);
            ADD("movie_id", tlv().field_22_movie_id);
            ADD("portal_type", tlv().field_24_portal_type);
            ADD("num_muds_for_shrykull", tlv().field_26_num_muds_for_shrykul);
        }
    };

    struct Path_BoomMachine : public TlvObjectBaseAO<AO::Path_BoomMachine>
    {
        CTOR_AO(Path_BoomMachine, "BoomMachine", AO::TlvTypes::BoomMachine_97)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("nozzle_side", tlv().field_1A_nozzle_side);
            ADD("disabled_resources", tlv().field_1C_disabled_resources);
            ADD("number_of_grenades", tlv().field_1E_number_of_grenades);
        }
    };

    struct Path_Mudokon : public TlvObjectBaseAO<AO::Path_Mudokon>
    {
        CTOR_AO(Path_Mudokon, "Mudokon", AO::TlvTypes::Mudokon_82)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("job", tlv().field_1A_job);
            ADD("direction", tlv().field_1C_direction);
            ADD("voice_adjust", tlv().field_1E_voice_adjust);
            ADD("rescue_id", tlv().field_20_rescue_id);
            ADD("deaf", tlv().field_22_deaf);
            ADD("disabled_resources", tlv().field_24_disabled_resources);
            ADD("persist", tlv().field_26_persist);
        }
    };

    struct Path_BirdPortalExit : public TlvObjectBaseAO<AO::Path_BirdPortalExit>
    {
        CTOR_AO(Path_BirdPortalExit, "BirdPortalExit", AO::TlvTypes::BirdPortalExit_53)
        {
            ADD("side", tlv().field_18_side);
            ADD("scale", tlv().field_1A_scale);
        }
    };

    struct Path_Slog : public TlvObjectBaseAO<AO::Path_Slog>
    {
        CTOR_AO(Path_Slog, "Slog", AO::TlvTypes::Slog_25)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("direction", tlv().field_1A_direction);
            ADD("wakeup_anger", tlv().field_1C_wakeup_anger);
            ADD("bark_anger", tlv().field_1E_bark_anger);
            ADD("sleeps", tlv().field_20_sleeps);
            ADD("chase_anger", tlv().field_22_chase_anger);
            ADD("jump_attack_delay", tlv().field_24_jump_attack_delay);
            ADD("disabled_resources", tlv().field_26_disabled_resources);
            ADD("anger_trigger_id", tlv().field_28_anger_trigger_id);
        }
    };

    struct Path_ChimeLock : public TlvObjectBaseAO<AO::Path_ChimeLock>
    {
        CTOR_AO(Path_ChimeLock, "ChimeLock", AO::TlvTypes::ChimeLock_69)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("solve_id", tlv().field_1A_solve_id);
            ADD("code1", tlv().field_1C_code1);
            ADD("code2", tlv().field_1E_code2);
            ADD("id", tlv().field_20_id);
        }
    };

    struct Path_FlintLockFire : public TlvObjectBaseAO<AO::Path_FlintLockFire>
    {
        CTOR_AO(Path_FlintLockFire, "FlintLockFire", AO::TlvTypes::FlintLockFire_73)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("id", tlv().field_1A_id);
        }
    };

    struct Path_LiftMover : public TlvObjectBaseAO<AO::Path_LiftMover>
    {
        CTOR_AO(Path_LiftMover, "LiftMover", AO::TlvTypes::LiftMover_68)
        {
            ADD("switch_id", tlv().field_18_switch_id);
            ADD("lift_id", tlv().field_1A_lift_id);
            ADD("direction", tlv().field_1C_direction);
        }
    };

    struct Path_Scrab : public TlvObjectBaseAO<AO::Path_Scrab>
    {
        CTOR_AO(Path_Scrab, "Scrab", AO::TlvTypes::Scrab_72)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("attack_delay", tlv().field_1A_attack_delay);
            ADD("patrol_type", tlv().field_1C_patrol_type);
            ADD("left_min_delay", tlv().field_1E_left_min_delay);
            ADD("left_max_delay", tlv().field_20_left_max_delay);
            ADD("right_min_delay", tlv().field_22_right_min_delay);
            ADD("right_max_delay", tlv().field_24_right_max_delay);
            ADD("attack_duration", tlv().field_26_attack_duration);
            ADD("disable_resources", tlv().field_28_disable_resources);
            ADD("roar_randomly", tlv().field_2A_roar_randomly);
        }
    };

    struct Path_SlogSpawner : public TlvObjectBaseAO<AO::Path_SlogSpawner>
    {
        CTOR_AO(Path_SlogSpawner, "SlogSpawner", AO::TlvTypes::SlogSpawner_107)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("num_slogs", tlv().field_1A_num_slogs);
            ADD("num_at_a_time", tlv().field_1C_num_at_a_time);
            ADD("direction", tlv().field_1E_direction);
            ADD("ticks_between_slogs", tlv().field_20_ticks_between_slogs);
            ADD("start_id", tlv().field_22_start_id);
        }
    };

    struct Path_RockSack : public TlvObjectBaseAO<AO::Path_RockSack>
    {
        CTOR_AO(Path_RockSack, "RockSack", AO::TlvTypes::RockSack_13)
        {
            ADD("side", tlv().field_18_side);
            ADD("x_vel", tlv().field_1A_x_vel);
            ADD("y_vel", tlv().field_1C_y_vel);
            ADD("scale", tlv().field_1E_scale);
            ADD("num_rocks", tlv().field_20_num_rocks);
        }
    };

    struct Path_SlogHut : public TlvObjectBaseAO<AO::Path_SlogHut>
    {
        CTOR_AO(Path_SlogHut, "SlogHut", AO::TlvTypes::SlogHut_111)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("switch_id", tlv().field_1A_switch_id);
            ADD("z_delay", tlv().field_1C_z_delay);
        }
    };

    struct Path_SecurityClaw : public TlvObjectBaseAO<AO::Path_SecurityClaw>
    {
        CTOR_AO(Path_SecurityClaw, "SecurityClaw", AO::TlvTypes::SecurityClaw_61)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("alarm_id", tlv().field_1A_alarm_id);
            ADD("alarm_time", tlv().field_1C_alarm_time);
            ADD("disabled_resources", tlv().field_1E_disabled_resources);
        }
    };

    struct Path_SecurityDoor : public TlvObjectBaseAO<AO::Path_SecurityDoor>
    {
        CTOR_AO(Path_SecurityDoor, "SecurityDoor", AO::TlvTypes::SecurityDoor_95)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("id", tlv().field_1A_id);
            ADD("code_1", tlv().field_1C_code_1);
            ADD("code_2", tlv().field_1E_code2);
            ADD("door_xpos", tlv().field_20_xpos);
            ADD("door_ypos", tlv().field_22_ypos);
        }
    };

    struct Path_TimedMine : public TlvObjectBaseAO<AO::Path_TimedMine>
    {
        CTOR_AO(Path_TimedMine, "TimedMine", AO::TlvTypes::TimedMine_22)
        {
            ADD("id", tlv().field_18_id);
            ADD("state", tlv().field_1A_state);
            ADD("scale", tlv().field_1C_scale);
            ADD("ticks_before_explode", tlv().field_1E_ticks_before_explode);
            ADD("disable_resources", tlv().field_20_disable_resources);
        }
    };

    struct Path_SligSpawner : public TlvObjectBaseAO<AO::Path_SligSpawner>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("scale", tlv().field_18_scale);
            ADD("start_state", tlv().field_1A_start_state);
            ADD("pause_time", tlv().field_1C_pause_time);
            ADD("pause_left_min", tlv().field_1E_pause_left_min);
            ADD("pause_left_max", tlv().field_20_pause_left_max);
            ADD("pause_right_min", tlv().field_22_pause_right_min);
            ADD("pause_right_max", tlv().field_24_pause_right_max);
            ADD("chal_type", tlv().field_26_chal_type);
            ADD("chal_time", tlv().field_28_chal_time);
            ADD("number_of_times_to_shoot", tlv().field_2A_number_of_times_to_shoot);
            ADD("unknown", tlv().field_2C_unknown);
            ADD("code1", tlv().field_2E_code1);
            ADD("code2", tlv().field_30_code2);
            ADD("chase_abe", tlv().field_32_chase_abe);
            ADD("start_direction", tlv().field_34_start_direction);
            ADD("panic_timeout", tlv().field_36_panic_timeout);
            ADD("num_panic_sounds", tlv().field_38_num_panic_sounds);
            ADD("panic_sound_timeout", tlv().field_3A_panic_sound_timeout);
            ADD("stop_chase_delay", tlv().field_3C_stop_chase_delay);
            ADD("time_to_wait_before_chase", tlv().field_3E_time_to_wait_before_chase);
            ADD("slig_id", tlv().field_40_slig_id);
            ADD("listen_time", tlv().field_42_listen_time);
            ADD("percent_say_what", tlv().field_44_percent_say_what);
            ADD("percent_beat_mud", tlv().field_46_percent_beat_mud);
            ADD("talk_to_abe", tlv().field_48_talk_to_abe);
            ADD("dont_shoot", tlv().field_4A_dont_shoot);
            ADD("z_shoot_delay", tlv().field_4C_z_shoot_delay);
            ADD("stay_awake", tlv().field_4E_stay_awake);
            ADD("disable_resources", tlv().field_50_disable_resources);
            ADD("noise_wake_up_distance", tlv().field_52_noise_wake_up_distance);
            ADD("id", tlv().field_54_id);
        }
    };

    struct Path_MotionDetector : public TlvObjectBaseAO<AO::Path_MotionDetector>
    {
        void AddTypes(TypesCollectionBase& types) override
        {
            types.AddEnum<AO::Path_MotionDetector::StartMoveDirection>("Enum_MotionDetectorStartMoveDirection",
                {
                    {AO::Path_MotionDetector::StartMoveDirection::eRight_0, "right"},
                    {AO::Path_MotionDetector::StartMoveDirection::eLeft_1, "left"},
                });
        }

        CTOR_AO(Path_MotionDetector, "MotionDetector", AO::TlvTypes::MotionDetector_62)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("device_x", tlv().field_1A_device_x);
            ADD("device_y", tlv().field_1C_device_y);
            ADD("speed_x256", tlv().field_1E_speed_x256);
            ADD("start_move_direction", tlv().field_20_start_move_direction);
            ADD("draw_flare", tlv().field_22_draw_flare);
            ADD("disable_id", tlv().field_24_disable_id);
            ADD("alarm_id", tlv().field_26_alarm_id);
            ADD("alarm_ticks", tlv().field_28_alarm_ticks);
        }
    };

    struct Path_BackgroundAnimation : public TlvObjectBaseAO<AO::Path_BackgroundAnimation>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("animation_id", tlv().field_18_animation_id);
            ADD("is_semi_trans", tlv().field_1A_is_semi_trans);
            ADD("semi_trans_mode", tlv().field_1C_semi_trans_mode);
            ADD("sound_effect", tlv().field_1E_sound_effect);
        }
    };

    struct Path_LCDStatusBoard : public TlvObjectBaseAO<AO::Path_LCDStatusBoard>
    {
        CTOR_AO(Path_LCDStatusBoard, "LCDStatusBoard", AO::TlvTypes::LCDStatusBoard_103)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_Preloader : public TlvObjectBaseAO<AO::Path_Preloader>
    {
        CTOR_AO(Path_Preloader, "Preloader", AO::TlvTypes::Preloader_102)
        {
            ADD("unload_cams_asap", tlv().unload_cams_ASAP);
        }
    };

    struct Path_Pulley : public TlvObjectBaseAO<AO::Path_Pulley>
    {
        CTOR_AO(Path_Pulley, "Pulley", AO::TlvTypes::Pulley_35)
        {
            ADD("scale", tlv().scale);
        }
    };

    struct Path_SoftLanding : public TlvObjectBaseAO<AO::Path_SoftLanding>
    {
        CTOR_AO(Path_SoftLanding, "SoftLanding", AO::TlvTypes::SoftLanding_114)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_MudokonPathTrans : public TlvObjectBaseAO<AO::Path_MudokonPathTrans>
    {
        CTOR_AO(Path_MudokonPathTrans, "MudokonPathTrans", AO::TlvTypes::MudokonPathTrans_89)
        {
            ADD("level", tlv().level);
            ADD("path", tlv().path);
            ADD("camera", tlv().camera);
        }
    };

    struct Path_AbeStart : public TlvObjectBaseAO<AO::Path_AbeStart>
    {
        CTOR_AO(Path_AbeStart, "AbeStart", AO::TlvTypes::AbeStart_37)
        {
            ADD("scale", tlv().scale);
        }
    };

    struct Path_ZSligCover : public TlvObjectBaseAO<AO::Path_ZSligCover>
    {
        CTOR_AO(Path_ZSligCover, "ZSligCover", AO::TlvTypes::ZSligCover_83)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_ScrabLeftBound : public TlvObjectBaseAO<AO::Path_ScrabLeftBound>
    {
        CTOR_AO(Path_ScrabLeftBound, "ScrabLeftBound", AO::TlvTypes::ScrabLeftBound_74)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_ScrabRightBound : public TlvObjectBaseAO<AO::Path_ScrabRightBound>
    {
        CTOR_AO(Path_ScrabRightBound, "ScrabRightBound", AO::TlvTypes::ScrabRightBound_75)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_ScrabNoFall : public TlvObjectBaseAO<AO::Path_ScrabNoFall>
    {
        CTOR_AO(Path_ScrabNoFall, "ScrabNoFall", AO::TlvTypes::ScrabNoFall_93)
        {
            EMPTY_CTOR_AO();
        }
    };

    struct Path_LiftMudokon : public TlvObjectBaseAO<AO::Path_LiftMudokon>
    {
        CTOR_AO(Path_LiftMudokon, "LiftMudokon", AO::TlvTypes::LiftMudokon_32)
        {
            ADD("how_far_to_walk", tlv().field_18_how_far_to_walk);
            ADD("lift_id", tlv().field_1A_lift_id);
            ADD("direction", tlv().field_1C_direction);
            ADD("silent", tlv().field_1E_silent);
            ADD("scale", tlv().field_20_scale);
            ADD("code1", tlv().field_22_code1);
            ADD("code2", tlv().field_24_code2);
        }
    };

    struct Path_HoneySack : public TlvObjectBaseAO<AO::Path_HoneySack>
    {
        CTOR_AO(Path_HoneySack, "HoneySack", AO::TlvTypes::HoneySack_36)
        {
            ADD("chase_ticks", tlv().field_18_chase_ticks);
            ADD("scale", tlv().field_1A_scale);
        }
    };

    struct Path_SlingMudokon : public TlvObjectBaseAO<AO::Path_SlingMudokon>
    {
        CTOR_AO(Path_SlingMudokon, "SlingMudokon", AO::TlvTypes::SlingMudokon_41)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("silent", tlv().field_1A_silent);
            ADD("code1", tlv().field_1C_code_1);
            ADD("code2", tlv().field_1E_code_2);
        }
    };

    struct Path_BeeSwarmHole : public TlvObjectBaseAO<AO::Path_BeeSwarmHole>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("what_to_spawn", tlv().field_18_what_to_spawn);
            ADD("interval", tlv().field_1A_interval);
            ADD("id", tlv().field_1C_id);
            ADD("movement_type", tlv().field_1E_movement_type);
            ADD("size", tlv().field_20_size);
            ADD("chase_time", tlv().field_22_chase_time);
            ADD("speed", tlv().field_24_speed);
            ADD("scale", tlv().field_26_scale);
        }
    };

    struct Path_MeatSack : public TlvObjectBaseAO<AO::Path_MeatSack>
    {
        CTOR_AO(Path_MeatSack, "MeatSack", AO::TlvTypes::MeatSack_71)
        {
            ADD("side", tlv().field_18_side);
            ADD("x_vel", tlv().field_1A_x_vel);
            ADD("y_vel", tlv().field_1C_y_vel);
            ADD("scale", tlv().field_1E_scale);
            ADD("amount_of_meat", tlv().field_20_amount_of_meat);
        }
    };

    struct Path_RollingBall : public TlvObjectBaseAO<AO::Path_RollingBall>
    {
        CTOR_AO(Path_RollingBall, "RollingBall", AO::TlvTypes::RollingBall_56)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("roll_direction", tlv().field_1A_roll_direction);
            ADD("release", tlv().field_1C_release);
            ADD("speed", tlv().field_1E_speed);
            ADD("acceleration", tlv().field_20_acceleration);
        }
    };

    struct Path_RollingBallStopper : public TlvObjectBaseAO<AO::Path_RollingBallStopper>
    {
        CTOR_AO(Path_RollingBallStopper, "RollingBallStopper", AO::TlvTypes::RollingBallStopper_59)
        {
            ADD("id_on", tlv().field_18_id_on);
            ADD("scale", tlv().field_1A_scale);
            ADD("id_off", tlv().field_1C_id_off);
            ADD("direction", tlv().field_1E_direction);
        }
    };

    struct Path_Bees : public TlvObjectBaseAO<AO::Path_Bees>
    {
        CTOR_AO(Path_Bees, "Bees", AO::TlvTypes::Bees_43)
        {
            ADD("id", tlv().id);
            ADD("swarm_size", tlv().swarm_size);
            ADD("chase_time", tlv().chase_time);
            ADD("speed", tlv().speed);
            ADD("disable_resources", tlv().disable_resources);
            ADD("num_bees", tlv().num_bees);
        }
    };

    struct Path_ZBall : public TlvObjectBaseAO<AO::Path_ZBall>
    {
        void AddTypes(TypesCollectionBase& types) override
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
            ADD("start_pos", tlv().field_18_start_pos);
            ADD("scale", tlv().field_1A_scale);
            ADD("speed", tlv().field_1C_speed);
        }
    };

    struct Path_FootSwitch : public TlvObjectBaseAO<AO::Path_FootSwitch>
    {
        void AddTypes(TypesCollectionBase& types) override
        {
            types.AddEnum<AO::FootSwitchTriggerBy>("Enum_FootSwitchTriggeredBy",
                {
                    {AO::FootSwitchTriggerBy::eOnlyAbe_0, "only_abe"},
                    {AO::FootSwitchTriggerBy::eAnyone_1, "anyone"},
                });
        }

        CTOR_AO(Path_FootSwitch, "FootSwitch", AO::TlvTypes::FootSwitch_60)
        {
            ADD("id", tlv().field_18_id);
            ADD("scale", tlv().field_1A_scale);
            ADD("action", tlv().field_1C_action);
            ADD("triggered_by", tlv().field_1E_trigger_by);
        }
    };

    struct Path_Paramite : public TlvObjectBaseAO<AO::Path_Paramite>
    {
        CTOR_AO(Path_Paramite, "Paramite", AO::TlvTypes::Paramite_48)
        {
            ADD("scale", tlv().field_18_scale);
            ADD("enter_from_web", tlv().field_1A_bEnter_from_web);
            ADD("attack_delay", tlv().field_1C_attack_delay);
            ADD("drop_in_timer", tlv().field_1E_drop_in_timer);
            ADD("meat_eating_time", tlv().field_20_meat_eating_time);
            ADD("attack_duration", tlv().field_22_attack_duration);
            ADD("disabled_resources", tlv().field_24_disabled_resources);
            ADD("id", tlv().field_26_id);
            ADD("hiss_before_attack", tlv().field_28_hiss_before_attack);
            ADD("delete_when_far_away", tlv().field_2A_delete_when_far_away);
        }
    };

    struct Path_Honey : public TlvObjectBaseAO<AO::Path_Honey>
    {
        CTOR_AO(Path_Honey, "Honey", AO::TlvTypes::Honey_20)
        {
            ADD("id", tlv().id);
            ADD("state", tlv().state);
            ADD("scale", tlv().scale);
        }
    };

    struct Path_HoneyDripTarget : public TlvObjectBaseAO<AO::Path_HoneyDripTarget>
    {
        CTOR_AO(Path_HoneyDripTarget, "HoneyDripTarget", AO::TlvTypes::HoneyDripTarget_42)
        {
            EMPTY_CTOR_AO();
        }
    };
}

#undef EMPTY_CTOR_AO
#undef CTOR_AO
