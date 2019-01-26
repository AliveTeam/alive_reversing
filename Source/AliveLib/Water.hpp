#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Water_Res
{
    int field_0_xpos;
    int field_4_ypos;
    int field_8_zpos;
    int field_C_delta_x;
    int field_10_delta_y;
    int field_14_delta_z;
    __int16 field_18_enabled;
    __int16 field_1A_splash_time;
    __int16 field_1C_state;
    __int16 field_1E_pad;
    Poly_FT4 field_20_polys[2];
};
ALIVE_ASSERT_SIZEOF(Water_Res, 0x78);

struct Path_Water_Data
{
    __int16 field_10_max_drops;
    __int16 field_12_id;
    __int16 field_14_splash_time;
    __int16 field_16_splash_x_velocity;
    __int16 field_18_splash_y_velocity;
    __int16 field_1A_timeout;
};
ALIVE_ASSERT_SIZEOF(Path_Water_Data, 0xC);

struct Path_Water : public Path_TLV
{
    Path_Water_Data field_10_data;
};
ALIVE_ASSERT_SIZEOF(Path_Water, 0x1C);

class Water : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Water* ctor_4E02C0(Path_Water* pTlv, int tlvInfo);

private:
    int field_E4_not_used[4];
    BYTE** field_F4_ppWaterRes;
    Water_Res *field_F8_pWaterRes;
    __int16 field_FC_state;
    __int16 field_FE_texture_mode;
    __int16 field_100_screen_x;
    __int16 field_102_screen_y;
    PSX_Point field_104_top_left;
    PSX_Point field_108_bottom_right;
    __int16 field_10C_update_count;
    __int16 field_10E_current_particle_idx;
    __int16 field_110_radius;
    __int16 field_112_centre;
    int field_114_tlvInfo;
    FP field_118_radius;
    FP field_11C_centre;
    __int16 field_120_frame_width;
    __int16 field_122_frame_height;
    Path_Water_Data field_124_tlv_data;
    int field_130_splash_x_vel;
    int field_134_emit_x_vel;
    __int16 field_138_splash_time;
    //__int16 field_13A_pad;
    int field_13C_not_in_camera_count;
    int field_140_time_on;
    int field_144_sound_channels;
    __int16 field_148_bHitTimeout;
    //__int16 field_14A_pad;
};
ALIVE_ASSERT_SIZEOF(Water, 0x14C);