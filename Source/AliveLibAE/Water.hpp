#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Water_Res
{
    FP field_0_xpos;
    FP field_4_ypos;
    FP field_8_zpos;
    FP field_C_delta_x;
    FP field_10_delta_y;
    FP field_14_delta_z;
    s16 field_18_enabled;
    s16 field_1A_splash_time;
    s16 field_1C_state;
    s16 field_1E_padding;
    Poly_FT4 field_20_polys[2];
};
ALIVE_ASSERT_SIZEOF(Water_Res, 0x78);

struct Path_Water_Data
{
    s16 field_10_max_drops;
    s16 field_12_id;
    s16 field_14_splash_time;
    s16 field_16_splash_x_velocity;
    s16 field_18_splash_y_velocity;
    s16 field_1A_water_duration;
};
ALIVE_ASSERT_SIZEOF(Path_Water_Data, 0xC);

struct Path_Water : public Path_TLV
{
    Path_Water_Data field_10_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Water, 0x1C);

class Water : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Water* ctor_4E02C0(Path_Water* pTlv, s32 tlvInfo);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
private:
    EXPORT void dtor_4E0880();
    EXPORT Water* vdtor_4E0850(s32 flags);
    EXPORT void vScreenChanged_4E1780();
    EXPORT void vStopAudio_4E1800();
    EXPORT void Disable_Water_Particle_4E0B10(s16 idx);
    EXPORT void Add_Water_Particle_4E09A0();
    EXPORT void vUpdate_4E0B50();
    EXPORT void vRender_4E1440(PrimHeader** ppOt);
private:
    BYTE** field_F4_ppWaterRes;
    Water_Res* field_F8_pWaterRes;

    enum class WaterState : s16
    {
        eInit_0 = 0,
        eStarting_1 = 1,
        eFlowing_2 = 2,
        eStopping_3 = 3,
        eInactive_4 = 4,
    };

    WaterState field_FC_state;
    TPageMode field_FE_texture_mode;
    // pad
    s16 field_100_screen_x;
    s16 field_102_screen_y;
    PSX_Point field_104_top_left;
    PSX_Point field_108_bottom_right;
    s16 field_10C_particle_count;
    s16 field_10E_current_particle_idx;
    s16 field_110_current_drops;
    s32 field_114_tlvInfo;
    FP field_118_radius;
    FP field_11C_centre;
    s16 field_120_frame_width;
    s16 field_122_frame_height;
    Path_Water_Data field_124_tlv_data;
    FP field_130_splash_x_vel;
    FP field_134_emit_x_vel;
    s16 field_138_splash_time;
    s32 field_13C_not_in_camera_count;
    s32 field_140_water_duration;
    s32 field_144_sound_channels;
    s16 field_148_bHitTimeout;
};
ALIVE_ASSERT_SIZEOF(Water, 0x14C);
