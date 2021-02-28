#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../AliveLibAO/Door.hpp"

#define CTOR_AO(className, objectTypeName)  className(TypesCollection& globalTypes, AO::Path_TLV* pTlv = nullptr) : TlvObjectBaseAO(objectTypeName, pTlv)

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
            //ADD("Hubs", mTlv.field_2A_hubs); // TODO: Handle array
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
}
