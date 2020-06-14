#pragma once

#include "PathData.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

enum class LevelIds : __int16;

struct Path_Door : public Path_TLV
{
    LevelIds field_18_level;
    __int16 field_1A_path;
    __int16 field_1C_camera;
    __int16 field_1E_scale;
    unsigned __int16 field_20_door_number;
    __int16 field_22_id;
    __int16 field_24_target_door_number;
    __int16 field_26_start_state;
    __int16 field_28_door_closed;
    __int16 field_2A_hubs[8];
    __int16 field_3A_wipe_effect;
    __int16 field_3C_movie_number;
    __int16 field_3E_x_offset;
    __int16 field_40_y_offset;
    __int16 field_42_wipe_x_org;
    __int16 field_44_wipe_y_org;
    __int16 field_46_abe_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Door, 0x48);

END_NS_AO
