#pragma once

#include "TlvObjectBase.hpp"
#include "../AliveLibAE/Path.hpp"
#include "../AliveLibAE/Switch.hpp"

#define CTOR_AE(className, objectTypeName, tlvType)  className(TypesCollection& globalTypes, Path_TLV* pTlv = nullptr) : TlvObjectBaseAE(tlvType, objectTypeName, pTlv)

namespace AETlvs
{
    class Path_Switch : public TlvObjectBaseAE<::Path_Switch>
    {
    public:
        void AddTypes(TypesCollection& types) override
        {
            types.AddEnum<::SwitchSoundType>("Enum_SwitchSoundType",
                {
                    {::SwitchSoundType::eWell_1, "well"},
                    {::SwitchSoundType::eUnknown_2, "unknown"},
                    {::SwitchSoundType::eDoor_3, "door"},
                    {::SwitchSoundType::eElectricWall_4, "electric_wall"},
                    {::SwitchSoundType::eSecurityOrb_5, "security_orb"},
                    {::SwitchSoundType::eLift_6, "lift"}
                });
        }

        CTOR_AE(Path_Switch, "Switch", TlvTypes::Switch_17)
        {
            ADD("TargetAction", mTlv.field_10_target_action);
            ADD("TargetAction", mTlv.field_12_scale);
            ADD("TargetAction", mTlv.field_14_on_sound);
            ADD("TargetAction", mTlv.field_16_off_sound);
            ADD("TargetAction", mTlv.field_18_sound_direction);
            ADD("TargetAction", mTlv.field_1A_trigger_id);
            ADD("TargetAction", mTlv.field_1C_persist_offscreen);
        }
    };

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

        CTOR_AE(Path_Hoist, "Hoist", TlvTypes::Hoist_2)
        {
            ADD("HoistType", mTlv.field_10_type);
            ADD("HoistEdgeType", mTlv.field_12_edge_type);
            ADD("Id", mTlv.field_14_id);
            ADD("Scale", mTlv.field_16_scale);
        }
    };
}
