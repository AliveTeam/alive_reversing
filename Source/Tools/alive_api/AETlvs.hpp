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
            ADD("HoistType", mData.field_10_type);
            ADD("HoistEdgeType", mData.field_12_edge_type);
            ADD("Id", mData.field_14_id);
            ADD("Scale", mData.field_16_scale);
        }

    private:
        ::Path_Hoist mData = {};
    };
}
