#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"

namespace AOTlvs
{
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
