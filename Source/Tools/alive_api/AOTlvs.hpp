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
        static void AddTypes(TypesCollection& types)
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
            ADD_PROP("Level", mData.field_18_level);
            ADD_PROP("Path", mData.field_1A_path);
            ADD_PROP("Camera", mData.field_1C_camera);
            ADD_PROP("Scale", mData.field_1E_scale);
            ADD_PROP("DoorNumber", mData.field_20_door_number);
            ADD_PROP("Id", mData.field_22_id);
            ADD_PROP("TargetDoorNumber", mData.field_24_target_door_number);
            ADD_PROP("StartState", mData.field_26_start_state);
            ADD_PROP("DoorClosed", mData.field_28_door_closed);
            ADD_PROP("Hubs", mData.field_2A_hubs); // TODO: Handle array
            ADD_PROP("WipeEffect", mData.field_3A_wipe_effect);
            ADD_PROP("MovieNumber", mData.field_3C_movie_number);
            ADD_PROP("XOffset", mData.field_3E_x_offset);
            ADD_PROP("YOffset", mData.field_40_y_offset);
            ADD_PROP("WipeXOrg", mData.field_42_wipe_x_org);
            ADD_PROP("WipeYOrg", mData.field_44_wipe_y_org);
            ADD_PROP("AbeDirection", mData.field_46_abe_direction);

            // TODO: Make read/write automatic
        }

        void PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties) override
        {
            READ_ENUMS(mData.field_18_level);
            READ_BASIC(mData.field_1A_path);
            READ_BASIC(mData.field_1C_camera);
            READ_BASIC(mData.field_1E_scale);
            READ_BASIC(mData.field_20_door_number);
            READ_BASIC(mData.field_22_id);
            READ_BASIC(mData.field_24_target_door_number);
            READ_ENUMS(mData.field_26_start_state);
            READ_BASIC(mData.field_28_door_closed);
            //READ_BASIC(mData.field_2A_hubs); // TODO
            READ_BASIC(mData.field_3A_wipe_effect);
            READ_BASIC(mData.field_3C_movie_number);
            READ_BASIC(mData.field_3E_x_offset);
            READ_BASIC(mData.field_40_y_offset);
            READ_BASIC(mData.field_42_wipe_x_org);
            READ_BASIC(mData.field_44_wipe_y_org);
            READ_BASIC(mData.field_46_abe_direction);
        }

        void PropertiesToJson(TypesCollection& types, jsonxx::Object& properties) override
        {
            WRITE_ENUMS(mData.field_18_level);
            WRITE_BASIC(mData.field_1A_path);
            WRITE_BASIC(mData.field_1C_camera);
            WRITE_BASIC(mData.field_1E_scale);
            WRITE_BASIC(mData.field_20_door_number);
            WRITE_BASIC(mData.field_22_id);
            WRITE_BASIC(mData.field_24_target_door_number);
            WRITE_ENUMS(mData.field_26_start_state);
            WRITE_BASIC(mData.field_28_door_closed);
           // WRITE_BASIC(mData.field_2A_hubs); // TODO
            WRITE_BASIC(mData.field_3A_wipe_effect);
            WRITE_BASIC(mData.field_3C_movie_number);
            WRITE_BASIC(mData.field_3E_x_offset);
            WRITE_BASIC(mData.field_40_y_offset);
            WRITE_BASIC(mData.field_42_wipe_x_org);
            WRITE_BASIC(mData.field_44_wipe_y_org);
            WRITE_BASIC(mData.field_46_abe_direction);
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
            ADD_PROP("ZoneNumber", mData.field_18_zone_number);
            ADD_PROP("ClearFromId", mData.field_1A_clear_from_id);
            ADD_PROP("ClearToId", mData.field_1C_clear_to_id);
            ADD_PROP("ElumRestarts", mData.field_1E_elum_restarts);
            ADD_PROP("AbeSpawnDirection", mData.field_20_abe_direction);
        }

        void PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties) override
        {
            READ_BASIC(mData.field_18_zone_number);
            READ_BASIC(mData.field_1A_clear_from_id);
            READ_BASIC(mData.field_1C_clear_to_id);
            READ_BASIC(mData.field_1E_elum_restarts);
            READ_BASIC(mData.field_20_abe_direction);
        }

        void PropertiesToJson(TypesCollection& types, jsonxx::Object& properties) override
        {
            WRITE_BASIC(mData.field_18_zone_number);
            WRITE_BASIC(mData.field_1A_clear_from_id);
            WRITE_BASIC(mData.field_1C_clear_to_id);
            WRITE_BASIC(mData.field_1E_elum_restarts);
            WRITE_BASIC(mData.field_20_abe_direction);
        }
    private:
        AO::Path_ContinuePoint mData = {};
    };

    class Path_Hoist : public TlvObjectBaseAO
    {
    public:
        static void AddTypes(TypesCollection& types)
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
            ADD_PROP("HoistType", mData.field_18_hoist_type);
            ADD_PROP("HoistEdgeType", mData.field_1A_edge_type);
            ADD_PROP("Id", mData.field_1C_id);
        }

        void PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties) override
        {
            READ_ENUMS(mData.field_18_hoist_type);
            READ_ENUMS(mData.field_1A_edge_type);
            READ_BASIC(mData.field_1C_id);
        }

        void PropertiesToJson(TypesCollection& types, jsonxx::Object& properties) override
        {
            WRITE_ENUMS(mData.field_18_hoist_type);
            WRITE_ENUMS(mData.field_1A_edge_type);
            WRITE_BASIC(mData.field_1C_id);
        }

    private:
        AO::Path_Hoist mData = {};
    };
}
