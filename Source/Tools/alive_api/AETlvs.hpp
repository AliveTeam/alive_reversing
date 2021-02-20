#pragma once

#include "TlvObjectBase.hpp"

namespace AETlvs
{
    class Path_Hoist : public TlvObjectBase
    {
    public:
        Path_Hoist(TypesCollection& globalTypes, Path_TLV* pTlv = nullptr) : TlvObjectBase("Hoist", mData)
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
