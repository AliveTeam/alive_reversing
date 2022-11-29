#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"

namespace relive
{
    struct Path_LevelLoader;
}

class LevelLoader final : public BaseGameObject
{
public:
    LevelLoader(relive::Path_LevelLoader* pTlv, const Guid& tlvId);

    virtual void VUpdate() override;

private:
    s16 field_20_switch_id = 0;
    enum class States : s16
    {
        eIdle_0 = 0,
        eFadingOut_1 = 1,
        eDone_2 = 2,
    };
    States field_22_state = States::eIdle_0;
    EReliveLevelIds field_24_level = EReliveLevelIds::eNone;
    s16 field_26_path = 0;
    s16 field_28_camera = 0;
    s16 field_2A_movie_id = 0;
    Guid field_2C_tlvInfo;
    Guid field_30_fade_id;
};
