#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAO/Abe.hpp"

namespace AOTlvs
{
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
