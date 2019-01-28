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
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;
private:
    EXPORT void dtor_4E0880();
    EXPORT Water* vdtor_4E0850(signed int flags);
    EXPORT void vScreenChanged_4E1780();
    EXPORT void vStopAudio_4E1800();
    EXPORT void Disable_Water_Particle_4E0B10(__int16 idx);
    EXPORT void Add_Water_Particle_4E09A0();
    EXPORT void vUpdate_4E0B50();
    EXPORT void vRender_4E1440(int** pOt);
private:
    int field_E4_not_used[4];
    BYTE** field_F4_ppWaterRes;
    Water_Res *field_F8_pWaterRes;
    enum class State : __int16
    {
        WaitForEnable_0 = 0,
        State_1 = 1,
        State_2 = 2,
        State_3 = 3,
        State_4 = 4,
    };
    State field_FC_state;
    __int16 field_FE_texture_mode;
    __int16 field_100_screen_x;
    __int16 field_102_screen_y;
    PSX_Point field_104_top_left;
    PSX_Point field_108_bottom_right;
    __int16 field_10C_particle_count;
    __int16 field_10E_current_particle_idx;
    __int16 field_110_current_drops;
    //__int16 field_112_pad;
    int field_114_tlvInfo;
    FP field_118_radius;
    FP field_11C_centre;
    __int16 field_120_frame_width;
    __int16 field_122_frame_height;
    Path_Water_Data field_124_tlv_data;
    FP field_130_splash_x_vel;
    FP field_134_emit_x_vel;
    __int16 field_138_splash_time;
    //__int16 field_13A_pad;
    int field_13C_not_in_camera_count;
    int field_140_time_on;
    int field_144_sound_channels;
    __int16 field_148_bHitTimeout;
    //__int16 field_14A_pad;
};
ALIVE_ASSERT_SIZEOF(Water, 0x14C);
