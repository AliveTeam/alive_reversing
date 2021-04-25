#pragma once

#include "TlvObjectBaseAO.hpp"

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
            TLVOBJECTBASE_ADD("fmv_id", mTlv.mData.fmvId);
            TLVOBJECTBASE_ADD("scale", mTlv.mData.scale);
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
            TLVOBJECTBASE_ADD("scale", mTlv.mData.scale);
            TLVOBJECTBASE_ADD("type", mTlv.mData.type);
            TLVOBJECTBASE_ADD("code1", mTlv.mData.code1);
            TLVOBJECTBASE_ADD("code2", mTlv.mData.code2);
            TLVOBJECTBASE_ADD("id", mTlv.mData.id);
        }
    };

    struct Path_HandStone : public TlvObjectBaseAO<AO::Path_HandStone>
    {
        CTOR_AO(Path_HandStone, "HandStone", AO::TlvTypes::HandStone_100)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.mData.scale);

            TLVOBJECTBASE_ADD("camera1_level", mTlv.mData.camera1.level);
            TLVOBJECTBASE_ADD("camera1_path", mTlv.mData.camera1.path);
            TLVOBJECTBASE_ADD("camera1_camera", mTlv.mData.camera1.camera);

            TLVOBJECTBASE_ADD("camera2_level", mTlv.mData.camera2.level);
            TLVOBJECTBASE_ADD("camera2_path", mTlv.mData.camera2.path);
            TLVOBJECTBASE_ADD("camera2_camera", mTlv.mData.camera2.camera);

            TLVOBJECTBASE_ADD("camera3_level", mTlv.mData.camera3.level);
            TLVOBJECTBASE_ADD("camera3_path", mTlv.mData.camera3.path);
            TLVOBJECTBASE_ADD("camera3_camera", mTlv.mData.camera3.camera);
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
            TLVOBJECTBASE_ADD("Level", mTlv.field_18_level);
            TLVOBJECTBASE_ADD("Path", mTlv.field_1A_path);
            TLVOBJECTBASE_ADD("Camera", mTlv.field_1C_camera);
            TLVOBJECTBASE_ADD("Scale", mTlv.field_1E_scale);
            TLVOBJECTBASE_ADD("DoorNumber", mTlv.field_20_door_number);
            TLVOBJECTBASE_ADD("Id", mTlv.field_22_id);
            TLVOBJECTBASE_ADD("TargetDoorNumber", mTlv.field_24_target_door_number);
            TLVOBJECTBASE_ADD("StartState", mTlv.field_26_start_state);
            TLVOBJECTBASE_ADD("DoorClosed", mTlv.field_28_door_closed);
            TLVOBJECTBASE_ADD("Hub1", mTlv.field_2A_hub1);
            TLVOBJECTBASE_ADD("Hub2", mTlv.field_2A_hub2);
            TLVOBJECTBASE_ADD("Hub3", mTlv.field_2A_hub3);
            TLVOBJECTBASE_ADD("Hub4", mTlv.field_2A_hub4);
            TLVOBJECTBASE_ADD("Hub5", mTlv.field_2A_hub5);
            TLVOBJECTBASE_ADD("Hub6", mTlv.field_2A_hub6);
            TLVOBJECTBASE_ADD("Hub7", mTlv.field_2A_hub7);
            TLVOBJECTBASE_ADD("Hub8", mTlv.field_2A_hub8);
            TLVOBJECTBASE_ADD("WipeEffect", mTlv.field_3A_wipe_effect);
            TLVOBJECTBASE_ADD("MovieNumber", mTlv.field_3C_movie_number);
            TLVOBJECTBASE_ADD("XOffset", mTlv.field_3E_x_offset);
            TLVOBJECTBASE_ADD("YOffset", mTlv.field_40_y_offset);
            TLVOBJECTBASE_ADD("WipeXOrg", mTlv.field_42_wipe_x_org);
            TLVOBJECTBASE_ADD("WipeYOrg", mTlv.field_44_wipe_y_org);
            TLVOBJECTBASE_ADD("AbeDirection", mTlv.field_46_abe_direction);
        }
    };

    struct Path_ContinuePoint : public TlvObjectBaseAO<AO::Path_ContinuePoint>
    {
        CTOR_AO(Path_ContinuePoint, "ContinuePoint",  AO::TlvTypes::ContinuePoint_0)
        {
            TLVOBJECTBASE_ADD("ZoneNumber", mTlv.field_18_zone_number);
            TLVOBJECTBASE_ADD("ClearFromId", mTlv.field_1A_clear_from_id);
            TLVOBJECTBASE_ADD("ClearToId", mTlv.field_1C_clear_to_id);
            TLVOBJECTBASE_ADD("ElumRestarts", mTlv.field_1E_elum_restarts);
            TLVOBJECTBASE_ADD("AbeSpawnDirection", mTlv.field_20_abe_direction);
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
            TLVOBJECTBASE_ADD("HoistType", mTlv.field_18_hoist_type);
            TLVOBJECTBASE_ADD("HoistEdgeType", mTlv.field_1A_edge_type);
            TLVOBJECTBASE_ADD("Id", mTlv.field_1C_id);
        }
    };

    struct Path_Change : public TlvObjectBaseAO<AO::Path_ChangeTLV>
    {
        CTOR_AO(Path_Change, "PathTransition", AO::TlvTypes::PathTransition_1)
        {
            TLVOBJECTBASE_ADD("Level", mTlv.field_18_level);
            TLVOBJECTBASE_ADD("HoistEdgeType", mTlv.field_1A_path);
            TLVOBJECTBASE_ADD("Camera", mTlv.field_1C_camera);
            TLVOBJECTBASE_ADD("Movie", mTlv.field_1E_movie); // TODO: Enum
            TLVOBJECTBASE_ADD("Wipe", mTlv.field_20_wipe); // TODO: Enum
            TLVOBJECTBASE_ADD("Scale", mTlv.field_22_scale); // TODO: Enum
        }
    };

    struct Path_Switch : public TlvObjectBaseAO<AO::Path_Switch>
    {
        CTOR_AO(Path_Switch, "Switch", AO::TlvTypes::Switch_26)
        {
            TLVOBJECTBASE_ADD("TriggerObject", mTlv.field_18_trigger_object);
            TLVOBJECTBASE_ADD("TriggerAction", mTlv.field_1A_trigger_object_action);
            TLVOBJECTBASE_ADD("Scale", mTlv.field_1C_scale);
            TLVOBJECTBASE_ADD("OnSound", mTlv.field_1E_on_sound); // TODO: Enum
            TLVOBJECTBASE_ADD("OffSound", mTlv.field_20_off_sound); // TODO: Enum
            TLVOBJECTBASE_ADD("SoundDirection", mTlv.field_22_sound_direction); // TODO: Enum
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
            TLVOBJECTBASE_ADD("Type", mTlv.field_18_type);
            TLVOBJECTBASE_ADD("Size", mTlv.field_1A_size);
            TLVOBJECTBASE_ADD("Id", mTlv.field_1C_id);
            TLVOBJECTBASE_ADD("FlipX", mTlv.field_1E_flip_x);
        }
    };

    struct Path_ElectricWall : public TlvObjectBaseAO<AO::Path_ElectricWall>
    {
        CTOR_AO(Path_ElectricWall, "ElectricWall", AO::TlvTypes::ElectricWall_67)
        {
            TLVOBJECTBASE_ADD("Scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("Id", mTlv.field_1A_id);
            TLVOBJECTBASE_ADD("State", mTlv.field_1C_start_state); // TODO: Enum
        }
    };

    struct Path_ContinueZone : public TlvObjectBaseAO<AO::Path_ContinueZone>
    {
        CTOR_AO(Path_ContinueZone, "ContinueZone", AO::TlvTypes::ContinueZone_2)
        {
            TLVOBJECTBASE_ADD("ZoneNumber", mTlv.field_10_zone_number);
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
            TLVOBJECTBASE_ADD("type", mTlv.field_18_type);
            TLVOBJECTBASE_ADD("can_grab", mTlv.field_1A_can_grab);
        }
    };

    struct Path_WellLocal : public TlvObjectBaseAO<AO::Path_WellLocal>
    {
        CTOR_AO(Path_WellLocal, "WellLocal", AO::TlvTypes::WellLocal_11)
        {
            // Well base
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("trigger_id", mTlv.field_1A_trigger_id);
            TLVOBJECTBASE_ADD("well_id", mTlv.field_1C_well_id);
            TLVOBJECTBASE_ADD("resource_id", mTlv.field_1E_res_id);
            TLVOBJECTBASE_ADD("exit_x", mTlv.field_20_exit_x);
            TLVOBJECTBASE_ADD("exit_y", mTlv.field_22_exit_y);
            TLVOBJECTBASE_ADD("dx", mTlv.field_24_off_level_or_dx.dx);
            TLVOBJECTBASE_ADD("dy", mTlv.field_26_off_path_or_dy);

            // Well local
            TLVOBJECTBASE_ADD("on_dx", mTlv.field_28_on_dx);
            TLVOBJECTBASE_ADD("on_dy", mTlv.field_2A_on_dy);
            TLVOBJECTBASE_ADD("emit_leaves", mTlv.field_2C_bEmit_leaves);
            TLVOBJECTBASE_ADD("leaf_x", mTlv.field_2E_leaf_x);
            TLVOBJECTBASE_ADD("leaf_y", mTlv.field_30_leaf_y);
        }
    };

    struct Path_WellExpress : public TlvObjectBaseAO<AO::Path_WellExpress>
    {
        CTOR_AO(Path_WellExpress, "WellExpress", AO::TlvTypes::WellExpress_45)
        {
            // Well base
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("trigger_id", mTlv.field_1A_trigger_id);
            TLVOBJECTBASE_ADD("well_id", mTlv.field_1C_well_id);
            TLVOBJECTBASE_ADD("resource_id", mTlv.field_1E_res_id);
            TLVOBJECTBASE_ADD("exit_x", mTlv.field_20_exit_x);
            TLVOBJECTBASE_ADD("exit_y", mTlv.field_22_exit_y);
            TLVOBJECTBASE_ADD("off_level", mTlv.field_24_off_level_or_dx.level);
            TLVOBJECTBASE_ADD("off_path", mTlv.field_26_off_path_or_dy);

            // Well express
            TLVOBJECTBASE_ADD("off_camera", mTlv.field_28_off_camera);
            TLVOBJECTBASE_ADD("off_well_id", mTlv.field_2A_off_well_id);
            TLVOBJECTBASE_ADD("on_level", mTlv.field_2C_on_level);
            TLVOBJECTBASE_ADD("on_path", mTlv.field_2E_on_path);
            TLVOBJECTBASE_ADD("on_camera", mTlv.field_30_on_camera);
            TLVOBJECTBASE_ADD("on_well_id", mTlv.field_32_on_well_id);
            TLVOBJECTBASE_ADD("emit_leaves", mTlv.field_34_emit_leaves);
            TLVOBJECTBASE_ADD("leaf_x", mTlv.field_36_leaf_x);
            TLVOBJECTBASE_ADD("leaf_y", mTlv.field_38_leaf_y);
            TLVOBJECTBASE_ADD("movie_id", mTlv.field_3A_movie_id);
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
            TLVOBJECTBASE_ADD("direction", mTlv.field_18_direction);
            TLVOBJECTBASE_ADD("id", mTlv.field_1A_id);
        }
    };

    struct Path_Slig : public TlvObjectBaseAO<AO::Path_Slig>
    {
        void AddTypes(TypesCollectionBase& types) override
        {
            types.AddEnum<AO::Path_Slig::StartState>("Enum_SligStartState",
                {
                    {AO::Path_Slig::StartState::Listening_0, "listening"},
                    {AO::Path_Slig::StartState::Paused_1, "paused"},
                    {AO::Path_Slig::StartState::Sleeping_2, "sleeping"},
                    {AO::Path_Slig::StartState::Chase_3, "chase"},
                    {AO::Path_Slig::StartState::GameEnder_4, "game_ender"},
                    {AO::Path_Slig::StartState::Paused_5, "paused"},
                });
        }

        CTOR_AO(Path_Slig, "Slig", AO::TlvTypes::Slig_24)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("start_state", mTlv.field_1A_start_state);
            TLVOBJECTBASE_ADD("pause_time", mTlv.field_1C_pause_time);
            TLVOBJECTBASE_ADD("pause_left_min", mTlv.field_1E_pause_left_min);
            TLVOBJECTBASE_ADD("pause_left_max", mTlv.field_20_pause_left_max);
            TLVOBJECTBASE_ADD("pause_right_min", mTlv.field_22_pause_right_min);
            TLVOBJECTBASE_ADD("pause_right_max", mTlv.field_24_pause_right_max);
            TLVOBJECTBASE_ADD("chal_type", mTlv.field_26_chal_type);
            TLVOBJECTBASE_ADD("chal_time", mTlv.field_28_chal_time);
            TLVOBJECTBASE_ADD("number_of_times_to_shoot", mTlv.field_2A_number_of_times_to_shoot);
            TLVOBJECTBASE_ADD("unknown", mTlv.field_2C_unknown);
            TLVOBJECTBASE_ADD("code1", mTlv.field_2E_code1);
            TLVOBJECTBASE_ADD("code2", mTlv.field_30_code2);
            TLVOBJECTBASE_ADD("chase_abe", mTlv.field_32_chase_abe);
            TLVOBJECTBASE_ADD("start_direction", mTlv.field_34_start_direction);
            TLVOBJECTBASE_ADD("panic_timeout", mTlv.field_36_panic_timeout);
            TLVOBJECTBASE_ADD("num_panic_sounds", mTlv.field_38_num_panic_sounds);
            TLVOBJECTBASE_ADD("panic_sound_timeout", mTlv.field_3A_panic_sound_timeout);
            TLVOBJECTBASE_ADD("stop_chase_delay", mTlv.field_3C_stop_chase_delay);
            TLVOBJECTBASE_ADD("time_to_wait_before_chase", mTlv.field_3E_time_to_wait_before_chase);
            TLVOBJECTBASE_ADD("slig_id", mTlv.field_40_slig_id);
            TLVOBJECTBASE_ADD("listen_time", mTlv.field_42_listen_time);
            TLVOBJECTBASE_ADD("percent_say_what", mTlv.field_44_percent_say_what);
            TLVOBJECTBASE_ADD("percent_beat_mud", mTlv.field_46_percent_beat_mud);
            TLVOBJECTBASE_ADD("talk_to_abe", mTlv.field_48_talk_to_abe);
            TLVOBJECTBASE_ADD("dont_shoot", mTlv.field_4A_dont_shoot);
            TLVOBJECTBASE_ADD("z_shoot_delay", mTlv.field_4C_z_shoot_delay);
            TLVOBJECTBASE_ADD("stay_awake", mTlv.field_4E_stay_awake);
            TLVOBJECTBASE_ADD("disable_resources", mTlv.field_50_disable_resources.Raw().all);
            TLVOBJECTBASE_ADD("noise_wake_up_distance", mTlv.field_52_noise_wake_up_distance);
            TLVOBJECTBASE_ADD("id", mTlv.field_54_id);
        }
    };

    struct Path_DeathDrop : public TlvObjectBaseAO<AO::Path_DeathDrop>
    {
        CTOR_AO(Path_DeathDrop, "DeathDrop", AO::TlvTypes::DeathDrop_5)
        {
            TLVOBJECTBASE_ADD_HIDDEN("animation", mTlv.animation);
            TLVOBJECTBASE_ADD_HIDDEN("sound", mTlv.sound);
            TLVOBJECTBASE_ADD_HIDDEN("id", mTlv.id);
            TLVOBJECTBASE_ADD_HIDDEN("action", mTlv.action);
            TLVOBJECTBASE_ADD_HIDDEN("set_value", mTlv.set_value);
        }
    };

    struct Path_SligLeftBound : public TlvObjectBaseAO<AO::Path_SligLeftBound>
    {
        CTOR_AO(Path_SligLeftBound, "SligLeftBound", AO::TlvTypes::eSligBoundLeft_57)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_slig_id);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all);
        }
    };

    struct Path_SligRightBound : public TlvObjectBaseAO<AO::Path_SligRightBound>
    {
        CTOR_AO(Path_SligRightBound, "SligRightBound", AO::TlvTypes::eSligBoundRight_76)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_slig_id);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all);
        }
    };

    struct Path_SligPersist : public TlvObjectBaseAO<AO::Path_SligPersist>
    {
        CTOR_AO(Path_SligPersist, "SligPersist", AO::TlvTypes::eSligPersist_77)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_slig_id);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_1A_disabled_resources.Raw().all);
        }
    };

    struct Path_SecurityOrb : public TlvObjectBaseAO<AO::Path_SecurityOrb>
    {
        CTOR_AO(Path_SecurityOrb, "SecurityOrb", AO::TlvTypes::SecurityOrb_29)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_1A_disable_resources);
        }
    };

    struct Path_FallingItem : public TlvObjectBaseAO<AO::Path_FallingItem>
    {
        CTOR_AO(Path_FallingItem, "FallingItem", AO::TlvTypes::FallingItem_15)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_id);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1A_scale);
            TLVOBJECTBASE_ADD("delay_time", mTlv.field_1C_delay_time);
            TLVOBJECTBASE_ADD("number_of_items", mTlv.field_1E_number_of_items);
            TLVOBJECTBASE_ADD("reset_id", mTlv.field_20_reset_id);
        }
    };

    struct Path_Mine : public TlvObjectBaseAO<AO::Path_Mine>
    {
        CTOR_AO(Path_Mine, "Mine", AO::TlvTypes::Mine_46)
        {
            TLVOBJECTBASE_ADD("num_patterns", mTlv.field_18_num_patterns);
            TLVOBJECTBASE_ADD("pattern", mTlv.field_1A_pattern);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1C_scale);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_1E_disabled_resources);
            TLVOBJECTBASE_ADD("persists_offscreen", mTlv.field_20_persists_offscreen);
        }
    };

    struct Path_Dove : public TlvObjectBaseAO<AO::Path_Dove>
    {
        CTOR_AO(Path_Dove, "Dove", AO::TlvTypes::Dove_12)
        {
            TLVOBJECTBASE_ADD("dove_count", mTlv.field_18_dove_count);
            TLVOBJECTBASE_ADD("pixel_perfect", mTlv.field_1A_pixel_perfect);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1C_scale);
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
            TLVOBJECTBASE_ADD("num_patterns", mTlv.field_18_num_patterns);
            TLVOBJECTBASE_ADD("pattern", mTlv.field_1A_pattern);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1C_scale);
            TLVOBJECTBASE_ADD("state", mTlv.field_1E_state);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_20_disabled_resources);
        }
    };

    struct Path_HintFly : public TlvObjectBaseAO<AO::Path_HintFly>
    {
        CTOR_AO(Path_HintFly, "HintFly", AO::TlvTypes::HintFly_92)
        {
            TLVOBJECTBASE_ADD("message_id", mTlv.field_18_message_id);
        }
    };

    struct Path_Bat : public TlvObjectBaseAO<AO::Path_Bat>
    {
        CTOR_AO(Path_Bat, "Bat", AO::TlvTypes::Bat_49)
        {
            TLVOBJECTBASE_ADD("ticks_before_moving", mTlv.field_18_ticks_before_moving);
            TLVOBJECTBASE_ADD("speed", mTlv.field_1A_speed);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1C_scale);
            TLVOBJECTBASE_ADD("attack_duration", mTlv.field_1E_attack_duration);
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
            TLVOBJECTBASE_ADD("centre_w", mTlv.field_18_centre_w);
            TLVOBJECTBASE_ADD("centre_h", mTlv.field_1A_centre_h);
            TLVOBJECTBASE_ADD("r", mTlv.field_1C_r);
            TLVOBJECTBASE_ADD("g", mTlv.field_1E_g);
            TLVOBJECTBASE_ADD("b", mTlv.field_20_b);
            TLVOBJECTBASE_ADD("id", mTlv.field_22_id);
            TLVOBJECTBASE_ADD("scale", mTlv.field_24_scale);
        }
    };

    struct Path_BellHammer : public TlvObjectBaseAO<AO::Path_BellHammer>
    {
        CTOR_AO(Path_BellHammer, "BellHammer", AO::TlvTypes::BellHammer_27)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_id);
            TLVOBJECTBASE_ADD("action", mTlv.field_1A_action);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1C_scale);
            TLVOBJECTBASE_ADD("direction", mTlv.field_1E_direction);
        }
    };

    struct Path_IdSplitter : public TlvObjectBaseAO<AO::Path_IdSplitter>
    {
        CTOR_AO(Path_IdSplitter, "IdSplitter", AO::TlvTypes::IdSplitter_94)
        {
            TLVOBJECTBASE_ADD("source_id", mTlv.field_18_source_id);
            TLVOBJECTBASE_ADD("delay", mTlv.field_1A_delay);
            TLVOBJECTBASE_ADD("id_1", mTlv.field_1C_id1);
            TLVOBJECTBASE_ADD("id_2", mTlv.field_1C_id2);
            TLVOBJECTBASE_ADD("id_3", mTlv.field_1C_id3);
            TLVOBJECTBASE_ADD("id_4", mTlv.field_1C_id4);
        }
    };

    struct Path_PullRingRope : public TlvObjectBaseAO<AO::Path_PullRingRope>
    {
        CTOR_AO(Path_PullRingRope, "PullRingRope", AO::TlvTypes::PullRingRope_18)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_id);
            TLVOBJECTBASE_ADD("action", mTlv.field_1A_action);
            TLVOBJECTBASE_ADD("rope_length", mTlv.field_1C_rope_length);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1E_scale);
            TLVOBJECTBASE_ADD("on_sound", mTlv.field_20_on_sound);
            TLVOBJECTBASE_ADD("off_sound", mTlv.field_22_off_sound);
            TLVOBJECTBASE_ADD("sound_direction", mTlv.field_24_sound_direction);
        }
    };

    struct Path_MusicTrigger : public TlvObjectBaseAO<AO::Path_MusicTrigger>
    {
        CTOR_AO(Path_MusicTrigger, "MusicTrigger", AO::TlvTypes::MusicTrigger_105)
        {
            TLVOBJECTBASE_ADD("type", mTlv.field_18_type);
            TLVOBJECTBASE_ADD("enabled_by", mTlv.field_1A_enabled_by);
            TLVOBJECTBASE_ADD("id", mTlv.field_1C_id);
            TLVOBJECTBASE_ADD("timer", mTlv.field_1E_timer);
        }
    };

    struct Path_ElumPathTrans : public TlvObjectBaseAO<AO::Path_ElumPathTrans>
    {
        CTOR_AO(Path_ElumPathTrans, "ElumPathTrans", AO::TlvTypes::ElumPathTrans_99)
        {
            TLVOBJECTBASE_ADD("level", mTlv.field_18_level);
            TLVOBJECTBASE_ADD("path", mTlv.field_1A_path);
            TLVOBJECTBASE_ADD("camera", mTlv.field_1C_camera);
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
            TLVOBJECTBASE_ADD("id", mTlv.field_18_id);
            TLVOBJECTBASE_ADD("is_start_point", mTlv.field_1A_bstart_point);
            TLVOBJECTBASE_ADD("lift_type", mTlv.field_1C_lift_type);
            TLVOBJECTBASE_ADD("lift_point_stop_type", mTlv.field_1E_lift_point_stop_type);
            TLVOBJECTBASE_ADD("scale", mTlv.field_20_scale);
            TLVOBJECTBASE_ADD("ignore_lift_mover", mTlv.field_22_bIgnore_lift_mover);
        }
    };

    struct Path_MovingBomb : public TlvObjectBaseAO<AO::Path_MovingBomb>
    {
        CTOR_AO(Path_MovingBomb, "MovingBomb", AO::TlvTypes::MovingBomb_86)
        {
            TLVOBJECTBASE_ADD("speed", mTlv.field_18_speed);
            TLVOBJECTBASE_ADD("id", mTlv.field_1A_id);
            TLVOBJECTBASE_ADD("start_type_triggered_by_alarm", mTlv.field_1C_bStart_type_triggered_by_alarm);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1E_scale);
            TLVOBJECTBASE_ADD("max_rise", mTlv.field_20_max_rise);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_22_disabled_resources);
            TLVOBJECTBASE_ADD("start_speed", mTlv.field_24_start_speed);
            TLVOBJECTBASE_ADD("persist_offscreen", mTlv.field_26_persist_offscreen);
        }
    };

    struct Path_MovingBombStopper : public TlvObjectBaseAO<AO::Path_MovingBombStopper>
    {
        CTOR_AO(Path_MovingBombStopper, "MovingBombStopper", AO::TlvTypes::MovingBombStopper_87)
        {
            TLVOBJECTBASE_ADD("min_delay", mTlv.field_18_min_delay);
            TLVOBJECTBASE_ADD("max_delay", mTlv.field_1A_max_delay);
        }
    };

    struct Path_RingMudokon : public TlvObjectBaseAO<AO::Path_RingMudokon>
    {
        CTOR_AO(Path_RingMudokon, "RingMudokon", AO::TlvTypes::RingMudokon_50)
        {
            TLVOBJECTBASE_ADD("facing", mTlv.field_18_facing);
            TLVOBJECTBASE_ADD("abe_must_be_same_direction", mTlv.field_1A_abe_must_be_same_direction);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1C_scale);
            TLVOBJECTBASE_ADD("silent", mTlv.field_1E_silent);
            TLVOBJECTBASE_ADD("code1", mTlv.field_20_code1);
            TLVOBJECTBASE_ADD("code2", mTlv.field_22_code2);
            TLVOBJECTBASE_ADD("action", mTlv.field_24_action);
            TLVOBJECTBASE_ADD("ring_timeout", mTlv.field_26_ring_timeout);
            TLVOBJECTBASE_ADD("instant_powerup", mTlv.field_28_instant_powerup);
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
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale_background);
            TLVOBJECTBASE_ADD("min_time_off1", mTlv.field_1A_min_time_off1);
            TLVOBJECTBASE_ADD("max_time_off1", mTlv.field_1C_max_time_off1);
            TLVOBJECTBASE_ADD("max_rise_time", mTlv.field_1E_max_rise_time);
            TLVOBJECTBASE_ADD("id", mTlv.field_20_id);
            TLVOBJECTBASE_ADD("type", mTlv.field_22_type);
            TLVOBJECTBASE_ADD("speed", mTlv.field_24_speed);
            TLVOBJECTBASE_ADD("start_state", mTlv.field_26_start_state);
            TLVOBJECTBASE_ADD("off_speed", mTlv.field_28_off_speed);
            TLVOBJECTBASE_ADD("min_time_off2", mTlv.field_2A_min_time_off2);
            TLVOBJECTBASE_ADD("max_time_off2", mTlv.field_2C_max_time_off2);
            TLVOBJECTBASE_ADD("initial_position", mTlv.field_2E_inital_position);
        }
    };

    struct Path_LCDScreen : public TlvObjectBaseAO<AO::Path_LCDScreen>
    {
        CTOR_AO(Path_LCDScreen, "LCDScreen", AO::TlvTypes::LCDScreen_98)
        {
            TLVOBJECTBASE_ADD("message_1_id", mTlv.field_18_message_1_id);
            TLVOBJECTBASE_ADD("message_rand_min", mTlv.field_1A_message_rand_min);
            TLVOBJECTBASE_ADD("message_rand_max", mTlv.field_1C_message_rand_max);
        }
    };

    struct Path_InvisibleSwitch : public TlvObjectBaseAO<AO::Path_InvisibleSwitch>
    {
        CTOR_AO(Path_InvisibleSwitch, "InvisibleSwitch", AO::TlvTypes::InvisibleSwitch_81)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_id);
            TLVOBJECTBASE_ADD("action", mTlv.field_1A_action);
            TLVOBJECTBASE_ADD("delay", mTlv.field_1C_delay);
            TLVOBJECTBASE_ADD("set_off_alarm", mTlv.field_1E_set_off_alarm);
            TLVOBJECTBASE_ADD("scale", mTlv.field_20_scale);
        }
    };

    struct Path_TrapDoor : public TlvObjectBaseAO<AO::Path_TrapDoor>
    {
        CTOR_AO(Path_TrapDoor, "TrapDoor", AO::TlvTypes::TrapDoor_55)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_id);
            TLVOBJECTBASE_ADD("start_state", mTlv.field_1A_start_state);
            TLVOBJECTBASE_ADD("self_closing", mTlv.field_1C_self_closing);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1E_scale);
            TLVOBJECTBASE_ADD("dest_level", mTlv.field_20_dest_level);
            TLVOBJECTBASE_ADD("direction", mTlv.field_22_direction);
            TLVOBJECTBASE_ADD("anim_offset", mTlv.field_24_anim_offset);
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
            TLVOBJECTBASE_ADD("side", mTlv.field_18_side);
            TLVOBJECTBASE_ADD("dest_level", mTlv.field_1A_dest_level);
            TLVOBJECTBASE_ADD("dest_path", mTlv.field_1C_dest_path);
            TLVOBJECTBASE_ADD("dest_camera", mTlv.field_1E_dest_camera);
            TLVOBJECTBASE_ADD("scale", mTlv.field_20_scale);
            TLVOBJECTBASE_ADD("movie_id", mTlv.field_22_movie_id);
            TLVOBJECTBASE_ADD("portal_type", mTlv.field_24_portal_type);
            TLVOBJECTBASE_ADD("num_muds_for_shrykull", mTlv.field_26_num_muds_for_shrykul);
        }
    };

    struct Path_BoomMachine : public TlvObjectBaseAO<AO::Path_BoomMachine>
    {
        CTOR_AO(Path_BoomMachine, "BoomMachine", AO::TlvTypes::BoomMachine_97)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("nozzle_side", mTlv.field_1A_nozzle_side);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_1C_disabled_resources);
            TLVOBJECTBASE_ADD("number_of_grenades", mTlv.field_1E_number_of_grenades);
        }
    };

    struct Path_Mudokon : public TlvObjectBaseAO<AO::Path_Mudokon>
    {
        CTOR_AO(Path_Mudokon, "Mudokon", AO::TlvTypes::Mudokon_82)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("job", mTlv.field_1A_job);
            TLVOBJECTBASE_ADD("direction", mTlv.field_1C_direction);
            TLVOBJECTBASE_ADD("voice_adjust", mTlv.field_1E_voice_adjust);
            TLVOBJECTBASE_ADD("rescue_id", mTlv.field_20_rescue_id);
            TLVOBJECTBASE_ADD("deaf", mTlv.field_22_deaf);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_24_disabled_resources);
            TLVOBJECTBASE_ADD("persist", mTlv.field_26_persist);
        }
    };

    struct Path_BirdPortalExit : public TlvObjectBaseAO<AO::Path_BirdPortalExit>
    {
        CTOR_AO(Path_BirdPortalExit, "BirdPortalExit", AO::TlvTypes::BirdPortalExit_53)
        {
            TLVOBJECTBASE_ADD("side", mTlv.field_18_side);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1A_scale);
        }
    };

    struct Path_Slog : public TlvObjectBaseAO<AO::Path_Slog>
    {
        CTOR_AO(Path_Slog, "Slog", AO::TlvTypes::Slog_25)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("direction", mTlv.field_1A_direction);
            TLVOBJECTBASE_ADD("wakeup_anger", mTlv.field_1C_wakeup_anger);
            TLVOBJECTBASE_ADD("bark_anger", mTlv.field_1E_bark_anger);
            TLVOBJECTBASE_ADD("sleeps", mTlv.field_20_sleeps);
            TLVOBJECTBASE_ADD("chase_anger", mTlv.field_22_chase_anger);
            TLVOBJECTBASE_ADD("jump_attack_delay", mTlv.field_24_jump_attack_delay);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_26_disabled_resources);
            TLVOBJECTBASE_ADD("anger_trigger_id", mTlv.field_28_anger_trigger_id);
        }
    };

    struct Path_ChimeLock : public TlvObjectBaseAO<AO::Path_ChimeLock>
    {
        CTOR_AO(Path_ChimeLock, "ChimeLock", AO::TlvTypes::ChimeLock_69)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("solve_id", mTlv.field_1A_solve_id);
            TLVOBJECTBASE_ADD("code1", mTlv.field_1C_code1);
            TLVOBJECTBASE_ADD("code2", mTlv.field_1E_code2);
            TLVOBJECTBASE_ADD("id", mTlv.field_20_id);
        }
    };

    struct Path_FlintLockFire : public TlvObjectBaseAO<AO::Path_FlintLockFire>
    {
        CTOR_AO(Path_FlintLockFire, "FlintLockFire", AO::TlvTypes::FlintLockFire_73)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("id", mTlv.field_1A_id);
        }
    };

    struct Path_LiftMover : public TlvObjectBaseAO<AO::Path_LiftMover>
    {
        CTOR_AO(Path_LiftMover, "LiftMover", AO::TlvTypes::LiftMover_68)
        {
            TLVOBJECTBASE_ADD("switch_id", mTlv.field_18_switch_id);
            TLVOBJECTBASE_ADD("lift_id", mTlv.field_1A_lift_id);
            TLVOBJECTBASE_ADD("direction", mTlv.field_1C_direction);
        }
    };

    struct Path_Scrab : public TlvObjectBaseAO<AO::Path_Scrab>
    {
        CTOR_AO(Path_Scrab, "Scrab", AO::TlvTypes::Scrab_72)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("attack_delay", mTlv.field_1A_attack_delay);
            TLVOBJECTBASE_ADD("patrol_type", mTlv.field_1C_patrol_type);
            TLVOBJECTBASE_ADD("left_min_delay", mTlv.field_1E_left_min_delay);
            TLVOBJECTBASE_ADD("left_max_delay", mTlv.field_20_left_max_delay);
            TLVOBJECTBASE_ADD("right_min_delay", mTlv.field_22_right_min_delay);
            TLVOBJECTBASE_ADD("right_max_delay", mTlv.field_24_right_max_delay);
            TLVOBJECTBASE_ADD("attack_duration", mTlv.field_26_attack_duration);
            TLVOBJECTBASE_ADD("disable_resources", mTlv.field_28_disable_resources);
            TLVOBJECTBASE_ADD("roar_randomly", mTlv.field_2A_roar_randomly);
        }
    };

    struct Path_SlogSpawner : public TlvObjectBaseAO<AO::Path_SlogSpawner>
    {
        CTOR_AO(Path_SlogSpawner, "SlogSpawner", AO::TlvTypes::SlogSpawner_107)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("num_slogs", mTlv.field_1A_num_slogs);
            TLVOBJECTBASE_ADD("num_at_a_time", mTlv.field_1C_num_at_a_time);
            TLVOBJECTBASE_ADD("direction", mTlv.field_1E_direction);
            TLVOBJECTBASE_ADD("ticks_between_slogs", mTlv.field_20_ticks_between_slogs);
            TLVOBJECTBASE_ADD("start_id", mTlv.field_22_start_id);
        }
    };

    struct Path_RockSack : public TlvObjectBaseAO<AO::Path_RockSack>
    {
        CTOR_AO(Path_RockSack, "RockSack", AO::TlvTypes::RockSack_13)
        {
            TLVOBJECTBASE_ADD("side", mTlv.field_18_side);
            TLVOBJECTBASE_ADD("x_vel", mTlv.field_1A_x_vel);
            TLVOBJECTBASE_ADD("y_vel", mTlv.field_1C_y_vel);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1E_scale);
            TLVOBJECTBASE_ADD("num_rocks", mTlv.field_20_num_rocks);
        }
    };

    struct Path_SlogHut : public TlvObjectBaseAO<AO::Path_SlogHut>
    {
        CTOR_AO(Path_SlogHut, "SlogHut", AO::TlvTypes::SlogHut_111)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("switch_id", mTlv.field_1A_switch_id);
            TLVOBJECTBASE_ADD("z_delay", mTlv.field_1C_z_delay);
        }
    };

    struct Path_SecurityClaw : public TlvObjectBaseAO<AO::Path_SecurityClaw>
    {
        CTOR_AO(Path_SecurityClaw, "SecurityClaw", AO::TlvTypes::SecurityClaw_61)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("alarm_id", mTlv.field_1A_alarm_id);
            TLVOBJECTBASE_ADD("alarm_time", mTlv.field_1C_alarm_time);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_1E_disabled_resources);
        }
    };

    struct Path_SecurityDoor : public TlvObjectBaseAO<AO::Path_SecurityDoor>
    {
        CTOR_AO(Path_SecurityDoor, "SecurityDoor", AO::TlvTypes::SecurityDoor_95)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("id", mTlv.field_1A_id);
            TLVOBJECTBASE_ADD("code_1", mTlv.field_1C_code_1);
            TLVOBJECTBASE_ADD("code_2", mTlv.field_1E_code2);
            TLVOBJECTBASE_ADD("door_xpos", mTlv.field_20_xpos);
            TLVOBJECTBASE_ADD("door_ypos", mTlv.field_22_ypos);
        }
    };

    struct Path_TimedMine : public TlvObjectBaseAO<AO::Path_TimedMine>
    {
        CTOR_AO(Path_TimedMine, "TimedMine", AO::TlvTypes::TimedMine_22)
        {
            TLVOBJECTBASE_ADD("id", mTlv.field_18_id);
            TLVOBJECTBASE_ADD("state", mTlv.field_1A_state);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1C_scale);
            TLVOBJECTBASE_ADD("ticks_before_explode", mTlv.field_1E_ticks_before_explode);
            TLVOBJECTBASE_ADD("disable_resources", mTlv.field_20_disable_resources);
        }
    };

    struct Path_SligSpawner : public TlvObjectBaseAO<AO::Path_SligSpawner>
    {
        void AddTypes(TypesCollectionBase& types) override
        {
            types.AddEnum<AO::Path_SligSpawner::StartState>("Enum_SligSpawnerStartState",
                {
                    {AO::Path_SligSpawner::StartState::Listening_0, "listening"},
                    {AO::Path_SligSpawner::StartState::Paused_1, "paused"},
                    {AO::Path_SligSpawner::StartState::Sleeping_2, "sleeping"},
                    {AO::Path_SligSpawner::StartState::Chase_3, "chase"},
                    {AO::Path_SligSpawner::StartState::GameEnder_4, "game_ender"},
                    {AO::Path_SligSpawner::StartState::Paused_5, "paused"},
                });
        }

        CTOR_AO(Path_SligSpawner, "SligSpawner", AO::TlvTypes::SligSpawner_66)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("start_state", mTlv.field_1A_start_state);
            TLVOBJECTBASE_ADD("pause_time", mTlv.field_1C_pause_time);
            TLVOBJECTBASE_ADD("pause_left_min", mTlv.field_1E_pause_left_min);
            TLVOBJECTBASE_ADD("pause_left_max", mTlv.field_20_pause_left_max);
            TLVOBJECTBASE_ADD("pause_right_min", mTlv.field_22_pause_right_min);
            TLVOBJECTBASE_ADD("pause_right_max", mTlv.field_24_pause_right_max);
            TLVOBJECTBASE_ADD("chal_type", mTlv.field_26_chal_type);
            TLVOBJECTBASE_ADD("chal_time", mTlv.field_28_chal_time);
            TLVOBJECTBASE_ADD("number_of_times_to_shoot", mTlv.field_2A_number_of_times_to_shoot);
            TLVOBJECTBASE_ADD("unknown", mTlv.field_2C_unknown);
            TLVOBJECTBASE_ADD("code1", mTlv.field_2E_code1);
            TLVOBJECTBASE_ADD("code2", mTlv.field_30_code2);
            TLVOBJECTBASE_ADD("chase_abe", mTlv.field_32_chase_abe);
            TLVOBJECTBASE_ADD("start_direction", mTlv.field_34_start_direction);
            TLVOBJECTBASE_ADD("panic_timeout", mTlv.field_36_panic_timeout);
            TLVOBJECTBASE_ADD("num_panic_sounds", mTlv.field_38_num_panic_sounds);
            TLVOBJECTBASE_ADD("panic_sound_timeout", mTlv.field_3A_panic_sound_timeout);
            TLVOBJECTBASE_ADD("stop_chase_delay", mTlv.field_3C_stop_chase_delay);
            TLVOBJECTBASE_ADD("time_to_wait_before_chase", mTlv.field_3E_time_to_wait_before_chase);
            TLVOBJECTBASE_ADD("slig_id", mTlv.field_40_slig_id);
            TLVOBJECTBASE_ADD("listen_time", mTlv.field_42_listen_time);
            TLVOBJECTBASE_ADD("percent_say_what", mTlv.field_44_percent_say_what);
            TLVOBJECTBASE_ADD("percent_beat_mud", mTlv.field_46_percent_beat_mud);
            TLVOBJECTBASE_ADD("talk_to_abe", mTlv.field_48_talk_to_abe);
            TLVOBJECTBASE_ADD("dont_shoot", mTlv.field_4A_dont_shoot);
            TLVOBJECTBASE_ADD("z_shoot_delay", mTlv.field_4C_z_shoot_delay);
            TLVOBJECTBASE_ADD("stay_awake", mTlv.field_4E_stay_awake);
            TLVOBJECTBASE_ADD("disable_resources", mTlv.field_50_disable_resources);
            TLVOBJECTBASE_ADD("noise_wake_up_distance", mTlv.field_52_noise_wake_up_distance);
            TLVOBJECTBASE_ADD("id", mTlv.field_54_id);
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
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("device_x", mTlv.field_1A_device_x);
            TLVOBJECTBASE_ADD("device_y", mTlv.field_1C_device_y);
            TLVOBJECTBASE_ADD("speed_x256", mTlv.field_1E_speed_x256);
            TLVOBJECTBASE_ADD("start_move_direction", mTlv.field_20_start_move_direction);
            TLVOBJECTBASE_ADD("draw_flare", mTlv.field_22_draw_flare);
            TLVOBJECTBASE_ADD("disable_id", mTlv.field_24_disable_id);
            TLVOBJECTBASE_ADD("alarm_id", mTlv.field_26_alarm_id);
            TLVOBJECTBASE_ADD("alarm_ticks", mTlv.field_28_alarm_ticks);
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
            TLVOBJECTBASE_ADD("animation_id", mTlv.field_18_animation_id);
            TLVOBJECTBASE_ADD("is_semi_trans", mTlv.field_1A_is_semi_trans);
            TLVOBJECTBASE_ADD("semi_trans_mode", mTlv.field_1C_semi_trans_mode);
            TLVOBJECTBASE_ADD("sound_effect", mTlv.field_1E_sound_effect);
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
            TLVOBJECTBASE_ADD("unload_cams_asap", mTlv.unload_cams_ASAP);
        }
    };

    struct Path_Pulley : public TlvObjectBaseAO<AO::Path_Pulley>
    {
        CTOR_AO(Path_Pulley, "Pulley", AO::TlvTypes::Pulley_35)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.scale);
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
            TLVOBJECTBASE_ADD("level", mTlv.level);
            TLVOBJECTBASE_ADD("path", mTlv.path);
            TLVOBJECTBASE_ADD("camera", mTlv.camera);
        }
    };

    struct Path_AbeStart : public TlvObjectBaseAO<AO::Path_AbeStart>
    {
        CTOR_AO(Path_AbeStart, "AbeStart", AO::TlvTypes::AbeStart_37)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.scale);
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
            TLVOBJECTBASE_ADD("how_far_to_walk", mTlv.field_18_how_far_to_walk);
            TLVOBJECTBASE_ADD("lift_id", mTlv.field_1A_lift_id);
            TLVOBJECTBASE_ADD("direction", mTlv.field_1C_direction);
            TLVOBJECTBASE_ADD("silent", mTlv.field_1E_silent);
            TLVOBJECTBASE_ADD("scale", mTlv.field_20_scale);
            TLVOBJECTBASE_ADD("code1", mTlv.field_22_code1);
            TLVOBJECTBASE_ADD("code2", mTlv.field_24_code2);
        }
    };

    struct Path_HoneySack : public TlvObjectBaseAO<AO::Path_HoneySack>
    {
        CTOR_AO(Path_HoneySack, "HoneySack", AO::TlvTypes::HoneySack_36)
        {
            TLVOBJECTBASE_ADD("chase_ticks", mTlv.field_18_chase_ticks);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1A_scale);
        }
    };

    struct Path_SlingMudokon : public TlvObjectBaseAO<AO::Path_SlingMudokon>
    {
        CTOR_AO(Path_SlingMudokon, "SlingMudokon", AO::TlvTypes::SlingMudokon_41)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("silent", mTlv.field_1A_silent);
            TLVOBJECTBASE_ADD("code1", mTlv.field_1C_code_1);
            TLVOBJECTBASE_ADD("code2", mTlv.field_1E_code_2);
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
            TLVOBJECTBASE_ADD("what_to_spawn", mTlv.field_18_what_to_spawn);
            TLVOBJECTBASE_ADD("interval", mTlv.field_1A_interval);
            TLVOBJECTBASE_ADD("id", mTlv.field_1C_id);
            TLVOBJECTBASE_ADD("movement_type", mTlv.field_1E_movement_type);
            TLVOBJECTBASE_ADD("size", mTlv.field_20_size);
            TLVOBJECTBASE_ADD("chase_time", mTlv.field_22_chase_time);
            TLVOBJECTBASE_ADD("speed", mTlv.field_24_speed);
            TLVOBJECTBASE_ADD("scale", mTlv.field_26_scale);
        }
    };

    struct Path_MeatSack : public TlvObjectBaseAO<AO::Path_MeatSack>
    {
        CTOR_AO(Path_MeatSack, "MeatSack", AO::TlvTypes::MeatSack_71)
        {
            TLVOBJECTBASE_ADD("side", mTlv.field_18_side);
            TLVOBJECTBASE_ADD("x_vel", mTlv.field_1A_x_vel);
            TLVOBJECTBASE_ADD("y_vel", mTlv.field_1C_y_vel);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1E_scale);
            TLVOBJECTBASE_ADD("amount_of_meat", mTlv.field_20_amount_of_meat);
        }
    };

    struct Path_RollingBall : public TlvObjectBaseAO<AO::Path_RollingBall>
    {
        CTOR_AO(Path_RollingBall, "RollingBall", AO::TlvTypes::RollingBall_56)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("roll_direction", mTlv.field_1A_roll_direction);
            TLVOBJECTBASE_ADD("release", mTlv.field_1C_release);
            TLVOBJECTBASE_ADD("speed", mTlv.field_1E_speed);
            TLVOBJECTBASE_ADD("acceleration", mTlv.field_20_acceleration);
        }
    };

    struct Path_RollingBallStopper : public TlvObjectBaseAO<AO::Path_RollingBallStopper>
    {
        CTOR_AO(Path_RollingBallStopper, "RollingBallStopper", AO::TlvTypes::RollingBallStopper_59)
        {
            TLVOBJECTBASE_ADD("id_on", mTlv.field_18_id_on);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1A_scale);
            TLVOBJECTBASE_ADD("id_off", mTlv.field_1C_id_off);
            TLVOBJECTBASE_ADD("direction", mTlv.field_1E_direction);
        }
    };

    struct Path_Bees : public TlvObjectBaseAO<AO::Path_Bees>
    {
        CTOR_AO(Path_Bees, "Bees", AO::TlvTypes::Bees_43)
        {
            TLVOBJECTBASE_ADD("id", mTlv.id);
            TLVOBJECTBASE_ADD("swarm_size", mTlv.swarm_size);
            TLVOBJECTBASE_ADD("chase_time", mTlv.chase_time);
            TLVOBJECTBASE_ADD("speed", mTlv.speed);
            TLVOBJECTBASE_ADD("disable_resources", mTlv.disable_resources);
            TLVOBJECTBASE_ADD("num_bees", mTlv.num_bees);
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
            TLVOBJECTBASE_ADD("start_pos", mTlv.field_18_start_pos);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1A_scale);
            TLVOBJECTBASE_ADD("speed", mTlv.field_1C_speed);
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
            TLVOBJECTBASE_ADD("id", mTlv.field_18_id);
            TLVOBJECTBASE_ADD("scale", mTlv.field_1A_scale);
            TLVOBJECTBASE_ADD("action", mTlv.field_1C_action);
            TLVOBJECTBASE_ADD("triggered_by", mTlv.field_1E_trigger_by);
        }
    };

    struct Path_Paramite : public TlvObjectBaseAO<AO::Path_Paramite>
    {
        CTOR_AO(Path_Paramite, "Paramite", AO::TlvTypes::Paramite_48)
        {
            TLVOBJECTBASE_ADD("scale", mTlv.field_18_scale);
            TLVOBJECTBASE_ADD("enter_from_web", mTlv.field_1A_bEnter_from_web);
            TLVOBJECTBASE_ADD("attack_delay", mTlv.field_1C_attack_delay);
            TLVOBJECTBASE_ADD("drop_in_timer", mTlv.field_1E_drop_in_timer);
            TLVOBJECTBASE_ADD("meat_eating_time", mTlv.field_20_meat_eating_time);
            TLVOBJECTBASE_ADD("attack_duration", mTlv.field_22_attack_duration);
            TLVOBJECTBASE_ADD("disabled_resources", mTlv.field_24_disabled_resources);
            TLVOBJECTBASE_ADD("id", mTlv.field_26_id);
            TLVOBJECTBASE_ADD("hiss_before_attack", mTlv.field_28_hiss_before_attack);
            TLVOBJECTBASE_ADD("delete_when_far_away", mTlv.field_2A_delete_when_far_away);
        }
    };

    struct Path_Honey : public TlvObjectBaseAO<AO::Path_Honey>
    {
        CTOR_AO(Path_Honey, "Honey", AO::TlvTypes::Honey_20)
        {
            TLVOBJECTBASE_ADD("id", mTlv.id);
            TLVOBJECTBASE_ADD("state", mTlv.state);
            TLVOBJECTBASE_ADD("scale", mTlv.scale);
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
