#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AETlvs
{
    class Path_Hoist : public TlvObjectBaseAE
    {
    public:
        static void AddTypes(TypesCollection& types)
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

        Path_Hoist(TypesCollection& globalTypes, Path_TLV* pTlv = nullptr) : TlvObjectBaseAE("Hoist", mData)
        {
            COPY_TLV();
            ADD_PROP("HoistType", mData.field_10_type);
            ADD_PROP("HoistEdgeType", mData.field_12_edge_type);
            ADD_PROP("Id", mData.field_14_id);
            ADD_PROP("Scale", mData.field_16_scale);
        }

        void PropertiesFromJson(TypesCollection& types, jsonxx::Object& properties) override
        {
            READ_ENUMS(mData.field_10_type);
            READ_ENUMS(mData.field_12_edge_type);
            READ_BASIC(mData.field_14_id);
            READ_ENUMS(mData.field_16_scale);
        }

        void PropertiesToJson(TypesCollection& types, jsonxx::Object& properties) override
        {
            WRITE_ENUMS(mData.field_10_type);
            WRITE_ENUMS(mData.field_12_edge_type);
            WRITE_BASIC(mData.field_14_id);
            WRITE_ENUMS(mData.field_16_scale);
        }

    private:
        ::Path_Hoist mData = {};
    };
}
