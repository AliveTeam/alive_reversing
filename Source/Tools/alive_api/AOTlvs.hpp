#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../AliveLibAO/Door.hpp"

namespace AOTlvs
{
    class Path_Door : public TlvObjectBaseAO
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

        Path_Door(TypesCollection& globalTypes, AO::Path_TLV* pTlv = nullptr) : TlvObjectBaseAO("Door", mData)
        {
            COPY_TLV();
            ADD("Level", mData.field_18_level);
            ADD("Path", mData.field_1A_path);
            ADD("Camera", mData.field_1C_camera);
            ADD("Scale", mData.field_1E_scale);
            ADD("DoorNumber", mData.field_20_door_number);
            ADD("Id", mData.field_22_id);
            ADD("TargetDoorNumber", mData.field_24_target_door_number);
            ADD("StartState", mData.field_26_start_state);
            ADD("DoorClosed", mData.field_28_door_closed);
            //ADD("Hubs", mData.field_2A_hubs); // TODO: Handle array
            ADD("WipeEffect", mData.field_3A_wipe_effect);
            ADD("MovieNumber", mData.field_3C_movie_number);
            ADD("XOffset", mData.field_3E_x_offset);
            ADD("YOffset", mData.field_40_y_offset);
            ADD("WipeXOrg", mData.field_42_wipe_x_org);
            ADD("WipeYOrg", mData.field_44_wipe_y_org);
            ADD("AbeDirection", mData.field_46_abe_direction);
        }

    private:
        AO::Path_Door mData = {};
    };

    class Path_ContinuePoint : public TlvObjectBaseAO
    {
    public:
        Path_ContinuePoint(TypesCollection& globalTypes, AO::Path_TLV* pTlv = nullptr) : TlvObjectBaseAO("ContinuePoint", mData)
        {
            COPY_TLV();
            ADD("ZoneNumber", mData.field_18_zone_number);
            ADD("ClearFromId", mData.field_1A_clear_from_id);
            ADD("ClearToId", mData.field_1C_clear_to_id);
            ADD("ElumRestarts", mData.field_1E_elum_restarts);
            ADD("AbeSpawnDirection", mData.field_20_abe_direction);
        }

    private:
        AO::Path_ContinuePoint mData = {};
    };

    class Path_Hoist : public TlvObjectBaseAO
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

        Path_Hoist(TypesCollection& globalTypes, AO::Path_TLV* pTlv = nullptr) : TlvObjectBaseAO("Hoist", mData)
        {
            COPY_TLV();
            ADD("HoistType", mData.field_18_hoist_type);
            ADD("HoistEdgeType", mData.field_1A_edge_type);
            ADD("Id", mData.field_1C_id);
        }

    private:
        AO::Path_Hoist mData = {};
    };
}
