#pragma once

#include "relive_tlvs_serialization.hpp"

// Convert an AO or AE TLV to a relive TLV

namespace relive {
class Path_ShadowZone_Converter
{
public:
    static Path_ShadowZone From(const AO::Path_ShadowZone& tlv)
    {
        Path_ShadowZone r;
        r.mRGB.SetRGB(tlv.field_1C_r, tlv.field_1E_g, tlv.field_20_b);
        r.mScale = From(tlv.field_24_scale);
        return r;
    }

    static Path_ShadowZone From(const ::Path_ShadowZone& tlv)
    {
        Path_ShadowZone r;
        r.mRGB.SetRGB(tlv.field_14_r, tlv.field_16_g, tlv.field_18_b);
        r.mScale = From(tlv.field_1C_scale);
        return r;
    }

private:
    static Path_ShadowZone::Scale From(::ShadowZoneScale aeScale)
    {
        switch (aeScale)
        {
            case ::ShadowZoneScale::eHalf_1:
                return Path_ShadowZone::Scale::eHalf;
            case ::ShadowZoneScale::eFull_2:
                return Path_ShadowZone::Scale::eFull;
            case ::ShadowZoneScale::eBoth_0:
                return Path_ShadowZone::Scale::eBoth;
        }
        ALIVE_FATAL("Bad AE shadow scale");
    }

    static Path_ShadowZone::Scale From(AO::ShadowZoneScale aoScale)
    {
        switch (aoScale)
        {
            case AO::ShadowZoneScale::eHalf_1:
                return Path_ShadowZone::Scale::eHalf;
            case AO::ShadowZoneScale::eFull_2:
                return Path_ShadowZone::Scale::eFull;
            case AO::ShadowZoneScale::eBoth_0:
                return Path_ShadowZone::Scale::eBoth;
        }
        ALIVE_FATAL("Bad AO shadow scale");
    }
};

} // namespace relive
