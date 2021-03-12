#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../AliveLibAO/Door.hpp"
#include "../AliveLibAO/Switch.hpp"
#include "../AliveLibAO/DoorLight.hpp"
#include "../AliveLibAO/ElectricWall.hpp"

#define CTOR_AO(className, objectTypeName)  className(TypesCollection& globalTypes, AO::Path_TLV* pTlv = nullptr) : TlvObjectBaseAO(objectTypeName, pTlv)

namespace AO
{
    struct Path_ContinueZone : public Path_TLV
    {
        int field_10_zone_number;
    };

    struct Path_StartController : public Path_TLV
    {
        // No fields
    };
}

namespace AOTlvs
{
    class Path_Door : public TlvObjectBaseAO<AO::Path_Door>
    {
    public:
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<AO::DoorStates>("Enum_DoorStates",
                {
                    {AO::DoorStates::eOpen_0, "Open"},
                    {AO::DoorStates::eClosed_1, "Closed"},
                    {AO::DoorStates::eOpening_2, "Opening"},
                    {AO::DoorStates::eClosing_3, "Closing"},
                });
        }

        CTOR_AO(Path_Door, "Door")
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

    class Path_ContinuePoint : public TlvObjectBaseAO<AO::Path_ContinuePoint>
    {
    public:
        CTOR_AO(Path_ContinuePoint, "ContinuePoint")
        {
            ADD("ZoneNumber", mTlv.field_18_zone_number);
            ADD("ClearFromId", mTlv.field_1A_clear_from_id);
            ADD("ClearToId", mTlv.field_1C_clear_to_id);
            ADD("ElumRestarts", mTlv.field_1E_elum_restarts);
            ADD("AbeSpawnDirection", mTlv.field_20_abe_direction);
        }
    };

    class Path_Hoist : public TlvObjectBaseAO<AO::Path_Hoist>
    {
    public:
        void AddTypes(TypesCollection& types) override
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

        CTOR_AO(Path_Hoist, "Hoist")
        {
            ADD("HoistType", mTlv.field_18_hoist_type);
            ADD("HoistEdgeType", mTlv.field_1A_edge_type);
            ADD("Id", mTlv.field_1C_id);
        }
    };

    class Path_Change : public TlvObjectBaseAO<AO::Path_ChangeTLV>
    {
    public:
        CTOR_AO(Path_Change, "PathTransition")
        {
            ADD("Level", mTlv.field_18_level);
            ADD("HoistEdgeType", mTlv.field_1A_path);
            ADD("Camera", mTlv.field_1C_camera);
            ADD("Movie", mTlv.field_1E_movie); // TODO: Enum
            ADD("Wipe", mTlv.field_20_wipe); // TODO: Enum
            ADD("Scale", mTlv.field_22_scale); // TODO: Enum
        }
    };

    class Path_Switch : public TlvObjectBaseAO<AO::Path_Switch>
    {
    public:
        CTOR_AO(Path_Switch, "Switch")
        {
            ADD("TriggerObject", mTlv.field_18_trigger_object);
            ADD("TriggerAction", mTlv.field_1A_trigger_object_action);
            ADD("Scale", mTlv.field_1C_scale);
            ADD("OnSound", mTlv.field_1E_on_sound); // TODO: Enum
            ADD("OffSound", mTlv.field_20_off_sound); // TODO: Enum
            ADD("SoundDirection", mTlv.field_22_sound_direction); // TODO: Enum
        }
    };

    class Path_LightEffect : public TlvObjectBaseAO<AO::Path_LightEffect>
    {
    public:
        void AddTypes(TypesCollection& types) override
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

        CTOR_AO(Path_LightEffect, "LightEffect")
        {
            ADD("Type", mTlv.field_18_type);
            ADD("Size", mTlv.field_1A_size);
            ADD("Id", mTlv.field_1C_id);
            ADD("FlipX", mTlv.field_1E_flip_x);
        }
    };

    class Path_ElectricWall : public TlvObjectBaseAO<AO::Path_ElectricWall>
    {
    public:
        CTOR_AO(Path_ElectricWall, "ElectricWall")
        {
            ADD("Scale", mTlv.field_18_scale);
            ADD("Id", mTlv.field_1A_id);
            ADD("State", mTlv.field_1C_start_state); // TODO: Enum
        }
    };

    class Path_ContinueZone : public TlvObjectBaseAO<AO::Path_ContinueZone>
    {
    public:
        CTOR_AO(Path_ContinueZone, "ContinueZone")
        {
            ADD("ZoneNumber", mTlv.field_10_zone_number);
        }
    };

    class Path_StartController : public TlvObjectBaseAO<AO::Path_StartController>
    {
    public:
        CTOR_AO(Path_StartController, "StartController")
        {
            // No fields
        }
    };
}
