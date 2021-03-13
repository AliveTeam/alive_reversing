#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/Switch.hpp"
#include "../AliveLibAE/ElectricWall.hpp"
#include "../AliveLibAE/Mudokon.hpp"
#include "../AliveLibAE/BirdPortal.hpp"
#include "../AliveLibAE/StatsSign.hpp"
#include "../AliveLibAE/Door.hpp"

#define CTOR_AE(className, objectTypeName, tlvType)  className(TypesCollection& globalTypes, Path_TLV* pTlv = nullptr) : TlvObjectBaseAE(tlvType, objectTypeName, pTlv)

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
    public:
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
            ADD("Level", mTlv.field_10_level);
            ADD("Level", mTlv.field_12_path);
            ADD("Level", mTlv.field_14_camera);
            ADD("Level", mTlv.field_16_scale);
            ADD("Level", mTlv.field_18_door_number);
            ADD("Level", mTlv.field_1A_id);
            ADD("Level", mTlv.field_1C_target_door_number);
            ADD("Level", mTlv.field_1E_type);
            ADD("Level", mTlv.field_20_start_state);
            ADD("Level", mTlv.field_22_hub1);
            ADD("Level", mTlv.field_22_hub2);
            ADD("Level", mTlv.field_22_hub3);
            ADD("Level", mTlv.field_22_hub4);
            ADD("Level", mTlv.field_22_hub5);
            ADD("Level", mTlv.field_22_hub6);
            ADD("Level", mTlv.field_22_hub7);
            ADD("Level", mTlv.field_22_hub8);
            ADD("Level", mTlv.field_32_wipe_effect);
            ADD("Level", mTlv.field_34_movie_number);
            ADD("Level", mTlv.field_36_x_offset);
            ADD("Level", mTlv.field_38_y_offset);
            ADD("Level", mTlv.field_3A_wipe_x_org);
            ADD("Level", mTlv.field_3C_wipe_y_org);
            ADD("Level", mTlv.field_3E_abe_direction);
            ADD("Level", mTlv.field_40_close_after_use);
            ADD("Level", mTlv.field_42_cancel_throwables);
        }
    };

    struct Path_Switch : public TlvObjectBaseAE<::Path_Switch>
    {
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::SwitchSoundType>("Enum_SwitchSoundType",
                {
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
            ADD("TargetAction", mTlv.field_10_target_action);
            ADD("TargetAction", mTlv.field_12_scale);
            ADD("TargetAction", mTlv.field_14_on_sound);
            ADD("TargetAction", mTlv.field_16_off_sound);
            ADD("TargetAction", mTlv.field_18_sound_direction);
            ADD("TargetAction", mTlv.field_1A_trigger_id);
            ADD("TargetAction", mTlv.field_1C_persist_offscreen);
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
}
