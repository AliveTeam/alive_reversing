#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Water_Res final
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

struct Path_Water_Data final
{
    s16 mMaxDrops;
    s16 mSwitchId;
    s16 mSplashTime;
    s16 mSplashVelX;
    s16 mSplashVelY; // unused
    s16 mWaterDuration;
};
ALIVE_ASSERT_SIZEOF(Path_Water_Data, 0xC);

struct Path_Water final : public Path_TLV
{
    Path_Water_Data mWaterData;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Water, 0x1C);

class Water final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Water(relive::Path_Water* pTlv, const Guid& tlvId);
    ~Water();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VStopAudio() override;

private:
    void Disable_Water_Particle(s16 idx);
    void Add_Water_Particle();

private:
    u8** field_F4_ppWaterRes = nullptr;
    Water_Res* field_F8_pWaterRes = nullptr;

    enum class WaterState : s16
    {
        eInit_0 = 0,
        eStarting_1 = 1,
        eFlowing_2 = 2,
        eStopping_3 = 3,
        eInactive_4 = 4,
    };

    WaterState field_FC_state = WaterState::eInit_0;
    TPageMode field_FE_texture_mode = TPageMode::e4Bit_0;
    s16 field_100_screen_x = 0;
    s16 field_102_screen_y = 0;
    PSX_Point field_104_top_left = {};
    PSX_Point field_108_bottom_right = {};
    s16 field_10C_particle_count = 0;
    s16 field_10E_current_particle_idx = 0;
    s16 field_110_current_drops = 0;
    Guid field_114_tlvInfo;
    FP field_118_radius = {};
    FP field_11C_centre = {};
    s16 field_120_frame_width = 0;
    s16 field_122_frame_height = 0;
    relive::Path_Water field_124_tlv_data = {};
    FP field_130_splash_x_vel = {};
    FP field_134_emit_x_vel = {};
    s16 field_138_splash_time = 0;
    s32 field_13C_not_in_camera_count = 0;
    s32 field_140_water_duration = 0;
    s32 field_144_sound_channels = 0;
    s16 field_148_bHitTimeout = 0;
};
ALIVE_ASSERT_SIZEOF(Water, 0x14C);
