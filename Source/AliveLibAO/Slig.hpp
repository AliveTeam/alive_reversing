#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

START_NS_AO

struct Path_Slig : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_start_state;
    __int16 field_1C_pause_time;
    __int16 field_1E_pause_left_min;
    __int16 field_20_pause_left_max;
    __int16 field_22_pause_right_min;
    __int16 field_24_pause_right_max;
    __int16 field_26_chal_type;
    __int16 field_28_chal_time;
    __int16 field_2A_number_of_times_to_shoot;
    __int16 field_2C_unknown;
    __int16 field_2E_code1;
    __int16 field_30_code2;
    __int16 field_32_chase_abe;
    __int16 field_34_start_direction;
    __int16 field_36_panic_timeout;
    __int16 field_38_num_panic_sounds;
    __int16 field_3A_panic_sound_timeout;
    __int16 field_3C_stop_chase_delay;
    __int16 field_3E_time_to_wait_before_chase;
    __int16 field_40_slig_id;
    __int16 field_42_listen_time;
    __int16 field_44_percent_say_what;
    __int16 field_46_percent_beat_mud;
    __int16 field_48_talk_to_abe;
    __int16 field_4A_dont_shoot;
    __int16 field_4C_z_shoot_delay;
    __int16 field_4E_stay_awake;
    __int16 field_50_disable_resources;
    __int16 field_52_noise_wake_up_distance;
    __int16 field_54_id;
    __int16 field_56_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Slig, 0x58);

class Slig : public BaseAliveGameObject
{
public:
    EXPORT Slig* ctor_464D40(Path_Slig* pTlv, int tlvInfo);
    
    EXPORT BaseAliveGameObject* dtor_465320();


    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Slig* Vdtor_465DC0(signed int flags);

    EXPORT void Init_46B890();

    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110;
    __int16 field_112;
    int field_114;
    int field_118;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126;
    int field_128;
    int field_12C;
    __int16 field_130;
    __int16 field_132;
    int field_134_tlvInfo;
    __int16 field_138;
    __int16 field_13A;
    int field_13C;
    int field_140;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    LevelIds field_14E_level;
    __int16 field_150_path;
    __int16 field_152_camera;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    __int16 field_170;
    __int16 field_172;
    Path_Slig field_174_tlv;
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
    int field_1E8;
    int field_1EC;
    int field_1F0;
    int field_1F4;
    int field_1F8_fn;
    int field_1FC;
    int field_200;
    int field_204;
    int field_208;
    __int16 field_20C;
    __int16 field_20E;
    BYTE** field_210_resources[17];
    __int16 field_254;
    __int16 field_256;
    int field_258;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x25C);

END_NS_AO
