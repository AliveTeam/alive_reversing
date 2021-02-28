#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAE/Path.hpp"

#define CTOR_AE(className, objectTypeName)  className(TypesCollection& globalTypes, Path_TLV* pTlv = nullptr) : TlvObjectBaseAE(objectTypeName)

namespace AETlvs
{
    class Path_Hoist : public TlvObjectBaseAE<::Path_Hoist>
    {
    public:
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

        CTOR_AE(Path_Hoist, "Hoist")
        {
            COPY_TLV();
            ADD("HoistType", mTlv.field_10_type);
            ADD("HoistEdgeType", mTlv.field_12_edge_type);
            ADD("Id", mTlv.field_14_id);
            ADD("Scale", mTlv.field_16_scale);
        }
    };
}
